(
    (SET A '1')
    (SET B '2')
    (SET C (LESS A B))
    (IF (EQUAL C '1') ((PRINT "YES")) ((PRINT "FAILURE")))
)