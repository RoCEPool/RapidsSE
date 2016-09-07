/*============================================================================*/
/*  SkipNode.hpp - a node in the list                                         */
/*============================================================================*/
#pragma once

#include "Structures.h"

namespace Structures
{
  class SkipNode : public Object
  {
  public:
    i4        m_iForward;    // number of forward pointers in the node
    void*     m_pObject;     // ptr to object
    SkipNode* m_pForward[1]; // set of forward pointers

    // must be last
    Footer m_xFooter;
    
    void* operator new(size_t iSize, i4 iLevel);
    SkipNode(void* pObject);
    SkipNode();
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/