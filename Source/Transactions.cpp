/*============================================================================*/
/* Transactions.cpp - transactions functions                                  */
/*============================================================================*/

#include "Transactions.h"

/*----------------------------------------------------------------------------*/
/* things to check at boot time                                               */
/*----------------------------------------------------------------------------*/
void Transactions::vCheckAssumptions(void)
{
  ASSERT(sizeof(Tx)           < C_SIZE_SMALL_OBJECT);
  ASSERT(sizeof(TxCollection) < C_SIZE_SMALL_OBJECT);
  ASSERT(sizeof(TxMap)        < C_SIZE_SMALL_OBJECT);
  ASSERT(sizeof(TxMapArray)   < C_SIZE_LARGE_BUFFER);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/