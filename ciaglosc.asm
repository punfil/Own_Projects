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

