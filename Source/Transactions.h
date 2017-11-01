/*============================================================================*/
/* Transactions.h - Transactions namespace                                    */
/*============================================================================*/
#pragma once

#include "Parsing.h"

//-----------------------------------------------------------------------------

namespace Transactions
{
  static const i8 C_SLOTS_PER_NODE = 128;
  static const i8 C_MAX_NODES = 32;
  static const i8 C_MAX_PARTITIONS = C_SLOTS_PER_NODE * C_MAX_NODES;
  
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
#include "TxSet.hpp"

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
