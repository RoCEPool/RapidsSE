/*============================================================================*/
/* TxSet.cpp -- Transaction Set                                               */
/*============================================================================*/

#include "Transactions.h"

/*----------------------------------------------------------------------------*/
/* compare two tx's by their local tx id's                                    */
/*----------------------------------------------------------------------------*/
bool bTxSetCompareLocal(void* pObjectInsert, void* pObjectScan, bool& bMatch)
{
  Tx* pInsertEntry = _pTx(pObjectInsert);
  Tx* pScanEntry   = _pTx(pObjectScan);
  
  if ( ! pObjectScan )
    return false;
  
  if (pInsertEntry->iLocalTxId() == pScanEntry->iLocalTxId())
    bMatch = true;
  else
    bMatch = false;
  
  if (pInsertEntry->iLocalTxId() > pScanEntry->iLocalTxId())
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
  
  RASSERT_TYPE(pTxSet,CN_TXSET);
  
  return pTxSet;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* TxSet::operator new(size_t)
{
  void* pObject = HeapMem::pAllocate(CN_TXSET);
  
  return pObject;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void TxSet::operator delete(void*)
{
  vFatal("fatal: TxSet class cannot be deleted"_Stz);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
TxSet::TxSet(i8 iNodeNo)
: Object(CN_TXSET)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TXSET,_FOOTER_);
  
  m_oLock.vInitLock(LK_TX_SET);
  m_iNodeNo = iNodeNo;
  m_iNextTxId = iNodeNo << 48;
  m_pLocalTxList = new SkipList(true,bTxSetCompareLocal);
  m_pDistTxList  = new SkipList(true,bTxSetCompareDist);
  
  _checkInitialised_(CN_TXSET);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_TXSET,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* add given local tx                                                         */
/*----------------------------------------------------------------------------*/
void TxSet::vAddLocalTx(Tx* pTx)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TXSET,_FOOTER_);
  
  m_oLock.vGrabWrite();
  
  i8 iTxId = m_iNextTxId++;
  
  pTx->vLocalTxId(iTxId);
  pTx->vDistTxId(iTxId);
  
  m_pLocalTxList->bInsert(pTx);
  m_pDistTxList->bInsert(pTx);
  
  m_oLock.vDropWrite();
}

/*----------------------------------------------------------------------------*/
/* add given distributed tx (non-locking)                                     */
/*----------------------------------------------------------------------------*/
void TxSet::vAddDistTx_NL(Tx* pTx)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TXSET,_FOOTER_);
  
  i8 iLocalTxId = m_iNextTxId++;
  
  pTx->vLocalTxId(iLocalTxId);
  
  m_pLocalTxList->bInsert(pTx);
  m_pDistTxList->bInsert(pTx);
}

/*----------------------------------------------------------------------------*/
/* dump all tx's                                                              */
/*----------------------------------------------------------------------------*/
void TxSet::vDumpAll(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TXSET,_FOOTER_);
  
  m_oLock.vGrabRead();
  
//  SkipCursor xCursor;
//  
//  vTrace("All tx's:\n"_Stz);
//  while ( Tx* pTx = _pTx(m_pLocalTxList->pEach(&xCursor)) )
//    pTx->vContentDump();

  vTrace("All tx's:\n"_Stz);
  for (SkipNode* pNode = m_pLocalTxList->pFirst(0);
       pNode;
       pNode = m_pLocalTxList->pNext(pNode,0)) {
       
    Tx* pTx = _pTx(pNode->m_pObject);
    pTx->vContentDump();
  }
  
  m_oLock.vDropRead();
}

/*----------------------------------------------------------------------------*/
/* find a tx by tx id                                                         */
/*----------------------------------------------------------------------------*/
Tx* TxSet::pFindTx(i8 iTxId)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TXSET,_FOOTER_);
  
  m_oLock.vGrabRead();
  
  Tx* pTxFound = pFindTx_NL(iTxId);
  
  m_oLock.vDropRead();
  
  return pTxFound;
}

/*----------------------------------------------------------------------------*/
/* find a tx by tx id (no locking)                                            */
/*----------------------------------------------------------------------------*/
Tx* TxSet::pFindTx_NL(i8 iTxId)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TXSET,_FOOTER_);
  
  ASSERT(m_oLock.bIsLocked());
  
  Tx* pTxFind = new Tx(false);
  
  i8 iNodeNo = iTxId >> 48;
  
  Tx* pTxFound = nullptr;
  
  if (iNodeNo == m_iNodeNo) {
    pTxFind->vLocalTxId(iTxId);
    pTxFound = _pTx(m_pLocalTxList->pFind(pTxFind));
  }
  else {
    pTxFind->vDistTxId(iTxId);
    pTxFound = _pTx(m_pDistTxList->pFind(pTxFind));
  }
  
  pTxFind->vLocalTxId(-1);
  delete pTxFind;
  
  return pTxFound;
}

/*----------------------------------------------------------------------------*/
/* remove given tx                                                            */
/*----------------------------------------------------------------------------*/
void TxSet::vRemoveTx(Tx* pTx)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TXSET,_FOOTER_);
  
  bool bRemoved;
  
  m_oLock.vGrabWrite();
  
  bRemoved = m_pLocalTxList->bRemove(pTx);
  
  bRemoved = m_pDistTxList->bRemove(pTx);
  
  m_oLock.vDropWrite();
}

/*----------------------------------------------------------------------------*/
/* find oldest active local tx id                                             */
/*----------------------------------------------------------------------------*/
i8 TxSet::iOldestActiveId(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TXSET,_FOOTER_);
  
  i8 iOldestActiveId = -1;
  
  m_oLock.vGrabRead();
  
  for (SkipNode* pNode = m_pLocalTxList->pFirst(0);
       pNode;
       pNode = m_pLocalTxList->pNext(pNode,0)) {
    
    Tx* pTx = _pTx(pNode->m_pObject);
    
    if (pTx->bIsActive()) {
      iOldestActiveId = pTx->iLocalTxId();
      break;
    }
  }
  
  m_oLock.vDropRead();
  
  return iOldestActiveId;
}

/*----------------------------------------------------------------------------*/
/* remove given tx (non-locking)                                              */
/*----------------------------------------------------------------------------*/
void TxSet::vRemoveTx_NL(Tx* pTx)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TXSET,_FOOTER_);
  
  ASSERT(m_oLock.bIsLocked());
  
  bool bRemoved;
  
  bRemoved = m_pLocalTxList->bRemove(pTx);
  
  bRemoved = m_pDistTxList->bRemove(pTx);
}

/*----------------------------------------------------------------------------*/
/* grab lock for write                                                        */
/*----------------------------------------------------------------------------*/
void TxSet::vGrabWrite(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TXSET,_FOOTER_);
  
  m_oLock.vGrabWrite();
}

/*----------------------------------------------------------------------------*/
/* drop lock for write                                                        */
/*----------------------------------------------------------------------------*/
void TxSet::vDropWrite(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TXSET,_FOOTER_);
  
  m_oLock.vDropWrite();
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
