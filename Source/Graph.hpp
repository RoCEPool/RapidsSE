/*============================================================================*/
/* Graph.hpp -- Graph of Nodes                                                */
/*============================================================================*/

#pragma once

#include "Structures.h"

namespace Structures
{
  class Graph : public Object
  {
  private:
    Lock       m_zLock;
    GraphNode* m_pRootNode;
    i8         m_iCount;
    
    // must be last
    Footer m_xFooter;
    
   public:
    void* operator new(size_t);
    void* operator new(size_t, Graph* pGraph);
    void  operator delete(void* pGraphAddress);
 
    Graph(void);
    ~Graph(void);
    
    static void vDump(eCN eClass, Object* pObject, const char* pTab);
    
    i8 iCount(void);
    
  private:
    
    Footer* pFooter(void);
  };
  
  Graph* _pGraph(void* pAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
