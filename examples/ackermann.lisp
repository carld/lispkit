( LETREC ACKERMANN
    ( ACKERMANN LAMBDA ( X Y )
     ( IF ( EQ X ( QUOTE 0 ) )
     ( ADD Y ( QUOTE 1 ) )
     ( IF ( EQ Y ( QUOTE 0 ) )
     ( ACKERMANN ( SUB X ( QUOTE 1 ) ) ( QUOTE 1 ) )
     ( ACKERMANN ( SUB X ( QUOTE 1 ) ) ( ACKERMANN X ( SUB Y (
       QUOTE 1 ) ) ) ) ) ) ) )                                                                                                                               QUOTE 1 ) ) ) ) ) ) ) )

