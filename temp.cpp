/******************************************************************
 Grafika komputerowa, �rodowisko MS Windows - program  przyk�adowy
 *****************************************************************/

#include <windows.h>
#include <gdiplus.h>
#include <math.h>
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
	WNDCLASS nasza_klasa; //klasa g��wnego okna aplikacji
	HWND okno;
	static char nazwa_klasy[] = "Podstawowa";

	GdiplusStartupInput gdiplusParametry;// parametry GDI+; domy�lny konstruktor wype�nia struktur� odpowiednimi warto�ciami
	ULONG_PTR	gdiplusToken;			// tzw. token GDI+; warto�� uzyskiwana przy inicjowaniu i przekazywana do funkcji GdiplusShutdown

	// Inicjujemy GDI+.
	GdiplusStartup(&gdiplusToken, &gdiplusParametry, NULL);

	//Definiujemy klase g��wnego okna aplikacji
	//Okreslamy tu wlasciwosci okna, szczegoly wygladu oraz
	//adres funkcji przetwarzajacej komunikaty
	nasza_klasa.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	nasza_klasa.lpfnWndProc = WndProc; //adres funkcji realizuj�cej przetwarzanie meldunk�w 
	nasza_klasa.cbClsExtra = 0;
	nasza_klasa.cbWndExtra = 0;
	nasza_klasa.hInstance = hInstance; //identyfikator procesu przekazany przez MS Windows podczas uruchamiania programu
	nasza_klasa.hIcon = 0;
	nasza_klasa.hCursor = LoadCursor(0, IDC_ARROW);
	nasza_klasa.hbrBackground = (HBRUSH)GetStockObject(GRAY_BRUSH);
	nasza_klasa.lpszMenuName = "Menu";
	nasza_klasa.lpszClassName = nazwa_klasy;

	//teraz rejestrujemy klas� okna g��wnego
	RegisterClass(&nasza_klasa);

	/*tworzymy okno g��wne
	okno b�dzie mia�o zmienne rozmiary, listw� z tytu�em, menu systemowym
	i przyciskami do zwijania do ikony i rozwijania na ca�y ekran, po utworzeniu
	b�dzie widoczne na ekranie */
	okno = CreateWindow(nazwa_klasy, "Grafika komputerowa", WS_OVERLAPPEDWINDOW | WS_VISIBLE,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL);


	/* wyb�r rozmiaru i usytuowania okna pozostawiamy systemowi MS Windows */
	ShowWindow(okno, nCmdShow);

	//odswiezamy zawartosc okna
	UpdateWindow(okno);

	// G��WNA P�TLA PROGRAMU
	while (GetMessage(&meldunek, NULL, 0, 0))
		/* pobranie komunikatu z kolejki; funkcja GetMessage zwraca FALSE tylko dla
		komunikatu WM_QUIT; dla wszystkich pozosta�ych komunikat�w zwraca warto�� TRUE */
	{
		TranslateMessage(&meldunek); // wst�pna obr�bka komunikatu
		DispatchMessage(&meldunek);  // przekazanie komunikatu w�a�ciwemu adresatowi (czyli funkcji obslugujacej odpowiednie okno)
	}

	GdiplusShutdown(gdiplusToken);

	return (int)meldunek.wParam;
}


int x, y, radius = 3, heart_index;
bool active = false;
POINT heart[13] = { {500, 300}, {550, 160}, {580, 180}, {630, 280}, {680, 180}, {710, 160}, {760, 300}, {750, 350}, {700, 400}, {630, 480}, {560, 400}, {510, 350}, {500, 300} };

/********************************************************************
FUNKCJA OKNA realizujaca przetwarzanie meldunk�w kierowanych do okna aplikacji*/
LRESULT CALLBACK WndProc(HWND okno, UINT kod_meldunku, WPARAM wParam, LPARAM lParam)
{
	HMENU mPlik, mInfo, mGlowne;

	/* PONI�SZA INSTRUKCJA DEFINIUJE REAKCJE APLIKACJI NA POSZCZEG�LNE MELDUNKI */
	switch (kod_meldunku)
	{
	case WM_CREATE:  //meldunek wysy�any w momencie tworzenia okna
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

	case WM_COMMAND: //reakcje na wyb�r opcji z menu
		switch (wParam)
		{
		case 100: if (MessageBox(okno, "Zapiszcze�?", "Pisk", MB_YESNO) == IDYES)
			MessageBeep(0);
			break;
		case 101: DestroyWindow(okno); //wysylamy meldunek WM_DESTROY
			break;
		case 200: MessageBox(okno, "Imi� i nazwisko:\nNumer indeksu: ", "Autor", MB_OK);
		}
		return 0;

	case WM_LBUTTONDOWN: //reakcja na lewy przycisk myszki
	{
		x = LOWORD(lParam);
		y = HIWORD(lParam);

		for (int i = 0; i < 13; i++)
		{
			POINT center = { ((heart[i].x - 3) + (heart[i].x + 5)) / 2, ((heart[i].y - 3) + (heart[i].y + 5)) / 2 };
			double distance = sqrt((x - center.x) * (x - center.x) + (y - center.y) * (y - center.y));

			if ((int)distance <= radius)
			{
				heart_index = i;
				active = true;
				break;
			}

		}

		return 0;
	}

	case WM_MOUSEMOVE:
		if (wParam & MK_LBUTTON && active)
		{
			int currentX = LOWORD(lParam);
			int currentY = HIWORD(lParam);

			int moveX = currentX - x;
			int moveY = currentY - y;

			heart[heart_index].x += moveX;
			heart[heart_index].y += moveY;

			if (heart_index == 0)
			{
				heart[12].x += moveX;
				heart[12].y += moveY;
			}

			x = currentX;
			y = currentY;

			InvalidateRect(okno, NULL, TRUE);
		}

		break;


	case WM_LBUTTONUP:
	{
		active = false;
		return 0;
	}

	case WM_PAINT:
	{
		PAINTSTRUCT paint;
		HDC kontekst;

		kontekst = BeginPaint(okno, &paint);

		// MIEJSCE NA KOD GDI

		//Pierwsza czesc kola, lewa
		HPEN pioro = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
		SelectObject(kontekst, pioro);
		HBRUSH brush = CreateSolidBrush(RGB(255, 255, 0));
		SelectObject(kontekst, brush);
		Pie(kontekst, 150, 120, 390, 360, 270, 120, 270, 360);

		//Druga czesc kola, prawa
		pioro = CreatePen(PS_SOLID, 1, RGB(0, 255, 0));
		SelectObject(kontekst, pioro);
		brush = CreateSolidBrush(RGB(0, 255, 0));
		SelectObject(kontekst, brush);
		Pie(kontekst, 150, 120, 390, 360, 270, 360, 270, 120);

		//Zewnetrzna czesc litery P
		POINT punkty_zewnetrzne[] = { {270, 350}, {270, 130}, {320, 130}, {330, 150}, {320, 170}, {280, 170}, {280, 350} };
		pioro = CreatePen(PS_SOLID, 1, RGB(0, 0, 255));
		SelectObject(kontekst, pioro);
		brush = CreateSolidBrush(RGB(0, 0, 255));
		SelectObject(kontekst, brush);
		Polygon(kontekst, punkty_zewnetrzne, 7);

		//Wewnetrzna czesc litery P
		POINT punkty_wewnetrzne[] = { {280, 140}, {310, 140}, {320, 150}, {310, 160}, {280, 160} };
		pioro = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
		SelectObject(kontekst, pioro);
		brush = CreateSolidBrush(RGB(255, 0, 0));
		SelectObject(kontekst, brush);
		Polygon(kontekst, punkty_wewnetrzne, 5);

		pioro = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
		brush = CreateSolidBrush(RGB(255, 0, 0));
		SelectObject(kontekst, pioro);
		SelectObject(kontekst, brush);
		PolyBezier(kontekst, heart, 13);

		for (int i = 0; i < 13; i++)
		{
			Ellipse(kontekst, heart[i].x - 3, heart[i].y - 3, heart[i].x + 5, heart[i].y + 5);
		}

		DeleteObject(brush);
		DeleteObject(pioro);

		// utworzenie obiektu umo�liwiaj�cego rysowanie przy u�yciu GDI+
		// (od tego momentu nie mo�na u�ywa� funkcji GDI
		Graphics grafika(kontekst);

		// MIEJSCE NA KOD GDI+


		// utworzenie czcionki i wypisanie tekstu na ekranie
		/*FontFamily  fontFamily(L"Times New Roman");
		Font        font(&fontFamily, 24, FontStyleRegular, UnitPixel);
		PointF      pointF(100.0f, 400.0f);
		SolidBrush  solidBrush(Color(255, 0, 0, 255));

		grafika.DrawString(L"To jest tekst napisany za pomoc� GDI+.", -1, &font, pointF, &solidBrush);*/

		EndPaint(okno, &paint);

		return 0;
	}

	case WM_DESTROY: //obowi�zkowa obs�uga meldunku o zamkni�ciu okna
		PostQuitMessage(0);
		return 0;

	default: //standardowa obs�uga pozosta�ych meldunk�w
		return DefWindowProc(okno, kod_meldunku, wParam, lParam);
	}
}
