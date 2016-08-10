/*============================================================================*/
/* TxMap.hpp - Transaction Map                                                */
/*============================================================================*/

#pragma once

#include "Transactions.h"

namespace Transactions {
    
  class TxMap : public Object
  {
  private:
    TxMapArray* m_pTxMapArray; 
    
  public:
    void* operator new(size_t, TxMap* pTxMap);
    void  operator delete(void*);
    
    TxMap(void);
    ~TxMap(void);
    
    void vSetTx(i8 iTx, Tx* pTx);
  };
    
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/