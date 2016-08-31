/*============================================================================*/
/* Transactions.h - includes for Transactions namespace                       */
/*============================================================================*/

#pragma once

#include "Structures.h"

#include "Tx.hpp"
#include "TxCollection.hpp"
#include "TxCursor.hpp"
#include "TxArray.hpp"
#include "TxMap.hpp"
#include "TxRef.hpp"

//-----------------------------------------------------------------------------
namespace Transactions
{
  void vCheckAssumptions(void);
}

using namespace Transactions;

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
