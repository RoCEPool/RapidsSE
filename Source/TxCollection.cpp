/*============================================================================*/
/* TxCollection.cpp -- Transaction Collection                                 */
/*============================================================================*/

#include "Transactions.h"

extern Heap*            gpSmallHeap;
extern TxCollection*    gpTxCollection;
extern TxMap*           gpTxMap;

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
  
  // we allocate all tx objects up front and push them onto free chain
  for (i8 iTxNo = 0; iTxNo < C_TX_ARRAY; iTxNo++) {
    
    // allocate tx
    Tx* pTx = reinterpret_cast<Tx*>(gpSmallHeap->pAllocObject());
    
    // initialize it
    pTx->vTxNo(iTxNo);
    
    // push in onto the freelist
    vPushFree(pTx);
  }
  
  gpTxCollection = this;
}

/*----------------------------------------------------------------------------*/
/* allocate a tx                                                              */
/*----------------------------------------------------------------------------*/
Tx* TxCollection::pAllocTx(void)
{
  _poolAddress_(this);
  
  Tx* pTx = pAllocObject();
  
  return pTx;
}

/*----------------------------------------------------------------------------*/
/* free a tx                                                                  */
/*----------------------------------------------------------------------------*/
void TxCollection::vFreeTx(Tx* pTx)
{
  _poolAddress_(this);
  
  vFreeObject(pTx);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/