%Zadanie 1 - Tree sort
dolacz( [],L,L ).
dolacz( [H|T],L,[H|X] ) :- dolacz( T,L,X ).

add(X,nil,t(X,nil,nil)).
add(X,t(Root,L,R),t(Root,L1,R)) :- X < Root, add(X,L,L1).
add(X,t(Root,L,R),t(Root,L,R1)) :- X >= Root, add(X,R,R1).


construct(L, T) :- construct(L,T,nil).

construct([],T,T).
construct([N|Ns],T,T0) :- add(N,T0,T1), construct(Ns,T,T1).

traverse(nil, []).
traverse(t(X, Left, Right), C) :-
	traverse(Left, R1),
	traverse(Right, R2),
	dolacz(R2, [X], N),
	dolacz(N, R1, C).
	%dolacz([X], R2, N), Wersja rosnaca
	%dolacz(R1, N, C).

tree_sort(L, X) :- construct(L, T), traverse(T, X).

%Zadanie 2 - Czy ciag jest graficzny
%Algorytm - https://en.wikipedia.org/wiki/Havel%E2%80%93Hakimi_algorithm
%-	Ciag jest graficzny jesli jego podciag jest graficzny
%-	Podciągiem jest ciąg z wykluczeniem największego wierzchołka n i z odjeciem od pierwych
% n wierzołków po nim liczby 1
%-	Podciąg nie jest graficzny jeśli jego długość jest mniejsza od liczby n oraz lub któryś
% element ciągu jest mniejszy od 0
%-	Podciąg jest minimalny, gdy składa się z 0
%-	Podciąg minimalny jest graficzny

dec(X,Y) :- Y is X-1.

czy_lista_nie_ma_ujemnej([]).
czy_lista_nie_ma_ujemnej([H|T]) :- H>=0 , czy_lista_nie_ma_ujemnej(T) .

dlugosc_listy([],0).
dlugosc_listy([_|T],N) :- dlugosc_listy(T,N1), N is N1+1.

zmniejsz_n_ele_o_1([],_,[]).
zmniejsz_n_ele_o_1(L,0,L).
zmniejsz_n_ele_o_1([H|T],N,[X|W]) :- X is H-1,dec(N,N1), zmniejsz_n_ele_o_1(T,N1,W).

minimalizuj([H|T], W) :-  tree_sort([H|T],[H1|T1]) ,dlugosc_listy(T1,N), N >= H ,
    zmniejsz_n_ele_o_1(T1,H1,W), czy_lista_nie_ma_ujemnej(W).

czy_minimalny([]).
czy_minimalny([H|T]) :- H == 0 , czy_minimalny(T).

suma_ele([],S) :- S = 0.
suma_ele([H|T],S) :- suma_ele(T,S1), S is S1 + H.

czy_graficzny(L) :-
    czy_minimalny(L),! ;
    minimalizuj(L,M),!,
    czy_graficzny(M).

czy_graficzny(L,X) :- czy_graficzny(L) ,!, X = "Ciag jest graficzny";
    X = "Ciag nie jest graficzny".

%Zadanie 3 - Czy graf jest spójny
%Algorytm zaczerpniety stad: http://szhorvat.net/pelican/hh-connected-graphs.html#connected
%Smallest-First Havel_Hakimi Algorithm
%Algorytm bazuje na algorytmie @Lukasz Smolinski, jednak do redukcji zawsze wybiera najmniejszy element.

%Zwraca ostatni element listy
ostatni_z_listy(X, [X]).
ostatni_z_listy(X, [_|Z]) :- ostatni_z_listy(X, Z).

%Wycina ostatni element listy jezeli jest zerem
wytnij_wlasciwe(0, L, W) :- bez_ostatniego(L, W).
wytnij_wlasciwe(_, L, L).

%Wycina ostatni element listy jezeli jest zerem
wytnij_zera(L, W) :- ostatni_z_listy(E, L), wytnij_wlasciwe(E, L, W).

%Odcina ostatni element listy, skraca liste o 1 od prawej
bez_ostatniego([_], []).
bez_ostatniego([X|Xs], [X|BezOstatniego]) :-
    bez_ostatniego(Xs, BezOstatniego).

%Znajduje najmniejszy element n i odejmuje go od n elementow (oprocz siebie). Element n zostanie usuniety z listy, a lista oczyszczona z zer.
minimalizuj_najmniejszym([H|T], W) :-  tree_sort([H|T],[H1|T1]) , ostatni_z_listy(C, [H1|T1]) , C\=0, dlugosc_listy([H1|T1],N), N >= H , bez_ostatniego([H1|T1], O),
    zmniejsz_n_ele_o_1(O,C,K), wytnij_zera(K, W), czy_lista_nie_ma_ujemnej(W).

%Sprawdza czy suma stopni wierzcholkow jest wieksza od liczba_wierzcholkow-1
czy_potencjalnie_spojny(L) :- suma_ele(L, S), dlugosc_listy(L, D), S >= 2*(D-1).

czy_spojny(L) :-
	czy_minimalny(L),!; %Sprawdzi, czy lista jest pusta
	minimalizuj_najmniejszym(L, M),!, %Minimalizuje liste po najmniejszym elemencie
	czy_spojny(M). %rekurencyjne wywolanie
czy_spojny(L, X) :- not(czy_graficzny(L))->
					X="Nie, ciag nie jest graficzny";
					czy_potencjalnie_spojny(L), czy_spojny(L) ->
						X = "Tak, z podanego ciagu powstanie graf spojny";
					X = "Nie, z podanego ciagu nie powstanie graf spojny".

%Zadanie 5.4.3 - Gra planszowa
% zwieksz_nty( Lista, Indeks, O_ile, Nowa_lista
% zwiększa n-ty element listy o podaną liczbę. (Liczba może być ujemna)
zwieksz_nty([H|T], 0, O_ile, [X|T]) :- X is H+O_ile.
zwieksz_nty([H|T], I, O_ile, [H|T2]) :- I > -1, Next_I is I-1, zwieksz_nty(T, Next_I, O_ile, T2), !.


% poprawny_ruch(Plansza, Pole, Ile_pól_w_prawo)
% sprawdza, czy dla danego pola planszy, ruch o daną ilość pól w prawo jest poprawny (zasady z pp.3)
poprawny_ruch_pp3(Plansza, Pole, Ruch) :-
    (Ruch = 1; Ruch = 3),
    nth0( Pole, Plansza, X), X > 0,
    Pole_docelowe is Pole+Ruch,
    nth0( Pole_docelowe, Plansza, Y), Y < 2.

% przenosi pionek z pola {Pole} o {Ruch} pól w prawo, zwraca wynik w zmiennej Nowa_plansza.
wykonaj_ruch(Plansza, Pole, Ruch, Nowa_plansza) :-
    zwieksz_nty(Plansza, Pole, -1, Plansza_tmp),
    Pole_docelowe is Pole+Ruch,
    zwieksz_nty(Plansza_tmp, Pole_docelowe, 1, Nowa_plansza).



% sprawdza, czy dla danej planszy istnieje jakikolwiek poprawny ruch (zasady z pp.3)
czy_istnieje_ruch_pp3(Plansza) :-
    poprawny_ruch_pp3(Plansza, _, _).



% określa czy dla podanej planszy gracz aktualnie wykonujący ruch i grający wg zasad z pp.3 wygra grę
czy_wygrywa_pp3(Plansza) :-
    poprawny_ruch_pp3(Plansza, Pole, Ruch),
    wykonaj_ruch(Plansza, Pole, Ruch, Nowa_plansza),
    write(Nowa_plansza),
    not( czy_wygrywa_pp4(Nowa_plansza) ),
    nl.



% poprawny_ruch(Plansza, Pole, Ile_pól_w_prawo)
% sprawdza, czy dla danego pola planszy, ruch o daną ilość pól w prawo jest poprawny (zasady z pp.4)
poprawny_ruch_pp4(Plansza, Pole, Ruch) :-
    (Ruch = 1; Ruch = 2; Ruch = 3),
    nth0( Pole, Plansza, X), X > 0,
    Pole_docelowe is Pole+Ruch,
    nth0( Pole_docelowe, Plansza, Y), Y < 1.


% czy_istnieje_ruch(Plansza).
% sprawdza, czy dla danej planszy istnieje jakikolwiek poprawny ruch (zasady z pp.4)
czy_istnieje_ruch_pp4(Plansza) :-
    poprawny_ruch_pp4(Plansza, _, _).


% określa czy dla podanej planszy gracz aktualnie wykonujący ruch i grający wg zasad z pp.4 wygra grę
czy_wygrywa_pp4(Plansza) :-
    (
    	(
	    	poprawny_ruch_pp4(Plansza, Pole, Ruch),
	        Ruch = 3,
	    	wykonaj_ruch(Plansza, Pole, Ruch, Nowa_plansza)
	    );
	    (
	    	poprawny_ruch_pp4(Plansza, Pole_1, Ruch_1),
	        Ruch_1 = 2,
	    	wykonaj_ruch(Plansza, Pole_1, Ruch_1, Nowa_plansza_1),

	    	poprawny_ruch_pp4(Nowa_plansza_1, Pole_2, Ruch_2),
	        Ruch_2 = 1,
	    	wykonaj_ruch(Nowa_plansza_1, Pole_2, Ruch_2, Nowa_plansza_2),
	        Nowa_plansza = Nowa_plansza_2
	    );
	    (
	    	poprawny_ruch_pp4(Plansza, Pole_1, Ruch_1),
	        Ruch_1 = 1,
	    	wykonaj_ruch(Plansza, Pole_1, Ruch_1, Nowa_plansza_1),
	    	poprawny_ruch_pp4(Nowa_plansza_1, Pole_2, Ruch_2),
	        Ruch_2 = 1,
	    	wykonaj_ruch(Nowa_plansza_1, Pole_2, Ruch_2, Nowa_plansza_2),
	    	poprawny_ruch_pp4(Nowa_plansza_2, Pole_3, Ruch_3),
	        Ruch_3 = 1,
	    	wykonaj_ruch(Nowa_plansza_2, Pole_3, Ruch_3, Nowa_plansza_3),
	        Nowa_plansza = Nowa_plansza_3
	    )
    ),
    write(Nowa_plansza),
    not( czy_wygrywa_pp3(Nowa_plansza) ),
    nl.


czy_wygrywa(Plansza) :- czy_wygrywa_pp4(Plansza).

