/*============================================================================*/
/* Grid.hpp -- Grid of Nodes                                                  */
/*============================================================================*/
#pragma once

namespace Grids
{
  class Grid : public Object
  {
  public:
    i8     m_nNodes;     // number of nodes in the grid
    i8     m_iThisNode;  // this node number
    Node*  m_pThisNode;  // this node pointer
    i8     m_iStartNode; // node that started the cluster
    bool   m_bHasStarted;// has the cluster started ?
    i8     m_nSlotsPerNode;     // no of partition slots on each node
    i8     m_nPartitions;  // total no of partitions in the grid
    
    // NOT HAPPY WITH NAME - alka
    i8     m_iPartition[C_MAX_SLOTS]; // partition no's on this node
  
    Node*  m_pNodes[C_MAX_NODES]; // array of nodes
  
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    Grid(i8 nNodes, i8 nSlotsPerNode);
    ~Grid(void) noexcept(false);
    
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);
  
    void vAddNode(Node* pNode);
    void vDumpPartitioning(void);
    void vHasStarted(bool bHasStarted);
    bool bHasStarted(void);
    Node* pFindNode(Stz* pName);
    Node* pGetNode(i8 i_Node);
    void vInit(i8 nNodes, i8 nSlotsPerNode);
    i8   nNodes(void);
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
