#Test whether two lisps congruent or not
(
  (SET A '(5 (1 2 3))')
  (SET B '(5 (1 2 3))')
  (IF (EQUAL A B) ((PRINT "YES")) ((PRINT "NO")))
  (IF (EQUAL A '(5 (1 2 3))') ((PRINT "YES")) ((PRINT "NO")))
  (IF (EQUAL B '(5 (1 2 3))') ((PRINT "YES")) ((PRINT "NO")))
  (IF (EQUAL '(5 (1 2 3))' A) ((PRINT "YES")) ((PRINT "NO")))
  (IF (EQUAL A '(5 (1 2))') ((PRINT "YES")) ((PRINT "NO")))
)