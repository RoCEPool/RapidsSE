/*============================================================================*/
/* TxMap.hpp -- Transaction Map                                               */
/*============================================================================*/

#pragma once

#include "Transactions.h"

namespace Transactions
{
  class TxMap : public Object
  {
  private:
    TxArray* m_pTxArray;
    
  public:
    void* operator new(size_t, TxMap* pTxMap);
    void  operator delete(void*);
    
    TxMap(void);
    ~TxMap(void);
    
    void vSetTx(i8 iTx, Tx* pTx);
    
    Tx*  pBeginTx(void);
    void vEndTx(Tx* pTx);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/