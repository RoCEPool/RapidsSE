/*============================================================================*/
/* SkipNode.hpp -- Node in Skip List                                          */
/*============================================================================*/
#pragma once

#include "Structures.h"

namespace Structures
{
  class SkipNode : public Object
  {
  public:
    void*     m_pObject;     // ptr to object
    i4        m_nForward;    // number of forward pointers in the node
    SkipNode* m_pForward[1]; // variable set of forward pointers

    
  public:
    void* operator new(size_t iSize, i4 iLevel);
    void  operator delete(void*);
    
    SkipNode(void* pObject);
    SkipNode();
    
    Footer* pFooter(void);
  };
  
  SkipNode* _pSkipNode(void* pAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/