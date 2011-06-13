( LET NULL?
  ( NULL? LAMBDA ( X )
    ( IF ( EQ X ( QUOTE NIL ) ) 
         ( QUOTE T ) 
         ( QUOTE F ) ) ) )

