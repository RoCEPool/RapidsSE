/*============================================================================*/
/*  SkipNode.hpp - a node in the list                                         */
/*============================================================================*/
#pragma once

#include "DataStructures.h"

namespace DataStructures
{
  
  class SkipNode : public mn
  {

  public:
    i4        m_iForward;    // number of forward pointers in the node
    void*     m_pObject;     // ptr to object
    SkipNode* m_pForward[1]; // set of forward pointers
    
    void* operator new(size_t iSize, i4 iLevel);
    SkipNode(void* pObject);
    SkipNode();
  };
  
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/