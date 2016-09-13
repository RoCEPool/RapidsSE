/*============================================================================*/
/* Node.hpp - Node                                                            */
/*============================================================================*/

#pragma once

#include "GridModel.h"

namespace GridModel
{
  class Node : public Object,
               public ListNode_t<Node>
  {
  private:
    i8    m_iNodeNo;
    
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void* operator new(size_t, Node* pNode);
    void  operator delete(void*);
    
    Node(void);
    ~Node(void);
    
    static void vDump(eCN eClass, Object* pObject, const char* pTab);
    
    void vInit(void);
    i8   iNodeNo(void);
    void vNodeNo(i8 iNodeNo);
    
    Footer* pFooter(void);
  };
  
  Node* _pNode(void* pAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/