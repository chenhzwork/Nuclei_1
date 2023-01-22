(
    (SET A '1')
    (SET B (PLUS A '2'))
    (SET C (PLUS B '-3'))
    (IF (EQUAL C '0') ((PRINT "YES")) ((PRINT "FAILURE")))
)