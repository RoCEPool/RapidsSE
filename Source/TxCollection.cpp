/*============================================================================*/
/* TxCollection.cpp -- Transaction Collection                                 */
/*============================================================================*/

#include "Transactions.h"

extern Heap*            gpSmallHeap;
extern TxCollection*    gpTxCollection;
extern TxMap*           gpTxMap;

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
TxCollection* Transactions::_pTxCollection(void* aAddr)
{
  TxCollection* pTxCollection = reinterpret_cast<TxCollection*>(aAddr);
  
  RASSERT( !pTxCollection || CN_TXCOLLECTION == pTxCollection->eClass() );
  
  return pTxCollection;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* TxCollection::operator new(size_t iSize, TxCollection* pTxCollection)
{
  RASSERT(iSize);
  return pTxCollection;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void TxCollection::operator delete(void* pBufferAddress)
{
  RASSERT(pBufferAddress);
  vFatal("fatal: gpTxCollection class cannot be deleted"_Str);
  
  NEVER_REACH;
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
  for (i8 i_TxSlot = 0; i_TxSlot < C_TX_ARRAY; i_TxSlot++) {
    
    // allocate tx
    Tx* pTx = reinterpret_cast<Tx*>(Heap::pAllocate(CN_TX));
    
    // initialize it
    new(pTx) Tx;
    pTx->vTxSlot(i_TxSlot);
    
    // push in onto the freelist
    vPushFree(pTx);
  }
  
  gpTxCollection = this;
  
  _checkInitialised_(CN_TXCOLLECTION);
  
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
Footer* TxCollection::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TXCOLLECTION);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
