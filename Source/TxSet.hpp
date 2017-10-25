/*============================================================================*/
/* TxSet.hpp -- Transaction Set                                               */
/*============================================================================*/
#pragma once

namespace Transactions
{
  class TxSet : public Object
  {
  private:
    
    SkipList* m_pLocalTxList; // skip list of local transactions
    SkipList* m_pDistTxList;  // skip list of distributed transactions
  
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    TxSet(void);
    ~TxSet(void);
  
    Footer* aFooter(void);
  };
  
  TxSet* _pTxSet(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
