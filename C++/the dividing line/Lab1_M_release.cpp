/*Lab1_M_release: основано на геометрии машинного обучения. 
  Создание и обучение перцептрона. Источники:
  https://ru.wikipedia.org/wiki/Перцептрон
  https://habr.com/ru/post/324736/  */

/*Lab1 - Лабораторная работа 1
  M - Первая буква фамилии преподавателя
  release - итоговая версия с косметикой/гримом/мейкапом/небом/Аллахом/etc, читаемым (но это не точно)
    кодом и кучей полезных (наверно) комментариев.  */

#include "stdafx.h"
#include "Lab1_M_release.h" 
#include <vector> // Необходим для хранения точек. Контейнерный класс.

#include <CommCtrl.h> // Библиотека, необходимая для работы StatusBar.
#pragma comment(lib,"ComCtl32.Lib") // Тоже нужно для StatusBar'a.

#define MAX_LOADSTRING 100

using namespace std;

// Глобальные переменные:

HINSTANCE hInst;                                // текущий экземпляр
WCHAR szTitle[MAX_LOADSTRING];                  // Текст строки заголовка
WCHAR szWindowClass[MAX_LOADSTRING];            // имя класса главного окна

// Отправить объявления функций, включенных в этот модуль кода:

ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);

/*Переменные, одна из которых содержит код цвета в RGB, а другая содержит координаты
  X и Y точки, которую будут ставить на экран. Так же colorPixel будет отвечать за цвет точки
  и её классификацию (синяя/красная, -1/1).  */

COLORREF colorPixel = RGB(0, 0, 255);
POINT Coordinates = { 0 };

/*Здесь объявляем о создании StatusBar'a, а так же обозначаем переменные, которые будут хранить
  время выполнения алгорится, количество итераций и диалоговые сообщения. Статистика, грубо говоря. */

HWND StatusBar;
int time;
int it;
TCHAR Time1[50] = L"Время выполнения = ";
TCHAR Time2[50] = L" мс.";
TCHAR Time3[50] = L"Время выполнения < ";
TCHAR Iterations[50] = L"Количество итераций - ";
TCHAR EndProg[50] = L"Готово. ";
TCHAR NoLine[50] = L"Разделяющая прямая не найдена. ";
TCHAR TCoordinatesXY[50]; // Содержит сообщение для пользователя о координатах курсора в момент времени.
TCHAR Buff[50];

bool reload = false; // Так как каждый раз программу нужно перезапускать во избежании ошибок, делаем это. 

/*В данной программе уравнение прямой имеет вид "ax + by + c = 0". Соответственно
  переменные "a" и "b" - это вектор нормали, перпендикулярной прямой. Назовём "a" и "b" - 
  веса. Изначально веса инициализируются случайным образом (пусть так, как указано снизу. Это не
  имеет особо значения). Однако на каждом шаге обучения для каждой точки весы пересчитываются. */

float a = 1.1;
float b = -0.5;
int c = 20;
int mismatch = 0, prediction_label;

/*Структура нужна для более удобного хранения информации о точках в векторе.  */

struct points
{
	float x;
	float y;
	int label;
};

points _point; // Объявляется структура...
vector<points> VPoints; // ...и от неё создаётся вектор точек.

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // Инициализация глобальных строк

    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_LAB1MRELEASE, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Выполнить инициализацию приложения:

    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_LAB1MRELEASE));
    MSG msg;

    // Цикл основного сообщения:

    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}

ATOM MyRegisterClass(HINSTANCE hInstance) // Данная функция регистрирует класс окна. 
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_LAB1MRELEASE));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_LAB1MRELEASE);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow) // Сохраняет маркер экземпляра и создает главное окно
{
   hInst = hInstance; // Сохранить маркер экземпляра в глобальной переменной

   /*Создание главного окна с указанными параметрами. */

   HWND hWnd = CreateWindowW(szWindowClass, TEXT("Lab1_M - release 11.10.2019"), 
	   WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX, // <-- делает размер окна фиксированным, блокирует изменения размера.
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   /*Создание верхнего горизонтального меню и всплывающего Popup-меню.*/

   HMENU MyMenu;
   HMENU MyPopupMenu;
   HMENU Tools;
   MyMenu = CreateMenu();
   MyPopupMenu = CreatePopupMenu();
   Tools = CreatePopupMenu();

   AppendMenu(MyMenu, MF_STRING | MF_POPUP, (UINT)MyPopupMenu, L"Меню");
   AppendMenu(MyMenu, MF_STRING | MF_POPUP, (UINT)Tools, L"Инструменты");

   AppendMenu(MyPopupMenu, MF_STRING, IDM_ABOUT, L"О программе");
   AppendMenu(MyPopupMenu, MF_STRING, IDM_EXIT, L"Выход");

   AppendMenu(Tools, MF_STRING, 100, L"Синий");
   AppendMenu(Tools, MF_STRING, 101, L"Красный");
   AppendMenu(Tools, MF_STRING, 777, L"Начать алгоритм");

   SetMenu(hWnd, MyMenu);
   SetMenu(hWnd, MyPopupMenu);

   /*Создаём StatusBar (полосочка внизу программы, строка состояния). Затем делим её на 3 части
     и добавляем модификаторы, такие как "видимость", "без борьеров" (минимализм - моё всё) и другие.
	 Так же в эти части statusbar'a можно передавать текст. Как раз это и будем делать.  */

   StatusBar = CreateStatusWindow(WS_CHILD | WS_VISIBLE | SBT_NOBORDERS, L"Test", hWnd, NULL);
   int pParts[6];
   pParts[0] = 200;
   pParts[1] = 400;
   pParts[2] = 600;
   pParts[3] = 1230; // Так как размер окна не изменяется, эти 5 значений можно спокойно просчитать. 
   pParts[4] = 1330;
   pParts[5] = 1430;
   SendMessage(StatusBar, SB_SETPARTS, 6, (LPARAM)&pParts);
   SendMessage(StatusBar, SB_SETTEXT, 0 | SBT_NOBORDERS, (LPARAM)L"Цвет точки: синий.");
   SendMessage(StatusBar, SB_SETTEXT, 1 | SBT_NOBORDERS, (LPARAM)L" ");
   SendMessage(StatusBar, SB_SETTEXT, 2 | SBT_NOBORDERS, (LPARAM)L" ");
   SendMessage(StatusBar, SB_SETTEXT, 3 | SBT_NOBORDERS, (LPARAM)L" ");
   SendMessage(StatusBar, SB_SETTEXT, 4 | SBT_NOBORDERS, (LPARAM)L" ");
   SendMessage(StatusBar, SB_SETTEXT, 5 | SBT_NOBORDERS, (LPARAM)L" ");

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) // Обрабатывает сообщения в главном окне.
{
    switch (message)
    {
    case WM_COMMAND: // Обработать меню приложения.
        {
            int wmId = LOWORD(wParam);
			HDC hdc = GetDC(hWnd);
            switch (wmId)
            {
			case 100: // Если была нажата кнопка с ID = 100:
				colorPixel = RGB(0, 0, 255); // Ставим синий цвет.
				SendMessage(StatusBar, SB_SETTEXT, 0 | SBT_NOBORDERS, (LPARAM)L"Цвет точки: синий."); // Пишем в StatusBar о том, какой сейчас цвет.
				break;
			case 101: // Если была нажата кнопка с ID = 101:
				colorPixel = RGB(255, 0, 0); // Ставим красный цвет. 
				SendMessage(StatusBar, SB_SETTEXT, 0 | SBT_NOBORDERS, (LPARAM)L"Цвет точки: красный."); // Пишем в StatusBar о том, какой сейчас цвет.
				break;
			case 777: // Если была нажата кнопка с ID = 777:

				if (reload)
				{
					MessageBox(hWnd, TEXT("Перед повторным запуском алгоритма необходимо перезапустить\n"
						"программу. К сожалению, это единственный способ сброса данных. "),
						TEXT("Lab1_M - Ошибка"), 0);
					break;
				}
				else reload = true;
				it = 0; // Счётчик итераций,
				time = GetTickCount(); // сохраняет стартовое время.

				/*На данном этапе начинается обучение перцептрона. Даётся на обучение максимум
				  1000 итераций. Если управится раньше - закончит раньше. */

				for (int q = 0; q < 10000; q++) // ограничение на итерации
				{
					it++;
					mismatch = 0; // Количество коррекций. 
					for (int i = 0; i < VPoints.size(); i++) // Начинаем обход всех точек. 
					{

						/*Вычисляем предсказание. Что это? Как выше сказано, "a" и "b" - это вектор нормали, 
						  перпендикулярной прямой. Таким образом, подставив в "ax + by + c" координаты точки,
						  мы получим некое число. Если оно будет меньше 0 - то точка лежит, условно, слева от прямой.
						  Если больше - справа (опять же, условно). Чем больше значение отличается от 0, тем дальше лежит
						  точка, но это нам уже не нужно. 
						  Считаем предсказание и записываем его в переменную prediction_label. */

						if ((a * VPoints[i].x + b * VPoints[i].y + c) < 0) prediction_label = -1;
						else prediction_label = 1;

						/*Если предсказание сходится с реальной классификацией точки (синяя - 1, красная - -1), то всё ок для
						  этой точки. Если не сходится, то прямая уже не подходит. Её необходимо скорректировать...  */

						if (VPoints[i].label != prediction_label)
						{

							/*...для этого к "a" прибавляем произведение "x" координаты на реальный класс точки (-1 или 1).
							  Соответственно и с "b". А вот для "c" уже другая история. 
							  К "с" добавляем просто классификацию точки. Однако, так как у нас площадь поля большая (координаты
							  точки могут быть вплоть до x = 1500, y = 1000), необходимо к "с" добавить некий коэф.. Этот коэф. - 
							  константа, найденная обычным перебором. */

							a = a + (VPoints[i].x * VPoints[i].label);
							b = b + (VPoints[i].y * VPoints[i].label);
							c = c + VPoints[i].label * 60000;
							mismatch++; // Считаем кол-во коррекций для статистики. 
						}
					}
					if (mismatch == 0) // Если прошли все точки и коррекций ещё не было на данной итерации, ни одной, то:
					{

						/*Рисуем прямую и заканчиваем цикл. Готово :)
						  Перцептрон обучен и найдена разделяющая прямая. */

						/*P.S. Как раз для функции MoveToEx нам нужно было домнажать "c" на такой большой коэф.. Видно, что
						  при выполнении операции -c/b число "b" - довольно велико, ибо координаты точки могут быть "1000+", и
						  при делении "c" на это большое число получится 0. Таким образом, алгоритм не будет работать.  */

						MoveToEx(hdc, 0, -c / b, NULL); 
						LineTo(hdc, 2000, -(a * 2000 + c) / b);
						SendMessage(StatusBar, SB_SETTEXT, 3 | SBT_NOBORDERS, (LPARAM)EndProg); // Пишет пользователю, что прямая найдена. 
						break;
					}
					/*Если пришлю сюда, где этот комментарий - значит пока что на q итерации была хотя бы одна корректировка, 
					  что означает неверно построенную разделяющую прямую. Компьютер будет продолжать исправляться и корректироваться до
					  тех пор, пока не придёт к правильному решению (ну или пока не закончится лимит итераций).  */
				}

				/*Далее работа со строками и вывод их в StatusBar для пользователя:  */

				if (GetTickCount() - time == 0)
				{
					swprintf_s(Buff, _T("%i"), 1);
					wcscat_s(Time3, Buff); // Объединяет TCHAR-строки Time3 и Buff, сохраняя результат в Time3.
					wcscat_s(Time3, Time2); // Объединяет TCHAR-строки Time3 и Time2, сохраняя результат в Time3.
					SendMessage(StatusBar, SB_SETTEXT, 1 | SBT_NOBORDERS, (LPARAM)Time3); // Отправляет StatusBar'y строку Time3.
				}
				else
				{
					swprintf_s(Buff, _T("%i"), GetTickCount() - time); // Переводит время из int в TCHAR и сохраняет в Buff.
					wcscat_s(Time1, Buff); // Объединяет TCHAR-строки Time1 и Buff, сохраняя результат в Time1.
					wcscat_s(Time1, Time2); // Объединяет TCHAR-строки Time1 и Time2, сохраняя результат в Time1.
					SendMessage(StatusBar, SB_SETTEXT, 1 | SBT_NOBORDERS, (LPARAM)Time1); // Отправляет StatusBar'y строку Time1.
				}
				swprintf_s(Buff, _T("%i"), it); // После переводим кол-во итераций из int в TCHAR
				wcscat_s(Iterations, Buff); // Соединяем две строки...
				SendMessage(StatusBar, SB_SETTEXT, 2 | SBT_NOBORDERS, (LPARAM)Iterations); // ...PROFIT! Выводим
				if (it > 999) SendMessage(StatusBar, SB_SETTEXT, 3 | SBT_NOBORDERS, (LPARAM)NoLine); // NEPROFIT(( - если нет разделяющей прямой. 
				break;
            case IDM_ABOUT: // Кнопочка "О программе".
				MessageBox(hWnd, TEXT("Lab1_M - релиз от 11.10.2019 (сборка 398). Реализовано на C++17\n\nУшаков М.Е. - БСБО-06-17\n"
					"Основано на: 'Геометрия машинного обучения'\n\nИсточники:\nhttps://habr.com/ru/post/324736/"
					"\nhttps://ru.wikipedia.org/wiki/Перцептрон"),
					TEXT("О программе"), 0);
                break;
            case IDM_EXIT: // Кнопка выхода из программы.
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
	case WM_SIZE: // Если размер окошка изменился...
		SendMessage(StatusBar, WM_SIZE, 0, 0); // ...сообщаем об этом StatusBar'y и он растягивается и смещается. 
		break;
	case WM_LBUTTONDOWN: // При нажатии на правую кнопку мыши: 
	{

		/*Берём координаты курсора и записываем их в Coordinates и структуру _point.
		  После, если цвет курсора синий, присваиваем поставленной точке классификацию "1" (для чего
		  нужно и алгоритм работы описан выше). В противном случае - "-1". Ну и добавляем структуру в вектор.  */

		_point.x = Coordinates.x = LOWORD(lParam);
		_point.y = Coordinates.y = HIWORD(lParam);
		if (colorPixel == RGB(0, 0, 255)) _point.label = 1;
		else _point.label = -1;
		VPoints.push_back(_point);
	}
		break;
	case WM_LBUTTONUP: // Когда правая кнопка мыши отпускается:
	{
		HDC hdc = GetDC(hWnd);
		HBRUSH hBrush = CreateSolidBrush(colorPixel); // Задаём сплошную кисть, закрашенную цветом RGB.
		SelectObject(hdc, hBrush); // Делаем кисть активной.
		Ellipse(hdc, Coordinates.x - 3, Coordinates.y - 3, Coordinates.x + 3, Coordinates.y + 3); // Создаём круг (визуальную точку). 
	}
		break;
	case WM_MOUSEMOVE: // Каждый раз, когда мышка двигается...
	{
		swprintf_s(Buff, _T("%i"), LOWORD(lParam)); // Вынимаем её координату по X, переводим из int в TCHAR...
		wcscat_s(TCoordinatesXY, L"x: ");
		wcscat_s(TCoordinatesXY, Buff); // ... и добавляем в строку,...
		SendMessage(StatusBar, SB_SETTEXT, 4 | SBT_NOBORDERS, (LPARAM)TCoordinatesXY); // ...которую в итоге выводим в StatusBar.
		swprintf_s(TCoordinatesXY, _T("%c"), ' '); // Обнуляем строку.
		wcscat_s(TCoordinatesXY, L"y: "); // И делаем тоже самое для Y.
		swprintf_s(Buff, _T("%i"), HIWORD(lParam)); // HIWORD - вынимает из всякого шлака положение мышки по Y.
		wcscat_s(TCoordinatesXY, Buff);
		SendMessage(StatusBar, SB_SETTEXT, 5 | SBT_NOBORDERS, (LPARAM)TCoordinatesXY);
		swprintf_s(TCoordinatesXY, _T("%c"), ' '); // Снова сбрасываем эту строку.

		// И так много-много раз. О-о-очень много (:
	}
		break;
    case WM_PAINT: // Отрисовка главного окна.
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);

            /*Сюда можно добавит любой код отрисовки, использующий HDC. Но мне это не понадобилось. 
			  Мб вообще удалю, если руки дотянуться. 
			  
			  UPD 11.10.19: как бы мне это нужно, чтобы точки и разделяющая линия снова отрисовывались после
			  того, как приложение свернулось или было перекрыто другим, но... мне лень и вообще, задание лабы заключается
			  не в затирании экрана. Так что баг подождёт костыля, уже заказал с алика. Обещали привезти через год.  */

            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY: // Отправить сообщение о выходе и вернуться.
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}
