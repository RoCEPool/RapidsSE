/*============================================================================*/
/* GraphNode.hpp -- Node in Graph of Nodes                                    */
/*============================================================================*/

#pragma once

#include "Structures.h"

namespace Structures
{
  class GraphNode : public Object
  {
  private:
    
    // must be last
    Footer m_xFooter;
    
   public:
    void* operator new(size_t);
    void* operator new(size_t, GraphNode* pGraphNode);
    void  operator delete(void* pGraphNodeAddress);
 
    GraphNode(void);
    ~GraphNode(void);
    
    static void vDump(eCN eClass, Object* pObject, const char* pTab);
    
  private:
    
    Footer* pFooter(void);
  };
  
  GraphNode* _pGraphNode(void* pAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
