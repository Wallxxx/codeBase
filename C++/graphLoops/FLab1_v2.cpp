/*F - фамилия преподавателя. 
  Lab1 - первая лабораторная работа. 
  v2 - исправлены ошибки первой версии. Эта же и оказалось последняя версия. */

#include "pch.h"
#include <iostream>
#include <vector>

using namespace std;

/*Объявляем переменные и вектора, которые нам понадобятся позже. Почему именно здесь? 
  Чтобы было видно везде и всем.
  from - обозначает номер вершины, от которой мы пойдем в другую. 
  k и n - вспомогательные переменные для занесения и удаления ребер. */

int arcs, tops; 
int from = 0, n = 0, k = 0;
vector <int> color; // Цвета вершин. 

/*Создаём структуру, от которой позже будем создавать вектор. Она нужна для того, чтобы в int векторе,
  например, можно было в одну ячейку поместить два значения, а не только одно. По сути, так можно и строки
  в одну ячейку вмещать, и всё, что угодно. Нам же угодно две цифры в одну ячейку - номер вершины, и номер, куда ведёт
  дуга из этой вершины.  */

struct MyVector
{
	int vertex_number;
	int connection_number;
};

vector <MyVector> buff; // Объявляем вектор, в котором будем хранить значения, чтобы не потерять.

/*Начинаем с определённой вершины. Красим её в 1, мол здесь побывали. Затем смотрим, куда можно из неё идти, при
  этом сохраняем эту точку и вершину, с которой она связана, в буфер. Если в связанной вершине ещё не были, то заходим
  туда (рекурсия). Таким образом проходим весь граф и выявляем циклы, а затем выводим их. */

void cycle_search(vector<MyVector> digraph, int v)
{
	n++; // Счётчик. 
	color[v] = 1; // Красим вершину, в которой сейчас. 
	for (int i = 0; i < arcs; i++) // Начинаем искать данную вершину в списке дуг...
	{
		if (digraph[i].vertex_number == v) // ...когда нашли, сохраняем в буфер и смотрим, куда можно пойти из неё. 
		{
			buff[n].vertex_number = digraph[i].vertex_number;
			buff[n].connection_number = digraph[i].connection_number;
			if (color[digraph[i].connection_number] == 0) // Если в вершине, в которую можно пойти из вершины V мы ещё не были...
			{
				from = v;
				cycle_search(digraph, digraph[i].connection_number); // ...переходим в неё. 
				while (buff[n].connection_number != v)
				{
					if (n > 0) n--;
				}
				n++;
			}
			else if (color[digraph[i].connection_number] == 1) // Если были в вершине, с которой можно содиниться...
			{

				k = n;
				while (buff[k].vertex_number != digraph[i].connection_number) // ...то проходим до пути (циклу в графе), попутно выводя его. 
				{
					if (k > 0) 
					{
						cout << buff[k].vertex_number << " -> " << buff[k].connection_number << "   ";
						k--;
					}
				}
				cout << buff[k].vertex_number << " -> " << buff[k].connection_number << "   " << endl;
				n--;

			}
		}
	}
	if (v == 0) color[v] = 2; // если эта вершина ещё не закрашена - красим в 2 и не рассматриваем больше. 
	else // Иначе...
	{
		if (color[from] == 1) color[v] = 0; // ...если в ней просто были, но цикл пока не увидели, то красим в 0, чтобы рассматривать снова позже. 
		else color[v] = 2; // Иначе, красим эту вершину в 2 и больше не рассматриваем. 
	}
}

int main()
{
	setlocale(0, ""); // Русская локализация. 
	vector <MyVector> digraph; // Объявляем экземпляр класса. 
	MyVector *info = new MyVector(); // А тут объявляем экземпляр структуры. 
	cout << "Введите количество вершин: ";
	cin >> tops;
	cout << "Введите количество дуг: ";
	cin >> arcs;
	for (int i = 0; i < arcs; i++)
	{
		/*Тут заполняем созданную структура и добавляем её в вектор. Затем снова и снова, пока не
		  запишем все дуги.  */

		cin >> info->vertex_number;
		cin >> info->connection_number;
		digraph.push_back(*info);
	}
	for (int i = 0; i < arcs; i++)
	{
		/*Тут заполняем буфер нулями.  */

		info->connection_number = 0;
		info->vertex_number = 0;
		buff.push_back(*info);
	}
	for (int i = 0; i < tops; i++)
	{
		color.push_back(0); // Цвета вершин тоже заполняем нулями. 
	}
	cout << "Ответ: \n";
	for (int i = 0; i < tops; i++)
	{
		if (color[i] == 0) cycle_search(digraph, i); // Затем обходим вершины, в которых ещё не были (с нулевой). 
	}
	cout << endl;
	system("pause");
	return 0;
}
