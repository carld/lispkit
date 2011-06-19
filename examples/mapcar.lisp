( LETREC MYPROG
  ( MYPROG LAMBDA ( X ) 
    ( MAPCAR INC X ) )

  ( INC LAMBDA ( X ) ( ADD ( QUOTE 1 ) X ) )

  ( MAPCAR LAMBDA ( F L )
    ( IF ( EQ L ( QUOTE NIL ) )
         ( QUOTE NIL )
         ( CONS ( F ( CAR L ) ) ( MAPCAR F ( CDR L ) ) ) ) )
)

