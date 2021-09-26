/*============================================================================*/
/* SkipNode.hpp -- Node in Skip List                                          */
/*============================================================================*/
#pragma once

namespace BASE
{
  static const i8 C_SKIPLIST_LEVELS = 16;
  
  class SkipNode : public Object
  {
  public:
    void*     m_pObject;    // ptr to object
    char      m_caFlag[C_SKIPLIST_LEVELS];  // debug/validation flags
    i8        m_nRight;     // number of right pointers in the node
    SkipNode* m_pRight[1];  // variable set of right pointers

    
  public:
    void* operator new(size_t, i8 iLevel, SlabAllocator* pSlabAllocator);
    void  operator delete(void*);
    
    SkipNode(void* pObject);
    SkipNode();
    
  private:
    
    Footer* aFooter(void);
    
    static i8 iMaxSize(void);
  };
  
  SkipNode* _pSkipNode(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
