( LETREC FACTORIAL
  ( FACTORIAL LAMBDA ( X ) 
    ( IF ( EQ X ( QUOTE 1 ) ) ( QUOTE 1 )
      ( MUL X ( FACTORIAL ( SUB X ( QUOTE 1 ) ) ) ) ) ) )

