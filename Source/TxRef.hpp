/*============================================================================*/
/* TxRef.hpp -- Transaction Reference                                         */
/*============================================================================*/

#pragma once

#include "Transactions.h"

namespace Transactions
{
  class TxRef : public Object
  {
  private:
    Tx* m_pTx;
    
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t, TxRef* pTxRef);
    void  operator delete(void*);
    
    TxRef(void);
    ~TxRef(void);
    
    i8   iTxId(void);
  
    bool bBeginTx(void);
    bool bCommitTx(void);
    bool bEndTx(void);
    bool bRollbackTx(void);
    
    Footer* pFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/