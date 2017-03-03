/*============================================================================*/
/* Graph.hpp -- Graph of Nodes                                                */
/*============================================================================*/
#pragma once

namespace Structures
{
  class Graph : public Object
  {
  private:
    Lock       m_zLock;      // lock over all nodes
    GraphNode* m_pRootNode;  // root node
    i8         m_iCount;     // number of nodes
    
    // must be last
    Footer m_xFooter;
    
   public:
    void* operator new(size_t);
    void* operator new(size_t, Graph* pGraph);
    void  operator delete(void* pGraphAddress);
 
    Graph(void);
    ~Graph(void);
    
    static void vDump(eCN eClass, Object* pObject, const char* aTab);
    
    i8         iCount(void);
    
    void       vGrabWrite(void);
    void       vDropWrite(void);
    
    GraphNode* pRootNode(void);
    void       vRootNode(GraphNode* pRootNode);
    
  private:
    
    Footer* pFooter(void);
  };
  
  Graph* _pGraph(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
