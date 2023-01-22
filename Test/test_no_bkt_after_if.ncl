#Test no right bracket after IF
(
    (IF (EQUAL (LENGTH NIL) '0' ((PRINT "NIL HAS ZERO LENGTH")) ((PRINT "FAILURE")))
)