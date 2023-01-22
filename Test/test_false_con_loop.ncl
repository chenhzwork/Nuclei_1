#Test Loop with a false condition
(
   (SET C '5')
   (SET B '0')
   (WHILE (LESS '6' C)(
      (PRINT C)
)
)
   (PRINT "LOOP SKIPPED")
)