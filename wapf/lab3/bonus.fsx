let SHOW x = printf "%A\n" x

//### Exercise 3.1
//##Implement `parseScore`.

//#### --------------- Your code goes below --------------- *)
let parseScore (chars: char list): int option list =
    let foldFunc (acc: int option list) (c: char) =
        match acc, c with
        | _ , 'X' -> Some 10 :: acc
        | _ , '-' -> Some 0 :: acc
        | _ , '/' ->
            match acc with
            | Some firstRoll :: _ -> Some (10 - firstRoll) :: acc
            | _ -> acc
        | _ , (c: char) when c >= '0' && c <= '9' -> 
            Some (int (string c)) :: acc
        | _ , _ -> acc
    List.fold foldFunc [] chars |> List.rev

let ``exercise 3.1`` =
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
let countScore (scores: int list): int =
    let scoreFold (acc: int, prevScores: int list) score =
        match prevScores with
        | 10 :: x :: y :: rest -> // Strike
            if rest.Length > 0 then
                acc + 10 + x + y, x :: y :: rest
            else
                acc + 10 + x + y, [] // If in last round, player has two additional moves.
        | x :: y :: z :: rest when x + y = 10 -> // Spare
            if rest.Length > 1 then
                acc + 10 + z, z :: rest
            else
                acc + 10 + z, [] // If in last round, player has an additional move.
        | x :: y :: rest -> acc + x + y, rest
        | _ -> acc, []

    List.fold scoreFold (0, scores) scores |> fst


let ``exercise 3.2`` =
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
    let rec sequence' acc optionals =
        match optionals, acc with
        | [], _ -> Option.map List.rev acc
        | Some h :: t, Some acc -> sequence' (Some(h :: acc)) t
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

let ``bonus homework`` =
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

//** #### Value of ``bonus homework`` *)
SHOW ``bonus homework``

//EXPECTED RESULTS   
//[Some 300; Some 90; Some 100; Some 111; Some 92;
//   Some 90; Some 111; Some 150; Some 155; Some 187]

//////////////////////////////////////////////////////////////
/// Indeks: 184657
/// Imię: Wojciech
/// Nazwisko: Panfil
/// 
/// Podsumowanie zalizowanych zadan: Podstawowe bez uzycia rekurencji