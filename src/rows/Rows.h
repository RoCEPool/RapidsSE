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
#include "RowSlab.hpp"

#include "RowBox_t.hpp"
#include "RowSingle.hpp"

#include "RowGenerator.hpp"
#include "RowUte.hpp"

#include "SP_Row.hpp"
#include "SP_RowPix.hpp"

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
