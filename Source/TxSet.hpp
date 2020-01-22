/*============================================================================*/
/* TxSet.hpp -- Transaction Set                                               */
/*============================================================================*/
#pragma once

namespace Transactions
{
  class TxSet : public Object
  {
  private:
    
    Lock m_okLock;  // lock for the set
  
    SkipListNL* m_pTxList; // skip list of tx's
    t8          m_tOldestPsp;  // oldest psp in the system
    
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    TxSet(void);
    ~TxSet(void) noexcept(false);
  
    void vAddTx(Tx* pTx);
    void vDumpAll(void);
    Tx*  pFindTx(i8 iTxId);
    Tx*  pFindTx_NL(i8 iTxId);
    i8   iOldestActiveId(void);
    t8   tOldestPsp(void);
    void vRemoveTx(Tx* pTx);
    void vRemoveTx_NL(Tx* pTx);
  
    void vGrabWrite(void);
    void vDropWrite(void);
  
    Footer* aFooter(void);
  };
  
  TxSet* _pTxSet(void* aAddr);
}

bool bTxSetCompareLocal(void* pObjectInsert, void* pObjectScan, bool& bMatch);

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
