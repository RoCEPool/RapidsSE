/*============================================================================*/
/* TxCursor.hpp -- Cursor over Tx's in TxCollection                           */
/*============================================================================*/

#pragma once

#include "Transactions.h"

namespace Transactions
{
  class TxCursor : public Object,
                   public CollectionCursor_t<Tx>
  {
  private:
    
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void* operator new(size_t, TxCursor* pTxCursor);
    void  operator delete(void*);
    
    TxCursor(Collection_t<Tx>* pTxCollection);
    ~TxCursor(void);
    
    void vInit(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/