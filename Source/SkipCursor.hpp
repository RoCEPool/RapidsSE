/*============================================================================*/
/*  SkipCursor.hpp - a cursor in the list                                     */
/*============================================================================*/
#pragma once

#include "DataStructures.h"

namespace DataStructures {
  
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