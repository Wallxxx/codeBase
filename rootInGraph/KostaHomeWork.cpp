#include "pch.h"
#include <conio.h>
#include <iostream>
#include <vector>

using namespace std;

/*Объявляем переменные и булевы вектора, которые нам понадобятся позже. Почему именно
  здесь? Чтобы было видно везде и всем. */

int tops, arcs;
vector<bool> nodes;
vector<bool> check;

/*Создаём структуру, от которой позже будет создавать вектор. Она нужна для того, чтобы в int векторе,
  например, можно было в одну ячейку поместить два значения, а не только одно. По сути, так можно и строки
  в одну ячейку вмещать, и всё, что угодно. Нам же угодно две цифры в одну ячейку - номер вершины, и номер, куда ведёт 
  дуга из этой вершины.  */

struct MyVector
{
	int vertex_number;
	int connection_number;
};

/*Тут функция, которая делает обход. Она начинает с указанной точки (в нашем случае с предпологаемого корня) и идёт 
  везде, где можно. Если у неё получилось пройти с указаной точки по всем остальным, то данная вершина становится не 
  предпологаемым корнем, а уверенным корнем. (Корень - вершина, в которую нельзя прийти, но из которой можно пройти по всем
  остальным вершинам. Если в вершину просто ничего не приходит, это ещё не значит, что это корень. Поэтому нужна эта функция. 
  Допустим, вот пример: 
  6      <- кол-во вершин
  7      <- кол-во дуг
  0 1    <- из 0 можно прийти в 1
  1 2    <- из 1 можно в 2 и т.д....
  1 3
  2 3
  4 0
  5 4
  5 2
  В вершину 5 никак не прийти (см. второй столбец), значит - это предпологаемый корень. Проверим, можно ли из него
  добраться во все остальные вершины. Вот эта функция и делает эту проверку. Получает на вход вектор с графом и стартовую
  вершину.  */

void search_root(vector<MyVector> digraph, int start)
{
	for (int i = 0; i < arcs; i++) 
	{
		if (digraph[i].vertex_number == start) // Ищем нужную нам вершину...
		{
			nodes[digraph[start].connection_number] = true;  // ...ставим, что туда, куда она ведёт, мы пришли.
			search_root(digraph, digraph[start].connection_number);  // Переходим туда, куда она ведёт посредством рекурсии. 
		}
	}
}

/*Далее начинается основная функция.  */

int main()
{
	setlocale(0, ""); // Русская локализация. 
	vector <MyVector> digraph; // Объявляем экземпляр класса. 
	MyVector *info = new MyVector(); // А тут объявляем экземпляр структуры. 
	int counter = 0, top, save; // Доп. переменные. 
	cout << "Введите количество вершин: ";
	cin >> tops;
	cout << "Введите количество дуг: ";
	cin >> arcs;

	/*Тут зануляем булевы вектора вершин, ибо мы пока обход не начинали и ещё нигде не были.  */

	for (int i = 0; i < tops; i++)
	{
		nodes.push_back(false);
		check.push_back(false);
	}

	cout << "Вводите номера вершин, которые связаны. (P.S. Запись '1 3' \n  означает, что из вершины 1 можно попасть в вершину\n  3, но из 3 нельзя попасть в 1)\n";

	for (int i = 0; i < arcs; i++)
	{

		/*Тут заполняем созданную структура и добавляем её в вектор. Затем снова и снова, пока не 
		  запишем все дуги.  */

		cin >> info->vertex_number;
		cin >> info->connection_number;
		digraph.push_back(*info);
	}

	/*А тут ищем предполагаемый корень (в какую вершину прийти нельзя).  */

	for (int i = 0; i < tops; i++)
	{
		check[digraph[i].connection_number] = true;
	}

	/*Затем ищем эту вершину уже в булевом векторе и запускаем от неё обход. После 
	  проверяем, если мы побывали после обхода во всех узлах - то выводим номер той вершины, 
	  с которой начинали обход, ибо теперь это не предполагаемый корень, а просто корень 
	  (уверенный). Если после обхода никуда не добрались -> вершина, с которой начинали - не корень. 
	  Обходим следующую вершину, которая может быть корнем (если есть более одного предполагаемого
	  корня). Если из всех предполагаемых корней по всем вершинам пройтись не получилось, то у данного
	  ациклического орграфа корней нет. 
	  Вот пример: 
	  8
      9
  	  0 3
	  0 5
	  1 3
	  1 6
	  2 5
	  3 7
	  5 4
	  5 7
	  5 6
	  У данного орграфа нет корней. У него есть три предполагаемых корня, но ни из одного из этих предполагаемых
	  корней нельзя обойти все оставшиеся вершины. Соответственно, у него нет корней. */

	for (int i = 0; i < tops; i++)
	{
		if (!check[i])
		{
			nodes[i] = true; // Вершина i - предполагаемый корень. 
			top = i; // Сохраняем её значение, на случай, если будут другие корни предполагаемые, но этот экажется верным. 
			search_root(digraph, i); // Обходим все вершины, начиная с корня. 
			for (int q = 0; q < tops; q++)
			{

				/*Если получилось обойти все вершины - это корень. Если не получилось, пробуем с другими
				  предполагаемыми корнями (если они есть). Ну и отчищаем после себя посещённые вершины.  */

				if (nodes[q])
				{
					counter++;
					nodes[q] = false;
				}

			}
			if (counter == tops) cout << "Корнем графа является вершина с номером " << top << endl;
			save = counter; // Сохраняем это значение, чтобы потом лишний раз не выводилось, что есть корень и чтобы выводилось, если его нет. 
			counter = 0;

		}
	}
	if (save != tops) cout << "У данного графа нет корня. \n";
	cout << "\nНажмите любую клавишу для выхода. ";
	_getch(); // Пауза до тех пор, пока не будет что либо нажато. 
}
