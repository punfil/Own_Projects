--Implementations of some math problem solving algorithms for Programming Languages Classes.


-- Zadanie 16 Dla danej liczby n podaj najwieksza mniejsza rowna podanej, ktora nie moze zostac zapisana za pomoca sumy dwoch liczb obfitych
module Main where

--Dzieli liczbe calkowita na dwa odrzucajac reszte
byHalf :: Int -> Int
byHalf n = quot n 2

 --Zwraca sume dzielnikow liczby, uzywa wbudowanego generatora, szukamy do n/2 bo nie szukamy dzielnikow pierwszych, lecz wszystkich
sumOfDivisors :: Int -> Int
sumOfDivisors n =  sum [x | x <- [1..byHalf n ], n `mod` x == 0]

-- Sprawdzenie, czy liczba jest obfita (suma dzielnikow liczby > liczby, liczba musi byc wieksza od 12)
isAbundant :: Int->Bool
isAbundant n = n >= 12 && sumOfDivisors n > n

--Stworz liste liczb obfitych, wykorzystuje wbudowany generator list oraz warunek isAbundantx
listAbundantNumbers :: Int -> [Int]
listAbundantNumbers n = [x | x <- [12 .. n], isAbundant x ]

--Sprawdza, czy dana liczba moze byc przedstawiona jako suma dwoch liczb obfitych, jezeli trafi na jedna taka przerywa
canBeSumofAbundant :: Int -> [Int] -> Bool
canBeSumofAbundant n v= any (\x -> any (\y -> x + y == n) v) v

-- Sprawdzenie danej liczby a pozniej nizej, kolejnej mniejszej
lowestAbundantSumHelper :: Int -> [Int] -> Int
lowestAbundantSumHelper n v = if not(canBeSumofAbundant n v) then n else lowestAbundantSumHelper (n-1) v

--Funkcja poczatkowa, dzieki niej mozemy raz obliczac v, czyli listeliczbobfitych. 20161 to najwieksza liczba niezapisywalna jako suma dwoch obfitych, wikipedia https://en.wikipedia.org/wiki/Abundant_number
lowestAbundantSum :: Int -> Int
lowestAbundantSum n  
    | n >= 20161 = 20161
    | otherwise = lowestAbundantSumHelper n (listAbundantNumbers n)

main :: IO()
main = do
    putStrLn "Podaj liczbe n:"
    x <- readLn
    print(lowestAbundantSum(x :: Int))



