/*============================================================================*/
/* CodeSynthCommon.h -- structures common to CodeSynthesis and SE             */
/*============================================================================*/
#pragma once

// NOTE: THis should match eIV enums in eIV.H
typedef enum eIVL // Interval Type
{
  /* 0  */ IVL_NONE=0,
  /* 1  */ IVL_YM_START,
  /* 2  */ IVL_YEAR,
  /* 3  */ IVL_MONTH,
  /* 4  */ IVL_YM,
  /* 5  */ IVL_YM_END,
  /* 6  */ IVL_DT_START,
  /* 7  */ IVL_DAY,
  /* 8  */ IVL_HOUR,
  /* 9  */ IVL_MINUTE,
  /* 10 */ IVL_SECOND,
  /* 11 */ IVL_DH,
  /* 12 */ IVL_DM,
  /* 13 */ IVL_DS,
  /* 14 */ IVL_HM,
  /* 15 */ IVL_HS,
  /* 16 */ IVL_MS,
  /* 17 */ IVL_DT_END,
  /* 18 */ IVL_ENUMS
} eIVL;

// Note: This is defined in CodeSynthCommon.cpp so that it isn't #included
//       into every compilation unit (producing lots of warnings).
extern eIVL eMacro_AddSubResultIVL[IVL_ENUMS][IVL_ENUMS];

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// BCD
union Bcd_T
{
  struct {
    i8 m_iScale: 8; //
    i8 m_iMantissa: 56; // 2's complement
  };
  i8 m_iBcd;
  i8 m_iAsI8;
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// BOL
union Bol_T
{
  i8 m_iBol;
  i8 m_iAsI8;
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// DTE
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

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// TME
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

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// FLT
union Flt_T {
  d8 m_dFlt;
  i8 m_iAsI8;
  //d8 dGetFloat() const {return *(d8*)&m_iAsI8;};  tinyC compilation error;
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// INT
union Int_T {
  i8 m_iInt;
  i8 m_iAsI8;
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// IVL
// Notes:
// We use two unioned structures to below to represent the 13 different interval
// types - one structure represents all of the YEAR-MONTH interval types and one
// represents all of the DAY-TIME interval types.
// Each structure only has a single field. This makes comparison and interval
// arithmetic easy, but care needs to be taken when the interval is being displayed
// so that only the right fields are shown.
//
// e.g. for a YEAR-MONTH interval a value of 15 would be represented as follows
// for each of the 3 YEAR-MONTH interval subtypes:
//   INTERVAL '15' MONTH
//   INTERVAL '1' YEAR
//   INTERVAL '1-3' YEAR TO MONTH
//
// The YM and DT interval classes store their values in a single field representing
// their smallest denomination - months for YM intervals and microseconds for DT
// intervals. This still gives a significant range of values for each interval type.
//
// The "__attribute__((__packed__))" is required to ensure each of the structures
// are not padded out to 8 bytes each.
//

union Ivl_T {
  struct  __attribute__((__packed__)) {
    u8 m_eType    : 5 ; // 0..31, eIV enum
    u8 m_bIsNeg   : 1 ;
    u8 m_bIsNull  : 1 ;
    u8 m_iSpare   : 1 ;

    union __attribute__((__packed__)) {

      // INTERVAL YEAR, MONTH & YEAR TO MONTH:
      struct __attribute__((__packed__)) {
        u8 m_uMonths  : 56; // 0..7.2E16
      } m_xIvlYM;

      // INTERVAL DAY, HOUR, MINUTE, SECOND,
      // DAY TO HOUR, DAY TO MINUTE, DAY TO SECOND,
      // HOUR TO MINUTE, HOUR TO SECOND,
      // and MINUTE TO SECOND:
      struct __attribute__((__packed__)) {
        u8 m_uMicros  : 56; // 0..7.2E16
      } m_xIvlDT;

      // Maximum displayable ranges for each interval type:
      //  (for each YM or DT group they are all the same value
      //   in months or microseconds)
      // -------------------------------------------------------
      // INTERVAL YEAR              :  6,004,799,503,160,661 years
      // INTERVAL MONTH             : 72,057,594,037,927,935 months
      // INTERVAL YEAR TO MONTH     :  6,004,799,503,160,661 years, 3 months
      //
      // INTERVAL DAY               :                833,999 days   (about 2283 years)
      // INTERVAL HOUR              :             20,015,998 hours
      // INTERVAL MINUTE            :          1,200,959,900 minutes
      // INTERVAL SECOND            :         72,057,594,037.927,935 seconds
      // INTERVAL DAY TO HOUR       :                833,999 days, 22 hours
      // INTERVAL DAY TO MINUTE     :                833,999 days, 22 hours, 20 minutes
      // INTERVAL DAY TO SECOND     :                833,999 days, 22 hours, 20 minutes, 37.927,935 seconds
      // INTERVAL HOUR TO MINUTE    :             20,015,998 hours, 20 minutes
      // INTERVAL HOUR TO SECOND    :             20,015,998 hours, 20 minutes, 37.927,935 seconds
      // INTERVAL MINUTE TO SECOND  :          1,200,959,900 minutes, 37.927,935 seconds
    } u;
  } m_xIvl;

  i8 m_iAsI8;
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// TME
union Tme_T {
  struct {
    u8 m_uMicro     : 20 ; // 0..1M       same as _Tsp.tme
    u8 m_uSecond    :  6 ; // 0..63
    u8 m_uMinute    :  6 ; // 0..63
    u8 m_uHour      :  5 ; // 0..31   37b

    u8 m_uSpare     : 27 ; //         27b
    //               ---
    //                64
    //               ===
  };
  i8 m_iAsI8;
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// TSP
union Tsp_T
{
  struct {
    u8 m_uLocal     :  1 ; //        1b
    
    u8 m_uMicro     : 20 ; // 0..1M
    u8 m_uSecond    :  6 ; // 0..63
    u8 m_uMinute    :  6 ; // 0..63
    u8 m_uHour      :  5 ; // 0..31   37b
    
    u8 m_uDay       :  5 ; // 0..31
    u8 m_uMonth     :  4 ; // 0..15
    u8 m_uYearDelta :  8 ; // 0..255  17b  (1900+8b == 2155)
    
    u8 m_uSpare     :  9 ; //        9b
    //               ---
    //                64
    //               ===
  };
  i8 m_iAsI8;
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// UNK
union Unk_T {
  i8 m_iInt;
  i8 m_iAsI8;
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// VCH
union Vch_T {
  i8 m_iOffset;
  char* m_aPointer;
  i8 m_iAsI8;
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// Headline
struct HeadLine_T
{
  byte m_yaByte[C_BYTES_PER_WORD];
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// Headline
struct OverLine_T
{
  byte m_yaByte[2 * C_BYTES_PER_WORD];
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// Tailine
struct TailLine_T
{
  byte m_yaByte[2 * C_BYTES_PER_WORD];
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// SpaceUnit
struct SpaceUnit_T
{
  byte m_yaByte[2 * C_BYTES_PER_WORD];
};

static const i8 C_HEADLINE_QUANTA  = C_BYTES_PER_WORD;
static const i8 C_SPACEUNIT_QUANTA = 2 * C_BYTES_PER_WORD;

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

enum eMC // meta code
{
  /* 0 */ MC_NUMERIC = 0, // numeric data 0B .. 8B
  /* 1 */ MC_DATE_E0,     // date era 0
  /* 2 */ MC_DATE_E1,     // date era 1
  /* 3 */ MC_DATE_E2,     // date era 2
  /* 4 */ MC_STRING_x1,   // short string 0B .. 15B
  /* 5 */ MC_NULL,        // null value
  /* 6 */ MC_SPARE0,      // unused
  /* 7 */ MC_OVERFLOW,    // 2B overflow
  
  /* 8 */ MC_STRING_x2,   // 2 x string (see macro)
  /* 9 */ MC_STRING_x4,   // 4 x string (see macro)
  /* A */ MC_STRING_x8,   // 8 x string (see macro)
  /* B */ MC_STRING_TAIL, // string in tail
  /* C */ MC_BLOB_0,      // blob type 0
  /* D */ MC_BLOB_1,      // blob type 1
  /* E */ MC_NEGATIVE,    // -ve digit -15 .. -0
  /* F */ MC_POSITIVE,    // +ve digit   0 .. 15

  /* 16 */ MC_ENUMS,
};

static const i8 _C_1M = 1000000;
static const i8 _C_16KB = 16*1024;
static const i8 _C_SECS_PER_MIN = 60;
static const i8 _C_SECS_PER_HR  = 3600;
static const i8 _C_SECS_PER_DAY = 3600*24;
static const i8 _C_HOURS_PER_DAY = 24;

static const i8 _C_TSP_LOCAL = 1;
static const i8 _C_TSP_UTC   = 0;

static const u8 _C_IVL_MONTH_MAX  = 72057594037927935; // (2^56 -1)
static const u8 _C_IVL_USEC_MAX   = 72057594037927935; // (2^56 -1)

static const i8 C_HEADLINE_OFS_WORDS = 0;
static const i8 C_HEADLINE_OFS_PERSPECTIVE = 1;
static const i8 C_HEADLINE_DELETED = 127;

static const i8 C_C_1B = 1;   // <C> 1B number of compressed bytes
//static const i8 C_L_1B = 1;   // <L> 1B string length in bytes
static const i8 C_M_1B = 1;   // <M> 1B overflowed metacode
static const i8 C_T_8B = 8;   // <T> 8B transaction perspective
static const i8 C_U_1B = 1;   // <U> 1B over/tail line size in units
static const i8 C_W_1B = 1;   // <W> 1B head line size in words

static const i8 C_L_2B = 2;   // <L> 2B string length in bytes

static const i8 C_FT_2B = 2;  // |FT| 2B absolute from-tail i_Line
static const i8 C_IT_2B = 2;  // [IT] 2B relative into-tail i_Line
static const i8 C_OT_2B = 2;  // [OT] 2B relative over-tail i_Line

static const i8 C_CONTROL_SIZE = 9; // 1 byte for the headline size (in words)
                                    // 8 bytes for the perspective

static const i8 C_NULL_1B = 1;// <NULL> '\0' 1B

#define ADDR_ADD(type,addr,bytes) ((type)((char*)addr + bytes))

#define BITWISE_AND(a,b) ( (u8)a & (u8)b )
#define BITWISE_NOT(a) ( ~(u8)a )

#define GET_u1B(addr,i_Byte) (* (u1*)((byte*)addr + i_Byte))
#define GET_u2B(addr,i_Byte) (* (u2*)((byte*)addr + i_Byte))

#define META_AREA(addr) (  ADDR_ADD(u1*,addr,C_CONTROL_SIZE)  )

#define METACODE_HIGH(__uCode) ( __uCode >> 4 )
#define METACODE_LOW(__uCode) ( __uCode & 0x0F )

#define METACODE_FORM(__high,__low) ( (u1) ( (u1)__high << 4 | (u1)__low )  )

#define METACODE_SPLIT(__uCode,__iHigh,__iLow) \
  __iHigh = (__uCode >> 4); \
  __iLow  = (__uCode &  0x0F);

#define STRx1(__uLow) (__uLow)           //   0B .. |  0 + (15*1 =  15)| ..  15B
#define STRx2(__uLow) ( 15 + 2 * __uLow) //  16B .. | 15 + (15*2 =  30)| ..  45B
#define STRx4(__uLow) ( 45 + 4 * __uLow) //  46B .. | 45 + (15 4 =  60)| .. 105B
#define STRx8(__uLow) (105 + 8 * __uLow) // 112B .. |105 + (15*8 = 120)| .. 225B

#define TO_BOUNDARY(addr,nByteBoundary) \
  ( (void*)BITWISE_AND(addr,BITWISE_NOT((nByteBoundary - 1))) )

#define UNIT_GET_REL(aHead,i_Tail_rel) \
  ( (void*)(((struct SpaceUnit_T*)TO_BOUNDARY(aHead,C_SPACEUNIT_QUANTA))+i_Tail_rel) )

// 8,16,24,...
#define nWordRoundedStrBytes(_Size) \
  ( ( (_Size / C_BYTES_PER_WORD) * C_BYTES_PER_WORD) + \
    (_Size % C_BYTES_PER_WORD ? C_BYTES_PER_WORD : 0) )

#define nDifference(addr1,addr2) \
  ( ((byte*)addr1 >= (byte*)addr2) \
    ? ((byte*)addr1 - (byte*)addr2) : ((byte*)addr2 - (byte*)addr1) )

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
