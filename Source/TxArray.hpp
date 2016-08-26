/*============================================================================*/
/* TxArray.hpp -- Transaction Array                                           */
/*============================================================================*/

#pragma once

#include "Transactions.h"

namespace Transactions
{
  const static i8 C_TX_ARRAY = 16;
  
  class TxArray : public Object
  {
    friend class TxMap;
    
  private:
    
    Tx* m_pTx[C_TX_ARRAY];
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    TxArray(void);
    ~TxArray(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/