#include <iostream>

using namespace std;

/*Функция "selectSupport". Находит опорный элемент. На вход принимает те же параметры, что и "partition".  */

void selectSupport(int mass[], int left, int right)
{

	/*Ставим левый, правый и средний элементы в порядке "средний-минимальный-максимальный" на позиции
	  "left, (left + right) / 2, right" соответственно. (Средний стоит слева, минимальный в центре, максимальный
	  справа)  */

	if (mass[(left + right) / 2] > mass[right]) swap(mass[(left + right) / 2], mass[right]);
	if (mass[left] < mass[(left + right) / 2]) swap(mass[left], mass[(left + right) / 2]);
	if (mass[left] > mass[right]) swap(mass[left], mass[right]);
}

/*Функция "partition". Проходит массив двумя итератарами слева направо.
  На вход принимает те же параметры, что и "statisticsSearch", только без k. */

int partition(int mass[], int left, int right)
{
	selectSupport(mass, left, right); // Ставим левый, средний и правый элементы на свои места (описано выше).
	int i = right; // i начинаем справо, как и j...
	int j = right; // ...так как в задании сказано сделать обходом с конца к началу.
	bool step = false; // Наличие шага. 

	while (j > left) // Пока j не дошло до начала...
	{
		while (mass[left] == mass[j] && j > left) // Ускоряем алгоритм, если элементы повторяются.
		{
			if (step) swap(mass[i--], mass[j--]); // Выполняется через раз (из-за step, которая постоянно меняется с true на false и обратно).
			else j--; // Двигаем j к началу.
			step = !step; // И инверсируем step (было false, стало true. Если было true, то стало false).
		}
		while (mass[j] < mass[left] && j > left) j--; // Пока всё верно, двигаем j дальше к началу. 
		while (mass[j] > mass[left] && j > left) swap(mass[j--], mass[i--]); // Если j-й элемент оказался больше левого, то меняем его местами с i-м элементом.
	}
	swap(mass[left], mass[i]); // Ну и после меняем левый и i-й элемент местами. 
	return i; // Возвращаем i.
}

/*Функция "statisticsSearch". Вызывается в функции "main" при выводе k-ой статистики. Данная функция на вход принимает
  массив чисел, позицию первого числа (изначально - 0), позицию последнего числа (размер массив минус единица) и номер статистики,
  которую нужно вывести. Модификация алгоритма быстрой сортировки.  */

int statisticsSearch(int mass[], int left, int right, int k)
{
	
	/*Дублируем значения left и right в соответствующие переменные, начальную позицию опорного элемента ставим на -1. 
	  Пока опорный элемент не равен нужной k-ой статистики, то берём опорный элемент с помощью функции "partition" и 
	  сравниваем: если он меньше k, то выбираем как опорный элемент тот, что правее. Иначе выбираем тот, что левее. И пока
	  позиция опорного элемента не равна k, повторяем. Когда наконец-то нашлась статистика, возвращаем элемент, который стоит на k позиции. */

	int secondLeft = left; 
	int secondRight = right;
	int supportPosition = -1;
	while (supportPosition != k)
	{
		supportPosition = partition(mass, secondLeft, secondRight);
		if (supportPosition < k) secondLeft = supportPosition + 1;
		else secondRight = supportPosition - 1;
	}
	return mass[k];
}

int main()
{
	int n, k; // Объявляем переменные n и k.
	cin >> n >> k; // Вводим их.
	int *mass = new int[n]; // Создаём массив размером n...
	for (int i = 0; i < n; i++) cin >> mass[i]; // ...и заполняем его. 
	cout << statisticsSearch(mass, 0, n - 1, k); // В завершении выводим эелемент, стоящий на k-й позиции в отсортированном массиве, который возвращает функция "statisticsSearch".
	delete[] mass; // Тут может быть синтаксическая ошибка, сорри. 
	return 0; // Выход. 
}
