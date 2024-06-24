/*============================================================================*/
/* vFLAT_Header.h -- Flattened function header                                */
/*============================================================================*/

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

#include "../src/primitives/BaseTypes_general.h"
#include "../src/base/CodeSynthCommon.h"
#include "../src/base/ClangStuff.h"
#include "../src/base/ClangDynamic.h"

#define NEVER_REACH  { \
  printf("NEVER_REACH CODE point in synthesised code was reached at line %d !!!\n", __LINE__); \
  aDynamicT->m_iErrorCode = (i8)EC_INTERNAL_ERROR;                                             \
  return 0;                                                                                    \
}

/*----------------------------------------------------------------------------*/
/* flip the bytes in each word of src string into dst                         */
/*----------------------------------------------------------------------------*/
i8 iWordFlipBack(char* psDst, i8 iDstBufSize, char* psSrc)
{

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  //vTrace("WORDFLIPBACK\n"_Stz);
  //MemLib::vDumpWords(aSrc, 16);

  struct _WordAsBytes {
    union {
      b1 m_yaByte[C_BYTES_PER_WORD];
      i8 m_iI8;
      u8 m_uU8;
    };
  };

  i8 iSrcSize =  strlen(psSrc) + 1;

  i8 iRoundUp = 0;
  i8 iRemainder = iSrcSize % C_BYTES_PER_WORD;
  if (iRemainder)
    iRoundUp = C_BYTES_PER_WORD - iRemainder;

  i8 iReqDstSize = iSrcSize + iRoundUp;
  //ASSERT(0 == iReqDstSize % C_BYTES_PER_WORD);

  //bool bRet = bCheckBufferSpace(__func__, iReqDstSize, iDstBufSize, pbResult);
  //if (!bRet) {
  //  return iReqDstSize;
  //}

	if (iDstBufSize < iReqDstSize) {
		printf("Warning: Unhandled string truncation occurred in function %s.\n"
					 "Destination buffer required %lld bytes, only had %lld bytes.\n",
					 __func__,
					 iReqDstSize,
					 iDstBufSize);
	}

	memset(psDst,0,iReqDstSize);
  //MemLib::vZero(psDst, iReqDstSize, CN_KEY);

  //memcpy(psDst,psSrc,(size_t)iReqDstSize);
  //MemLib::vCopy(psDst, psSrc, iReqDstSize, CN_KEY);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  w8* aWrd = (w8*)psDst;
  w8* aWrdSrc = (w8*)psSrc;
	i8 nWords;
  for (nWords = iReqDstSize / C_BYTES_PER_WORD; nWords; nWords--) {

    //MemLib::vWordFlip(aWrd++, aWrdSrc++);

    struct _WordAsBytes* aAsBytes = (struct _WordAsBytes*)aWrd++;
    struct _WordAsBytes* aAsBytesSrc = (struct _WordAsBytes*)aWrdSrc++;

    byte yByte;

    yByte = aAsBytesSrc->m_yaByte[0];
    aAsBytes->m_yaByte[0] = aAsBytesSrc->m_yaByte[7];
    aAsBytes->m_yaByte[7] = yByte;

    yByte = aAsBytesSrc->m_yaByte[1];
    aAsBytes->m_yaByte[1] = aAsBytesSrc->m_yaByte[6];
    aAsBytes->m_yaByte[6] = yByte;

    yByte = aAsBytesSrc->m_yaByte[2];
    aAsBytes->m_yaByte[2] = aAsBytesSrc->m_yaByte[5];
    aAsBytes->m_yaByte[5] = yByte;

    yByte = aAsBytesSrc->m_yaByte[3];
    aAsBytes->m_yaByte[3] = aAsBytesSrc->m_yaByte[4];
    aAsBytes->m_yaByte[4] = yByte;
  }

  //vTrace("\nAFTER BACK : %s\n"_Stz, aDst);
  //MemLib::vDumpWords(aSrc, iDstSize);

  return iReqDstSize;
}

/*----------------------------------------------------------------------------*/
/* flip the bytes in each word of src string and get the size                 */
/*----------------------------------------------------------------------------*/
i8 iWordFlipSize(char* psSrc) {

  char cDst[1024];
  return iWordFlipBack(&cDst[0], 1024, psSrc);
}

/*----------------------------------------------------------------------------*/
/* flattened function -- STARTMARKER HANDLER                                  */
/*----------------------------------------------------------------------------*/
i8 iStartHandler(struct Dynamic_T* aDynamicT) {

	// Initialize the function pointers to the extern "C" functions here

#ifdef WITH_CLANG
	aDynamicT->m_fp_vExtern_EntWrkAppendElementStz = &vExtern_EntWrkAppendElementStz;
	aDynamicT->m_fp_aExtern_EntBufferNextEntry     = &aExtern_EntBufferNextEntry;
	aDynamicT->m_fp_vExtern_EntPayloadCreate       = &vExtern_EntPayloadCreate;
	aDynamicT->m_fp_vExtern_EntWrkReset            = &vExtern_EntWrkReset;

	aDynamicT->m_fp_aExtern_IndexTreeAllotPayStr = &aExtern_IndexTreeAllotPayStr;
	aDynamicT->m_fp_vExtern_IndexFind            = &vExtern_IndexFind;
  aDynamicT->m_fp_vExtern_IndexFindGlobal      = &vExtern_IndexFindGlobal;
	aDynamicT->m_fp_vExtern_IndexNew             = &vExtern_IndexNew;

	aDynamicT->m_fp_aExtern_IndexTreeBegin            = &aExtern_IndexTreeBegin;
	aDynamicT->m_fp_aExtern_IndexTreeFindEntryAsc     = &aExtern_IndexTreeFindEntryAsc;
	aDynamicT->m_fp_aExtern_IndexTreeFindEntryAscDsc  = &aExtern_IndexTreeFindEntryAscDsc;
	aDynamicT->m_fp_vExtern_IndexFindGlobal           = &vExtern_IndexFindGlobal;
  aDynamicT->m_fp_aExtern_IndexTreeFindInsertGenAsc = &aExtern_IndexTreeFindInsertGenAsc;
  aDynamicT->m_fp_iExtern_IndexTreeGlobalPtnRrf     = &iExtern_IndexTreeGlobalPtnRrf;
	aDynamicT->m_fp_aExtern_IndexTreeInsertEnt        = &aExtern_IndexTreeInsertEnt;
  aDynamicT->m_fp_aExtern_IndexTreeInsertEntTop     = &aExtern_IndexTreeInsertEntTop;
  aDynamicT->m_fp_vExtern_IndexTreeTop              = &vExtern_IndexTreeTop;

	aDynamicT->m_fp_vExtern_IndexTreePositionGE_gen = &vExtern_IndexTreePositionGE_gen;
	aDynamicT->m_fp_vExtern_IndexTreePositionGT_gen = &vExtern_IndexTreePositionGT_gen;
  aDynamicT->m_fp_vExtern_IndexTreePositionLE_gen = &vExtern_IndexTreePositionLE_gen;
  aDynamicT->m_fp_vExtern_IndexTreePositionLT_gen = &vExtern_IndexTreePositionLT_gen;

	aDynamicT->m_fp_vExtern_IndexTreePositionGE_int = &vExtern_IndexTreePositionGE_int;
	aDynamicT->m_fp_vExtern_IndexTreePositionGT_int = &vExtern_IndexTreePositionGT_int;
  aDynamicT->m_fp_vExtern_IndexTreePositionLE_int = &vExtern_IndexTreePositionLE_int;
  aDynamicT->m_fp_vExtern_IndexTreePositionLT_int = &vExtern_IndexTreePositionLT_int;

	aDynamicT->m_fp_vExtern_PotAllotRowPds       = &vExtern_PotAllotRowPds;
  aDynamicT->m_fp_vExtern_PotAllotRowPdsPtn    = &vExtern_PotAllotRowPdsPtn;
	aDynamicT->m_fp_aExtern_PdsFirstRowCellArray = &aExtern_PdsFirstRowCellArray;
	aDynamicT->m_fp_aExtern_PdsNextRowCellArray  = &aExtern_PdsNextRowCellArray;
  aDynamicT->m_fp_vExtern_PotAllotRowPds       = &vExtern_PotAllotRowPds;

	aDynamicT->m_fp_vExtern_PotRowStrSize = &vExtern_PotRowStrSize;

	aDynamicT->m_fp_aExtern_StreamDefNew = &aExtern_StreamDefNew;

	aDynamicT->m_fp_vExtern_TubAllotRowPds = &vExtern_TubAllotRowPds;
	aDynamicT->m_fp_vExtern_TubRowStrSize  = &vExtern_TubRowStrSize;
#endif

	return 1;
}

typedef signed char i1;

typedef unsigned short u2;
typedef long long i8;
typedef unsigned char byte;
typedef double d8;
typedef unsigned long long m8;
typedef unsigned long long u8;
typedef char Stz;

typedef unsigned char eEO;
typedef unsigned short tET;

static const i8 C_1MB = 1048576;

static const void* C_ADR_REGION = (void*)0x204800000000;
static const i8 C_DATUM_NULL = -9223372036854775807;
static const i8 C_I8_MIN = -9223372036854775806;
static const i8 C_I8_MAX = 9223372036854775806;
static const i8 C_BCD_MIN = -36028797018963967;
static const i8 C_BCD_MAX = 36028797018963967;
static const i8 C_CRW_HEADBYTES = 32;
static const i8 C_CRW_FIRSTHEAD = 640;
static const tET C_ENT_END_TICKET = 0xFFFF;
static const d8 C_FLT_MAX = 1.797693134862315708145274237317043567981e+308;
static const d8 C_FLT_MIN = -1.797693134862315708145274237317043567981e+308;
static const u8 C_YEAR_BASE  = 1900;
static const u8 C_YEAR_RANGE = 255;
static const u8 C_YEAR_MAX   = 2155;
static const i8 TMP_TBLCOL_SCALE  = 2;

static i8 iDaysPerMonth[13]     = { -1,31,28,31,30,31,30,31,31,30,31,30,31};
static i8 iDaysPerMonthLeap[13] = { -1,31,29,31,30,31,30,31,31,30,31,30,31};

#define C_BITS_PER_WORD 64
#define C_MAX_LVL_LEVELS 6
#define C_LOGIC_LOCALS 32
#define C_UNKNOWN -1

#include "../src/store/Ent_int.h"
#include "../src/base/eEC.h"
#include "../src/base/CastMacros.h"
#include "../src/base/LogicalMacros.h"
#include "../src/base/RelationalMacros.h"
#include "../src/base/MathMacros.h"
#include "../src/base/FunctionMacros.h"

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/