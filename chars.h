#ifndef __TX_CHARS_H__
  #define __TX_CHARS_H__

  /* Uncomment the char set you wish to use */

  #define USE_UNICODE
  //#define USE_ASCII

  /* The chars */

  #ifdef USE_UNICODE
    #define TB_BOX_ew 0x2500		// => S / N
    #define TB_BOX_sw 0x2510		// => NE
    #define TB_BOX_ns 0x2502		// => E / W
    #define TB_BOX_nw 0x2518		// => SE
    #define TB_BOX_ne 0x2514		// => SW
    #define TB_BOX_se 0x250C		// => NW

    #define TB_CHAR_SPACE  0x0020
    #define TB_CHAR_ENTER  0x21B5
	#define TB_CHAR_CURSOR 0x005F
  #endif

  #ifdef USE_ASCII
    #define TB_BOX_ew '-'		// => S / N
    #define TB_BOX_sw '+'		// => NE
    #define TB_BOX_ns '|'		// => E / W
    #define TB_BOX_nw '+'		// => SE
    #define TB_BOX_ne '+'		// => SW
    #define TB_BOX_se '+'		// => NW

    #define TB_CHAR_SPACE  ' '
    #define TB_CHAR_ENTER  '+'
	#define TB_CHAR_CURSOR '_'
  #endif

#endif
