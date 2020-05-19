/*Заголовочный файл "reversePN.h
  Тут содержится класс и его функции (именно тут, а не в reverseRP.cpp, так как определение функций
  шаблонного класса должно быть в файле ".h" (таков язык). */

#pragma once // Защита от многоразового подключения заголовочного файла. 

/*Далее следуют инструкции препроцессора: */

#ifndef _IOSTREAM_ // Если библиотека iostream не подключена...
#include <iostream> // ...подключаем.
#endif

#include <cmath>

#ifndef _STRING_ // Если библиотека string не подключена...
#include <string> // ...подключаем.
#endif


#ifndef _VECTOR_ // Если библиотека vector не подключена...
#include <vector> // ...подключаем.
#endif

#define _REVERSEPN_ // Наименование нашей библиотеки для других классов. 

/*Далее идут так же инструкции препроцессора, которые отвечают за коды ошибок. */

#define _BUILDING_DATA_ERROR_ -10000 // Ошибка построения примера. Имеются символы, не относящиеся к математике, например. 
#define _DATA_TYPE_ERROR_ -10001 // Ошибка типа данных. Если класс создаётся типа "string" и аналогичных не числовых типов. 
#define _INVALID_NUMBER_OF_BRACKETS_ -10002 // Возникает, если в примере кол-ву открывающих скобок не соответствует кол-во закрывающих. 

#define _IOSTREAM_NOT_FOUND_ -20000 // Возникает, если не удалось подключить библиотеку iostream.
#define _STRING_NOT_FOUND_ -20001 // Если не удалось подключить string.
#define _VECTOR_NOT_FOUND_ -20002 // Если не удалось подключить vector.

using namespace std; // Пространство имён std.

/*Тут начинается определение класса. Он содержир дэфолтный конструктор и описанный ниже деструктор. 
  Функция "getAnswer" возвращает результат последнего посчитанного примера. А функция "calc" принимает ссылку на строку с примером,
  сохраняет её и начинает просчёт. 
  Структура "node" нужна для более удобной записи выходной строки. Она содержит поле value (тип определяет пользователь) и 
  operation (тип char, содержит символ операции). Так же имеются три конструктура. Первый: просто зануляет переменные; второй -
  принимает на вход значение и записывает его; третий - операцию. 
  Так же в классе есть приватные функции "str_to_dataType", "check_brackets_error" и "cleaning". Первая - преобразует все числа в строке
  в определённый тип данных (int, double или другой, какой будет указан при создании объекта класса). Вторая - проверяет кол-во открывающих
  и закрывающих скобок в строке. Третья - чистит временные переменные/массивы. */

template<typename _dataType> // "_dataType" - временное название типа данных. Компилятор сам его заменит на нужный. 
class ReversePolishNotation
{
public:
	ReversePolishNotation() {}; // Конструктор по умолчанию.
	~ReversePolishNotation(); // Деструктор.
	_dataType getAnswer(); // Функция, возвращающая результат последнего посчитанного примера.
	_dataType calc(string &input); // Функция, считающая выражение уже преобразованное в обратную польскую нотацию. 
	_dataType exp(); // Функция, возвращающая значение экспоненты.
	_dataType pi(); // Функция, возвращающая значение числа pi.
private:
	struct node
	{
		node() : value(NULL), operation('\0') {} // Конструктор по умолчанию.
		node(_dataType value) : value(value), operation('\0') {} // Конструктор со входным параметром value.
		node(char operation) : value(NULL), operation(operation) {} // Конструктор со входным параметром operator.
		_dataType value; // Переменные.
		char operation;
	};
	int str_to_dataType(); // Функция перевода входной строк в нужный типа данных и построение обратной польской нотации. 
	bool check_brackets_error(); // Функция проверки правильности скобок. 
	void cleaning(); // Отчищение временных переменных. 
	string input; // Копия входной строки. 
#ifdef _VECTOR_ // Если библиотека vector подключена...
	vector<node> output; // ...создаём вектор от структуры, который будет содержать обратную запись. 
#endif
	_dataType answer; // Переменная для хранения ответа. 
};

/*Функция получения результата последнего посчитанного примера. */

template<typename _dataType>
_dataType ReversePolishNotation<_dataType>::getAnswer()
{
	return answer; // Просто возвращает приватное поле answer. 
}

template<typename _dataType>
_dataType ReversePolishNotation<_dataType>::exp() // В зависимости от типа данных возвращает константу в нужном типе. 
{
	if (typeid(_dataType) == typeid(int)) return 2;
	else if (typeid(_dataType) == typeid(long)) return 2;
	else if (typeid(_dataType) == typeid(float)) return float(2.71828);
	else if (typeid(_dataType) == typeid(double)) return 2.71828182845904;
}

template<typename _dataType>
_dataType ReversePolishNotation<_dataType>::pi() // Аналогично функции экспоненты. 
{
	if (typeid(_dataType) == typeid(int)) return 3;
	else if (typeid(_dataType) == typeid(long)) return 3;
	else if (typeid(_dataType) == typeid(float)) return float(3.14159);
	else if (typeid(_dataType) == typeid(double)) return 3.1415926535897;
}

/*Далее следуют инструкции препроцессора. Если подключить iostream не получилось, то выводим сообщение
  об этом и код ошибки. При этом функции cleaning() и деструктор остаются пустые, так как записи в них 
  никакой не происходило. */

#ifndef _IOSTREAM_ 

template<typename _dataType>
_dataType ReversePolishNotation<_dataType>::calc(string &input) // Объявление функции.
{
	printf("\n\nError: _IOSTREAM_NOT_FOUND_(-20000)\n\n"); // Вывод сообщения об ошибки с кодом.
	return _IOSTREAM_NOT_FOUND_; // Возвращение кода ошибки. 
}

template<typename _dataType>
void ReversePolishNotation<_dataType>::cleaning()
{
	// Пустая функция отчистки. 
}

template<typename _dataType>
ReversePolishNotation<_dataType>::~ReversePolishNotation() { cleaning(); } // Деструктор, который вызывает пустую функцию. 

#endif

/*Аналогично для библиотеки string. */

#ifndef _STRING_

template<typename _dataType>
_dataType ReversePolishNotation<_dataType>::calc(string &input)
{
	printf("\n\nError: _STRING_NOT_FOUND_(-20001)\n\n");
	return _STRING_NOT_FOUND_;
}

template<typename _dataType>
void ReversePolishNotation<_dataType>::cleaning()
{

}

template<typename _dataType>
ReversePolishNotation<_dataType>::~ReversePolishNotation() { cleaning(); }

#endif

/*Аналогично для библиотеки vector. */

#ifndef _VECTOR_

template<typename _dataType>
_dataType ReversePolishNotation<_dataType>::calc(string &input)
{
	printf("\n\nError: _VECTOR_NOT_FOUND_(-20002)\n\n");
	return _VECTOR_NOT_FOUND_;
}

template<typename _dataType>
void ReversePolishNotation<_dataType>::cleaning()
{

}

template<typename _dataType>
ReversePolishNotation<_dataType>::~ReversePolishNotation() { cleaning(); }

#endif

/*Если все три необходимые библиотеки подключены, то определяем функции. */

#ifdef _VECTOR_
#ifdef _STRING_
#ifdef _IOSTREAM_



/*Функция подсчёта выражения, которое уже в обратной польской записи. */

template<typename _dataType>
_dataType ReversePolishNotation<_dataType>::calc(string &input)
{
	this->input = input; // Получаем копию строки с выражением, которую ввёл пользователь. 
	switch (str_to_dataType()) // Преобразовываем из string в нужный типа. Если в результате преобразования возникали ошибки, выводим их. 
	{
	case _DATA_TYPE_ERROR_: // Если в результате преобразования был введён неверный тип данных. 
		cout << "\n\nError: _DATA_TYPE_ERROR_(-10001)\n\n";
		return _DATA_TYPE_ERROR_;
	case _BUILDING_DATA_ERROR_: // Если была ошибка построения (есть описание выше, около определения макроса).
		cout << "\n\nError: _BUILDING_DATA_ERROR_(-10000)\n\n";
		return _BUILDING_DATA_ERROR_;
	case _INVALID_NUMBER_OF_BRACKETS_: // Если кол-во открывающих скобок не соответствует кол-ву закрывающих. 
		cout << "\n\nError: _INVALID_NUMBER_OF_BRACKETS_(-10002)\n\n";
		return _INVALID_NUMBER_OF_BRACKETS_;
	}
	int counter = 0; // Счётчик. Изначально равен нулю.

	/*Далее идёт цикл, которые ищет знаки операций и выполняет найденную операцию для двух значений перед собой. Стоит отметить, что
	  если выражение составлено првильно, то за пределы векторы мы не выйдет и перед знаком операции всегда будут идти два значения. */

	while (output.size() != 1) // Цикл выполняется, пока в выходной строке не осталось одно значение. Оно и будет ответом. 
	{
		switch (output[counter].operation) // Смотрим операцию. 
		{
		case '+': // Если это плюс, то на место операции и двух предыдущих значений ставим сумму этих значений...
			output.insert(output.begin() + counter - 2, output[counter - 1].value + output[counter - 2].value);
			for (int i = 0; i < 3; i++) output.erase(output.begin() + counter - 1); // ...удаляя сами значения. 
			counter = 0; // Счётчик возвращается в начало и снова проходим строку с нуля. 
			break;
		case '-': // Аналогично с вычитанием...
			output.insert(output.begin() + counter - 2, output[counter - 2].value - output[counter - 1].value);
			for (int i = 0; i < 3; i++) output.erase(output.begin() + counter - 1);
			counter = 0;
			break;
		case '*': // ...умнодением...
			output.insert(output.begin() + counter - 2, output[counter - 1].value * output[counter - 2].value);
			for (int i = 0; i < 3; i++) output.erase(output.begin() + counter - 1);
			counter = 0;
			break;
		case '/': // ...делением...
			output.insert(output.begin() + counter - 2, output[counter - 2].value / output[counter - 1].value);
			for (int i = 0; i < 3; i++) output.erase(output.begin() + counter - 1);
			counter = 0;
			break;
		case '^': // ...и возведением в степень. 
			output.insert(output.begin() + counter - 2, _dataType(pow(output[counter - 2].value, output[counter - 1].value)));
			for (int i = 0; i < 3; i++) output.erase(output.begin() + counter - 1);
			counter = 0;
			break;
		}
		counter++;
	}
	answer = output[0].value; // Записываем ответ в поле answer.
	cleaning(); // Отчищаем временные значения.
	return answer; // Выводим результат. 
}

/*Далее функция преобразования в числовой тип данных и построения записи. */

template<typename _dataType>
int ReversePolishNotation<_dataType>::str_to_dataType()
{
	string stack; // Стэк. 
	int start = -1; // Начальный индекс числа. 
	if (!check_brackets_error()) return _INVALID_NUMBER_OF_BRACKETS_; // Если кол-во скобок не соответствует, возвращаем ошибку.
	for (int i = 0; i <= input.size(); i++) // Проходим всю строку. 
	{
		if ((input[i] >= '0' && input[i] <= '9' || input[i] == '.')) // Если встретили цифру - запоминаем индекс начала числа. 
		{
			if (start == -1) start = i;
		}
		else // Иначе, если встретили не цифру и не точку...
		{
			if (start != -1) // ...то это конец числа. Преобразовываем его в тип, указанный пользователем. 
			{

				/*Функция substr делает срез строки от индекса начала числа, и берёт кол-во чисел, которые надо срезать от начала. 
				  Затем функция stoi или другая аналогичная переподит этот срез в численный типа данных. */

				_dataType value;
				if (typeid(_dataType) == typeid(int)) value = stoi(input.substr(start, i - start));
				else if (typeid(_dataType) == typeid(long)) value = stol(input.substr(start, i - start));
				else if (typeid(_dataType) == typeid(float)) value = stof(input.substr(start, i - start));
				else if (typeid(_dataType) == typeid(double)) value = stod(input.substr(start, i - start));
				else return _DATA_TYPE_ERROR_; // Если нужного типа данных нет, значит мы не сможем верно посчитать. Возвращаем ошибку.
				output.push_back(node(value)); // Добавляем теперь уже число (а не char символы из строки) в выходную строку. 
				start = -1; // Индекс начала числа возвращается в -1. Ждём следующее число. 
			}
			if (input[i] == '+' || input[i] == '-') // Если встретили плюс или минус...
			{
				if (stack.size() != NULL && (stack.back() == '+' || stack.back() == '-')) // ...и в стеке последняя операция тоже плюс или минус...
				{
					output.push_back(node(stack.back())); // ...то выталкиваем из стека последнюю операцию в выходную строку...
					stack.pop_back(); // ...и на её место...
					stack.push_back(input[i]); // ...записываем текущую встреченную операцию.
				}
				else if (stack.size() != NULL && (stack.back() == '*' || stack.back() == '/')) // Если в стеке последняя операция умножение или деление...
				{
					while (stack.size() != NULL) // Всё выталкиваем из стека в выходную строку...
					{
						output.push_back(node(stack.back()));
						stack.pop_back();
					}
					stack.push_back(input[i]); // ...и в стек добавляем текущий элемент. 
				}
				else stack.push_back(input[i]); // Иначе просто добавляем в стек встреченный плюс или минус. 
			}
			else if (input[i] == '*' || input[i] == '/') // Если встретили умножение или деление...
			{

				/*...и последний символ в стеке уже умножение, деление или возведение в степень, то из стека
				  выталкиваем последний символ в выходную строку, а на его место записываем текущий символ (операцию).  */

				if (stack.size() != NULL && (stack.back() == '*' || stack.back() == '/' || stack.back() == '^')) 
				{
					output.push_back(node(stack.back()));
					stack.pop_back();
					stack.push_back(input[i]);
				}
				else stack.push_back(input[i]); // Иначе просто добавляем его в стек. 
			}
			else if (input[i] == '^') // Если встретили возведение в степерь...
			{
				if (stack.size() != NULL && stack.back() == '^') // И если предыдущий символ в стеке такой же, то...
				{
					output.push_back(node(stack.back())); // ...меняем их местами как и выше с умножением/делением было. 
					stack.pop_back();
					stack.push_back(input[i]);
				}
				else stack.push_back(input[i]); // Иначе просто добавляем в стек. 
			}

			/*Далее идут константы. Если встретили символ "p", то просто записываем значение числа pi в выходную строку. 
			  Аналогично для экспоненты. */

			else if (input[i] == 'p' || input[i] == 'P') output.push_back(node(pi()));
			else if (input[i] == 'e' || input[i] == 'E') output.push_back(node(exp()));
			else if (input[i] == '(') { stack.push_back(input[i]); } // Если встретили открывающую скобку, добавляем её в стек. 
			else if (input[i] == ')') // Если встретили закрывающую...
			{
				while (stack.back() != '(' && stack.size() != NULL) // То выталкиваем всё, из стека в выходную строку, пока не встретим открывающую. 
				{
					output.push_back(node(stack.back()));
					stack.pop_back();
				}
				stack.pop_back();
			}
			else if (input[i] != ' ' && input[i] != '.' && input[i] != '\0') // Если встретили неизвестный символ - возвращаем ошибку. 
			{
				return _BUILDING_DATA_ERROR_;
			}
		}
	}
	for (int i = stack.size(); i > 0; i--) // Когда прошли всё строку, которую ввёл пользователь, оставшиеся символы из стека выталкиваем в выходную строку.
	{
		output.push_back(node(stack.back()));
		stack.pop_back();
	}
	return 0; // Если всё хорошо, возвращаем 0. 
}

/*Определение функции проверки кол-ва скобок.  */

template<typename _dataType>
bool ReversePolishNotation<_dataType>::check_brackets_error()
{
	int check = 0; // Переменная "баланса". Изначально равна 0. 
	for (auto i : input) // Проходим итератором по всей строке.
	{
		if (i == '(') check++; // Если встретили открывающую скобку - счётчик увеличиваем на один.
		else if (i == ')') check--; // Если закрывающую - уменьшаем на один. 
	}
	if (check == 0) return true; // В итоге если получилось 0, то кол-во открывающих скобок соответствует кол-ву закрывающих. 
	return false; // В противном случае не соответствует. 
}

template<typename _dataType>
ReversePolishNotation<_dataType>::~ReversePolishNotation() // Деструктор, который вызывает функцию отчистки. 
{
	cleaning();
}

template<typename _dataType>
void ReversePolishNotation<_dataType>::cleaning() // Функция отчистки. 
{
	output.clear(); // Чистим выходную строку...
	input.clear(); // ...и входную.

}

#endif
#endif
#endif
