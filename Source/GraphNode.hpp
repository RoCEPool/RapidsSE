/*============================================================================*/
/* GraphNode.hpp -- Node in Graph of Nodes                                    */
/*============================================================================*/
#pragma once

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
    ~GraphNode(void) noexcept(false);
    
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);
    
    GraphNode* pChild(void);
    void       vChild(GraphNode* pChild);

    GraphNode* pNext(void);
    void       vNext(GraphNode* pNext);
    
    GraphNode* pParent(void);
    void       vParent(GraphNode* pParent);
    
    GraphNode* pPrev(void);
    void       vPrev(GraphNode* pPrev);
    
  private:
    
    Footer* aFooter(void);
  };
  
  GraphNode* _pGraphNode(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
