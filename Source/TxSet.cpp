/*============================================================================*/
/* TxSet.cpp -- Transaction Set                                               */
/*============================================================================*/

#include "Transactions.h"

extern Heap* gpSmallHeap;

/*----------------------------------------------------------------------------*/
/* compare two local tx's                                                     */
/*----------------------------------------------------------------------------*/
bool bTxSetCompareLocal(void* pObjectInsert, void* pObjectScan, bool& bMatch)
{
  Tx* pInsertEntry = _pTx(pObjectInsert);
  Tx* pScanEntry   = _pTx(pObjectScan);
  
  if ( ! pObjectScan )
    return false;
  
  if (pInsertEntry->iTxId() == pScanEntry->iTxId())
    bMatch = true;
  else
    bMatch = false;
  
  if (pInsertEntry->iTxId() > pScanEntry->iTxId())
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* compare two distributed tx's                                               */
/*----------------------------------------------------------------------------*/
bool bTxSetCompareDist(void* pObjectInsert, void* pObjectScan, bool& bMatch)
{
  Tx* pInsertEntry = _pTx(pObjectInsert);
  Tx* pScanEntry   = _pTx(pObjectScan);
  
  if ( ! pObjectScan )
    return false;
  
  if (pInsertEntry->iDistTxId() == pScanEntry->iDistTxId())
    bMatch = true;
  else
    bMatch = false;
  
  if (pInsertEntry->iDistTxId() > pScanEntry->iDistTxId())
    return true;
  else
    return false;
}

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
TxSet::TxSet(i8 iNodeNo)
: Object(CN_TXSET)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TXSET,_FOOTER_);
  
  m_xLock.vInitLock(LK_TX_MAP);
  m_iNodeNo = iNodeNo;
  m_iNextTxId = iNodeNo << 48;
  m_pLocalTxList = new SkipList(true,bTxSetCompareLocal);
  m_pDistTxList  = new SkipList(true,bTxSetCompareDist);
  
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
