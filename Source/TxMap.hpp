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
    Lock     m_xLock;      // lock
    i8       m_iSlot;      // given slot allocated
    i8       m_iId;        // id generated
    TxArray* m_pTxArray;   // the mapping array
    
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t, TxMap* pTxMap);
    void  operator delete(void*);
    
    TxMap(void);
    ~TxMap(void);
    
    Tx*  pBeginTx(void);
    void vEndTx(Tx* pTx);

  private:
    void vGiveSlot(Tx* pTx);
    void vFindSlot(void);
    void vTakeSlot(Tx* pTx);
    
    Footer* pFooter(void);
  };
  
  TxMap* _pTxMap(void* pAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/