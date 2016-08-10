/*============================================================================*/
/* TxList.cpp - Transaction Set                                                */
/*============================================================================*/

#include "Transactions.h"

extern Heap*      gpSmallHeap;
extern TxList*    gpTxList;

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* TxList::operator new(size_t iSize, TxList* pTxList)
{
  return pTxList;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void TxList::operator delete(void* pBufferAddress)
{
  vFatal("fatal: TxList class cannot be deleted");
  
  //vExitFatal();
  
  exit(EXIT_FAILURE);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
TxList::TxList(void)
: Object(CN_TXLIST)
{
  _poolAddress_(this);
  
  ASSERT( ! gpTxList );
  
  gpTxList = this;
}

/*----------------------------------------------------------------------------*/
/* allocate a tx                                                              */
/*----------------------------------------------------------------------------*/
Tx* TxList::pAllocTx(void)
{
  _poolAddress_(this);
  
  Tx* pTx = reinterpret_cast<Tx*>(gpSmallHeap->pAllocObject());
  
  vAppend(pTx);
  
  return pTx;
}


/*----------------------------------------------------------------------------*/
/* free a tx                                                                  */
/*----------------------------------------------------------------------------*/
void TxList::vFreeTx(Tx* pTx)
{
  _poolAddress_(this);
  
  vRemove(pTx);
  
  gpSmallHeap->vFreeObject(pTx);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/