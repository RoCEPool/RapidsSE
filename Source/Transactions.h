/*============================================================================*/
/* Transactions.h - Transactions namespace                                    */
/*============================================================================*/

#pragma once

#include "Parsing.h"

//-----------------------------------------------------------------------------

namespace Transactions
{
  void vCheckAssumptions(void);
}

using namespace Transactions;

//-----------------------------------------------------------------------------

#include "Tx.hpp"
#include "TxCollection.hpp"
#include "TxCursor.hpp"
#include "TxArray.hpp"
#include "TxMap.hpp"
#include "TxRef.hpp"

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
