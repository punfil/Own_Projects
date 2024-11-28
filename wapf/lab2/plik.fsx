let SHOW x = printf "%A\n" x
// ### Exercise 2.1
// ## Implement `parseNumber` function.
// ## You might find following functions useful:
// ## `ToCharArray()` (String member), `Array.forall`, `System.Char.IsDigit`, `System.Int32.Parse`.

// #### --------------- Your code goes below ---------------
let parseNumber (value: string): int option =
    if value.ToCharArray() |> Array.forall(fun (c: char) -> c |> System.Char.IsDigit) then
        Some(value |> System.Int32.Parse)
    else
        None

let ``exercise 2.1``: int option = parseNumber "42"

// #### Value of ``exercise 2.1``

SHOW ``exercise 2.1``

// ### Exercise 2.2
// ## Declare `splitBy` function - a wrapper function arround `Split` method from `String` object.
// ## Hints: Use `Split` method from `String` and `Array.toList` function to convert array to list type.
// #### --------------- Your code goes below ---------------
let splitBy (separator: char) (str: string): string list =
    str.Split separator |> Array.toList

let ``exercise 2.2``: string list = "1,3,5,8,10" |> splitBy ','

// #### Value of ``exercise 2.2``
SHOW ``exercise 2.2``

// ### Exercise 3.1
// ##Define `Operator` and `Symbol` Discriminated Union Types.
// ##
// ##`Symbol` should use `Operator` as field in one case
// ##

// #### --------------- Your code goes below ---------------
// `Int` is used here only so that the code compiles.
// Remove it and instead define proper Discriminated Union cases:
// Operator might be one of the following: Plus, Minus, Multiply or Divide
type Operator =
    | Plus
    | Minus
    | Multiply
    | Divide

// Same as above:
// Symbol might be either a NumSymbol (with int) or OpSymbol (with Operator)
type Symbol =
    | NumSymbol of int
    | OpSymbol of Operator

// ### Exercise 3.2
// # With help of pattern matching, implement `apply` function.

// #### --------------- Your code goes below ---------------
let apply (operator: Operator) (left: int) (right: int): int =
    match operator with
    | Plus -> left + right
    | Minus -> left - right
    | Multiply -> left * right
    | Divide -> left / right

// test the function, e.g. `apply Divide 15 4`
let ``exercise 3.2``: int = apply Multiply 15 4

// #### Value of ``exercise 3.2``
SHOW ``exercise 3.2``

// ### Exercise 3.3
// ##Implement `parseSymbol` - try parse all operators first, and then in nested `match` expression use `parseNumber` function

// #### --------------- Your code goes below ---------------
let parseSymbol (token: string): Symbol option =
    match token with
    | "+" -> Some(OpSymbol Plus)
    | "-" -> Some(OpSymbol Minus)
    | "*" -> Some(OpSymbol Multiply)
    | "/" -> Some(OpSymbol Divide)
    | (x: string) -> match parseNumber x with
                     | Some n -> Some(NumSymbol n)
                     | None -> None

let ``exercise 3.3``: Symbol option list =
    List.map parseSymbol [ "+"; "/"; "12"; "uups" ]
// #### Value of ``exercise 3.3``
SHOW ``exercise 3.3``

// ### Helper function "sequenceOpts"
// ##if all elements are Some values, return Some of those values
// ##otherwise if there's at least one None, return None

let rec sequenceOpts (optionals: 'a option list): 'a list option =
    match optionals with
    | [] -> Some []
    | None :: _ -> None
    | Some h :: t -> sequenceOpts t |> Option.map (fun (t: 'a list) -> h :: t)

// ### Exercise 3.4
// ##Implement `parseSymbols`. Useful functions: `List.map`, `sequenceOpts` as well as `splitBy` and `parseSymbol`

// #### --------------- Your code goes below ---------------
let parseSymbols (expression: string): Symbol list option =
    splitBy ' ' expression |> List.map(fun (c: string) -> parseSymbol c) |> sequenceOpts

let ``exercise 3.4``: Symbol list option = "1 2 / +" |> parseSymbols
let ``exercise 3.41``: Symbol list option = "1 2 / + oops" |> parseSymbols

// #### Value of ``exercise 3.4``
SHOW ``exercise 3.4``
SHOW ``exercise 3.41``

//////////////////////////////////////////////////////////////
// Indeks: 184657
// Imię: Wojciech
// Nazwisko: Panfil
// 
// Podsumowanie zrealizowanych zadan: