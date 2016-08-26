/*============================================================================*/
/* Transactions.h - includes for Transactions namespace                       */
/*============================================================================*/

#pragma once

#include "Threading.h"

#include "Tx.hpp"
#include "TxCollection.hpp"
#include "TxCursor.hpp"
#include "TxMapArray.hpp"
#include "TxMap.hpp"

//-----------------------------------------------------------------------------
namespace Transactions
{
  void vCheckAssumptions(void);
}

using namespace Transactions;

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
