/*============================================================================*/
/* Records.h -- Records namespace                                             */
/*============================================================================*/
#pragma once

#include "Rows.h"

//------------------------------------------------------------------------------

namespace Records
{  
  void vCheckAssumptions(void);
}

//------------------------------------------------------------------------------

#include "CsvCursor.hpp"
#include "CsvField.hpp"
#include "CsvRecord.hpp"

#include "SP_Record.hpp"

#include "Record.hpp"
typedef HV<Record,StreamDef>   HV_Rec;

#include "RecordBuffer.hpp"

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
