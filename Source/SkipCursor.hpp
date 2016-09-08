/*============================================================================*/
/*  SkipCursor.hpp - a cursor in the list                                     */
/*============================================================================*/
#pragma once

#include "Structures.h"

namespace Structures
{
  class SkipCursor : public Object
  {
    friend class SkipList;
    
  private:
    SkipNode* m_pCurrentNode;
    
    // must be last
    Footer m_xFooter;
    
  public:
    SkipCursor(void);
    ~SkipCursor(void);
    
    Footer* pFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/