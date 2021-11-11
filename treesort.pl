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
	%dolacz([X], R2, N),
	%dolacz(R1, N, C).

tree_sort(L, X) :- construct(L, T), traverse(T, X).