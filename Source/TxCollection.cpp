/*============================================================================*/
/* TxCollection.cpp -- Transaction Collection                                 */
/*============================================================================*/

#include "Transactions.h"

extern Heap*            gpSmallHeap;
extern TxCollection*    gpTxCollection;

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* TxCollection::operator new(size_t iSize, TxCollection* pTxCollection)
{
  return pTxCollection;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void TxCollection::operator delete(void* pBufferAddress)
{
  vFatal("fatal: gpTxCollection class cannot be deleted");
  
  //vExitFatal();
  
  exit(EXIT_FAILURE);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
TxCollection::TxCollection(void)
: Object(CN_TXCOLLECTION)
{
  _poolAddress_(this);
  
  ASSERT( ! gpTxCollection );
  
  gpTxCollection = this;
}

/*----------------------------------------------------------------------------*/
/* allocate a tx                                                              */
/*----------------------------------------------------------------------------*/
Tx* TxCollection::pAllocTx(void)
{
  _poolAddress_(this);
  
  Tx* pTx = reinterpret_cast<Tx*>(gpSmallHeap->pAllocObject());
  
  vAppend(pTx);
  
  return pTx;
}

/*----------------------------------------------------------------------------*/
/* free a tx                                                                  */
/*----------------------------------------------------------------------------*/
void TxCollection::vFreeTx(Tx* pTx)
{
  _poolAddress_(this);
  
  vRemove(pTx);
  
  gpSmallHeap->vFreeObject(pTx);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/