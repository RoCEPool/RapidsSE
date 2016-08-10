/*============================================================================*/
/* TxMapArray.hpp - Transaction Array Map                                     */
/*============================================================================*/

#pragma once

#include "Transactions.h"

namespace Transactions {
    
  class TxMapArray : public Object
  {
    friend class TxMap;
    
  private:
    const static i8 C_TX_ARRAY = 64 * C_1KB;
    
    Tx* m_pMap[C_TX_ARRAY];
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    TxMapArray(void);
    ~TxMapArray(void);
  };
    
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/