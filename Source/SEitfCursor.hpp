/*============================================================================*/
/* SEitfCursor.hpp - cursor over SEitf list                                   */
/*============================================================================*/

#pragma once

#include "Interface.h"

namespace Interface {
    
  class SEitfCursor : public Object,
                      public ListCursor_t<SEitf>
  {
  private:
    
    // must be last
    Footer m_xFooter;
    
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