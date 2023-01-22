#Test double Loops in a function
(
   (SET C '5')
   (SET B '0')
   (WHILE (LESS '0' C)(
      (WHILE (GREATER '6' B)(
      (PRINT C)
      (PRINT B)
      (SET B (PLUS B '1'))
      (SET A (PLUS '-1' C))
      (SET C A))
   )
)
)
)