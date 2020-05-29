/*============================================================================*/
/* TxSet.hpp -- Transaction Set                                               */
/*============================================================================*/
#pragma once

namespace Transactions
{
  class TxSet : public Object
  {
  private:
    
    Lock      m_okLock;        // lock for the set
    i8        m_iNodeNo;      // this node no
    i8        m_iNextTxId;    // next tx id
    SkipListNL* m_pLocalTxList; // skip list of tx's by local tx id
    SkipListNL* m_pDistTxList;  // skip list of tx's by distributed tx id
  
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    TxSet(i8 iNodeNo);
    ~TxSet(void) noexcept(false);
  
    void vAddLocalTx(Tx* pTx);
    void vAddDistTx_NL(Tx* pTx);
    void vDumpAll(void);
    Tx*  pFindTx(i8 iTxId);
    Tx*  pFindTx_NL(i8 iTxId);
    i8   iOldestActiveId(void);
    void vRemoveTx(Tx* pTx);
    void vRemoveTx_NL(Tx* pTx);
  
    void vGrabWrite(void);
    void vDropWrite(void);
  
    Footer* aFooter(void);
  };
  
  TxSet* _pTxSet(void* aAddr);
}

bool bTxSetCompareLocal(void* pObjectInsert, void* pObjectScan, bool& bMatch);
bool bTxSetCompareDist(void* pObjectInsert, void* pObjectScan, bool& bMatch);

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
