/*============================================================================*/
/* gpTxCollection.hpp -- Transaction Collection                               */
/*============================================================================*/

#pragma once

#include "Transactions.h"

namespace Transactions {
    
  class TxCollection : public Object,
                       public Collection_t<Tx>
  {
  private:
    
  public:
    void* operator new(size_t, TxCollection* pTxCollection);
    void  operator delete(void*);
    
    TxCollection(void);
    ~TxCollection(void);
    
    Tx*   pAllocTx(void);
    void  vFreeTx(Tx* pTx);
  };
    
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/