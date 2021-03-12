/*============================================================================*/
/* Grid.hpp -- Grid of Nodes                                                  */
/*============================================================================*/
#pragma once

#include <list>
#include <iterator>

namespace STORE
{
  class Grid : public Object
  {
  private:
  
    Article* m_pArticle;
  
    s32    m_sGridNm;          // name of this grid
    i8     m_iConnectCode;     // connect code for this grid (not yet used)
    
    i8     m_iThisNodeNo;      // this node number
    Node*  m_pThisNode;        // this node pointer
  
    bool   m_bHasStarted;      // has the cluster started ?
    i8     m_iStartNodeNo;     // node that started the cluster

    i8     m_nBootNodes;       // number of nodes supplied via SE -node
    i8     m_nDefinedNodes;    // number of nodes in the nodes array
    i8     m_nConnectedNodes;  // number of connected nodes in the grid
    i8     m_nNodes;           // number of nodes in array


    /* oldcode
    Node*  m_pNodes[C_MAX_NODES]; // array of nodes
    */

    // newcode
    std::list<Node*> m_lNodes;
  
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    Grid(Stz* szGridNm, i8 iConnectCode);
   ~Grid(void) noexcept(false);
    
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);
  
    Article* pArticle(void);
    void vAddNode(i8 iNodeNo, Stz* pszName, Stz* pszAddr, i8 iPort);
    i8   nBootNodes(void);
    i8   nDefinedNodes(void);
    i8   nConnectedNodes(void);
    void vDsv(void);
    Stz* szGridNm(void);
    void vHasStarted(bool bHasStarted);
    bool bHasStarted(void);
    Node* pFindNode(Stz* pName);
    Node* pFindNodeByNodeNo(i8 iNodeNo);
    Node* pGetNode(i8 i_NodeNo);
    i8   iGetNodeNo(i8 i_Node);
    void vIncBootNodes(void);
    void vIncDefinedNodes(void);
    void vIncConnectedNodes(void);
    void vDecConnectedNodes(void);
    void vDecDefinedNodes(void);
    void vDecBootNodes(void);
    void vIncNodes(void);
    void vDecNodes(void);
    i8   nNodes(void);
    void vPutNode(Node* pNode);
    void vPutNodeR(Node* pNode);
    void vRemoveNode(Node* pNode);
    void vStartNodeNo(i8 iStartNodeNo);
    i8   iStartNodeNo(void);
    i8   iThisNodeNo(void);
    Node* pThisNode(void);
    void vThisNodeNo(i8 iNode);
    void vSetThisNode (Node* pNode); // newcode
    bool bFirstNode(i8 iNodeNo);

    // newcode
    std::list<Node*>::const_iterator itBeginNodes(void);
    std::list<Node*>::const_iterator itEndNodes(void);
  
    Footer* aFooter(void);
  };
    
  Grid* _pGrid(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
