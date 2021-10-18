-- Zadanie 16
module Main where

byHalf :: Int -> Int
byHalf n = quot n 2

 --Zwraca sume dzielnikow liczby
sumOfDivisors :: Int -> Int
sumOfDivisors n =  sum [x | x <- [1..byHalf n ], n `mod` x == 0]

-- Sprawdzenie, czy liczba jest obfita
isAbundant :: Int->Bool
isAbundant n = n >= 12 && sumOfDivisors n > n

listAbundantNumbers :: Int -> [Int]
listAbundantNumbers n = [x | x <- [12 .. n], isAbundant x ]

canBeSumofAbundant :: Int -> [Int] -> Bool
canBeSumofAbundant n v= any (\x -> any (\y -> x + y == n) v) v

-- Sprawdzenie danej liczby a pozniej nizej
lowestAbundantSumHelper :: Int -> [Int] -> Int
lowestAbundantSumHelper n v = if not(canBeSumofAbundant n v) then n else lowestAbundantSumHelper (n-1) v

lowestAbundantSum :: Int -> Int
lowestAbundantSum n  
    | n >= 20161 = 20161
    | otherwise = lowestAbundantSumHelper n (listAbundantNumbers n)

main :: IO()
main = do
    putStrLn "Podaj liczbe n:"
    x <- readLn
    print(lowestAbundantSum(x :: Int))


--0m27s



