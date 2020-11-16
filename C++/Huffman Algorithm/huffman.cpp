#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <list>
#include <filesystem> // Так проще получать пути расположения файлов и прочую информацию о них. 

typedef unsigned char byte;  // Теперь тип данных "unsigned char" можно называть короче - "byte".

using namespace std;

/*Структура "node". Содержит символ, количество его упоминаний в тексте, конструктор и функцию.  */

struct node
{
	byte symbol = NULL;
	unsigned long amount = 0;
	node *left = NULL, *right = NULL;
	node() : left(NULL), right(NULL), symbol(NULL), amount(0) {}
	node(node *newLeft, node *newRight)
	{
		left = newLeft;
		right = newRight;
		amount = newLeft->amount + newRight->amount;
	}
};

struct myCompare // Перегружает скобки. Позволяет сортировать узлы по параметру "amount".  
{
	bool operator()(const node *left, const node *right) const
	{
		return left->amount < right->amount; 
	}
};

map<byte, unsigned long> symbolsAmount; // Двусвязный список. Тут будут символы и их кол-во в тексте. 
list<node*> tree; // Структура узло. Будет строить дерево. 
node *root = NULL; // Указатель на корень дерева (верхушку).
vector<bool> way; // Вектор, содержащий пути от корня до разных букв. 
map<byte, vector<bool>> table; // Таблица в виде "буква - [её код]".
unsigned long strLen = 0; // Кол-во символов в оригинальном файле. Нушно при расшифровке. 
unsigned long symbCount = 0; // Кол-во различных символов в оригинальном файле. Нужно при расшифровке. 

/*Тут создаётся таблица. Обходим рекурсией дерево от корня. Если встречаем букву, записываем её код
  в table.  */

void createTable(node *temp)
{
	if (temp->left != NULL)
	{
		way.push_back(0); // Если пошли налево - добавляем в путь нолик.
		createTable(temp->left);
	}
	if (temp->right != NULL)
	{
		way.push_back(1); // Если направо - то единицу. 
		createTable(temp->right);
	}
	if (temp->left == NULL && temp->right == NULL) // Если слева и справа пусто - мы нашли символ. 
	{
		symbCount++; // +1 в копилку символов. 
		table[temp->symbol] = way; // И в таблицу записываем путь. 
	}
	if (way.size() == 0) return; // Если всё обошли - выходим. Иначе, двигаемся дальше. 
	way.pop_back();
}

/*Тут сохранение таблицы. Первое число - количество символов, которое должно быть в итоге. Второе - 
  кол-во разных символов, которые есть. Осталось цифру за цифрой записать это, а данные будем разделять
  символом "del", его код 127. Его не используют в обычных текстах. Да и вообще, почти нигде не используют. */

void saveTable(ofstream &compressed)
{
	while (strLen > 0)
	{
		compressed << strLen % 10; // Записываем младший разряд от числа...
		strLen /= 10; // И убираем этот разряд из числа. И так пока всё число не запишется.
	}
	compressed << char(127); // Ставим разделитель. 
	while (symbCount > 0) // Тут так же.
	{
		compressed << symbCount % 10;
		symbCount /= 10;
	}
	compressed << char(127);
	for (auto &temp : table) // Далее записываем букву и её код, так же после каждой буквы - разделитель. 
	{
		compressed << temp.first;
		for (int i = 0; i < temp.second.size(); i++) compressed << temp.second[i];
		compressed << char(127);
	}
}

/*А тут самое сложное. Загрузка этой таблицы из зашифрованного файла. Как мы помним, сначала первые два
  числа этого файла - это длина текста и кол-во различных символов. Так вот, первым делом их и загружаем.  */

void loadingTable(ifstream &compressed)
{
	node *counter = new node;
	root = counter;
	strLen = 0;
	symbCount = 0;
	byte buff;
	int i = 0;
	buff = compressed.get();
	while (buff != char(127)) // Читаем первую цифру и строим из неё число по формуле. Например, если нам надо...
	{ // ...надо считать число 437, то получается так: 7 * 1 + 3 * 10 + 4 * 100 = 7 + 30 + 400 = 437. 
		strLen += (int(buff) - 48) * pow(10, i);
		i++;
		buff = compressed.get();
	}
	i = 0;
	buff = compressed.get();
	while (buff != char(127)) // Аналогично. 
	{
		symbCount += (int(buff) - 48) * pow(10, i);
		i++;
		buff = compressed.get();
	}
	i = 0;
	while (i < symbCount) // Теперь читаем буквы. 
	{
		byte temp;
		buff = compressed.get(); // Берём букву. 
		temp = compressed.get(); // И её первый символ. 
		while(temp != char(127))
		{
			if (temp == '0') // Строим дерево. 
			{
				if (counter->left == NULL)
				{
					node *left = new node;
					counter->left = left;
					counter = counter->left;
				}
				else counter = counter->left;
			}
			else
			{
				if (counter->right == NULL)
				{
					node *right = new node;
					counter->right = right;
					counter = counter->right;
				}
				else counter = counter->right;
			}
			temp = compressed.get();
			if (temp == char(127)) // Если встретился разделитель - пора начинать записывать новую букву. 
			{
				counter->symbol = buff;
				counter = root;
			}
		}
		i++;
	}
}

void showTable() // Функция отображает дерево. Нужно было для отладки, но решил оставить на всякий случай. Просто я её нигде не вызываю. 
{
	for (auto &temp : table)
	{
		cout << temp.first << " : ";
		for (int i = 0; i < temp.second.size(); i++) cout << temp.second[i];
		cout << endl;
	}
}

void showTree(node *temp) // Аналогично с предыдущим. 
{
	if (temp->left != NULL) showTree(temp->left);
	if (temp->right != NULL) showTree(temp->right);
	if (temp->left == NULL && temp->right == NULL)
	{
		cout << temp->symbol << " ";
	}
}

void Encode(ifstream &original, ofstream &compressed) 
{
	vector<byte> str;
	byte value = 0;
	unsigned long bitsCounter = 0, iterationCounter = 0;

	while (!original.eof()) // Записываем всё из файла и при этом считаем каждую букву. 
	{
		byte temp = original.get();
		str.push_back(temp);
		symbolsAmount[temp]++;
	}
	strLen = str.size();
	for (auto &temp : symbolsAmount) // После создаём для каждой буквы узел. 
	{
		node *myNode = new node;
		myNode->symbol = temp.first;
		myNode->amount = temp.second;
		tree.push_back(myNode);
	}

	while (tree.size() > 1) // Теперь создаём дерево по алгоритму, что рисовал на занятии. 
	{
		tree.sort(myCompare());
		node *left = new node;
		left = tree.front();
		tree.pop_front();
		node *right = new node;
		right = tree.front();
		tree.pop_front();
		node *head = new node(left, right);
		tree.push_back(head);
	}

	root = tree.front(); // Теперь верхушка - наш корень. 

	createTable(root); // Создаём от него таблицу...

	//showTree(root);
	
	saveTable(compressed); // ...и сохраняем её.

	while (iterationCounter < str.size()) // Теперь каждую букву шифруем и отправляем в файл. 
	{
		vector<bool> temp = table[str[iterationCounter]];
		for (unsigned long i = 0; i < temp.size(); i++)
		{
			value |= temp[i] << (7 - bitsCounter);
			bitsCounter++;
			if (bitsCounter == 8)
			{
				bitsCounter = 0;
				compressed << value;
				value = 0;
			}
		}
		iterationCounter++;
	}

	/*Подчищаем всё.  */

	symbolsAmount.clear();
	tree.clear();
	way.clear();
	table.clear();
	root = NULL;
}

void Decode(ifstream &compressed, ofstream &original)
{
	loadingTable(compressed); // Загружаем таблицу из файла. 
	//showTree(root);
	byte value;
	unsigned long printCounter = 0, counter = 0;
	node *temp = root;
	value = compressed.get();
	while (true) // И по ней делаем рашифровку. Т.е. читаем байт, а в нём каждый бит, и проходим по таблице. 
	{
		byte b = value & (1 << (7 - counter));
		if (b)
		{
			temp = temp->right;
		}
		else
		{
			temp = temp->left;
		}
		if (temp->left == NULL && temp->right == NULL) // Если нашли букву, то записываем в файл и возвращаемся в начало дерева. 
		{
			if (printCounter == strLen - 1) break; // Так до тех пор, пока не запишем столько символов, сколько было в исходном файле. 
			original << temp->symbol;
			printCounter++;
			temp = root;
		}
		counter++;
		if (counter == 8)
		{
			counter = 0;
			value = compressed.get();
		}
	}
}

int main()
{
	setlocale(0, ""); // Подключаем русскую локализацию.

	/*Тут будут имена файлов. */

	string originalName;
	string compressedName;

	/*Тут пути файлов (C:\...).  */

	string absoluteFirstFile;
	string absoluteSecondFile;
	string cmdFcCommand;

	/*Эксперименты - путь к познанию чего-то нового xD
	  Шутка. Сюда будут записываться пути файлов до перевода их в нужный формат. Абсолютная ссылка.  
	  
	  В моей версии (2017) библиотека "filesystem" вроде ещё полностью не утверждена. А вроде и утверждена, точно не знаю, поэтому
	  приходится вызывать через "experimental" (находится в "std"). В новых версиях может быть уже можно и без этого, но это не точно.  */
	
	experimental::filesystem::path firstFile;
	experimental::filesystem::path secondFile;

	/*Потоки. Для шифровки и расшифровки.  */

	ifstream original;
	ofstream compressed;

	ofstream orig;
	ifstream compres;

	/*Далее меню.  */

	cout << "*Файлы сохраняются в папке проекта.\n";

	cout << "1 - Зашифровать.\n2 - Расшифровать.\n3 - Зашифровать и расшифровать.\n4 - сравнение двух файлов.\nЛюбое другое число/буква - выход.\n";

	int act;

	cin >> act;

	switch (act)
	{
	case 1:
		cout << "Введите название файла с оригинальным текстом (пример: original.txt): ";
		cin >> originalName; // Вводим имя файла. 
		original.open(originalName, ios::binary); // Открываем его.
		if (!original.is_open()) // Если файл открыть не удалось, сообщаем об этом. 
		{
			cout << "Ошибка открытия файла '" << originalName << "'. Файл не найден. \n";
			break;
		} // Если удалось открыть, то идём дальше. 
		cout << "Введите название файла, куда записывать зашифрованный текст (пример: compressed.txt): ";
		cin >> compressedName; // Берём имя второго файла.
		compressed.open(compressedName, ios::binary); // И так же открываем файл с этим именем. Если такого файла нет, то он создастся. 
		cout << "Шифровка...";
		Encode(original, compressed); // Вызываем функцию шифровки. 
		cout << " ...готово.\n";
		original.close(); // Закрываем файлы. 
		compressed.close();
		cout << "Готово. \n";
		break;
	case 2: // Тут аналогично первому. 
		cout << "Введите название файла с зашифрованным текстом текстом (пример: compressed.txt): ";
		cin >> compressedName;
		compres.open(compressedName, ios::binary);
		if (!compres.is_open())
		{
			cout << "Ошибка открытия файла '" << compressedName << "'. Файл не найден. \n";
			break;
		}
		cout << "Введите название файла, куда записывать расшифрованный текст (пример: original.txt): ";
		cin >> originalName;
		orig.open(originalName, ios::binary);
		cout << "Расшифровка...";
		Decode(compres, orig);
		cout << " ...готово.\n";
		compres.close();
		orig.close();
		cout << "Готово. \n";
		break;
	case 3: // Тут первое и второе вместе. 

		/*Берём имена файлов, открываем. Так же берём пути файлов. Запускаем функцию кодировки. После закрываем файлы.  */

		cout << "Введите название файла с оригинальным текстом (пример: original.txt): ";
		cin >> originalName;
		original.open(originalName, ios::binary);
		if (!original.is_open())
		{
			cout << "Ошибка открытия файла '" << originalName << "'. Файл не найден. \n";
			break;
		}
		firstFile = experimental::filesystem::absolute(originalName); // Ссылка на абсолютный путь берём тут...
		absoluteFirstFile = firstFile.u8string(); // ...и здесь конвертируем в utf8 строку. 
		cout << "Введите название файла, куда записывать зашифрованный текст (пример: compressed.txt): ";
		cin >> compressedName;
		compressed.open(compressedName, ios::binary);
		cout << "Шифровка...";
		Encode(original, compressed);
		cout << " ...готово.\n";
		original.close();
		compressed.close();

		/*Шифровка закончилась. Нужно теперь расшифровывать. Берём имя файла, в который будем записывать результат. 
		  Брять имя файла с шифровкой не имеет смысла, так как мы его уже знаем. Брали при выполнении предыдущего блока. 
		  Всё, что надо, взяли. Теперь расшифровываем. */

		cout << "Введите название файла, куда записывать расшифрованный текст (пример: original2.txt): ";
		cin >> originalName;
		secondFile = experimental::filesystem::absolute(originalName);
		absoluteSecondFile = secondFile.u8string();
		compres.open(compressedName, ios::binary);
		orig.open(originalName, ios::binary);
		cout << "Расшифровка...";
		Decode(compres, orig);
		cout << " ...готово.\n";

		/*Расшифровали. Вызываем консольную команду сравнения двух файлов.
		  P.S. "fc" - Сравнение двух файлов или двух наборов файлов и вывод различий между ними. Команда содержится в
		  самой windows, вызываем с помощью операции "system". */

		cout << "-----------Сравнение двух файлов (win cmd)-----------\n";
		cmdFcCommand = "fc /N " + absoluteFirstFile + " " + absoluteSecondFile;
		orig.close();
		compres.close();
		system(cmdFcCommand.c_str()); // Вот тут вызывается команда.  
		cout << "\nГотово. \n";
		break;
	case 4: // Тут аналогично концу предыдущего пункта. Берём два файла и сравниваем. 
		cout << "Введите имя первого файла (пример: exemple.txt): ";
		cin >> originalName;
		firstFile = experimental::filesystem::absolute(originalName);
		absoluteFirstFile = firstFile.u8string();
		cout << "Введите имя второго файла (пример: MUAHAHAHAHA.txt): ";
		cin >> compressedName;
		secondFile = experimental::filesystem::absolute(compressedName);
		absoluteSecondFile = secondFile.u8string();
		cout << "\n-----------Сравнение двух файлов (win cmd)-----------\n";
		cmdFcCommand = "fc /N " + absoluteFirstFile + " " + absoluteSecondFile;
		system(cmdFcCommand.c_str());
		cout << "\nГотово. \n";
	}
	return 0;
}
