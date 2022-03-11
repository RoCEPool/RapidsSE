/*============================================================================*/
/* TccStuff.h -- Tcc Stuff                                                    */
/*============================================================================*/
#pragma once

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// BCD
union Bcd_T
{
  struct {
    i8 m_iSpare: 8; // usually 0
    i8 m_iScale: 8; //
    i8 m_iMantissa: 48; // 2's complement
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
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// INT
union Int_T {
  i8 m_iInt;
  i8 m_iAsI8;
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// IVL
union Ivl_T {
  struct {
    u8 m_uMicro     : 20 ; // 0..1M
    u8 m_uSecond    : 44 ; // 0..1.7E13
    //               ---
    //                64
    //               ===
  };
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
static const i8 C_TAILLINE_QUANTA  = 2 * C_BYTES_PER_WORD;

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

static const i8 C_HEADLINE_OFS_WORDS = 0;
static const i8 C_HEADLINE_OFS_PERSPECTIVE = 1;
static const i8 C_HEADLINE_DELETED = 127;

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

#define STRx1(__uLow) (__uLow)            //   0B .. |1 * 16 =  16| ..  15B
#define STRx2(__uLow) ( 16 + 2 * __uLow)  //  16B .. |2 * 16 =  32| ..  47B
#define STRx4(__uLow) ( 48 + 4 * __uLow)  //  48B .. |4 * 16 =  64| .. 111B
#define STRx8(__uLow) (112 + 8 * __uLow)  // 112B .. |8 * 16 = 128| .. 239B

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


struct Combined_T
{
  void*   m_pTub;
  void*   m_pPot;
  i8      m_bTrace;
  i8      m_iErrorCode;

  i8      m_iHashRows;

  // table T scan inflate (crwBuffer)
  struct  HeadLine_T* m_aHeadLine0;
  i8        m_iHeadSize;
  enum eSI  m_eStrInflateRule;

  // expression stack
  i8      m_aW[256];

  // rowBuffer T
  void*   m_aRowBuffer;

  // index scan T
  void*   m_aIndexT;
  void*   m_aIndexTreeT;
  void*   m_aIndexDefT;
  void*   m_aEntKeyT;
  void*   m_aEntWrkT;
  void*   m_aEntUpperT;
  void*   m_aEntBufferT;
  struct Ent_int* m_aEntIntT;

  // common inflate
  void*   m_pRowSingleT;
  void*   m_aRowCellArrayT;
  char*   m_racStrAreaT;
  char*   m_racStrAreaTSav;
  i8      m_nRows;
  i8      m_nCols;

  // index join with J
  i8      m_iIndexJoinInit;
  void*   m_aRowSingleJ;
  void*   m_aRowCellArrayJ;
  char*   m_racStrAreaJ;
  char*   m_racStrAreaJSav;
  void*   m_aIndexJ;
  void*   m_aIndexTreeJ;
  void*   m_aIndexDefJ;
  void*   m_aEntKeyJ;
  void*   m_aEntWrkJ;
  void*   m_aEntUpperJ;
  void*   m_aEntBufferJ;
  struct Ent_int* m_aEntIntJ;

  // join rows T and J
  void*   m_aRowSingleTJ;
  void*   m_aRowCellArrayTJ;
  char*   m_racStrAreaTJ;
  char*   m_racStrAreaTJSav;

  // project (col/exp)
  i8      m_iProjectInit;
  void*   m_aRowSingleP;
  void*   m_aRowCellArrayP;
  char*   m_racStrAreaP;
  char*   m_racStrAreaSavP;

  // project (agg)
  i8      m_iRowsProcessed;
  void*   m_aRowSingleAgg;
  void*   m_aRowCellArrayAgg;
  char*   m_racStrAreaAgg;
  char*   m_racStrAreaSavAgg;

  // project (gby)
  i8      m_nAggRows;
  i8      m_iProcessRow;

  // payload (gby/srt/top)
  i8*     m_aiPayload;
  void*   m_aPayloadRow;
  char*   m_racPayloadArea;
  void*   m_aIndexTreePL;
  void*   m_aIndexDefPL;
  void*   m_aEntBufferPL;
  void*   m_aEntWrkPL;
  void*   m_aEntKeyPL;
  void*   m_aEntBottomPL;
  struct Ent_int* m_aEntIntPL;

  // top
  i8      m_iTopInit;
  i8      m_iInsertCntTOP;

  // limit
  i8     m_iLimitRows;

  // final tub
  void*   m_aRowTub;
  void*   m_aRowTubCellArray;
  char*   m_racTubStrArea;
  i8      m_iTubRowSize;

  // final pot
  void*   m_aRowPot;
  void*   m_aRowPotCellArray;
  char*   m_racPotStrArea;
};

void vEnt_payloadCreate(void* pEntWrk, i8 nPayloadBytes);

void* aEntBuffer_nextEntry(void** ppEntBuffer, void* aPrevEntry);

void vEntWrk_appendElementEntInt(void* pEntWrk, i8 iInt);
void vEntWrk_appendElementEntStz(void* pEntWrk, char* sValue);
void vEntWrk_entKind(void* pEntWrk, i8 bPayLoad, i8 eKeyKind);
void vEntWrk_payloadCreate(void* pEntWrk, i8 nPayloadBytes);
void vEntWrk_print(void* pEntWrk, void* pIndexDef);

void* aIndexTree_begin(void* pIndexTree, void** ppEntBuffer);
void* aIndexTree_findEntry(void* aIndexTree, void* aKey);
void* aIndexTree_insertEnt(void* aIndexDef, void* aEnt);
void* aIndexTree_insertEntTop(void* aIndexTree, void* aEnt, void** aaEntBottom, i8 iTopCount);
void* aIndexTree_positionGE(void* aIndexTree, void** aaEntBuffer, void* aKey);
void* aIndexTree_positionGT(void* aIndexTree, void** aaEntBuffer, void* aKey);
void *aIndexTree_positionLE(void* aIndexTree, void* aKey);
void *aIndexTree_positionLT(void* aIndexTree, void* aKey);

void* aRowBuffer_GetCellArray(void* aRowBuffer, i8 iRow);
void  vRowSingle_Print(void* pRowSingle);

void* aPot_AllotRow(void* pPot, i8 iMixIn, char** aazStrArea);
void  vPot_RowStrSize(void* pPot, i8 iMixin, i8 nRowStrSize);

void* aTub_AllotRow(void* pTub, char** aazStrArea);
void  vTub_RowStrSize(void* pTub, i8 nRowStrSize);

i8    StrLib_iLike2(char* azStr, char* azRegExp);
i8    StrLib_iLike3(char* azStr, char* azRegExp, char cEsc);
i8    StrLib_iWordFlipBack(char* aDst, char* aSrc);

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
