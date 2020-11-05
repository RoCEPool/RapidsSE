/*============================================================================*/
/* Grid.hpp -- Grid of Nodes                                                  */
/*============================================================================*/
#pragma once

namespace Grids
{
  class Grid : public Object
  {
  private:
  
    Article* m_pArticle;
  
    s32    m_sGridNm;
  
    i8     m_nSlotsPerNode;    // no of partition slots on each node
  
    i8     m_nDefinedNodes;    // total number of nodes in the grid
    i8     m_nConnectedNodes;  // number of connected nodes in the grid

    i8     m_iThisNode;        // this node number
    Node*  m_pThisNode;        // this node pointer
  
    bool   m_bHasStarted;      // has the cluster started ?

    i8     m_iStartNode;       // node that started the cluster

    Node*  m_pNodes[C_MAX_NODES]; // array of nodes
  
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    Grid(i8 nDefinedNodes, i8 nSlotsPerNode);
   ~Grid(void) noexcept(false);
    
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);
  
    void vAddNode(Node* pNode);
    Article* pArticle(void);
    i8   nConnectedNodes(void);
    i8   nDefinedNodes(void);
    void vDumpPartitioning(void);
    Stz* szGridNm(void);
    void vHasStarted(bool bHasStarted);
    bool bHasStarted(void);
    Node* pFindNode(Stz* pName);
    Node* pGetNode(i8 i_Node);
    void vIncConnectedNodes(void);
    void vInit(i8 nNodes, i8 nSlotsPerNode);
    void vRemoveNode(Node* pNode);
    i8   nPartitions(void);
    i8   nSlotsPerNode(void);
    void vStartNode(i8 iStartNode);
    i8   iStartNode(void);
    i8   iThisNode(void);
    Node* pThisNode(void);
    void vThisNode(i8 iNode);
  
    Footer* aFooter(void);
  };
    
  Grid* _pGrid(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
