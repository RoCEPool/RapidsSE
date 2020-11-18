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
  
  if (pInsertEntry->tPerspective() == pScanEntry->tPerspective())
    bMatch = true;
  else
    bMatch = false;
  
  if (pInsertEntry->tPerspective() > pScanEntry->tPerspective())
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
  vThrow(FK_FATAL,VL_FATAL,__func__,__FILE__,__LINE__,
         "fatal: TxSet class cannot be deleted"_Stz);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
TxSet::TxSet(void)
: Object(CN_TXSET)
, m_okLock(LK_TX_SET,this)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TXSET,_FOOTER_);
  
  m_pTxList = new SkipListNL(C_UNQIUE_KEY,bTxSetCompareLocal);
  m_tOldestPsp = C_UNKNOWN; // AM::iNextTxId();
  
  _checkInitialised_(CN_TXSET);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_TXSET,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* add given tx                                                               */
/*----------------------------------------------------------------------------*/
void TxSet::vAddTx(Tx* pTx)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TXSET,_FOOTER_);
  
  m_okLock.vGrabWrite();

  m_pTxList->bInsert(pTx);
  
  m_okLock.vDropWrite();
}

/*----------------------------------------------------------------------------*/
/* dump all tx's                                                              */
/*----------------------------------------------------------------------------*/
void TxSet::vDumpAll(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TXSET,_FOOTER_);
  
  m_okLock.vGrabRead();
  
//  SkipCursor xCursor;
//  
//  vTrace("All tx's:\n"_Stz);
//  while ( Tx* pTx = _pTx(m_pTxList->pEach(&xCursor)) )
//    pTx->vContentDump();

  vTrace("All tx's:\n"_Stz);
  for (SkipNode* pNode = m_pTxList->pFirst(0);
       pNode;
       pNode = m_pTxList->pNext(pNode,0)) {
       
    Tx* pTx = _pTx(pNode->m_pObject);
    pTx->vContentDump();
  }
  
  m_okLock.vDropRead();
}

/*----------------------------------------------------------------------------*/
/* find a tx by tx id                                                         */
/*----------------------------------------------------------------------------*/
Tx* TxSet::pFindTx(i8 iTxId)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TXSET,_FOOTER_);
  
  m_okLock.vGrabRead();
  
  Tx* pTxFound = pFindTx_NL(iTxId);
  
  m_okLock.vDropRead();
  
  return pTxFound;
}

/*----------------------------------------------------------------------------*/
/* find a tx by tx id (no locking)                                            */
/*----------------------------------------------------------------------------*/
Tx* TxSet::pFindTx_NL(i8 iTxId)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TXSET,_FOOTER_);
  
  ASSERT(m_okLock.bIsLocked());
  
  Tx* pTxFind = new Tx;
  
  i8 iNodeNo = iTxId >> 48;
  
  Tx* pTxFound = nullptr;
  
    pTxFind->vPerspective(iTxId);
    pTxFound = _pTx(m_pTxList->pFind(pTxFind));
  
  pTxFind->vPerspective(-1);
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
  
  m_okLock.vGrabWrite();
  
  bRemoved = m_pTxList->bRemove(pTx);
  
  if (pTx->tPerspective() > m_tOldestPsp)
    m_tOldestPsp = pTx->tPerspective();
  
  m_okLock.vDropWrite();
}

/*----------------------------------------------------------------------------*/
/* find oldest active local tx id                                             */
/*----------------------------------------------------------------------------*/
i8 TxSet::iOldestActiveId(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TXSET,_FOOTER_);
  
  i8 iOldestActiveId = -1;
  
  m_okLock.vGrabRead();
  
  for (SkipNode* pNode = m_pTxList->pFirst(0);
       pNode;
       pNode = m_pTxList->pNext(pNode,0)) {
    
    Tx* pTx = _pTx(pNode->m_pObject);
    
    if (pTx->bIsActive()) {
      iOldestActiveId = pTx->tPerspective();
      break;
    }
  }
  
  m_okLock.vDropRead();
  
  return iOldestActiveId;
}

/*----------------------------------------------------------------------------*/
/* get oldest psp                                                             */
/*----------------------------------------------------------------------------*/
i8 TxSet::tOldestPsp(void)
{
  return m_tOldestPsp;
}

/*----------------------------------------------------------------------------*/
/* remove given tx (non-locking)                                              */
/*----------------------------------------------------------------------------*/
void TxSet::vRemoveTx_NL(Tx* pTx)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TXSET,_FOOTER_);
  
  ASSERT(m_okLock.bIsLocked());
  
  bool bRemoved;
  
  bRemoved = m_pTxList->bRemove(pTx);
}

/*----------------------------------------------------------------------------*/
/* grab lock for write                                                        */
/*----------------------------------------------------------------------------*/
void TxSet::vGrabWrite(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TXSET,_FOOTER_);
  
  m_okLock.vGrabWrite();
}

/*----------------------------------------------------------------------------*/
/* drop lock for write                                                        */
/*----------------------------------------------------------------------------*/
void TxSet::vDropWrite(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TXSET,_FOOTER_);
  
  m_okLock.vDropWrite();
}


/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
__aFooterFunction(TxSet)

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
