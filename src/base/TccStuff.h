/*============================================================================*/
/* TccStuff.h -- Tcc Stuff                                                    */
/*============================================================================*/
#pragma once

//#include "TccStuff.h"

#define ADDR_ADD(type,addr,bytes) ((type)((char*)addr + bytes))

union Dte_T
{
  struct {
    u8 m_uDay       :  5 ; // 0..31       same as _Tsp.dte
    u8 m_uMonth     :  4 ; // 0..15
    u8 m_uYearDelta :  8 ; // 0..255  17b (1900+8b == 2155)
    
    u8 m_uSpare     : 47 ; //         47b
    //               ---
    //                64
    //               ===
  };
  i8 m_iAsI8;
};

union Edt_T
{
  struct {
    u8 m_uDay       :  5 ; // 0..31
    u8 m_uMonth     :  4 ; // 0..15
    u8 m_uYearInEra :  7 ; // 0..127  16b (eraBase+7b == 127)
    
    u8 m_uSpare     : 48 ; //         48b
    //               ---
    //                64
    //               ===
  };
  i8 m_iAsI8;
};

struct HeadLine_T
{
  byte m_yaByte[C_BYTES_PER_WORD];
};

struct OverLine_T
{
  byte m_yaByte[2 * C_BYTES_PER_WORD];
};

struct TailLine_T
{
  byte m_yaByte[2 * C_BYTES_PER_WORD];
};

#define METACODE_HIGH(__uCode) ( __uCode >> 4 )
#define METACODE_LOW(__uCode) ( __uCode & 0x0F )

#define METACODE_SPLIT(__uCode,__iHigh,__iLow) \
  __iHigh = (__uCode >> 4); \
  __iLow  = (__uCode &  0x0F);

#define STRx1(__uLow) (__uLow)            //   0B .. |1 * 16 =  16| ..  15B
#define STRx2(__uLow) ( 16 + 2 * __uLow)  //  16B .. |2 * 16 =  32| ..  47B
#define STRx4(__uLow) ( 48 + 4 * __uLow)  //  48B .. |4 * 16 =  64| .. 111B
#define STRx8(__uLow) (112 + 8 * __uLow)  // 112B .. |8 * 16 = 128| .. 239B

enum eSI // string inflate
{
  /*  0 */ SI_NONE=0,
  /*  3 */ SI_CELLARRAY_ADR, // put address into given cell (no inflate per se)
  /*  4 */ SI_CELLARRAY_ANT, // inflate into cell(s), with size before
  /*  5 */ SI_CELLARRAY_PST, // inflate into cell(s), with size after
  /*  6 */ SI_KEY,           // inflate into cell(s), with key semantics
  /*  1 */ SI_STRAREA_ADR,   // inflate into str area, put address into cell
  /*  2 */ SI_STRAREA_OFS,   // inflate into str area, put offset into cell
  /*  7 */ SI_ENUMS
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
