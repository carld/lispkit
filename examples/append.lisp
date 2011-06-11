( LETREC APPEND
  ( APPEND LAMBDA ( X Y )
    ( IF ( EQ X ( QUOTE NIL ) ) Y 
      ( CONS ( CAR X )
        ( APPEND ( CDR X ) Y ) ) ) ) ) 
