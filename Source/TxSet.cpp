/*============================================================================*/
/* TxSet.cpp -- Transaction Set                                               */
/*============================================================================*/

#include "Transactions.h"

extern Heap* gpSmallHeap;

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
TxSet* Transactions::_pTxSet(void* aAddr)
{
  TxSet* pTxSet = reinterpret_cast<TxSet*>(aAddr);
  
  RASSERT( !pTxSet || CN_TXSET == pTxSet->eClass() );
  
  return pTxSet;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* TxSet::operator new(size_t iSize)
{
  RASSERT(iSize);
  
  void* pObject = Heap::pAllocate(CN_TXSET);
  
  return pObject;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void TxSet::operator delete(void* pBufferAddress)
{
  RASSERT(pBufferAddress);
  vFatal("fatal: gpTxSet class cannot be deleted"_Stz);
  
  NEVER_REACH;
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
TxSet::TxSet(void)
: Object(CN_TXSET)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TXSET,_FOOTER_);
  
  m_xLock.vInitLock(LK_TX_MAP);
  m_pLocalTxList = nullptr;
  m_pDistTxList  = nullptr;
  
  _checkInitialised_(CN_TXSET);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_TXSET,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* TxSet::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TXSET);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
