/*============================================================================*/
/* DataBuffers.h -- DataBuffers namespace                                     */
/*============================================================================*/
#pragma once

#include "Records.h"

//------------------------------------------------------------------------------

namespace DataBuffers
{  
  void vCheckAssumptions(void);
}

//------------------------------------------------------------------------------

typedef       HV <RowSingle,   StreamDef>                      HV_Row;

//------------------------------------------------------------------------------

#include "DataBuffer.hpp"

#include "RowUte.hpp"

#include "RowBuffer.hpp"
#include "CsvMeta.hpp"
#include "CsvBuffer.hpp"

#include "HashSet.hpp"
#include "Pot.hpp"

#include "SP_DataBuffer.hpp"

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
