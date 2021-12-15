#include <stdio.h> 
#include <xmmintrin.h>
__m128 szybki_max(short int t1[], short int t2[]);

int main()
{
    short int val1[8] = { 1,-1,2,-2,3,-3,4,-4 };
    short int val2[8] = { -4,-3,-2,-1,0,1,2,3 };
    __m128 t1 = szybki_max(val1, val2);
 
    return 0;
}
//USTAW WYRAZENIE KONTROLNE T1, BREAKPOINT NA RETURN 0, ROZWIN T1 i szukaj tablicy m128_i16, w niej jest wynik

.686
.XMM
.model flat
public _szybki_max

.data
.code

_szybki_max PROC
	push ebp
	mov ebp, esp
	push ebx
	push esi
	push edi

	mov esi, [ebp+8]
	mov ebx, [ebp+12];

	movups xmm1, [esi]
	movups xmm0, [ebx]
	PMAXSW xmm1, xmm0
	movaps xmm0, xmm1

	pop edi
	pop esi
	pop ebx
	pop ebp	
	ret
_szybki_max ENDP
END


#include <stdio.h> 
#define _USE_MATH_DEFINES
#include <math.h>
#include <xmmintrin.h>
float find_max_range(float v, float alpha);

int main()
{
    float v = 5.0;
    float alpha = M_PI/6;
    float wynik = find_max_range(v, alpha);
    printf("Wynik to: %f metrow\n", wynik);
    return 0;
}



.686
.XMM
.model flat
public _find_max_range
.data
g_value dd 9.81
.code

_find_max_range PROC
	push ebp
	mov ebp, esp
	;ebp+8 v
	;ebp+12 alpha

	finit
	fld dword ptr [ebp+12]
	fsincos
	fmul st(0), st(1)
	fstp st(1)
	fld dword ptr [ebp+8]
	fld dword ptr [ebp+8]
	fmul
	sub esp, 4
	mov [esp], dword ptr 2
	fild dword ptr [esp]
	fmul
	fmul
	fld dword ptr [g_value]
	fdiv

	add esp, 4
	pop ebp
	ret
_find_max_range ENDP
END




#include <stdio.h> 
float objetosc_stozka(unsigned int big_r, unsigned int small_r, float h);
int main()
{
    float wynik = objetosc_stozka(7, 3, 4.2);
    printf("Objetosc podanego stozka to: %f \n", wynik);
    return 0;
}

.686
.XMM
.model flat
public _objetosc_stozka

.data


.code
_objetosc_stozka PROC
	push ebp
	mov ebp, esp
	push esi
	push edi
	push ebx

	;EBP+8 DUZE R
	;EBP+12 MALE_R
	;EBP+16 H
	finit
	fild dword ptr [ebp+12] ;Zaladowanie male_r
	fild dword ptr [ebp+8] ;Zaladowanie duze_r
	;R*r
	fmul st(0), st(1)
	fstp st(1)
	;r^2
	fild dword ptr [ebp+12] ;Zaladowanie male_r
	fmul st(0), st(0)
	;R^2
	fild dword ptr [ebp+8] ;Zaladowanie duze_r
	fmul st(0), st(0)
	fadd
	fadd
	;Wczytanie 1 i 3
	sub esp, 4
	mov [esp], dword ptr 3
	fild dword ptr [esp]
	fld1
	fdiv st(0), st(1)
	fstp st(1)
	fldpi
	fld dword ptr [ebp+16]
	fmul
	fmul
	fmul


	

	add esp, 4
	pop ebx
	pop edi
	pop esi
	pop ebp
	ret
_objetosc_stozka ENDP

END


Wersja 2
.686
.XMM
.model flat

public _objetosc_stozka

.data


.code
_objetosc_stozka PROC
	push ebp
	mov ebp, esp
	push esi
	push edi
	push ebx

	;EBP+8 - duze_r
	;EBP+12 - male_r
	;EBP+16
	fild dword ptr [ebp+8]
	fst st(1)
	fmulp

	fild dword ptr [ebp+12]
	fst st(2)
	fmul st(0), st(2)
	fstp st(2)

	fild dword ptr [ebp+8]
	fild dword ptr [ebp+12]
	fmulp

	faddp
	faddp

	fldpi
	sub esp, 4
	mov [esp], dword ptr 3
	fld1
	fild dword ptr [esp]
	fdivp
	fmulp
	fld dword ptr [ebp+16]
	fmulp
	fmulp



	add esp, 4
	pop ebx
	pop edi
	pop esi
	pop ebp	
	ret
_objetosc_stozka ENDP

END




#include <stdio.h> 
void szybki_max(int t_1[], int t_2[], int t_wynik[], int n);
int main()
{
    int val1[8] = { 1,-1,2,-2,3,-3,4,-4 };
    int val2[8] = { -4, -3, -2, -1, 0, 1, 2, 3 };
    int wynik[8];
    szybki_max(val1, val2, wynik, 8);
    for (int i = 0; i < 8; i++) {
        printf("%d\n", wynik[i]);
    }
    return 0;
}

.686
.XMM
.model flat
public _szybki_max

.data


.code
_szybki_max PROC
	push ebp
	mov ebp, esp
	push esi
	push edi
	push ebx

	mov esi, [ebp+8] ;Adres tablic1
	mov ebx, [ebp+12] ;Tablica 2
	mov edi, [ebp+16];Tablica na wynik
	mov eax, [ebp+20];Ilosc czworek

	xor edx, edx
	mov ecx, 4
	div ecx
	mov ecx, eax
ptl:
	movups xmm1, [esi]
	movups xmm0, [ebx]
	PMAXSD xmm1, xmm0
	movups [edi], xmm1
	add esi, 16
	add ebx, 16
	add edi, 16
	loop ptl
	
	
	pop ebx
	pop edi
	pop esi
	pop ebp
	ret
_szybki_max ENDP

END


#include <stdio.h> 
#include <xmmintrin.h>
__m128 mul_at_once(__m128 one, __m128 two);

int main()
{
    __m128 one = { .m128_i32 = {1,2,3,4} };
    __m128 two = { .m128_i32 = {4,3,2,1} };
    __m128 wynik = mul_at_once(one, two);
    printf("Wynik to: %d %d %d %d\n", wynik.m128_i32[0], wynik.m128_i32[1], wynik.m128_i32[2], wynik.m128_i32[3]);
    return 0;
}



.686
.XMM
.model flat
public _mul_at_once
.data
.code

_mul_at_once PROC
	push ebp
	mov ebp, esp
	
	pmulld xmm0, xmm1

	pop ebp
	ret
_mul_at_once ENDP
END



#include <stdio.h> 
float srednia_kwadratowa(float* tablica, unsigned int n);
int main()
{
    float tablica[] = { 2.0, 2.0, 5.0, 7.0 };
    int n = 4;
    float wynik = srednia_kwadratowa(tablica, n);
    printf("%f\n", wynik);
    return 0;
}

.686
.XMM
.model flat
public _srednia_kwadratowa

.data


.code
_srednia_kwadratowa PROC
	push ebp
	mov ebp, esp
	push ebx
	push esi
	push edi

	mov esi, [ebp+8] ;Tablica z liczbami
	mov ecx, [ebp+12] ;Ilosc liczb

	finit
	fld dword ptr [esi]
	add esi, 4
	fmul st(0), st(0)
	dec ecx
ptl:
	fld dword ptr [esi]
	add esi, 4
	fmul st(0), st(0)
	fadd st(0), st(1)
	fstp st(1)
	loop ptl

	fidiv dword ptr [ebp+12]
	fsqrt

	pop edi
	pop esi
	pop ebx
	pop ebp
	ret
_srednia_kwadratowa ENDP

END



ZADANIA Z INSTRUKCJI
.686
.model flat

public _srednia_harm
public _nowy_exp
.data

.code
;Zadanie 5.2
_nowy_exp PROC
	push ebp
	mov ebp, esp
	push ebx
	push esi
	push edi

	mov ecx, 18 ;n-2
	sub esp, 4
	mov edi, esp ;Wynik
	sub esp, 4
	mov esi, esp ;Pomocnicza, zapis x^ebx
	sub esp, 4 ;Silnia

	finit
	fld1 ;Pierwsz jedynka
	fld dword ptr [ebp+8] ;x (drugi wyraz ciagu)
	fadd st(0), st(1)
	fstp dword ptr [edi]
	fstp st(0)
	;Obliczenie kwadratu liczby i zapisanie go
	fld dword ptr [ebp+8]
	fstp dword ptr [esi]
	fstp st(0)
	;Obliczenie 1! i zapis
	mov eax, 1
	;Obliczenie ktora silnia
	mov ebx, 2

ptl_glowna:
	;Obliczenie kolejnej potegi x
	fld dword ptr [ebp+8]
	fld dword ptr [esi]
	fmul st(0), st(1)
	fstp dword ptr [esi]
	fstp st(0)

	;Obliczenie kolejnej silni
	xor edx, edx
	mul ebx
	inc ebx
	mov [esp], eax

	;Obliczenie x^[ebx]/silnia i dodanie
	fild dword ptr [esp]
	fld dword ptr [esi]
	fdiv st(0), st(1)
	fadd dword ptr [edi]
	fstp dword ptr [edi]
	fstp st(0)
	
	loop ptl_glowna

	fld dword ptr [edi]
	add esp, 12
	pop edi
	pop esi
	pop ebx
	pop ebp
	ret
_nowy_exp ENDP



;Zadanie 5.1
_srednia_harm PROC
	push ebp
	mov ebp, esp
	push ebx
	push esi
	push edi
	
	mov esi, [ebp+8];Adres tablicy
	mov ecx, [ebp+12];Liczba elementow tablicy (n)
	sub esp, 4
	mov edi, esp ;Tu suma
	mov [edi], dword ptr 0;Wyzerowanie
	mov ebx, 0
	finit
ptl:
	fld dword ptr [esi+4*ebx]
	inc ebx
	fld1
	fdiv st(0), st(1)
	fadd dword ptr [edi]
	fstp dword ptr [edi]
	fstp st(0)
	loop ptl

	fld dword ptr [edi]
	add esp, 4
	fild dword ptr [ebp+12]
	fdiv st(0), st(1)
	
KONIEC:
	pop edi
	pop esi
	pop ebx
	pop ebp
	ret
_srednia_harm ENDP

END


#include <stdio.h>
#include <stdlib.h>
#include <math.h>

//Zadanie 5.1
float srednia_harm(float* tablica, unsigned int n);

//Zadanie 5.2
float nowy_exp(float x);

int silnia(int n) {
	int wynik = 1;
	for (int i = 1; i <= n; i++) {
		wynik *= i;
	}
	return wynik;
}
float nowy_exp_c(float x) {
	float suma = 0;
	float temp;
	for (int i = 0; i < 20; i++) {
		temp = pow(x, i);
		temp /= silnia(i);
		suma += temp;
	}
	return suma;
}

int main() {
	//Zadanie 5.1
	/*
	float tablica[] = { 2.0f, 4.0f, 5.0f };
	int ilosc_liczb = 3;
	float wynik = srednia_harm(tablica, ilosc_liczb);
	printf("Wynik dzialania funkcji to: %f\n", wynik);
	*/
	//Zadanie 5.2
	float wynik = nowy_exp(2.0);
	printf("Wynik dzialania funkcji to: %f\n", wynik);
	wynik = nowy_exp_c(2.0);
	printf("Wynik dzialania funkcji to: %f\n", wynik);
	return 0;
}

.686
.XMM
.model flat


public _dodaj_SSE

.code
_dodaj_SSE PROC
	push ebp
	mov ebp, esp
	push ebx
	push esi
	push edi

	mov esi, [ebp+8];Tablica A
	mov ebx, [ebp+12];Tablica B
	mov edi, [ebp+16];Tablica wynikowa

	movups xmm5, [esi]
	movups xmm6, [ebx]
	paddsb xmm5, xmm6

	movups [edi], xmm5

	pop edi
	pop esi
	pop ebx
	pop ebp
	ret
_dodaj_SSE ENDP
END

#include <stdio.h>

void dodaj_SSE(char*, char*, char*);

int main() {
	char liczby_A[16] = { -128, -127, -126, -125, -124, -123, -122,
 -121, 120,  121,  122,  123,  124,  125,  126, 127 };

	char liczby_B[16] = { -3, -3, -3, -3, -3, -3, -3, -3,
		 3,  3,  3,  3,  3,  3,  3,  3 };
	char wynik[16];
	dodaj_SSE(liczby_A, liczby_B, wynik);
	printf("Liczby A:\n");
	for (int i = 0; i < 16; i++)
		printf("%d ", liczby_A[i]);
	printf("\n");
	printf("Liczby B:\n");
	for (int i = 0; i < 16; i++)
		printf("  %d ", liczby_B[i]);
	printf("\n");
	printf("Liczby wynik:\n");
	for (int i = 0; i < 16; i++)
		printf("%d ", wynik[i]);
	printf("\n");
}

#include <stdio.h>

void int2float(int* calkowite, float* zmienno_przec);

int main() {
	int tab_calkowite[] = { 2, -50};
	float tab_floaty[4];
	int2float(tab_calkowite, tab_floaty);
	return 0;
}


.686
.XMM
.model flat
public _int2float
.data
ALIGN 16

.code
_int2float PROC
	push ebp
	mov ebp, esp
	push esi
	push edi
	
	mov esi, [ebp+8];Tablica wejsciowa
	mov edi, [ebp+12];Tablica wyjsciowa
	cvtpi2ps xmm5, qword PTR [esi]
	movups [edi], xmm5
	
	pop edi
	pop esi
	pop ebp	
	ret
_int2float ENDP
END

#include <stdio.h>

void pm_jeden(float* tabl);

int main() {
    float tablica[4] = { 27.5,143.57,2100.0, -3.51 };
    printf("\n%f   %f   %f   %f\n", tablica[0],
        tablica[1], tablica[2], tablica[3]);
    pm_jeden(tablica);
    printf("\n%f   %f   %f   %f\n", tablica[0],
        tablica[1], tablica[2], tablica[3]);
    return 0;
}

.686
.XMM
.model flat

public _pm_jeden

.data
tablica dd 1.0, 1.0, 1.0, 1.0

.code
_pm_jeden PROC
  push ebp
  mov  ebp, esp

  push edi
  mov edi, [ebp+8]

  movups   xmm5, [edi]
  movups   xmm6, tablica
  addsubps xmm5, xmm6
  movups   [edi], xmm5

  pop edi
  pop ebp
  ret
_pm_jeden ENDP
END

//INNA WERSJA
.686
.XMM
.model flat

public _pm_jeden

.data
tablica dd 4 dup (1.0)

.code
_pm_jeden PROC
  push ebp
  mov  ebp, esp

  push edi
  mov edi, [ebp+8]

  movups   xmm5, [edi]
  movups   xmm6, tablica
  addsubps xmm5, xmm6
  movups   [edi], xmm5

  pop edi
  pop ebp
  ret
_pm_jeden ENDP
END


#include <stdio.h> 
void dodawanie_SSE(float* a);
int main()
{
    float wyniki[4];
    dodawanie_SSE(wyniki);
    printf("\nWyniki = %f  %f  %f  %f\n",
        wyniki[0], wyniki[1], wyniki[2], wyniki[3]);
    return 0;
}

.686
.XMM
.model flat
public _dodawanie_SSE

.data
ALIGN 16
tabl_A dd 1.0, 2.0, 3.0, 4.0
tabl_B dd 2.0, 3.0, 4.0, 5.0
liczba db 1
tabl_C dd 3.0, 4.0, 5.0, 6.0

.code
_dodawanie_SSE  PROC
push      ebp
mov           ebp, esp
mov           eax, [ebp + 8]

movaps  xmm2, tabl_A

movaps  xmm3, tabl_B
movups  xmm4, tabl_C

addps  xmm2, xmm3
addps  xmm2, xmm4
movups[eax], xmm2

pop  ebp
ret
_dodawanie_SSE  ENDP
END






