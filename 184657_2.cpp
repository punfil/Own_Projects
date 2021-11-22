/******************************************************************
 Grafika komputerowa, środowisko MS Windows - program  przykładowy
 *****************************************************************/
#include <vector>

#include <windows.h>
#include <gdiplus.h>
using namespace Gdiplus;


//deklaracja funkcji obslugi okna
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

//funkcja Main - dla Windows
int WINAPI WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR     lpCmdLine,
	int       nCmdShow)
{
	MSG meldunek;		  //innymi slowy "komunikat"
	WNDCLASS nasza_klasa; //klasa głównego okna aplikacji
	HWND okno;
	static char nazwa_klasy[] = "Podstawowa";

	GdiplusStartupInput gdiplusParametry;// parametry GDI+; domyślny konstruktor wypełnia strukturę odpowiednimi wartościami
	ULONG_PTR	gdiplusToken;			// tzw. token GDI+; wartość uzyskiwana przy inicjowaniu i przekazywana do funkcji GdiplusShutdown

	// Inicjujemy GDI+.
	GdiplusStartup(&gdiplusToken, &gdiplusParametry, NULL);

	//Definiujemy klase głównego okna aplikacji
	//Okreslamy tu wlasciwosci okna, szczegoly wygladu oraz
	//adres funkcji przetwarzajacej komunikaty
	nasza_klasa.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	nasza_klasa.lpfnWndProc = WndProc; //adres funkcji realizującej przetwarzanie meldunków 
	nasza_klasa.cbClsExtra = 0;
	nasza_klasa.cbWndExtra = 0;
	nasza_klasa.hInstance = hInstance; //identyfikator procesu przekazany przez MS Windows podczas uruchamiania programu
	nasza_klasa.hIcon = 0;
	nasza_klasa.hCursor = LoadCursor(0, IDC_ARROW);
	nasza_klasa.hbrBackground = (HBRUSH)GetStockObject(GRAY_BRUSH);
	nasza_klasa.lpszMenuName = "Menu";
	nasza_klasa.lpszClassName = nazwa_klasy;

	//teraz rejestrujemy klasę okna głównego
	RegisterClass(&nasza_klasa);

	/*tworzymy okno główne
	okno będzie miało zmienne rozmiary, listwę z tytułem, menu systemowym
	i przyciskami do zwijania do ikony i rozwijania na cały ekran, po utworzeniu
	będzie widoczne na ekranie */
	okno = CreateWindow(nazwa_klasy, "Grafika komputerowa", WS_OVERLAPPEDWINDOW | WS_VISIBLE,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL);


	/* wybór rozmiaru i usytuowania okna pozostawiamy systemowi MS Windows */
	ShowWindow(okno, nCmdShow);

	//odswiezamy zawartosc okna
	UpdateWindow(okno);

	// GŁÓWNA PĘTLA PROGRAMU
	while (GetMessage(&meldunek, NULL, 0, 0))
		/* pobranie komunikatu z kolejki; funkcja GetMessage zwraca FALSE tylko dla
		komunikatu WM_QUIT; dla wszystkich pozostałych komunikatów zwraca wartość TRUE */
	{
		TranslateMessage(&meldunek); // wstępna obróbka komunikatu
		DispatchMessage(&meldunek);  // przekazanie komunikatu właściwemu adresatowi (czyli funkcji obslugujacej odpowiednie okno)
	}

	GdiplusShutdown(gdiplusToken);

	return (int)meldunek.wParam;
}
POINT car[] = { {529, 314}, {535,285}, {562, 317}, {577, 282} , {592,254}, {624, 267}, {622, 264}, {651,261}, {634, 285}, {658, 295}, {684,296}, {698, 295}, {694, 310}, {694,311}, {696, 330}, {667, 323}, {656,305}, {633, 310}, {630, 322}, {573,324}, {596, 324}, {573, 324}, {567,308}, {550, 312}, {542, 325},{527,328}, {524, 326}, {528, 314}, };
int car_count = 28;
int x, y, radius = 3;
int car_index;
bool active_car = false;
bool new_point = false;
std::vector <Point> punkty_curve;
Point* punkty_curve_array;
/********************************************************************
FUNKCJA OKNA realizujaca przetwarzanie meldunków kierowanych do okna aplikacji*/
LRESULT CALLBACK WndProc(HWND okno, UINT kod_meldunku, WPARAM wParam, LPARAM lParam)
{
	HMENU mPlik, mInfo, mGlowne;

	/* PONIŻSZA INSTRUKCJA DEFINIUJE REAKCJE APLIKACJI NA POSZCZEGÓLNE MELDUNKI */
	switch (kod_meldunku)
	{
	case WM_CREATE:  //meldunek wysyłany w momencie tworzenia okna
		mPlik = CreateMenu();
		AppendMenu(mPlik, MF_STRING, 100, "&Zapiszcz...");
		AppendMenu(mPlik, MF_SEPARATOR, 0, "");
		AppendMenu(mPlik, MF_STRING, 101, "&Koniec");
		mInfo = CreateMenu();
		AppendMenu(mInfo, MF_STRING, 200, "&Autor...");
		mGlowne = CreateMenu();
		AppendMenu(mGlowne, MF_POPUP, (UINT_PTR)mPlik, "&Plik");
		AppendMenu(mGlowne, MF_POPUP, (UINT_PTR)mInfo, "&Informacja");
		SetMenu(okno, mGlowne);
		DrawMenuBar(okno);

	case WM_COMMAND: //reakcje na wybór opcji z menu
		switch (wParam)
		{
		case 100: if (MessageBox(okno, "Zapiszczeć?", "Pisk", MB_YESNO) == IDYES)
			MessageBeep(0);
			break;
		case 101: DestroyWindow(okno); //wysylamy meldunek WM_DESTROY
			break;
		case 200: MessageBox(okno, "Imię i nazwisko:\nNumer indeksu: ", "Autor", MB_OK);
		}
		return 0;

	case WM_LBUTTONDOWN: //reakcja na lewy przycisk myszki
	{
		x = LOWORD(lParam);
		y = HIWORD(lParam);

		for (int i = 0; i < car_count; i++)
		{
			POINT center = { ((car[i].x - 3) + (car[i].x + 5)) / 2, ((car[i].y - 3) + (car[i].y + 5)) / 2 };
			double distance = sqrt((x - center.x) * (x - center.x) + (y - center.y) * (y - center.y));

			if ((int)distance <= radius)
			{
				car_index = i;
				active_car = true;
				break;
			}

		}

		return 0;
	}

	case WM_MOUSEMOVE:
		if (wParam & MK_LBUTTON && active_car)
		{
			int currentX = LOWORD(lParam);
			int currentY = HIWORD(lParam);

			int moveX = currentX - x;
			int moveY = currentY - y;

			car[car_index].x += moveX;
			car[car_index].y += moveY;

			if (car_index == 0)
			{
				car[27].x += moveX;
				car[27].y += moveY;
			}

			x = currentX;
			y = currentY;

			InvalidateRect(okno, NULL, TRUE);
		}

		break;


	case WM_LBUTTONUP:
	{
		active_car = false;
		return 0;
	}
	case WM_RBUTTONDOWN:
	{
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		new_point = true;
		punkty_curve.emplace_back(Point(x, y));
		InvalidateRect(okno, NULL, TRUE);
		return 0;
	}
	case WM_RBUTTONDBLCLK:
	{
		new_point = true;
		punkty_curve.clear();
		InvalidateRect(okno, NULL, TRUE);
		return 0;
	}
	case WM_PAINT:
	{
		PAINTSTRUCT paint;
		HDC kontekst;

		kontekst = BeginPaint(okno, &paint);

		// MIEJSCE NA KOD GDI
		HPEN pioro = CreatePen(PS_SOLID, 1, RGB(0, 255, 0));
		SelectObject(kontekst, pioro);
		HBRUSH brush = CreateSolidBrush(RGB(0, 255, 0));
		SelectObject(kontekst, brush);
		Pie(kontekst, 150, 120, 390, 360, 340, 150, 210, 330);



		pioro = CreatePen(PS_SOLID, 1, RGB(0, 0, 255));
		SelectObject(kontekst, pioro);
		brush = CreateSolidBrush(RGB(0, 0, 255));
		SelectObject(kontekst, brush);
		Pie(kontekst, 150, 120, 390, 360, 350, 190, 200, 290);

		pioro = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
		SelectObject(kontekst, pioro);
		brush = CreateSolidBrush(RGB(255, 0, 0));
		SelectObject(kontekst, brush);
		Pie(kontekst, 150, 120, 390, 360, 340, 150, 200, 290);

		pioro = CreatePen(PS_SOLID, 1, RGB(255, 255, 0));
		SelectObject(kontekst, pioro);
		brush = CreateSolidBrush(RGB(255, 255, 0));
		SelectObject(kontekst, brush);
		Pie(kontekst, 150, 120, 390, 360, 210, 330, 350, 190);

		POINT prostokat_gora[] = { {240, 190}, {300, 190},{300, 200}, {240, 200} };
		POINT prostokat_gora2[] = { {300, 200}, {300, 270}, {290, 280}, {280, 290}, {270, 280}, {260, 270}, {280, 260}, {290, 270}, {290, 280}, {290, 270}, {290, 200} };
		//Gora
		pioro = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
		SelectObject(kontekst, pioro);
		brush = CreateSolidBrush(RGB(0, 0, 0));
		SelectObject(kontekst, brush);
		Polygon(kontekst, prostokat_gora, 4);

		pioro = CreatePen(PS_SOLID, 1, RGB(255, 255, 255));
		SelectObject(kontekst, pioro);
		brush = CreateSolidBrush(RGB(255, 255, 255));
		SelectObject(kontekst, brush);
		Polygon(kontekst, prostokat_gora2, 11);



		//Kolo 1
		pioro = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
		SelectObject(kontekst, pioro);
		brush = CreateSolidBrush(RGB(0, 0, 0));
		SelectObject(kontekst, brush);
		Ellipse(kontekst, 545, 325, 565, 345);

		//Kolo 2
		pioro = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
		SelectObject(kontekst, pioro);
		brush = CreateSolidBrush(RGB(0, 0, 0));
		SelectObject(kontekst, brush);
		Ellipse(kontekst, 635, 325, 655, 345);

		pioro = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
		SelectObject(kontekst, pioro);
		brush = CreateSolidBrush(RGB(0, 0, 0));
		SelectObject(kontekst, brush);
		PolyBezier(kontekst, car, car_count);

		for (int i = 0; i < car_count; i++) {
			Ellipse(kontekst, car[i].x - 3, car[i].y - 3, car[i].x + 3, car[i].y + 3);
		}

		DeleteObject(pioro);
		DeleteObject(brush);




		// utworzenie obiektu umożliwiającego rysowanie przy użyciu GDI+
		// (od tego momentu nie można używać funkcji GDI
		Graphics grafika(kontekst);

		// MIEJSCE NA KOD GDI+
		Pen GreenPen(Color::Green, 3);
		if (new_point) {
			delete[] punkty_curve_array;
			punkty_curve_array = new Point[punkty_curve.size()];
			for (int i = 0; i < punkty_curve.size(); i++) {
				punkty_curve_array[i] = punkty_curve[i];
			}
			new_point = false;
		}
		
		grafika.DrawCurve(&GreenPen, punkty_curve_array, int(punkty_curve.size()));
		
		
		// utworzenie czcionki i wypisanie tekstu na ekranie
		FontFamily  fontFamily(L"Times New Roman");
		Font        font(&fontFamily, 24, FontStyleRegular, UnitPixel);
		PointF      pointF(100.0f, 400.0f);
		SolidBrush  solidBrush(Color(255, 0, 0, 255));

		grafika.DrawString(L"To jest tekst napisany za pomocą GDI+.", -1, &font, pointF, &solidBrush);

		EndPaint(okno, &paint);

		return 0;
	}

	case WM_DESTROY: //obowiązkowa obsługa meldunku o zamknięciu okna
		PostQuitMessage(0);
		return 0;

	default: //standardowa obsługa pozostałych meldunków
		return DefWindowProc(okno, kod_meldunku, wParam, lParam);
	}
}
