/*============================================================================*/
/* TxSet.hpp -- Transaction Set                                               */
/*============================================================================*/
#pragma once

namespace Transactions
{
  class TxSet : public Object
  {
  private:
    
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
