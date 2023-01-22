#Test IF in the Loop function
(
   (SET C '5')
   (SET B '(2)')
   (WHILE (LESS '0' C)
   (
     (SET A (PLUS '-1' C))
     (IF (EQUAL '(2)' B) ((PRINT A)) ((PRINT "FAILURE")))
     (SET C A))
   )
   )
)