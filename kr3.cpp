#include <iostream>
#include <windows.h>
#include "Hashtable.h"

using namespace std;

//8.	Форма представления : хеш – таблица с открытой адресацией
//Тип ключа k - строка текста произвольной длины(символы – заглавные буквы кириллицы).
//Преобразование k к натуральному значению k’ по схеме Горнера.
//Метод хеширования k’ - мультипликативный.
//Метод разрешения коллизий - линейный.

int main() {
	setlocale(LC_ALL, "RU");

	Hashtable<int> hashtable = Hashtable<int>(0);
	auto it = hashtable.begin();
	int n;
	while (true) {
		cout << endl << "Меню. Выберите действие:" << endl << endl;
		cout << "0. Создать хеш-таблицу с заданным размером" << endl;
		cout << "1. Вставка" << endl;
		cout << "2. Удаление" << endl;
		cout << "3. Емкость" << endl;
		cout << "4. Размер" << endl;
		cout << "5. Проверка на пустоту" << endl;
		cout << "6. Поиск элемента по ключу" << endl;
		cout << "7. Вывод структуры на экран" << endl;
		cout << "8. Очистка" << endl;
		cout << "9. Опрос числа проб, выполненных предыдущей операцией" << endl;
		cout << "10. Итератор на начало" << endl;
		cout << "11. Показать содержание итератора" << endl;
		cout << "12. Перевести итератор на следующий элемент" << endl;
		cout << "13. Присвоить итератору новое значение" << endl;
		cout << endl;
		cin >> n;
		cout << endl;
		SetConsoleCP(1251);
		SetConsoleOutputCP(1251);
		try {
			switch (n) {
			case 0: {
				int s;
				cin >> s;
				hashtable = Hashtable<int>(s);
				break;
			}
			case 1: {
				string key;
				int value;
				cin >> key;
				cin >> value;
				cout << hashtable.Insert(key, value) << endl;
				break;
			}
			case 2: {
				string key;
				cin >> key;
				cout << hashtable.Delete(key) << endl;
				break;
			}
			case 3: {
				cout << hashtable.getCapacity() << endl;
				break;
			}
			case 4: {
				cout << hashtable.getSize() << endl;
				break;
			}
			case 5: {
				cout << hashtable.IsEmpty() << endl;
				break;
			}
			case 6: {
				string key;
				cin >> key;
				cout << hashtable.Search(key) << endl;
				break;
			}
			case 7: {
				hashtable.Show();
				break;
			}
			case 8: {
				hashtable.Clear();
				break;
			} 
			case 9: {
				cout << hashtable.getProbesForPrevOp() << endl;
				break;
			}
			case 10: {
				it = hashtable.begin();
				break;
			}
			case 11: {
				cout << *it << endl;
				break;
			}
			case 12: {
				++it;
				break;
			}
			case 13: {
				cin >> *it;
				break;
			}
			default: {
				cout << "Попробуйте еще раз" << endl;
				break;
			}
			}
		}
		catch (exception e) {
			cout << e.what() << endl;
		}
	}
}
