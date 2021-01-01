/*============================================================================*/
/* MetaCodeMacros.h -- MetaCode Macros                                        */
/*============================================================================*/
#pragma once

static const i8 C_C_1B = 1;   // <C> 1B number of compressed bytes
static const i8 C_L_1B = 1;   // <L> 1B string length in bytes
static const i8 C_M_1B = 1;   // <M> 1B overflowed metacode
static const i8 C_T_8B = 8;   // <T> 8B transaction perspective
static const i8 C_U_1B = 1;   // <U> 1B over/tail line size in units
static const i8 C_W_1B = 1;   // <W> 1B head line size in words

static const i8 C_L_2B = 2;   // <L> 2B string length in bytes

static const i8 C_FT_2B = 2;  // |FT| 2B absolute from-tail i_Line
static const i8 C_IT_2B = 2;  // [IT] 2B relative into-tail i_Line
static const i8 C_OT_2B = 2;  // [OT] 2B relative over-tail i_Line

static const i8 C_NULL_1B = 1;// <NULL> '\0' 1B

#define STRx1(__uLow) (__uLow)            //   0B .. |1 * 16 =  16| ..  15B
#define STRx2(__uLow) ( 16 + 2 * __uLow)  //  16B .. |2 * 16 =  32| ..  47B
#define STRx4(__uLow) ( 48 + 4 * __uLow)  //  48B .. |4 * 16 =  64| .. 111B
#define STRx8(__uLow) (112 + 8 * __uLow)  // 112B .. |8 * 16 = 128| .. 239B

#define METACODE_FORM(__high,__low) ( (u1) ( (u1)__high << 4 | (u1)__low )  )

#define METACODE_HIGH(__uCode) ( __uCode >> 4 )
#define METACODE_LOW(__uCode) ( __uCode & 0x0F )

#define METACODE_SPLIT(__uCode,__iHigh,__iLow) \
  __iHigh = (__uCode >> 4); \
  __iLow  = (__uCode &  0x0F);

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
