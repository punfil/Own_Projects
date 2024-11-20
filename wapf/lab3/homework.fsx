let SHOW x = printf "%A\n" x
type Tree =
    | Empty
    | Node of int * Tree * Tree

//#### Binary Tree as DU

let tree =
    Node (8,
         Node(3, Node(1, Empty, Empty), Node(6, Node(4, Empty, Empty), Node(7, Empty, Empty))),
         Node(10, Empty, Node(14, Node(13, Empty, Empty), Empty)))

//### Exercise 1.4
//##Insert element into Binary Search Tree

//#### --------------- Your code goes below --------------- *)
let rec insertBST (value: int) (tree: Tree): Tree =
  match tree with
  | Empty -> Node(value, Empty, Empty)
  | Node (v: int, left: Tree, right: Tree) when value <= v -> Node(v, (insertBST value left), right)
  | Node (v: int, left: Tree, right: Tree) -> Node(v, left, (insertBST value right))

let ``exercise 1.4 raw``: Tree = insertBST 5 tree 
//** #### Value of ``exercise 1.4`` *)
SHOW ``exercise 1.4 raw``

//### Exercise 3.1
//##Implement `parseScore`.

//#### --------------- Your code goes below --------------- *)
let rec parseScore (chars: char list): int option list =
    match chars with
    | [] -> []
    | 'X' :: rest -> Some 10 :: parseScore rest
    | c :: '/' :: rest when c = '-' || (c >= '0' && c <= '9') ->
        let firstRoll: int = if c = '-' then 0 else int (string c)
        Some firstRoll :: Some (10 - firstRoll) :: parseScore rest
    | '-' :: rest -> Some 0 :: parseScore rest
    | c :: rest when c >= '0' && c <= '9' -> Some (int (string c)) :: parseScore rest
    | _ :: rest -> parseScore rest

let ``exercise 3.1``: int option list =
    parseScore [ 'X'
                 '4'
                 '/'
                 '2'
                 '-'
                 'N' ]
//** #### Value of ``exercise 3.1`` *)
SHOW ``exercise 3.1``

//### Exercise 3.2
//##Implement `countScore`

//#### --------------- Your code goes below --------------- *)
let rec countScore (scores: int list): int =
    match scores with
    | 10 :: x :: y :: rest -> // Strike
        if rest.Length > 0 then
            10 + x + y + countScore (x :: y :: rest)
        else
            10 + x + y // If in last round, player has two additional moves.
    | x :: y :: z :: rest when x + y = 10 -> // Spare
        if rest.Length > 1 then
            10 + z + countScore (z :: rest)
        else
            10 + z // If in last round, player has one additional move.
    | x :: y :: rest -> x + y + countScore rest
    | _ -> 0

let ``exercise 3.2``: int list =
    [ [ 10
        10
        10
        10
        10
        10
        10
        10
        10
        10
        10
        10 ]
      [ 9
        0
        9
        0
        9
        0
        9
        0
        9
        0
        9
        0
        9
        0
        9
        0
        9
        0
        9
        0 ]
      [ 5
        5
        5
        5
        5
        5
        5
        5
        5
        5
        5
        5
        5
        5
        5
        5
        5
        5
        5
        5
        5 ]
      [ 10
        9
        1
        5
        5
        7
        2
        10
        10
        10
        9
        0
        8
        2
        9
        1
        10 ] ]
    |> List.map countScore
//** #### Value of ``exercise 3.2`` *)
SHOW ``exercise 3.2``

//### sequenceOpts function *)
let sequenceOpts (optionals: 'a option list): 'a list option =
    let rec sequence' (acc: 'b list option) (optionals: 'b option list) =
        match optionals, acc with
        | [], _ -> Option.map List.rev acc
        | Some h :: t, Some (acc: 'b list) -> sequence' (Some(h :: acc)) t
        | _ -> None

    sequence' (Some []) optionals


//### Homework 1
//##Implement `bowlingScore`.

//###Hint: Use `sequenceOpts` to convert from list of options to option of list
let bowlingScore (score: string): int option =
    let parsedScore: int list option = score |> Seq.toList |> parseScore |> sequenceOpts
    match parsedScore with
    | None -> None
    | Some s -> Some(countScore s)

let ``homework 1`` =
    [ "XXXXXXXXXXXX"
      "9-9-9-9-9-9-9-9-9-9-"
      "9--/9-9-9-9-9-9-9-9-"
      "X-/9-9-9-9-9-9-9-9-"
      "9-X9-9-X--9-9-9-9-"
      "9-9-9-9-9-9-9-9-9-9-"
      "9-9-9-9-9-9-9-9-9-XXX"
      "5/5/5/5/5/5/5/5/5/5/5"
      "5/5/5/5/5/5/5/5/5/5/X"
      "X9/5/72XXX9-8/9/X" ]
    |> List.map bowlingScore

//** #### Value of ``homework 1`` *)
SHOW ``homework 1``

//EXPECTED RESULTS   
//[Some 300; Some 90; Some 100; Some 111; Some 92;
//   Some 90; Some 111; Some 150; Some 155; Some 187]


//### Homework 2
//###Write new, **tail-recursive** versions of `parseScore` and `countScore`.
//###Implement `bowlingScoreTail` to use those 2 new functions

let rec parseScoreTail (chars: char list) (acc: int option list): int option list =
    match chars with
    | [] -> List.rev acc
    | 'X' :: rest -> parseScoreTail rest (Some 10 :: acc)
    | '-' :: '/' :: rest -> parseScoreTail rest (Some 0 :: Some 10 :: acc)
    | c :: '/' :: rest when c = '-' || (c >= '0' && c <= '9') ->
        let firstRoll: int = if c = '-' then 0 else int (string c)
        parseScoreTail rest (Some (10 - firstRoll) :: Some firstRoll :: acc)
    | '-' :: rest -> parseScoreTail rest (Some 0 :: acc)
    | c :: rest when c >= '0' && c <= '9' -> parseScoreTail rest (Some (int (string c)) :: acc)
    | _ :: rest -> parseScoreTail rest acc

let rec countScoreTail (scores: int list) (acc: int): int =
    match scores with
    | [] -> acc
    | 10 :: x :: y :: rest -> // Strike
        if rest.Length > 0 then
            countScoreTail (x :: y :: rest) (acc + 10 + x + y)
        else
            acc + 10 + x + y // If in last round, player has two additonal moves.
    | x :: y :: z :: rest when x + y = 10 -> // Spare
        if rest.Length > 1 then
            countScoreTail (z :: rest) (acc + 10 + z)
        else
            acc + 10 + z // If in last round, player has one additional move.
    | x :: y :: rest -> countScoreTail rest (acc + x + y)
    | _ -> acc

let bowlingScoreTail (score: string): int option =
    let parsedScore = score |> Seq.toList |> (fun chars -> parseScoreTail chars []) |> sequenceOpts
    match parsedScore with
    | None -> None
    | Some s -> Some(countScoreTail s 0)

let ``homework 2``: int option list =
    [ "XXXXXXXXXXXX"
      "9-9-9-9-9-9-9-9-9-9-"
      "9--/9-9-9-9-9-9-9-9-"
      "X-/9-9-9-9-9-9-9-9-"
      "9-X9-9-X--9-9-9-9-"
      "9-9-9-9-9-9-9-9-9-9-"
      "9-9-9-9-9-9-9-9-9-XXX"
      "5/5/5/5/5/5/5/5/5/5/5"
      "5/5/5/5/5/5/5/5/5/5/X"
      "X9/5/72XXX9-8/9/X" ]
    |> List.map bowlingScoreTail 
//** #### Value of ``homework 2`` *)
SHOW ``homework 2``

//EXPECTED RESULTS   
//[Some 300; Some 90; Some 100; Some 111; Some 92;
//   Some 90; Some 111; Some 150; Some 155; Some 187]

//////////////////////////////////////////////////////////////
// Indeks: 184657
// Imię: Wojciech
// Nazwisko: Panfil
// 
// Podsumowanie zrealizowanych zadan: 1.4 + exercise 3x + Homework 1 + Homework 2