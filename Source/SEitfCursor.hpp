/*============================================================================*/
/* SEitfCursor.hpp - SEitf List                                                 */
/*============================================================================*/

#pragma once

#include "Interface.h"

namespace Interface {
    
  class SEitfCursor : public Object,
                      public ListCursor_t<SEitf>
  {
  private:
    
  public:
    void* operator new(size_t, void* pSEitfCursorAddress);
    void  operator delete(void*);
    
    SEitfCursor(List_t<SEitf>* pList);
    ~SEitfCursor(void);
    
  };
    
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/