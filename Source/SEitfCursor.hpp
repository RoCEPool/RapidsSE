/*============================================================================*/
/* SEitfCursor.hpp - cursor over SEitf list                                   */
/*============================================================================*/
#pragma once

#include "Interface.h"

namespace Interface
{    
  class SEitfCursor : public Object,
                      public ListCursor_t<SEitf>
  {
  private:
    
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t, SEitfCursor* pSEitfCursor);
    void  operator delete(void*);
    
    SEitfCursor(List_t<SEitf>* pList);
    ~SEitfCursor(void);
    
    Footer* pFooter(void);
  };
    
  SEitfCursor* _pSEitfCursor(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
