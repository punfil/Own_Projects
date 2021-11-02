
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdio.h>

#define TRUE 1
#define FALSE 0

#define npoints_P 24
#define npoints_W 16

int red, green, blue, yellow;
unsigned long foreground, background;

XArc circle1 = {50, 50, 150, 150, 0, 360*4};
XArc circle2 = {250, 50, 150, 150, 0, 360*4};
double fi = 0.1;

// punkty położenia liter
XPoint P_all_points[npoints_P] = {{295, 80}, {305, 80}, {305, 170}, {295, 170},	// lewy
		{295, 80}, {345, 80}, {345, 90}, {295, 90},	// gora
		{335, 80}, {345, 80}, {355, 100}, {345, 100},
		{345, 100}, {355, 100}, {355, 110}, {345, 110},
		{345, 110}, {355, 110}, {345, 130}, {335, 130},
		{295, 120}, {345, 120}, {345, 130}, {295, 130}}; // dol
		
XPoint W_all_points[npoints_W] =  {{80,80}, {95, 80}, {110, 140}, {110, 145}, 
            {110, 140}, {110, 145}, {125, 115}, {125, 110},
            {125, 115}, {125, 110}, {140, 140}, {140, 145}, 
            {140, 145}, {140, 140}, {155, 80}, {170, 80}};

XPoint mousePosition;



// obwódki liter
XPoint p1[6] = {{295, 170}, {295, 80}, {345, 80}, {355, 100}, {355, 110}, {345, 130}};
XPoint p2[7] = {{305, 120}, {305, 90}, {340, 90}, {345, 100}, {345, 110}, {340, 120}, {305, 120}};
XPoint p3[7] = {{295, 170}, {305, 170}, {305, 130}, {345, 130}};
XPoint l[11] = {{80,80}, {95, 80}, {110, 140},{125, 110}, 
{140, 140}, {155, 80}, {170, 80}, {140, 145}, {125, 115}, {110, 145}, {80,80}};
                        //

//*************************************************************************************************************************
//funkcja przydzielania kolorow

int AllocNamedColor(char *name, Display* display, Colormap colormap)
  {
    XColor col;
    XParseColor(display, colormap, name, &col);
    XAllocColor(display, colormap, &col);
    return col.pixel;
  } 

//*************************************************************************************************************************
// inicjalizacja zmiennych globalnych okreslajacych kolory

int init_colors(Display* display, int screen_no, Colormap colormap)
{
  background = WhitePixel(display, screen_no);  //niech tlo bedzie biale
  foreground = BlackPixel(display, screen_no);  //niech ekran bedzie czarny
  red=AllocNamedColor("red", display, colormap);
  green=AllocNamedColor("green", display, colormap);
  blue=AllocNamedColor("blue", display, colormap);
  yellow=AllocNamedColor("yellow", display, colormap);
}

void drawCircle (Display *d, Window w, GC gc, XArc circle, int color)
{
    XSetForeground(d, gc, color);
    XFillArc(d, w, gc, circle.x, circle.y, circle.width, circle.height, circle.angle1, circle.angle2*64);
}

void drawBorders(Display *d, Window w, GC gc)
{
  XSetForeground(d, gc, foreground);
  
  // obwódka B
  XDrawLines(d, w, gc, p1, 6 , CoordModeOrigin);
  XDrawLines(d, w, gc, p2, 7, CoordModeOrigin);
  XDrawLines(d, w, gc, p3, 4, CoordModeOrigin);
  
  XDrawLines(d, w, gc, l, 11, CoordModeOrigin); // obwódka W
}


void drawInitials (Display *d, Window w, GC gc)
{ 			
  XSetForeground(d, gc, red);
  XPoint points[4];
  
  // rysowanie litery P
  int j = 0;
  for (int i = 0; i < npoints_P; i++)
  {
    points[j] = P_all_points[i];
    
    if ((i+1) % 4 == 0)
    {
	    XFillPolygon(d, w, gc, points, 4, Convex, CoordModeOrigin);
	    j = 0;
    }
    else
      j++;
  }  
  j = 0;
  for (int i = 0; i < npoints_W; i++)
  {
    points[j] = W_all_points[i];
    
    if ((i+1) % 4 == 0)
    {
	    XFillPolygon(d, w, gc, points, 4, Convex, CoordModeOrigin);
	    j = 0;
    }
    else
      j++;
  } 
  
  
  //XFillPolygon(d, w, gc, W_all_points, npoints_W, Convex, CoordModeOrigin); // rysowanie litery L
  
  // rysowanie obwódki
  drawBorders(d, w, gc);
}

void move(XEvent event)
{
  int movementX = event.xbutton.x - mousePosition.x;
  int movementY = event.xbutton.y - mousePosition.y;
  
  if (((mousePosition.x-(circle1.x+circle1.width/2))*(mousePosition.x-(circle1.x+circle1.width/2)) + (mousePosition.y-(circle1.y+circle1.height/2))*(mousePosition.y-(circle1.y+circle1.height/2))) <= (circle1.width/2)*(circle1.width/2))
  { 
    circle1.x += movementX;
    circle1.y += movementY;
    
    for (int i = 0; i < npoints_W; i++)
    {
    	W_all_points[i].x += movementX;
    	W_all_points[i].y += movementY;
    }
    
    for (int i = 0; i < 12; i++)
    {
    	l[i].x += movementX;
    	l[i].y += movementY;
    }

    
    mousePosition.x = event.xbutton.x;
    mousePosition.y = event.xbutton.y;
  }
  
  else if (((mousePosition.x-(circle2.x+circle2.width/2))*(mousePosition.x-(circle2.x+circle2.width/2)) + (mousePosition.y-(circle2.y+circle2.height/2))*(mousePosition.y-(circle2.y+circle2.height/2))) <= (circle2.width/2)*(circle2.width/2))
  {
    circle2.x += movementX;
    circle2.y += movementY;
    
    for (int i = 0; i < npoints_P; i++)
    {
    	P_all_points[i].x += movementX;
    	P_all_points[i].y += movementY;
    }
    
    for (int i = 0; i < 6; i++)
    {
    	p1[i].x += movementX;
    	p1[i].y += movementY;
    }
    
    for (int i = 0; i < 7; i++)
    {
    	p2[i].x += movementX;
    	p2[i].y += movementY;
    	
    	p3[i].x += movementX;
    	p3[i].y += movementY;
    }
    
    mousePosition.x = event.xbutton.x;
    mousePosition.y = event.xbutton.y;
  }
}


void rotate()
{
  
  //double new_x = cos(fi) * (circle1.x - circle2.x) - sin(fi) * (circle1.y - circle2.y) + circle2.x;
  //double new_y = sin(fi) * (circle1.x - circle2.x) + cos(fi) * (circle1.y - circle2.y) + circle2.y;
  
  double new_x = circle2.x + cos(fi) * circle2.width;
  double new_y = circle2.y + sin(fi) * circle2.height;
  
  circle1.x = new_x;
  circle1.y = new_y;
  
  fi += 0.1;
}

//*************************************************************************************************************************
// Glowna funkcja zawierajaca petle obslugujaca zdarzenia */

int main(int argc, char *argv[])
{
  char            icon_name[] = "Grafika";
  char            title[]     = "Grafika komputerowa";
  Display*        display;    //gdzie bedziemy wysylac dane (do jakiego X servera)
  Window          window;     //nasze okno, gdzie bedziemy dokonywac roznych operacji
  GC              gc;         //tu znajduja sie informacje o parametrach graficznych
  XEvent          event;      //gdzie bedziemy zapisywac pojawiajace sie zdarzenia
  KeySym          key;        //informacja o stanie klawiatury 
  Colormap        colormap;
  int             screen_no;
  XSizeHints      info;       //informacje typu rozmiar i polozenie ok
  
  char            buffer[8];  //gdzie bedziemy zapamietywac znaki z klawiatury
  int             hm_keys;    //licznik klawiszy
  int             to_end;
  
  XPoint	   position;

  display    = XOpenDisplay("");                //otworz polaczenie z X serverem pobierz dane od zmiennej srodowiskowej DISPLAY ("")
  screen_no  = DefaultScreen(display);          //pobierz domyslny ekran dla tego wyswietlacza (0)
  colormap = XDefaultColormap(display, screen_no);
  init_colors(display, screen_no, colormap);

  //okresl rozmiar i polozenie okna
  info.x = 100;
  info.y = 150;
  info.width = 500;
  info.height = 300;
  info.flags = PPosition | PSize;

  //majac wyswietlacz, stworz okno - domyslny uchwyt okna
  window = XCreateSimpleWindow(display, DefaultRootWindow(display),info.x, info.y, info.width, info.height, 7/* grubosc ramki */, foreground, background);
  XSetStandardProperties(display, window, title, icon_name, None, argv, argc, &info);
  //utworz kontekst graficzny do zarzadzania parametrami graficznymi (0,0) domyslne wartosci
  gc = XCreateGC(display, window, 0, 0);
  XSetBackground(display, gc, background);
  XSetForeground(display, gc, foreground);

  //okresl zdarzenia jakie nas interesuja, np. nacisniecie klawisza
  XSelectInput(display, window, (KeyPressMask | ExposureMask | ButtonPressMask| ButtonReleaseMask | Button1MotionMask));
  XMapRaised(display, window);  //wyswietl nasze okno na samym wierzchu wszystkich okien
        
  to_end = FALSE;

 /* petla najpierw sprawdza, czy warunek jest spelniony
     i jesli tak, to nastepuje przetwarzanie petli
     a jesli nie, to wyjscie z petli, bez jej przetwarzania */
  while (to_end == FALSE)
  {
    drawCircle(display, window, gc, circle1, green);
    drawCircle(display, window, gc, circle2, blue);
    //rotate();
   
    // rysowanie inicjalow
    drawInitials(display, window, gc);
    
    XNextEvent(display, &event);  // czekaj na zdarzenia okreslone wczesniej przez funkcje XSelectInput
    XClearWindow(display, window);
    

    switch(event.type)
    {
      case Expose:
        if (event.xexpose.count == 0)
        {

        }
        break;

      case MappingNotify:
        XRefreshKeyboardMapping(&event.xmapping); // zmiana ukladu klawiatury - w celu zabezpieczenia sie przed taka zmiana trzeba to wykonac
        break;

      case ButtonPress:
        if (event.xbutton.button == Button1)  // sprawdzenie czy wcisnieto lewy przycisk		
        {
            mousePosition.x = event.xbutton.x;
            mousePosition.y = event.xbutton.y;   
            printf("%d %d\n", position.x, position.y);
            printf("%d %d %d\n", (circle1.x+circle1.width/2), circle1.y+circle1.height/2, circle1.width/2);
        }
        break;


      case KeyPress:
        hm_keys = XLookupString(&event.xkey, buffer, 8, &key, 0);
        if (hm_keys == 1)
        {
          if (buffer[0] == 'q') to_end = TRUE;        // koniec programu
          
        }
        
      case MotionNotify:
      	printf("Motion: %d %d\n", event.xbutton.x, event.xbutton.y);
      	move(event);
      	
      default:
        break;
    }
  }

  XFreeGC(display, gc);
  XDestroyWindow(display, window);
  XCloseDisplay(display);

  return 0;
}
