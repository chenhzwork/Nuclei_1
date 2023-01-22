#Test the sum of atomic lisp
(
  (SET A '(1 2 (7) 3)')
  (SET B (SUM A))
  (SET C '(1 2 3 4)')
  (SET D (SUM C))
  (SET E (SUM '(2 3 5)'))
  (IF (EQUAL B '13') ((PRINT "YES")) ((PRINT "FAILURE")))
  (IF (EQUAL D '6') ((PRINT "FAILURE")) ((PRINT "NO")))
  (IF (EQUAL E '10') ((PRINT "YES")) ((PRINT "FAILURE")))
)