%Zadanie 1 - Tree sort @Wojciech Panfil
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

%Zadanie 2 - Czy ciag jest graficzny @Łukasz Smolinski
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

%Zadanie 3 - Czy graf jest spójny @Wojciech Panfil
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
					X="Ciag nie jest graficzny"; 
					czy_potencjalnie_spojny(L), czy_spojny(L) ->
						X = "Z podanego ciagu powstanie graf spojny";
					X = "Z podanego ciagu nie powstanie graf spojny".