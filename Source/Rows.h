/*============================================================================*/
/* Rows.h -- Rows namespace                                                   */
/*============================================================================*/
#pragma once

#include "ScanSprint.h"

//------------------------------------------------------------------------------

namespace Rows
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

static const i8 C_TRAY_ROWS = (C_SIZE_LARGE_OBJECT - C_1KB) / sizeof(HeadLine);

//typedef       HV <Record,ColumnVec>                            HV_Rec;
typedef       HV <Row,   ColumnVec>                            HV_Row;

typedef    Nub_t <CN_ROWTRAY,HeadLine,OverLine,TailLine,Table> RowNub;

typedef TrayNL_t <CN_ROWTRAY,HeadLine,OverLine,TailLine,Table,
                  (C_TRAY_ROWS * sizeof(HeadLine))>            RowTray;

//------------------------------------------------------------------------------

#include "Encoding.hpp"
#include "SP_Encoding.hpp"

#include "CrwBuffer.hpp"
#include "QuickSort.hpp"
#include "RowCompactor.hpp"
#include "RowDecoder.hpp"
#include "RowExtractor.hpp"
#include "RowInflator.hpp"

#include "RefSet.hpp"
#include "RefBuffer.hpp"

#include "SrtSet.hpp"
#include "SrtBuffer.hpp"

#include "BufferCursor.hpp"
#include "BufferSort.hpp"
#include "IntBuffer.hpp"

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
