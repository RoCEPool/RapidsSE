/*============================================================================*/
/* Buffers.h -- Buffers namespace                                             */
/*============================================================================*/
#pragma once

#include "Records.h"

//------------------------------------------------------------------------------

namespace Buffers
{  
  void vCheckAssumptions(void);
}

//------------------------------------------------------------------------------

#include "HeadLine.hpp"
#include "OverLine.hpp"
#include "TailLine.hpp"
#include "TailStr.hpp"
#include "Crw.hpp"

//------------------------------------------------------------------------------

static const i8 C_TRAY_ROWS = (C_SIZE_LARGE_OBJECT - C_1KB) / sizeof(HeadLine);

typedef       HV <Row,   StreamDef>                            HV_Row;

typedef    Nub_t <CN_ROWTRAY,HeadLine,OverLine,TailLine,Table> RowNub;

typedef TrayNL_t <CN_ROWTRAY,HeadLine,OverLine,TailLine,Table,
                  (C_TRAY_ROWS * sizeof(HeadLine))>            RowTray;

//------------------------------------------------------------------------------

#include "RowCoder.hpp"
#include "ExtractRow.hpp"
#include "Encoding.hpp"
#include "Packing.hpp"
#include "SP_Encoding.hpp"

#include "DataBuffer.hpp"


#include "IrwBuffer.hpp"
#include "CrwBuffer.hpp"
#include "CsvBuffer.hpp"
#include "QuickSort.hpp"
#include "RowCompactor.hpp"
#include "RowDecoder.hpp"
#include "RowInflator.hpp"

#include "SrtSet.hpp"
#include "SrtBuffer.hpp"

#include "BufferCursor.hpp"
#include "BufferSort.hpp"
#include "IntBuffer.hpp"
#include "WriteBehind.hpp"

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
