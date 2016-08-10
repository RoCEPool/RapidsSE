/*============================================================================*/
/* TxList.hpp - Transaction List                                              */
/*============================================================================*/

#pragma once

#include "Transactions.h"

namespace Transactions {
    
  class TxList : public Object,
                 public Collection_t<Tx>
  {
  private:
    
  public:
    void* operator new(size_t, TxList* pTxList);
    void  operator delete(void*);
    
    TxList(void);
    ~TxList(void);
    
    Tx*   pAllocTx(void);
    void  vFreeTx(Tx* pTx);
  };
    
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/