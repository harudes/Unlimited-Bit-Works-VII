--Funcion esPar:
esPar :: Int -> Bool
esPar a = (mod a 2 == 0)

--Funcion que calcule las raices
raices :: Float -> Float -> Float -> (Float,Float) 
raices a b c = (((-b)+sqrt((b^2)-(4*a*c)))/(2*a),((-b)-sqrt((b^2)-(4*a*c)))/(2*a))

--Mayor de edad
edad :: Int -> [Char] -> [Char]
edad x gender | x < 5 = "Infante"
              | x < 11 = "Nino"
              | x < 15 = "Puberto"
              | (x == 15 && gender == "Dama") = "Quinceanero"
              | (x == 17 && gender == "Varon")= "17anero"
              | x >= 21 = "Mayor de edad en EEUU"
              | x >= 18 = "Mayor de edad en Peru"
              | otherwise = "Menor de edad"



imprimir x | x==1 = "1"
           | otherwise = show(x) ++ imprimir(x-1)

factorial :: Integer -> Integer
factorial x | x==1 = 1
            |otherwise = x * factorial (x-1)

fibonacci :: Integer -> Integer
fibonacci x | x == 0 = 1
            | x == 1 = 1
            | otherwise = fibonacci (x-1) + fibonacci (x-2)

recorrer a | a == [] = ""
           | otherwise = show(head a) ++ "," ++ recorrer (tail a)

recorrerr (x:xs) | xs==[] = show x
                 | otherwise = show (x) ++ "," ++ recorrerr xs

funcion::[Char] -> [Char]
funcion x = y
            where y=(x++x)

busquedaBinaria [] _ _ _ = (-1)
busquedaBinaria arreglo l r x | l>r = (-1)
                              | arreglo !! medio == x = medio
                              | arreglo !! medio > x = busquedaBinaria arreglo l (medio-1) x
                              | arreglo !! medio < x = busquedaBinaria arreglo (medio+1) r x
                              where medio = (div (l + r) 2)

busquedaTernaria [] _ _ _ = (-1)
busquedaTernaria arreglo l r x | l > r = (-1)
                               | arreglo !! (fst medio) == x = (fst medio)
                               | arreglo !! (snd medio) == x = (snd medio)
                               | x < arreglo !! (fst medio) = busquedaTernaria arreglo l ((fst medio) - 1) x
                               | x > arreglo !! (fst medio) && x < arreglo !! (snd medio) = busquedaTernaria arreglo ((fst medio)+1) ((snd medio) - 1) x
                               | x > arreglo !! (snd medio) = busquedaTernaria arreglo ((snd medio)+1) r x
                               where medio = ( l + (div (r - l) 3) , r - (div (r - l) 3)) 

multiplyString s i | i == 0 = ""
                   | otherwise = s ++ multiplyString s (i-1)

printEscalera a i | a == [] = ""
                  | tail a == [] = multiplyString " " i ++ show (head a)
                  | otherwise = multiplyString " " i ++ show (head a) ++ "\n" ++ printEscalera (tail a) (i+1)


escalera a = putStrLn (printEscalera a 0)

data Lista tipo = Vacia | Nodo tipo (Lista tipo) deriving (Show)

crearLista a | a == [] = Vacia
             | otherwise = Nodo (head a) (crearLista (tail a))

crearListaVacia = Vacia

crearListaPredeterminada = Nodo 1 (Nodo 2 (Nodo 3 (Nodo 4 (Nodo 5 Vacia) ) ) ) 

printEscaleraLista Vacia _ = ""
printEscaleraLista (Nodo t Vacia ) i = multiplyString " " i ++ show t
printEscaleraLista (Nodo t t2 ) i = multiplyString " " i ++ show t ++ "\n" ++ printEscaleraLista t2 (i+1)

escaleraLista l = putStrLn (printEscaleraLista l 0)

