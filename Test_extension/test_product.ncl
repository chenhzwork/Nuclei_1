#Test the product of every atomic lisp
(
  (SET A '(1 2 (7) 3)')
  (SET B (PRODUCT A))
  (SET C '(1 2 3 4)')
  (SET D (PRODUCT C))
  (SET E (PRODUCT '(2 3 5)'))
  (IF (EQUAL B '42') ((PRINT "YES")) ((PRINT "NO")))
  (IF (EQUAL D '24') ((PRINT "YES")) ((PRINT "NO")))
  (IF (EQUAL E '30') ((PRINT "YES")) ((PRINT "NO")))
)