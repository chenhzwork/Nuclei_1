(
    (SET A '1')
    (SET B '1')
    (SET C (EQUAL A B))
    (IF (EQUAL C '1') ((PRINT "YES")) ((PRINT "FAILURE")))
)