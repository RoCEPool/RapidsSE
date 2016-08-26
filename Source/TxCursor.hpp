/*============================================================================*/
/* TxCursor.hpp -- Cursor over Tx's in TxList                                 */
/*============================================================================*/

#pragma once

#include "Threading.h"

namespace Transactions
{
  class TxCursor : public Object,
                   public ListCursor_t<Tx>
  {
  private:
    
  public:
    void* operator new(size_t);
    void* operator new(size_t, TxCursor* pTxCursor);
    void  operator delete(void*);
    
    TxCursor(List_t<Tx>* pTxList);
    ~TxCursor(void);
    
    void vInit(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/