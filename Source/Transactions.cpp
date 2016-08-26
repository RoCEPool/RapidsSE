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
  ASSERT(sizeof(TxArray)      < C_SIZE_LARGE_BUFFER);
  ASSERT(sizeof(TxCollection) < C_SIZE_SMALL_OBJECT);
  ASSERT(sizeof(TxMap)        < C_SIZE_SMALL_OBJECT);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/