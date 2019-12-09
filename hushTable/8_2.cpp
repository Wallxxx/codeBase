#include <iostream> // Тут обычная библиотека.
#include <string> // С этой библиотекой можно объявлять строки. Например: "string str;". Как int, только string xD
#include <vector> // Можно создавать вектора. Это тот же самый массив, только более удобен в использовании. 

/*"default_size" - начальный размер таблицы (указано в задании).
  "coefficient" - коэфицент заполнения таблицы (тоже в задании указано). Например,
  если из 10 ячеек заполненно 7, то всё норм. Когда становится заполнено 8, то надо уже расширяться.  */

#define default_size 8
#define coefficient 0.75

  /*Долго ломал голову, что это за рандомные числа. В интернете есть много решений данного задания, и у всех
	эти константы разные. Как оказалось (прочитал в википедии) это рандомное простое число (простое число - это
	число, которое делится без остатся только на 1 и на само себя. Например: 2, 3, 5, 7, 11, 13, 17, 19, 23, 29,
	31, 37, 41, 43, 47, 53, 59, 61...). Эти числа означают размер интервала между ячейками. Можно поиграться
	с этими числами.  */

#define firstConst 17
#define secondConst 19

using namespace std; // Пространство имён для вывода и ввода (coud << "Hi!";   cin >> value;).

/*Создаём таблицу с помощью класса. По сути таблица - это сама структура, которая находится внутри класса.
  А сам класс просто позволяет разграничить полномочия (public, protected и private). А все операторы и перегрузки
  можно и со структурой сделать. Но в примере кода, который вы скинули, написано именно так (почти так).  */

template<class T>
class HashTable
{
public:
	HashTable(size_t initial_size); // Конструктор, принимающий на вход начальный размер таблицы (дано в задании).
	~HashTable(); // Деструктор. Ниже описана функция для него. 

	/*Всевозможные дополнительные конструкторы и операторы присваивания. Не особо понимаю,
	  зачем нужны здесь. Без них итак всё хорошо, но в файле с семинара есть. Наверно для перестраховки.  */

	HashTable(const HashTable&) = delete;
	HashTable(HashTable&&) = delete;
	HashTable& operator = (const HashTable&) = delete;
	HashTable& operator = (HashTable&&) = delete;

	/*Булевы функции (возвращают true или false в зависимости от того, получилось ли добавить, удалить или найти
	  ключ).  */

	bool Add(const string& arg_key);
	bool Remove(const string& arg_key);
	bool Has(const string& arg_key);
private: // Всё, что описано в классе далее, доступно только в этом классе и его функциях. Извне вызвать не получится. 
	struct node // Структура, содержащая ключ и переменную "удалён ли он". Ну и конструктор. 
	{
		string key;
		bool isDelete; // Зачем это нужно написано в 35-ом слайде. 
		node(string arg_key) : key(arg_key), isDelete(false) {} // Конструктор структуры. 
	};
	vector<node*> table; // Вектор таблицы.
	void rehash();
	int tableSize; // Размер буфера (изначально 8, как в задании). 
	int size; // Кол-во ключей (записей) в таблице. 
	int firstHash(const string& arg_key, int size); // Просчёт первого хеширования...
	int secondHash(const string& arg_key, int size); // И второго. Подробнее ниже
};

/*Конструктор. Присваиваем переменным нужные значения. */

template<class T>
HashTable<T>::HashTable(size_t initial_size) : table(initial_size, nullptr),
tableSize(default_size), size(0) {}

template<class T>
HashTable<T>::~HashTable() // Удаляем всё. 
{
	for (int i = 0; i < table.size(); i++) delete table[i];
}

/*Добавление элемента в таблицу. Подробнее об алгоритме написано с 30-го слайда. Только
  тут ещё проверка на то, есть ли уже такой ключ.  */

template<class T>
bool HashTable<T>::Add(const std::string& arg_key)
{
	/*Делим кол-во элементов в таблице на её начальный размер. Если получается, что заполненно
	  3/4 или более таблицы, то нужно её расширить. */

	if (((double)size / (double)tableSize) >= coefficient) rehash();

	/*После делаем двойное хеширование. Считаем первый и второй хэши (пробы, если как в презентации). Подробнее
	  об их просчёте написано (пока что нет) ниже, в комментарии к функциям "firstHash()" и "secondHash()".  */

	int fHash = firstHash(arg_key, tableSize), sHash = secondHash(arg_key, tableSize);
	int i = 0, del = -1;
	while (table[fHash] != nullptr && i < tableSize)
	{
		if (table[fHash]->key == arg_key && !table[fHash]->isDelete) return false; // Такой ключ уже есть. 
		if (table[fHash]->isDelete && del < 0) del = fHash;
		fHash = (fHash + sHash) % tableSize;
		i++;
	}
	if (del < 0) table[fHash] = new node(arg_key);
	else
	{
		table[del]->key = arg_key;
		table[del]->isDelete = false;
	}
	size++;
	return true;
}

template<class T>
bool HashTable<T>::Remove(const string& arg_key)
{
	int fHash = firstHash(arg_key, tableSize), hsHash = secondHash(arg_key, tableSize);
	int i = 0;
	while (table[fHash] != nullptr && i < tableSize)
	{
		if (table[fHash]->key == arg_key && !table[fHash]->isDelete)
		{
			table[fHash]->isDelete = true;
			size--;
			return true;
		}
		fHash = (fHash + hsHash) % tableSize;
		i++;
	}
	return false;
}

template<class T>
bool HashTable<T>::Has(const string& arg_key)
{
	int fHash = firstHash(arg_key, tableSize), hsHash = secondHash(arg_key, tableSize);
	int i = 0;
	while (table[fHash] != nullptr && i < tableSize)
	{
		if (table[fHash]->key == arg_key && !table[fHash]->isDelete) return true;
		fHash = (fHash + hsHash) % tableSize;
		i++;
	}
	return false;
}

template<class T>
void HashTable<T>::rehash()
{
	int newtableSize = tableSize * 2;
	vector<node*> newtable(newtableSize, nullptr);
	for (int i = 0; i < tableSize; i++)
	{
		if (table[i] != nullptr && !table[i]->isDelete)
		{
			int fHash = firstHash(table[i]->key, newtableSize), hsHash = secondHash(table[i]->key, newtableSize);
			int j = 0;
			while (newtable[fHash] != nullptr && j < newtableSize)
			{
				fHash = (fHash + hsHash) % newtableSize;
				j++;
			}
			newtable[fHash] = table[i];
		}
		else delete table[i];
		table[i] = nullptr;
	}
	table = newtable;
	tableSize = newtableSize;
}

template<class T>
int HashTable<T>::firstHash(const string& arg_key, int size)
{
	int hash = 0;
	for (int i = arg_key.size(); i >= 0; i--) hash += (firstConst * hash + arg_key[i]) % size;
	return hash % size;
}

template<class T>
int HashTable<T>::secondHash(const string& arg_key, int size)
{
	int hash = 0;
	for (int i = arg_key.size(); i >= 0; i--) hash += (secondConst * hash + arg_key[i]) % size;
	return (2 * hash + 1) % size;
}

int main()
{
	HashTable<string> myTable(default_size); // Объявляем экземпляр класса.
	char command = ' '; // Переменная, в которую помещается только 1 символ. Она и будет содержать команду. 
	string key; // Ключик (строка).

	/*Теперь всё это вводим. Сначала "command" (1 симлов, ибо это тип данных char), а затем "key" (много символов,
	  так как это string. Закончить ввод можно пробелом. Получается, если написать "+Key", то первый символ уйдёт в command, ибо
	  он первый вводится в операторе "while", а всё, что в него не влезло, перейдёт в следующую переменную "key").*/

	while (cin >> command)
	{
		switch (command) // Разветвление событий. Если "command"...
		{
		case '?': // ... равно символу "?", то выполняем соответствующую команду. Аналогично с остальными. 
			cin >> key;
			cout << (myTable.Has(key) ? "OK" : "FAIL") << endl;
			break;
		case '+':
			cin >> key;
			cout << (myTable.Add(key) ? "OK" : "FAIL") << endl;
			break;
		case '-':
			cin >> key;
			cout << (myTable.Remove(key) ? "OK" : "FAIL") << endl;
			break;
		case '~': // Команда выхода. 
			return 0;
		}
	}
	return 0;
}