
#define DIGIT   0x01
#define PRINT   0x02
#define ALPHA   0x04
#define SPACE   0x08

char charset[] = {
/* 00 nul */
/* 01 soh */
/* 02 stx */
/* 03 etx */
/* 04 eot */
/* 05 enq */
/* 06 ack */
/* 07 bel */
/* 08 bs  */
/* 09 ht  */ SPACE,
/* 0a nl  */ SPACE,
/* 0b vt  */ SPACE,
/* 0c np  */
/* 0d cr  */ SPACE,
/* 0e so  */
/* 0f si  */
/* 10 dle */
/* 11 dc1 */
/* 12 dc2 */
/* 13 dc3 */
/* 14 dc4 */
/* 15 nak */
/* 16 syn */
/* 17 etb */
/* 18 can */
/* 19 em  */
/* 1a sub */
/* 1b esc */
/* 1c fs  */
/* 1d gs  */
/* 1e rs  */
/* 1f us  */
/* 20 sp  */ PRINT | SPACE,
/* 21  !  */ PRINT, 
/* 22  "  */ PRINT, 
/* 23  #  */ PRINT, 
/* 24  $  */ PRINT,
/* 25  %  */ PRINT,
/* 26  &  */ PRINT,
/* 27  '  */ PRINT,
/* 28  (  */ PRINT,
/* 29  )  */ PRINT,
/* 2a  *  */ PRINT,
/* 2b  +  */ PRINT,
/* 2c  ,  */ PRINT,
/* 2d  -  */ PRINT,
/* 2e  .  */ PRINT,
/* 2f  /  */ PRINT,
/* 30  0  */ PRINT | DIGIT,
/* 31  1  */ PRINT | DIGIT,
/* 32  2  */ PRINT | DIGIT,
/* 33  3  */ PRINT | DIGIT,
/* 34  4  */ PRINT | DIGIT,
/* 35  5  */ PRINT | DIGIT,
/* 36  6  */ PRINT | DIGIT,
/* 37  7  */ PRINT | DIGIT,
/* 38  8  */ PRINT | DIGIT,
/* 39  9  */ PRINT | DIGIT,
/* 3a  :  */ PRINT,
/* 3b  ;  */ PRINT,
/* 3c  <  */ PRINT,
/* 3d  =  */ PRINT,
/* 3e  >  */ PRINT,
/* 3f  ?  */ PRINT,
/* 40  @  */ PRINT,
/* 41  A  */ PRINT | ALPHA,
/* 42  B  */ PRINT | ALPHA,
/* 43  C  */ PRINT | ALPHA,
/* 44  D  */ PRINT | ALPHA,
/* 45  E  */ PRINT | ALPHA,
/* 46  F  */ PRINT | ALPHA,
/* 47  G  */ PRINT | ALPHA,
/* 48  H  */ PRINT | ALPHA,
/* 49  I  */ PRINT | ALPHA,
/* 4a  J  */ PRINT | ALPHA,
/* 4b  K  */ PRINT | ALPHA,
/* 4c  L  */ PRINT | ALPHA,
/* 4d  M  */ PRINT | ALPHA,
/* 4e  N  */ PRINT | ALPHA,
/* 4f  O  */ PRINT | ALPHA,
/* 50  P  */ PRINT | ALPHA,
/* 51  Q  */ PRINT | ALPHA,
/* 52  R  */ PRINT | ALPHA,
/* 53  S  */ PRINT | ALPHA,
/* 54  T  */ PRINT | ALPHA,
/* 55  U  */ PRINT | ALPHA,
/* 56  V  */ PRINT | ALPHA,
/* 57  W  */ PRINT | ALPHA,
/* 58  X  */ PRINT | ALPHA,
/* 59  Y  */ PRINT | ALPHA,
/* 5a  Z  */ PRINT | ALPHA,
/* 5b  [  */ PRINT,
/* 5c  \  */ PRINT,
/* 5d  ]  */ PRINT,
/* 5e  ^  */ PRINT,
/* 5f  _  */ PRINT,
/* 60  `  */ PRINT,
/* 61  a  */ PRINT | ALPHA,
/* 62  b  */ PRINT | ALPHA,
/* 63  c  */ PRINT | ALPHA,
/* 64  d  */ PRINT | ALPHA,
/* 65  e  */ PRINT | ALPHA,
/* 66  f  */ PRINT | ALPHA,
/* 67  g  */ PRINT | ALPHA,
/* 68  h  */ PRINT | ALPHA,
/* 69  i  */ PRINT | ALPHA,
/* 6a  j  */ PRINT | ALPHA,
/* 6b  k  */ PRINT | ALPHA,
/* 6c  l  */ PRINT | ALPHA,
/* 6d  m  */ PRINT | ALPHA,
/* 6e  n  */ PRINT | ALPHA,
/* 6f  o  */ PRINT | ALPHA,
/* 70  p  */ PRINT | ALPHA,
/* 71  q  */ PRINT | ALPHA,
/* 72  r  */ PRINT | ALPHA,
/* 73  s  */ PRINT | ALPHA,
/* 74  t  */ PRINT | ALPHA,
/* 75  u  */ PRINT | ALPHA,
/* 76  v  */ PRINT | ALPHA,
/* 77  w  */ PRINT | ALPHA,
/* 78  x  */ PRINT | ALPHA,
/* 79  y  */ PRINT | ALPHA,
/* 7a  z  */ PRINT | ALPHA,
/* 7b  {  */ PRINT,
/* 7c  |  */ PRINT,
/* 7d  }  */ PRINT,
/* 7e  ~  */ PRINT
/* 7f del */
};

#define is_print(x)  (x >= 0 && x <= 0x7f && (charset[x] & PRINT))
#define is_digit(x)  (x >= 0 && x <= 0x7f && (charset[x] & DIGIT))
#define is_alpha(x)  (x >= 0 && x <= 0x7f && (charset[x] & ALPHA))


