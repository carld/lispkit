( LETREC REVERSE 
  ( REVERSE LAMBDA ( L )
    ( LETREC ( REV0 ( QUOTE NIL ) L )
      ( REV0 LAMBDA ( A L )
        ( IF ( EQ L ( QUOTE NIL ) ) 
             A
             ( REV0 ( CONS ( CAR L ) A ) ( CDR L ) ) ) ) ) ) )

