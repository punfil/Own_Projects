// 1. GETTING SOME DATA
open System.IO
let trainingFile: string array =
    File.ReadAllLines("trainingsample.csv");
let validationFile: string array =
    File.ReadAllLines(path = "validationsample.csv");

// 2. EXTRACTING COLUMNS
let extractColumns (file: string[]) =
    file
    |> Array.map (fun (c: string) -> c.Split(','))

// 3. CLEANING UP HEADERS
let cleanHeaders (rows: string[][]) =
    rows.[1..]

// 4. CONVERTING FROM STRINGS TO INTS
open System
let convertStringsToInts (rows: string[][]) =
    rows
    |> Array.map (Array.map Convert.ToInt32)

// 5. CONVERTING ARRAYS TO RECORDS
type Example = { Label: int; Pixels: int[] }
let convertToRecords (rows: int[][]) =
    rows |> Array.map (fun (a: int array) -> { Label = a.[0]; Pixels = a.[1..] })

// 5.5 CONVERT TO SETS
let convertFile (file: string[]) =
    file
    |> extractColumns
    |> cleanHeaders
    |> convertStringsToInts
    |> convertToRecords

// Load set
let trainingSet: Example array =
    trainingFile
    |> convertFile
let validationSet: Example array =
    validationFile
    |> convertFile

// 6. COMPUTING DISTANCES
let distanceEuclidean (p1: int[]) (p2: int[]) =
    (Array.map2 (fun (x1: int) (x2: int) -> float((x1 - x2) * (x1 - x2))) p1 p2)
    |> Array.sum
    |> sqrt

let distanceManhattan (p1: int[]) (p2: int[]) =
    Array.map2 (fun (x1: int) (x2: int) -> abs(x1 - x2)) p1 p2
    |> Array.sum

let distanceChebyshev (p1: int[]) (p2: int[]) =
    Array.map2 (fun (x1: int) (x2: int) -> abs(x1 - x2)) p1 p2
    |> Array.max


// 7. WRITING THE CLASSIFIER FUNCTION
let classify1NN (distanceFun) (point: int[]) =
    (trainingSet
    |> Array.minBy (fun (x: Example) -> distanceFun x.Pixels point)
    ).Label

let mostFrequentElement (elements: int[]) =
    (elements
    |> Seq.countBy id
    |> Seq.maxBy snd
    |> fst)

let classifyKNN (distanceFun) (K:int) (point: int[]) =
    trainingSet
    |> Array.sortBy (fun (x: Example) -> distanceFun x.Pixels point)
    |> Array.take K
    |> Array.map (fun (x: Example) -> x.Label)
    |> mostFrequentElement
    

let equals (expected: int) (actual: int) =
    if expected = actual then 1 else 0


let distanceFun = distanceChebyshev
// 8. 1NN
let correct1NN: int =
    validationSet
    |> Array.map( fun (e: Example) -> equals (e.Pixels |> classify1NN distanceFun) e.Label)
    |> Array.sum
printfn "1NN - validation accuracy %.2f%%." (float(correct1NN) / float(Array.length validationSet) * 100.0)

// 9. 2NN
let correct2NN: int = 
    validationSet
    |> Array.map( fun (e: Example) -> equals (e.Pixels |> classifyKNN distanceFun (2)) e.Label)
    |> Array.sum
printfn "2NN - validation accuracy %.2f%%." (float(correct2NN) / float(Array.length validationSet) * 100.0)

// 9. KNN
let K = 10
let correctKNN: int = 
    validationSet
    |> Array.map( fun (e: Example) -> equals (e.Pixels |> classifyKNN distanceFun K) e.Label)
    |> Array.sum
printfn "10NN - validation accuracy %.2f%%." (float(correctKNN) / float(Array.length validationSet) * 100.0)

///////podsumowanie
// Wojciech Panfil 184657
// Wyniki:
// - Prosty klasyfikator n=1, metryka euklidesowa -> 94.4% dokładności
// - Klasyfikator n=2, metryka euklidesowa -> 94.4% dokładności
// - Klasyfikator n=10, metryka euklidesowa -> 93.6% dokładności
// - Prosty klasyfikator n=1, metryka manhattan -> 93.4% dokładności
// - Klasyfikator n=2, metryka manhattan -> 93.4% dokładności
// - Klasyfikator n=10, metryka manhattan -> 92.4% dokładności
// - Prosty klasyfikator n=1, metryka czebyszewa -> 63.4% dokładności
// - Klasyfikator n=2, metryka czebyszewa -> 66.4% dokładności
// - Klasyfikator n=10, metryka czebyszewa -> 63.4% dokładności