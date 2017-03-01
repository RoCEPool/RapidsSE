/*============================================================================*/
/* SkipNode.hpp -- Node in Skip List                                          */
/*============================================================================*/
#pragma once

#include "Structures.h"

namespace Structures
{
  static const i4 C_SKIPLIST_LEVELS = 16;
  
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
    
  private:
    
    Footer* pFooter(void);
    
    static i8 iMaxSize(void);
  };
  
  SkipNode* _pSkipNode(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
