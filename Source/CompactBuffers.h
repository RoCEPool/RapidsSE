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

#include "Encoding_v1.hpp"
#include "Packing.hpp"

#include "CrwBuffer.hpp"
#include "RowCompactor.hpp"
#include "ColDecoder.hpp"
#include "RowInflator.hpp"

#include "Lrw.hpp"
#include "LrwBuffer.hpp"

#include "SP_Crw.hpp"
#include "SP_Encoding.hpp"

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
