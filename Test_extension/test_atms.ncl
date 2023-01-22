#Test the number of atomic lisp
(
  (SET A '(1 2 (7) 3)')
  (SET B (ATMS A))
  (SET C '(1 2 3 4)')
  (SET D (ATMS C))
  (SET E (ATMS '(2 3 5)'))
  (IF (EQUAL B '4') ((PRINT "YES")) ((PRINT "NO")))
  (IF (EQUAL D '4') ((PRINT "YES")) ((PRINT "NO")))
  (IF (EQUAL E '3') ((PRINT "YES")) ((PRINT "NO")))
)