/*============================================================================*/
/* Transactions.h - Transactions namespace                                    */
/*============================================================================*/
#pragma once

#include "Maps.h"

//-----------------------------------------------------------------------------

namespace Transactions
{
  static const i8 C_SLOTS_PER_NODE = 128;
  static const i8 C_MAX_NODES = 16;
  static const i8 C_MAX_PARTITIONS = C_SLOTS_PER_NODE * C_MAX_NODES;
  
  void vCheckAssumptions(void);
}

using namespace Transactions;

//-----------------------------------------------------------------------------

#include "Tx.hpp"
#include "TxSet.hpp"

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
