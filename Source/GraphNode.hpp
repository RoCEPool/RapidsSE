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
    GraphNode* m_pParent;  // parent of this node
    GraphNode* m_pNext;    // next sibling
    GraphNode* m_pPrev;    // prev sibling
    GraphNode* m_pChild;   // child of this node
    
    // must be last
    Footer m_xFooter;
    
   public:
    void* operator new(size_t);
    void* operator new(size_t, GraphNode* pGraphNode);
    void  operator delete(void* pGraphNodeAddress);
 
    GraphNode(void);
    ~GraphNode(void);
    
    static void vDump(eCN eClass, Object* pObject, const char* aTab);
    
    GraphNode* pChild(void);
    void       vChild(GraphNode* pChild);

    GraphNode* pNext(void);
    void       vNext(GraphNode* pNext);
    
    GraphNode* pParent(void);
    void       vParent(GraphNode* pParent);
    
    GraphNode* pPrev(void);
    void       vPrev(GraphNode* pPrev);
    
  private:
    
    Footer* pFooter(void);
  };
  
  GraphNode* _pGraphNode(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
