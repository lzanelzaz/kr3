#include <vector>
#include <iostream>

using namespace std;

template <class ValueType>
class Hashtable {

private:
	enum class State { FREE, BUSY, DELETED };

	class Node {
	public:
		State state = State::FREE;
		string key;
		ValueType value{};

		Node() {}
		Node(string key, ValueType value) : state(State::BUSY), key(key), value(value) {}

		void clear() {
			state = State::FREE;
			key = "";
			value = ValueType{};
		}
	};

	size_t size = 0;
	size_t probes = 0;

	vector<Node> data;

public:
	Hashtable(size_t size);

	bool Insert(string key, ValueType data);

	bool Delete(string key);

	void Show();

	ValueType Search(string key);

	size_t getCapacity();

	size_t getSize();

	size_t getProbesForPrevOp();

	bool IsEmpty();

	void Clear();

	class Iterator {
	private:
		Hashtable* table = nullptr;
		Node* current = nullptr;
		size_t currentIndex = 0;
	public:
		Iterator(Hashtable* table, bool isBegin) {
			this->table = table;
			if (!isBegin) {
				currentIndex = table->getCapacity();
				current = nullptr;
				return;
			}
			currentIndex = 0;
			while (currentIndex < table->getCapacity() && table->data[currentIndex].state != State::BUSY) {
				currentIndex++;
			}
			if (currentIndex == table->getCapacity() || table->data[currentIndex].state != State::BUSY) {
				current = nullptr;
			}
			else {
				current = &table->data[currentIndex];
			}
		}

		ValueType& operator* () {
			if (current == nullptr) {
				throw exception("Вышли за пределы таблицы");
			}
			return current->value;
		}

		Iterator& operator ++() {
			if (current != nullptr) {
				currentIndex++;
				while (currentIndex < table->getCapacity() && table->data[currentIndex].state != State::BUSY) {
					currentIndex++;
				}
				if (currentIndex == table->getCapacity() || table->data[currentIndex].state != State::BUSY) {
					current = nullptr;
				}
				else {
					current = &table->data[currentIndex];
				}
			}
			return *this;
		}

		bool operator == (const Iterator& other) { // проверка равенства
			return table == other.table && current == other.current;
		}

		bool operator != (const Iterator& other) { // проверка неравенства
			return table == other.table && current != other.current;
		}
	};

	Iterator begin() {
		return Iterator(this, true);
	}
	Iterator end() {
		return Iterator(this, false);
	}
private:
	size_t hashcode(string key, size_t i = 0);
};

template<class ValueType>
Hashtable<ValueType>::Hashtable(size_t size) {
	size_t capacity = 1;
	size_t minCapacity = size * 2;
	while (capacity < minCapacity) {
		capacity *= 2;
	}
	data.clear();
	data.resize(capacity, Node());
}

template<class ValueType>
bool Hashtable<ValueType>::Insert(string key, ValueType value) {
	probes = 0;
	int pos = -1;
	size_t i = 0;
	size_t j;
	do {
		j = hashcode(key, i);
		if (data[j].state == State::DELETED && pos == -1) {
			pos = j;
		}
		else if (data[j].key == key && data[j].state == State::BUSY) {
			return false;
		}
		i++;
	} while (!(i == getCapacity() || data[j].state == State::FREE));
	if (i == getCapacity() && pos == -1) {
		probes = i;
		return false;
	}
	if (pos == -1) {
		pos = j;
	}
	data[pos] = Node(key, value);
	size++;
	probes = i;
	return true;
}

template<class ValueType>
bool Hashtable<ValueType>::Delete(string key) {
	probes = 0;
	size_t i = 0;
	size_t j;
	do {
		j = hashcode(key, i);
		if (data[j].key == key && data[j].state == State::BUSY) {
			size--;
			probes = i + 1;
			if (size == 0) {
				Clear();
				return true;
			}
			data[j].clear();
			data[j].state = State::DELETED;
			return true;
		}
		i++;
	} while (!(data[j].state == State::FREE || i == getCapacity()));
	probes = i;
	return false;
}

template<class ValueType>
void Hashtable<ValueType>::Show() {
	for (int i = 0; i < data.size(); i++) {
		cout << i << " ";
		switch (data[i].state) {
		case State::FREE: {
			cout << "f ";
			break;
		};
		case  State::BUSY: {
			cout << "b ";
			cout << data[i].key;
			break;
		};
		case State::DELETED: {
			cout << "d ";
			break;
		}
		}
		cout << endl;
	}
}

template<class ValueType>
ValueType Hashtable<ValueType>::Search(string key) {
	probes = 0;
	size_t i = 0;
	size_t j;
	do {
		j = hashcode(key, i);
		if (data[j].key == key && data[j].state == State::BUSY) {
			probes = i+1;
			return data[j].value;
		}
		i++;
	} while (!(data[j].state == State::FREE || i == getCapacity()));
	probes = i;
	throw exception("Ключ не существует!");
}

template<class ValueType>
size_t Hashtable<ValueType>::hashcode(string s, size_t probe) {
	int k = 31, mod = 1e13;
	unsigned long long m = 1;
	unsigned long long hash_result = 0;
	double A = (sqrt(5) - 1) / 2;
	for (int i = 0; i < s.size(); ++i) {
		int _s = s[i] - 'А' + 7;
		hash_result = hash_result * (int)pow(2, s.size());
		hash_result = (hash_result % mod + m * _s) % mod;
		m = (m * k) % mod;
	}
	double buf = A * hash_result;
	buf = buf - (size_t)buf;
	hash_result = ((size_t)(getCapacity() * buf) + probe) % getCapacity();
	cout << "Сгенерированный индекс: " << hash_result << endl;
	return hash_result;
}

template<class ValueType>
size_t Hashtable<ValueType>::getCapacity() {
	return data.size();
}

template<class ValueType>
size_t Hashtable<ValueType>::getSize() {
	return size;
}

template<class ValueType>
size_t Hashtable<ValueType>::getProbesForPrevOp() {
	return probes;
}

template<class ValueType>
bool Hashtable<ValueType>::IsEmpty() {
	return size == 0;
}

template<class ValueType>
void Hashtable<ValueType>::Clear() {
	for (int i = 0; i < getCapacity(); i++) {
		data[i].clear();
	}
	size = 0;
}
