#include <iostream>

/*Подключаем пространство имён "std". Там такие функции, как "cin" и "cout".  */

using namespace std;

/*Создаём шаблонный класс "tree". В нём публично доступны две функции и конструктор. 
  Только внутри этого класса (так как находятся в привате) доступны структура данных
  "node" и указатель "root". Структура - это как обычная переменная, только их в ней несколько. 
  Например, создадим экземпляр (или как-то так правильно говорится) структуры: 

  node element = new node;
  
  Теперь можно обратиться к этому элементу и заполнить его компоненты:
  
  element->value = 5;
  element->right = NULL; // Указатель в никуда.    */

template<typename T>
class tree
{
public: // То, что видно везде.
	tree(); // Объявление конструктора.
	void push(T value); // Функция добавления элемента в дерево.
	void show(); // Эта функция обходит все элементы в порядке pre-order.
private: // То, что будет далее, доступно только этому классу. 
	struct node // Создаём структуру и добавляем в неё...
	{
		T value; // ...переменную, которая будет хранить значение (ключ),...
		node *left; // ...указатель на левый узел,...
		node *right; // ...указатель на правый узел и...
		node *parent; // ...указатель на предыдущий (родитель).
		node() : left(NULL), right(NULL), parent(NULL) {} // Конструктор функции, изначально всё зануляет. 
	};
	node *root; // Указатель на верхушку (корень).
};

/*Далее будут идти тела функций.  */

/*Конструктор класса. Он вызывается автоматически, как только в какой-либо функции (в нашем
  случае - main) создаётся экземпляр класса (типа переменная, например, tree<int> mytree).
  Таким образом, указатель root автоматически отчищается от мусора.  */

template<typename T> 
tree<T>::tree() : root(NULL) {}

/*Функция добавления элемента в дерево. Обычное дерево: начальный узел, из него
  идут указатели на узлы справа и слева, а из них так же. */

template<typename T>
void tree<T>::push(T value)
{
	node *iterator = root; // Создаём итератор, чтобы его перемещать по узлам. 
	node *element = new node; // Выделяем память под новый узел и...
	element->value = value; // ...даём ему значение, которое передаётся в функцию (обычное число).
	if (iterator != NULL) // Если в дереве уже есть элементы, то: 
	{
		while (iterator != NULL) // Пока не пришли к концу, выполняем то, что ниже.
		{

			/*Если рассматриваемого узла больше, чем значение добавляемого, то нам нужно влево. 
			  Проверяем, есть ли там элемент. Если есть, двигаемся левее и начинаем цикл "while" заново,
			  проверяя следующий узел. Если слева ничего нет, то добавляем туда узел.  */

			if (iterator->value > value)
			{
				if (iterator->left != NULL) iterator = iterator->left;
				else
				{
					iterator->left = element; // Указатель "left" от сравниваемого элемента теперь указывает на добавляемый.
					element->parent = iterator; // У добавленного указатель "parent" теперь указывает на сравниваемый. 
					break; // Выходим из цикла. 
				}
			}

			/*Если же рассматриваемый элемент меньше или равен добавляемому, то нужно вправо. В принципе, тут всё
			  тоже самое, что и выше, только "left" везде заменено на "right".  */

			else
			{
				if (iterator->right != NULL) iterator = iterator->right;
				else
				{
					iterator->right = element;
					element->parent = iterator;
					break;
				}
			}
		}
	}
	else root = element; // Иначе (если в дереве ещё ничего нет), "root" будет указывать на добавляемый элемент. 
}

/*Функция "show" выводит элемент в порядке pre-order и заодно подчищает за собой память (уже 
  рассмотренные элементы удаляет).  */

template<typename T>
void tree<T>::show()
{
	cout << root->value << " "; // Выводим корень. 
	node *iterator = root; // Ставим итератор в начало дерева.
	while (root->left != NULL || root->right != NULL) // Пока слева и справа от корня ещё что-то есть:
	{
		if (iterator->left != NULL) // Если слева ещё что-то есть, выводим левый элемент и переходим в него. 
		{
			cout << iterator->left->value << " ";
			iterator = iterator->left;
		}
		else // Иначе: 
		{
			if (iterator->right != NULL) // Если справа ещё что-то есть, то выводим правый элемент и переходим в него. 
			{
				cout << iterator->right->value << " ";
				iterator = iterator->right;
			}
			else // Если и справа ничего нет, то...
			{
				if (iterator == iterator->parent->left) // ...ищем, слева мы или справа от предыдущего. Если слева, то: 
				{
					iterator = iterator->parent; // Переходим в предыдущий узел...
					delete iterator->left; // ...левый его узел удаляем и...
					iterator->left = NULL; // ...зануляем указатель на него, чтоб мусора в нём не было. 
				}
				else // Если мы справа от предыдущего, то всё тоже самое, только вместо "left" идёт "right".
				{
					iterator = iterator->parent;
					delete iterator->right;
					iterator->right = NULL;
				}
			}
		}
	}
	delete iterator; // Удаляем верхушку. 
}


int main() // Основная функция. 
{
	/*Создаём экземпляр класса, даём ему имя и указывает тип данных. Пришлось выбрать "long long", так как в задании
	  сказано, что число может быть [-2^31, 2^31]. Квадратные скобки, значит включительно. А вот у типа "integer"
	  диапазон [-2^31, 2^31). Получается, если пользователь введёт число 2 147 483 648‬, то программа будет работать 
	  некорректно, ведь в "integer" максимальное число - 2 147 483 647. Поэтому используем "long long".  */

	tree<long long> mytree; 
	int n; // А вот 10^6 спокойно поместится в "int", поэтому для кол-ва элементов будем использовать его. 
	long long a;
	cin >> n; // Вводим кол-во элементов и...
	for (int i = 0; i < n; i++) // добавляем сами элементы в дерево. 
	{
		cin >> a;
		mytree.push(a);
	}
	mytree.show(); // Затем выводим его на экран. Удалять уже ничего не нужно, "show" всё подчистила. 
	return 0; // Выход. 
}
