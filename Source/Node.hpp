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
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    Node(i8 iNodeNo);
    ~Node(void);
    
    static void vDump(eCN eClass, Object* pObject, const char* pTab);
    
    void vInit(void);
    i8   iNodeNo(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/