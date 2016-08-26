/*============================================================================*/
/* TxCursor.hpp -- Cursor over Tx's in gpTxCollection                                 */
/*============================================================================*/

#pragma once

#include "Threading.h"

namespace Transactions
{
  class TxCursor : public Object,
                   public CollectionCursor_t<Tx>
  {
  private:
    
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