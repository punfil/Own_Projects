-- Zadanie 16
module Main where
  
intSqrt :: Int -> Int 
intSqrt n = floor (sqrt (fromIntegral n))

sumOfDividorsCounter :: Int -> Int -> Int
sumOfDividorsCounter 0 n = 0
sumOfDividorsCounter d n =
  if n `mod` d == 0 then d + sumOfDividorsCounter (d-1) n else sumOfDividorsCounter (d-1) n

-- Zwraca sume, jego pomocnicza to sumOfDividorsCounter
sumOfDividors :: Int -> Int
sumOfDividors n = sumOfDividorsCounter (n-1) n

-- Sprawdzenie, czy liczba jest obfita
isAbundant :: Int->Bool
isAbundant n = n >= 12 && sumOfDividors n > n

canBeSumofAbundantChecker :: Int -> Int -> Bool
canBeSumofAbundantChecker beginning end
  | isAbundant beginning && isAbundant end && beginning <= end = True
  | beginning <= end = canBeSumofAbundantChecker (beginning+1) (end-1)
  | otherwise = False

--Sprawdza, czy dana liczba moze byc zapisana jako suma dwoch liczb obfitych
canBeSumofAbundant :: Int -> Bool
canBeSumofAbundant n
  | n < 24 = False
  | canBeSumofAbundantChecker 12 (n-12) = True
  | otherwise = False


-- Sprawdzenie danej liczby a pozniej nizej
lowestAbundantSum :: Int->Int
lowestAbundantSum n = if not(canBeSumofAbundant n) then n else lowestAbundantSum (n-1)


main :: IO()
main = do
    putStrLn "Podaj liczbe n:"
    x <- readLn
    print(lowestAbundantSum(x :: Int))






