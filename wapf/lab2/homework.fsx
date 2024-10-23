let SHOW x = printf "%A\n" x
// ### Exercise 2.1
// ## Implement `parseNumber` function.
// ## You might find following functions useful:
// ## `ToCharArray()` (String member), `Array.forall`, `System.Char.IsDigit`, `System.Int32.Parse`.

// #### --------------- Your code goes below ---------------
let parseNumber (value: string): int option =
    if value.ToCharArray() |> Array.forall System.Char.IsDigit then
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
    | OpenParenthesis
    | CloseParenthesis

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
let ``exercise 3.2``: int = apply Divide 15 4

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
    | "(" -> Some(OpenParenthesis)
    | ")" -> Some(CloseParenthesis)
    | _ -> match parseNumber token with
           | Some (number: int) -> Some(NumSymbol number)
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

// #### Value of ``exercise 3.4``
SHOW ``exercise 3.4``


// ### Homework 4.1
// ##Implement `computeonp` function (AiSD or [Wiki](https://pl.wikipedia.org/wiki/Odwrotna_notacja_polska)). Hint: `::` is "right-associative"

// #### --------------- Your code goes below ---------------
let rec computeonp (stack: int list) (symbols: Symbol list): int option =
    match symbols with
    | [] -> 
        match stack with
        | [ result: int ] -> Some(result)
        | _ -> None
    | NumSymbol numValue :: tail -> computeonp (numValue :: stack) tail
    | OpSymbol operator :: tail ->
        match stack with
        | operand2 :: operand1 :: stackTail -> computeonp ((apply operator operand1 operand2) :: stackTail) tail
        | _ -> None
    | _ -> None


// test the function, e.g. `computeonp [] [NumSymbol 4; NumSymbol 2; OpSymbol Multiply]`
// Important!!!! Replace the None with commented out assignment o computeonp
let ``homework 4.1``: int option = computeonp [] [NumSymbol 4; NumSymbol 2; OpSymbol Multiply]

// #### Value of ``exercise 4.1``
SHOW ``homework 4.1``

// ### Homework 4.2
// ##Using `parseSymbols` and `compute`, write `onp` function

// #### --------------- Your code goes below ---------------
let onp (expression: string): int option =
    match parseSymbols expression with
    | Some (list: Symbol list) -> computeonp [] list
    | None -> None

let ``homework 4.2``: int option = onp "2 7 + 3 / 14 3 - 4 * + 3 +"

// #### Value of ``exercise 4.2``
SHOW ``homework 4.2``

// ### Homework 4.3
// ##Implement `conv2onp` function (AiSD or (https://pl.wikipedia.org/wiki/Odwrotna_notacja_polska)).

// #### --------------- Your code goes below ---------------
let symbolPrecedence symbol =
    match symbol with
    | OpSymbol Plus | OpSymbol Minus -> 1
    | OpSymbol Multiply | OpSymbol Divide -> 2
    | _ -> 0 // Number or ()

let rec processOperator (operator: Operator) (stack: Symbol list) (output: Symbol list) =
    match stack with
    | symbol :: tail when symbolPrecedence symbol >= symbolPrecedence (OpSymbol operator) ->
        processOperator operator tail (symbol :: output)
    | _ -> (OpSymbol operator :: stack, output)

let rec popTillOpenParenthesis (stack: Symbol list) (output: Symbol list) =
    match stack with
    | OpSymbol _ as symbol :: tail -> popTillOpenParenthesis tail (symbol :: output)
    | OpenParenthesis :: tail -> (tail, output)
    | _ -> (stack, output)

let rec _conv2onp (stack: Symbol list) (tokens: string list) (output: Symbol list) =
    match tokens with
    | [] -> Some((List.rev output) @ stack)
    | token :: tail ->
        match parseSymbol token with
        | Some (NumSymbol (number: int)) -> _conv2onp stack tail (NumSymbol number :: output)
        | Some (OpSymbol (operator: Operator)) ->
            let (stack2: Symbol list), (output2: Symbol list) = processOperator operator stack output
            _conv2onp stack2 tail output2
        | Some(OpenParenthesis) -> _conv2onp (OpenParenthesis :: stack)tail output
        | Some (CloseParenthesis) -> 
            let (stack2: Symbol list), (output2: Symbol list) = popTillOpenParenthesis stack output
            _conv2onp stack2 tail output2
        | _ -> None

let conv2onp (expression: string): Symbol list option =
    match splitBy ' ' expression with
    | [] -> None
    | (tokens: string list) -> _conv2onp  [] tokens []

let ``homework 4.3``: Symbol list option =
    conv2onp "( 2 + 5 ) * 3 - 4 * ( 16 + 5 )"

// #### Value of ``exercise 4.3``

SHOW ``homework 4.3``

// ### Homework 4.4

// #### --------------- Your code goes below ---------------
let compute (expression: string): int option =
    match conv2onp expression with
    | None -> None
    | Some(symbols) ->
        match symbols with
        | [] -> None
        | symbols -> computeonp [] symbols


let ``homework 4.4`` =
    compute "( 2 + 5 ) * 3 - 4 * ( ( 16 - 1 ) * 2 + 5 )"

// #### Value of ``exercise 4.4``
SHOW ``homework 4.4``

// ### Homework 5.1
// ##Implement `computeonp` function (AiSD or [Wiki](https://pl.wikipedia.org/wiki/Odwrotna_notacja_polska)). Hint: `::` is "right-associative"

// #### --------------- Your code goes below ---------------
let computeonpFold (symbols: Symbol list): int option =
    let folder (stack: int list) (symbol: Symbol) =
        match symbol, stack with
        | NumSymbol numValue, _ -> numValue :: stack
        | OpSymbol operator, operand2 :: operand1 :: stackTail ->
            let result = apply operator operand1 operand2
            result :: stackTail
        | _ -> []

    let finalStack = List.fold folder [] symbols
    match finalStack with
    | [result] -> Some(result)
    | _ -> None


// test the function, e.g. `computeonp [] [NumSymbol 4; NumSymbol 2; OpSymbol Multiply]`
// Important!!!! Replace the None with commented out assignment o computeonp
let ``homework 5.1``: int option = computeonpFold [NumSymbol 4; NumSymbol 2; OpSymbol Multiply]

// #### Value of ``exercise 5.1``
SHOW ``homework 5.1``

// ### Homework 5.2
// ##Using `parseSymbols` and `compute`, write `onp` function

// #### --------------- Your code goes below ---------------
let onpFold (expression: string): int option =
    match parseSymbols expression with
    | Some (list: Symbol list) -> computeonpFold list
    | None -> None

let ``homework 5.2``: int option = onpFold "2 7 + 3 / 14 3 - 4 * + 3 +"

// #### Value of ``exercise 5.2``
SHOW ``homework 5.2``


// ### Homework 5.3
// ##Implement `conv2onp` function (AiSD or (https://pl.wikipedia.org/wiki/Odwrotna_notacja_polska)).

// #### --------------- Your code goes below ---------------

let processOperatorFold (operator: Operator) (stack: Symbol list) (output: Symbol list) =
    let folder (acc: Symbol list * Symbol list * bool) symbol =
        let (newStack, newOutput, isLowerPrecedenceEncountered) = acc

        if isLowerPrecedenceEncountered then
            (symbol :: newStack, newOutput, true)
        else
            if symbolPrecedence symbol >= symbolPrecedence (OpSymbol operator) then
                (newStack, symbol :: newOutput, false)
            else
                (symbol :: newStack, newOutput, true)

    let (finalStack, finalOutput, _) = List.fold folder ([], output, false) stack
    
    ((OpSymbol operator :: (List.rev finalStack) ), finalOutput)


let popTillOpenParenthesisFold (stack: Symbol list) (output: Symbol list) =
    let folder (accStack: Symbol list, accOutput: Symbol list, flag: bool) symbol =
        match symbol with
        | OpSymbol _ when not flag -> 
            (accStack, symbol :: accOutput, false)
        | OpenParenthesis when not flag -> 
            (accStack, accOutput, true)
        | _ when not flag -> 
            (symbol :: accStack, accOutput, false)
        | _ -> 
            (symbol :: accStack, accOutput, true)

    let (finalStack, finalOutput, _) = List.fold folder ([], output, false) stack
    let finalStackRev: Symbol list = List.rev finalStack

    match List.tryFindIndex ((=) OpenParenthesis) finalStackRev with
    | Some (index: int) -> 
        let newStack: Symbol list = List.skip (index) finalStackRev
        (newStack, finalOutput)
    | None -> (finalStackRev, finalOutput)


let _conv2onpFold (tokens: string list): Symbol list option =
    let folder (stack: Symbol list, output: Symbol list) token =
        match parseSymbol token with
        | Some (NumSymbol (number: int)) -> 
            (stack, NumSymbol number :: output)
        | Some (OpSymbol (operator: Operator)) -> 
            let (stack2: Symbol list), (output2: Symbol list) = processOperatorFold operator stack output
            (stack2, output2)
        | Some(OpenParenthesis) -> 
            (OpenParenthesis :: stack, output)
        | Some (CloseParenthesis) ->
            let (stack2: Symbol list), (output2: Symbol list) = popTillOpenParenthesisFold stack output
            (stack2, output2)
        | _ ->  ([], [])

    let (finalStack: Symbol list), (finalOutput: Symbol list) = List.fold folder ([], []) tokens

    Some((List.rev finalOutput) @ finalStack)

let conv2onpFold (expression: string): Symbol list option =
    match splitBy ' ' expression with
    | [] -> None
    | (tokens: string list) -> _conv2onpFold tokens

let ``homework 5.3``: Symbol list option =
    conv2onpFold "( 2 + 5 ) * 3 - 4 * ( 16 + 5 )"

// #### Value of ``exercise 5.3``

SHOW ``homework 5.3``

// ### Homework 5.4

// #### --------------- Your code goes below ---------------
let computeFold (expression: string): int option =
    match conv2onpFold expression with
    | None -> None
    | Some(symbols) ->
        match symbols with
        | [] -> None
        | symbols -> computeonpFold  symbols

let ``homework 5.4`` =
    computeFold "( 2 + 5 ) * 3 - 4 * ( ( 16 - 1 ) * 2 + 5 )"

// #### Value of ``exercise 5.4``
SHOW ``homework 5.4``


//////////////////////////////////////////////////////////////
// Indeks: 184657
// Imię: Wojciech
// Nazwisko: Panfil
// 
// Podsumowanie zrealizowanych zadan:
// 4.1, 4.2, 4.3, 4.4, 5.1, 5.2, 5.3, 5.4