/*============================================================================*/
/* Rows.h -- Rows namespace                                                   */
/*============================================================================*/
#pragma once

#include "Transactions.h"

//------------------------------------------------------------------------------

namespace Rows
{  
  void vCheckAssumptions(void);
}

//------------------------------------------------------------------------------

static const i8 C_WSP_ROWBUFFER = C_SIZE_LARGE_OBJECT - C_16KB; // ref NmlStr

#include "Ref.hpp"

#include "Row.hpp"
#include "NmlStr.hpp"
#include "SP_RowPix.hpp"
#include "Record.hpp"
typedef HV<Record,RowPix>       HV_Record;
#include "RowBox_t.hpp"


#include "RowSingle.hpp"

#include "RowGenerator.hpp"


//------------------------------------------------------------------------------

typedef HV<RowSingle,StreamDef> HV_RowSingle;

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
