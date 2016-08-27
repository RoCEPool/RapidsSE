/*============================================================================*/
/*  SkipCursor.hpp - a cursor in the list                                     */
/*============================================================================*/
#pragma once

#include "Structures.h"

namespace Structures
{
  
  class SkipCursor : public mn
  {
    friend class Skip;
    
  private:
    SkipNode* m_pCurrentNode;
    
  public:
    SkipCursor(void);
    ~SkipCursor(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/