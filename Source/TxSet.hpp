/*============================================================================*/
/* TxSet.hpp -- Transaction Set                                               */
/*============================================================================*/
#pragma once

namespace Transactions
{
  class TxSet : public Object
  {
  private:
    
    Lock      m_xLock;        // lock for the set
    i8        m_iNodeNo;      // this node no
    i8        m_iNextTxId;    // next tx id
    SkipList* m_pLocalTxList; // skip list of tx's by local tx id
    SkipList* m_pDistTxList;  // skip list of tx's by distributed tx id
  
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    TxSet(i8 iNodeNo);
    ~TxSet(void);
  
    void vAddTx(Tx* pTx);
    Tx*  pAllocLocalTx(void);
    Tx*  pFindTx(i8 iTxId);
    void vRemoveTx(Tx* pTx);
    Tx*  pTestDistTx(i8 iDistTxId);
  
    void vFreeTx(Tx* pTx);
  
    Footer* aFooter(void);
  };
  
  TxSet* _pTxSet(void* aAddr);
}

bool bTxSetCompareLocal(void* pObjectInsert, void* pObjectScan, bool& bMatch);
bool bTxSetCompareDist(void* pObjectInsert, void* pObjectScan, bool& bMatch);

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
