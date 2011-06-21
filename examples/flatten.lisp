( LETREC FLATTEN
  ( FLATTEN LAMBDA ( L )
    ( IF ( EQ L ( QUOTE NIL ) ) 
         ( QUOTE NIL )
         ( IF ( ATOM L ) 
              ( CONS L ( QUOTE NIL ) )
              ( IF ( ATOM ( CAR L ) ) 
                   ( CONS ( CAR L ) ( FLATTEN ( CDR L ) ) )
                   ( APPEND ( FLATTEN ( CAR L ) ) ( FLATTEN ( CDR L ) ) ) ) ) ) )

  ( APPEND LAMBDA ( X Y )
    ( IF ( EQ X ( QUOTE NIL ) ) 
         Y
         ( CONS ( CAR X ) ( APPEND ( CDR X ) Y ) ) ) ) 
) 

