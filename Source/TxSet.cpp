/*============================================================================*/
/* TxSet.cpp -- Transaction Set                                               */
/*============================================================================*/

#include "Transactions.h"

extern Heap* gpSmallHeap;

/*----------------------------------------------------------------------------*/
/* compare two tx's by their local tx id's                                    */
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
/* compare two tx's by their distributed tx id's                              */
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
/* allocate a local tx                                                        */
/*----------------------------------------------------------------------------*/
Tx* TxSet::pAllocLocalTx(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TXSET,_FOOTER_);
  
  m_xLock.vGrabWrite();
  
  Tx* pTx = new Tx;
  
  i8 iTxId = m_iNextTxId++;
  
  pTx->vTxId(iTxId);
  pTx->vDistTxId(iTxId);
  
  m_pLocalTxList->bInsert(pTx);
  m_pDistTxList->bInsert(pTx);

  m_xLock.vDropWrite();
  
  return pTx;
}

/*----------------------------------------------------------------------------*/
/* find a tx by tx id                                                         */
/*----------------------------------------------------------------------------*/
Tx* TxSet::pFindTx(i8 iTxId)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TXSET,_FOOTER_);
  
  Tx xTxFind;
  
  i8 iNodeNo = iTxId >> 48;
  
  m_xLock.vGrabRead();
  
  Tx* pTxFound = nullptr;
  
  if (iNodeNo == m_iNodeNo) {
    xTxFind.vTxId(iTxId);
    pTxFound = _pTx(m_pLocalTxList->pFind(&xTxFind));
  }
  else {
    xTxFind.vDistTxId(iTxId);
    pTxFound = _pTx(m_pDistTxList->pFind(&xTxFind));
  }
  
  m_xLock.vDropRead();
  
  return pTxFound;
}

/*----------------------------------------------------------------------------*/
/* attempt to find a tx by dist tx id, create if not found                    */
/*----------------------------------------------------------------------------*/
Tx* TxSet::pTestDistTx(i8 iDistTxId)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TXSET,_FOOTER_);
  
  Tx* pDistTx = nullptr;
  
  Tx xTxFind;
  xTxFind.vDistTxId(iDistTxId);
  
  m_xLock.vGrabWrite();
  
  Tx* pTxFound = _pTx(m_pDistTxList->pFind(&xTxFind));
  
  if (pTxFound) {
    m_xLock.vDropWrite();
    return pDistTx;
  }
  
  Tx* pTx = new Tx;
  
  i8 iTxId = m_iNextTxId++;
  
  pTx->vTxId(iTxId);
  pTx->vDistTxId(iDistTxId);
  
  m_pLocalTxList->bInsert(pTx);
  m_pDistTxList->bInsert(pTx);
  
  m_xLock.vDropWrite();
  
  return pDistTx;
}

/*----------------------------------------------------------------------------*/
/* free given tx                                                              */
/*----------------------------------------------------------------------------*/
void TxSet::vFreeTx(Tx* pTx)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TXSET,_FOOTER_);
  
  bool bRemoved;
  
  m_xLock.vGrabWrite();
 
  bRemoved = m_pLocalTxList->bRemove(pTx);

  bRemoved = m_pDistTxList->bRemove(pTx);
  
  m_xLock.vDropWrite();
 
  delete pTx;
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
