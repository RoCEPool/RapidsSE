/*============================================================================*/
/* CompactBuffers.h -- CompactBuffers namespace                               */
/*============================================================================*/
#pragma once

#include "InfoData.h"

//------------------------------------------------------------------------------

namespace CompactBuffers
{  
  void vCheckAssumptions(void);
}

//------------------------------------------------------------------------------

#include "RowCoder.hpp"
#include "HeadLine.hpp"
#include "OverLine.hpp"
#include "TailLine.hpp"
#include "TailStr.hpp"
#include "Crw.hpp"

//------------------------------------------------------------------------------

static const i8 C_CRW_HEADBYTES = 32;

static const i8 C_CRW_FIRSTHEAD  = 0x210;

static const i8 C_TRAY_ROWS = (C_SIZE_LARGE_OBJECT - C_1KB) / sizeof(HeadLine);

typedef    Nub_t <CN_ROWTRAY,HeadLine,OverLine,TailLine,StreamDef> RowNub;

typedef TrayNL_t <CN_ROWTRAY,HeadLine,OverLine,TailLine,StreamDef,
                  (C_TRAY_ROWS * sizeof(HeadLine))>            RowTray;

//------------------------------------------------------------------------------

#define STRx1(__uLow) (__uLow)            //   0B .. |1 * 16 =  16| ..  15B
#define STRx2(__uLow) ( 16 + 2 * __uLow)  //  16B .. |2 * 16 =  32| ..  47B
#define STRx4(__uLow) ( 48 + 4 * __uLow)  //  48B .. |4 * 16 =  64| .. 111B
#define STRx8(__uLow) (112 + 8 * __uLow)  // 112B .. |8 * 16 = 128| .. 239B

#define METACLUE_FORM(__high,__low) ( (u1) ( (u1)__high << 4 | (u1)__low )  )

#define METACLUE_HIGH(__uClue) ( __uClue >> 4 )
#define METACLUE_LOW(__uClue) ( __uClue & 0x0F )

#define METACLUE_SPLIT(__uClue,__iHigh,__iLow) \
  __iHigh = (__uClue >> 4); \
  __iLow  = (__uClue &  0x0F);

#include "Encoding_v1.hpp"
#include "Encoding_v2.hpp"
#include "Encoding.hpp"
#include "Packing_v1.hpp"
#include "Packing_v2.hpp"

#include "CrwBuffer.hpp"
#include "RowCompactor.hpp"
#include "ColDecoder.hpp"
#include "RowInflator.hpp"

#include "Lrw.hpp"
#include "LrwBuffer.hpp"

#include "SP_Crw.hpp"
#include "SP_Encoding.hpp"
#include "SP_Encoding_v2.hpp"

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
