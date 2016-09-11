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
  ASSERT(iSize);
  return pTxCollection;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void TxCollection::operator delete(void* pBufferAddress)
{
  ASSERT(pBufferAddress);
  vFatal("fatal: gpTxCollection class cannot be deleted");
  
  ASSERT(0);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
TxCollection::TxCollection(void)
: Object(CN_TXCOLLECTION)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TXCOLLECTION,_FOOTER_);
  
  ASSERT( ! gpTxCollection );
  
  // we allocate all tx objects up front and push them onto free chain
  for (i8 iTxSlot = 0; iTxSlot < C_TX_ARRAY; iTxSlot++) {
    
    // allocate tx
    Tx* pTx = reinterpret_cast<Tx*>(gpSmallHeap->pAllocObject(CN_TX));
    
    // initialize it
    new(pTx) Tx;
    pTx->vTxSlot(iTxSlot);
    
    // push in onto the freelist
    vPushFree(pTx);
  }
  
  gpTxCollection = this;
  
  Memory::vCheckInitialised(this,sizeof(TxCollection));
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_TXCOLLECTION,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* allocate a tx                                                              */
/*----------------------------------------------------------------------------*/
Tx* TxCollection::pAllocTx(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TXCOLLECTION,_FOOTER_);
  
  Tx* pTx = pAllocObject();
  
  return pTx;
}

/*----------------------------------------------------------------------------*/
/* free a tx                                                                  */
/*----------------------------------------------------------------------------*/
void TxCollection::vFreeTx(Tx* pTx)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TXCOLLECTION,_FOOTER_);
  
  vFreeObject(pTx);
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* TxCollection::pFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TXCOLLECTION);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/