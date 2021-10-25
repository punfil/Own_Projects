.686
.model flat
extern _ExitProcess@4 : PROC
extern __write : PROC
extern __read : PROC
extern _MessageBoxW@16 : PROC
public _main
.data

liczba_znakow dd 80;
magazyn_wejscie db 80 dup (?)
magazyn_wyjscie db 120 dup (?)
zapamietanie dd 0

.code
_main PROC
	;TEKST WEJSCIOWY = TEKST KTORY WPROWADZAMY PRZEZ __READ
	;TEKST WYJSCIOWY = TEKST KTORY POKAZANY BEDZIE PRZEZ __WRITE
	;TEKST PRZEPISYWANY = OSTATNIO NAPOTKANY WYRAZ
	push [liczba_znakow]
	push OFFSET magazyn_wejscie
	push 0 
	call __read 
	add esp, 12 
	; ESI - WYRAZ KONCOWY EDI - OSTATNI POCZATEK WYRAZU EBX - WYRAZ WEJSCIE
	mov ecx, eax ;Przeniesienie liczby znakow odczytanej
	mov ebx, 0 ;Ustawienie poczatku petli, indeks we wczytanym zdaniu
	mov esi, 0 ;Indeks koncowy wyrazu na wyjscie
	mov edi, 0 ;Indeks ostatniego poczatku wyrazu
ptl:
	mov dl, magazyn_wejscie[ebx] ;Przejscie do kolejnego znaku w napisie wejsciowym
	cmp dl, ' ' ;Porownanie ze spacja, jezeli tak skocz do SPACJA
	je SPACJA
PRZEPISANIE:
	mov magazyn_wyjscie[esi], dl ;Przepisz z wejscia na wyjscie
	jmp DALEJ ;Skocz do DALEJ, nastepna iteracja petli
SPACJA:
	mov dl, magazyn_wejscie[ebx+1] ;Sprawdzamy, czy nastepny po spacji jest backslash jesli tak to idziemy do wykryto
	cmp dl, '\'
	je WYKRYTO
IGNOROWANIE:
	mov edi, ebx ;Jezeli mamy zignorowac, to nastepny znak (jestesmy w spacji) jest nowym wyrazem, potrzebujemy jego indeksu w tablicy wejsciowej
	inc edi ;Zwiekszamy, zebysmy nie byli na spacji, a na poczatku wyrazu nastepnego
	mov dl, magazyn_wejscie[ebx] ;Przygotowanie do przepisania spacji na output
	jmp PRZEPISANIE
WYKRYTO:
	mov dl, magazyn_wejscie[ebx+2] ;Kontrolnie sprawdzamy, czy to na pewno \d, nie np \e
	cmp dl, 'd'
	jne IGNOROWANIE ;Jezeli to nie \d to ignorujemy, przepisujemy na output
	mov magazyn_wyjscie[esi], ' ' ;Dodajemy spacje miedzy wyrazami na wyjscie
	inc esi ;Zwiekszamy licznik indeksowy wyjscia
	mov zapamietanie, edi ;Zapamietujemy, gdzie byl poczatek ostatniego wyrazu, aby moc przywrocic EDI
	ptl2:
		mov dl, magazyn_wejscie[edi] ;Nastepny znak
		mov magazyn_wyjscie[esi], dl ;Przepisujemy na wyjscie znak
		inc edi ;Zwiekszamy indeks w wyrazie przepisywanym
		inc esi ; Zwiekszamy indeks w wyrazie wyjsciowym
		cmp magazyn_wejscie[edi], ' ' ;Jezeli nie dotarlismy do spacji mozemy przepisywac dalej
		jne ptl2
	mov edi, zapamietanie ;Wracamy do poczatku ostatnio zapamietanego wyrazu
	add ebx, 2 ;Ignorujemy spacje oraz /
DALEJ:
	inc esi ;Zwiekszamy indeks w tekscie wyjsciowym
	inc ebx ;Zwiekszamy indeks w tekscie wejsciowym
	loop ptl ;Zapetlamy


	push 80
	push OFFSET magazyn_wyjscie
	push 1
	call __write

	push 0
	call _ExitProcess@4
_main ENDP
END