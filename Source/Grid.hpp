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
  
    s32    m_sGridNm;          // name of this grid
    i8     m_iConnectCode;     // connect code for this grid (not yet used)
    
    i8     m_iThisNode;        // this node number
    Node*  m_pThisNode;        // this node pointer
  
    bool   m_bHasStarted;      // has the cluster started ?
    i8     m_iStartNode;       // node that started the cluster

    i8     m_nBootNodes;       // number of nodes supplied via SE -node
    i8     m_nDefinedNodes;    // number of nodes in the nodes array
    i8     m_nConnectedNodes;  // number of connected nodes in the grid
    i8     m_nNodes;           // number of nodes in array
    Node*  m_pNodes[C_MAX_NODES]; // array of nodes
  
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    Grid(Stz* szGridNm, i8 iConnectCode);
   ~Grid(void) noexcept(false);
    
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);
  
    Article* pArticle(void);
    i8   nBootNodes(void);
    i8   nDefinedNodes(void);
    i8   nConnectedNodes(void);
    void vDsv(void);
    Stz* szGridNm(void);
    void vHasStarted(bool bHasStarted);
    bool bHasStarted(void);
    Node* pFindNode(Stz* pName);
    Node* pGetNode(i8 i_Node);
    void vIncBootNodes(void);
    void vIncDefinedNodes(void);
    void vIncConnectedNodes(void);
    i8   nNodes(void);
    void vPutNode(Node* pNode);
    void vRemoveNode(Node* pNode);
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
