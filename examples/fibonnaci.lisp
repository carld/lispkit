( LETREC FIBONACCI
  ( FIBONACCI LAMBDA ( X )
     ( IF ( EQ X ( QUOTE 0 ) ) ( QUOTE 0 )
     ( IF ( EQ X ( QUOTE 1 ) ) ( QUOTE 1 )
       ( ADD ( FIBONACCI ( SUB X ( QUOTE 1 ) ) ) 
             ( FIBONACCI ( SUB X ( QUOTE 2 ) ) ) ) ) ) ) )

