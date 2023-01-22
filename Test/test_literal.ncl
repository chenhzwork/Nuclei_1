#Test Literal format
(
  (SET A '(1)')
  (SET B '(1 2 3 (4 5))')
  (SET C '2')
  (SET D '((1 2) 3 4 5)')
  (SET E '(((1 2)) 3 4 5)')
  (PRINT A)
  (PRINT B)
  (PRINT C)
  (PRINT D)
  (PRINT E)
)