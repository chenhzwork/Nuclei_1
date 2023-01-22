(
    (SET A '3')
    (SET B '2')
    (SET C (GREATER A B))
    (IF (EQUAL C '1') ((PRINT "YES")) ((PRINT "FAILURE")))
)