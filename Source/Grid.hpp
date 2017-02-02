/*============================================================================*/
/* Grid.hpp -- Grid of Nodes                                                  */
/*============================================================================*/

#pragma once

#include "GridModel.h"

namespace GridModel
{
  class Grid : public Object,
               public List_t<Node>
  {
  private:
    i8     m_iThisNode;  // this node number
    Node*  m_pThisNode;  // this node
    i8     m_iStartNode; // node that started the cluster
    bool   m_bHasStarted;
    
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t, Grid* pGrid);
    void  operator delete(void*);
    
    Grid(void);
    ~Grid(void);
    
    static void vDump(eCN eClass, Object* pObject, const char* pTab);
    void vHasStarted(bool bHasStarted);
    bool bHasStarted(void);
    Node* pFindNode(char* pName);
    Node* pFindNode(i8 iNode);
    void vStartNode(i8 iStartNode);
    i8   iStartNode(void);
    void vThisNode(i8 iNode);
    i8   iThisNode(void);
    Node* pThisNode(void);
    void vInit(void);
    
    Footer* pFooter(void);
  };
    
  Grid* _pGrid(void* pAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
