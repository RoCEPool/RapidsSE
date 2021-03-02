/*============================================================================*/
/* Grid.cpp -- Grid                                                           */
/*============================================================================*/

#include "BASE.h"

#include "Node.hpp"
#include "Node_I.hpp"
#include "Grid.hpp"

#include "CmdLineArgs.hpp"

extern CmdLineArgs gxCmdLineArgs;
extern AttachMap*   gpAttachMap;

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Grid* STORE::_pGrid(void* aAddr)
{
  Grid* pGrid = reinterpret_cast<Grid*>(aAddr);
  
  RASSERT_TYPE(pGrid,CN_GRID);
  
  return pGrid;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* Grid::operator new(size_t)
{
  void* pObject = Heaps::pAllocate(CN_GRID);
  
  return pObject;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void Grid::operator delete(void*)
{
  vThrow(FK_FATAL,VL_FATAL,__func__,__FILE__,__LINE__,
         "fatal: Grid class cannot be deleted"_Stz);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Grid::Grid(Stz* szGridNm, i8 iConnectCode)
: Object(CN_GRID)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_GRID,_FOOTER_);
  
  m_sGridNm = szGridNm; //"SEgrid"_Stz;
  m_iConnectCode = iConnectCode;
  
  m_pArticle = new Article(m_sGridNm,this,nullptr);
  
  m_iThisNode       = C_UNKNOWN;
  m_pThisNode       = nullptr;
  
  m_iStartNode      = C_UNKNOWN;
  m_bHasStarted     = false;
      
  // initialize the node array
  m_nBootNodes      = 0;
  m_nDefinedNodes   = 0;
  m_nConnectedNodes = 0;
  m_nNodes          = 0;
  for (i8 i_Node = 0; i_Node < C_MAX_NODES; i_Node++) {
    m_pNodes[i_Node] = nullptr;
  }

  _checkInitialised_(CN_GRID);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_GRID,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Grid::~Grid(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_GRID,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dump grid                                                                  */
/*----------------------------------------------------------------------------*/
void Grid::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  //Grid* pGrid = _pGrid(pObject);
  
  //vTrace("\n"_Stz);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get article                                                                */
/*----------------------------------------------------------------------------*/
Article* Grid::pArticle(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_GRID,_FOOTER_);

  return m_pArticle;
}

/*----------------------------------------------------------------------------*/
/* get number of defined nodes in the grid                                    */
/*----------------------------------------------------------------------------*/
i8 Grid::nBootNodes(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_GRID,_FOOTER_);
  
//  vTrace("/nnDefinedNode()\n"_Stz);
//
//  vThreadCallStackDump();
  
  return m_nBootNodes;
}

/*----------------------------------------------------------------------------*/
/* get number of connected nodes                                              */
/*----------------------------------------------------------------------------*/
i8 Grid::nConnectedNodes(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_GRID,_FOOTER_);
  
//  vTrace("/nnConnectedNodes()\n"_Stz);
//
//  vThreadCallStackDump();

  return m_nConnectedNodes;
}

/*----------------------------------------------------------------------------*/
/* get number of defined nodes                                                */
/*----------------------------------------------------------------------------*/
i8 Grid::nDefinedNodes(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_GRID,_FOOTER_);
  
//  vTrace("/nnConnectedNodes()\n"_Stz);
//
//  vThreadCallStackDump();

  return m_nDefinedNodes;
}

/*----------------------------------------------------------------------------*/
/* dump distributed state vector                                              */
/*----------------------------------------------------------------------------*/
void Grid::vDsv(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_GRID,_FOOTER_);
  
  vTrace("\nDsv of Grid: %s (connect code %lld)\n"_Stz,
         m_sGridNm.szStz(),
         m_iConnectCode);
  
  vTrace("  this node %lld %s\n"_Stz,
         m_iThisNode,
         m_pThisNode ? m_pThisNode->szNodeNm() : "no m_pNode"_Stz);

  vTrace("  %s started; start node %lld\n"_Stz,
         m_bHasStarted ? "has"_Stz : "not"_Stz,
         m_iStartNode
         );
  
  vTrace("  bootNodes %lld definedNodes %lld "
         "connectedNodes %lld nodes %lld \n"_Stz,
         m_nBootNodes,
         m_nDefinedNodes,
         m_nConnectedNodes,
         m_nNodes);

  /* commented out because experiment with the idea that the first node might be null
  for (Node_I oNode_I(0,C_ALL); oNode_I.bMore(); oNode_I.vNext()) {
    Node *pNode = (*oNode_I);
    if (pNode) pNode->vDsv();
  }*/

  for (i8 i_Node = 0; i_Node < C_MAX_NODES; i_Node++) {
    Node* pNode = m_pNodes[i_Node];
    if (pNode) pNode->vDsv();
  }
}

/*----------------------------------------------------------------------------*/
/* find node                                                                  */
/*----------------------------------------------------------------------------*/
Node* Grid::pFindNode(Stz* sName)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_GRID,_FOOTER_);

  for (i8 i_Node = 0; i_Node < C_MAX_NODES; i_Node++) {
    Node* pNode = m_pNodes[i_Node];
    if (pNode && pNode->szNodeNm()->bSame(sName)) {
      //vTrace("Found by name:%s\n"_Stz, sName->azChar());
      return pNode;
    }
  }
  /*
  for (Node_I oNode_I(0,C_ALL); oNode_I.bMore(); oNode_I.vNext()) {
    Node* pNode = (*oNode_I);
    if (pNode && pNode->szNodeNm()->bSame(sName))
      return pNode;
  }*/
  
  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* find node                                                                  */
/*----------------------------------------------------------------------------*/
Node* Grid::pFindNodeByNodeNo(i8 iNodeNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_GRID,_FOOTER_);
  //vTrace("%lld % lld % lld %lld\n"_Stz, iNodeNo, m_nBootNodes, m_nConnectedNodes, m_nDefinedNodes);
  // OK, we got recursive call between pFindNode and pFindNodeBYNodeNo, from the pNode = (*oNode_I)
  for (i8 i_Node = 0; i_Node < C_MAX_NODES; i_Node++) {
    Node* pNode = m_pNodes[i_Node];
    if (pNode && pNode->iNodeNo() == iNodeNo) {
      return pNode;
    }
  }

  /* commented out because experiment with the idea that the first node might be null
  for (Node_I oNode_I(0,C_ALL); oNode_I.bMore(); oNode_I.vNext()) {
    Node* pNode = (*oNode_I);
    if (pNode) vTrace("pFindNodeByNodeNo found %s"_Stz, pNode->szNodeNm()->azChar());
    if (pNode && pNode->iNodeNo() == iNodeNo) {
      vTrace("pFindNodeByNodeNo found %s"_Stz, pNode->szNodeNm()->azChar());
      return pNode;
    }
  }*/

  return nullptr;
}

/**
 * @brief returns a node object from the index
 * @details returns a node object from the index
 * <p>
 * @param[in] i_Node - index of the node
 *
 * @return Node*  if found or nullptr
 *
 */
Node* Grid::pGetNode(i8 i_Node)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_GRID,_FOOTER_);

  // The new model might not work with the guard
  // The node index can be anywhere, due to dropped nodes and new nodes
  /*RASSERT(i_Node >= 0 and
          (i_Node < m_nBootNodes or
           i_Node < m_nConnectedNodes or
           i_Node < m_nDefinedNodes));*/
  
  //Node* pNode = m_pNodes[i_Node];
  //vTrace("pGetNode i_Node:%lld\n"_Stz, i_Node);
  Node* pNode = pFindNodeByNodeNo(i_Node);



  //if (pNode)
  //  vTrace("%lld % lld % lld %lld\n"_Stz, i_Node, m_nBootNodes, m_nConnectedNodes, m_nDefinedNodes);
  
  return pNode;
}

/*----------------------------------------------------------------------------*/
/* return grid name                                                           */
/*----------------------------------------------------------------------------*/
Stz* Grid::szGridNm(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_GRID,_FOOTER_);
  
  return m_sGridNm;
}

/*----------------------------------------------------------------------------*/
/* get grid started                                                           */
/*----------------------------------------------------------------------------*/
bool Grid::bHasStarted(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_GRID,_FOOTER_);
  
  return m_bHasStarted;
}

/*----------------------------------------------------------------------------*/
/* set grid started                                                           */
/*----------------------------------------------------------------------------*/
void Grid::vHasStarted(bool bHasStarted)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_GRID,_FOOTER_);
  
  m_bHasStarted = bHasStarted;
}

/*----------------------------------------------------------------------------*/
/* increment number of put nodes                                              */
/*----------------------------------------------------------------------------*/
void Grid::vIncBootNodes(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_GRID,_FOOTER_);
  
  m_nBootNodes++;
}

/*----------------------------------------------------------------------------*/
/* decrement number of put nodes                                              */
/*----------------------------------------------------------------------------*/
void Grid::vDecBootNodes(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_GRID,_FOOTER_);

  m_nBootNodes--;
}

/*----------------------------------------------------------------------------*/
/* increment number of connected nodes                                        */
/*----------------------------------------------------------------------------*/
void Grid::vIncConnectedNodes(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_GRID,_FOOTER_);
  
  m_nConnectedNodes++;
}

/*----------------------------------------------------------------------------*/
/* decrement number of connected nodes                                        */
/*----------------------------------------------------------------------------*/
void Grid::vDecConnectedNodes(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_GRID,_FOOTER_);

  m_nConnectedNodes--;
}

/*----------------------------------------------------------------------------*/
/* increment number of defined nodes                                          */
/*----------------------------------------------------------------------------*/
void Grid::vIncDefinedNodes(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_GRID,_FOOTER_);
  
  m_nDefinedNodes++;
}

/*----------------------------------------------------------------------------*/
/* decrement number of defined nodes                                         */
/*----------------------------------------------------------------------------*/
void Grid::vDecDefinedNodes(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_GRID,_FOOTER_);

  m_nDefinedNodes--;
}

/*----------------------------------------------------------------------------*/
/* decrement number of nodes in the node array                                */
/*----------------------------------------------------------------------------*/
void Grid::vDecNodes(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_GRID,_FOOTER_);

  --m_nNodes;
}

void Grid::vIncNodes(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_GRID,_FOOTER_);

  ++m_nNodes;
}

/*----------------------------------------------------------------------------*/
/* get number of nodes in the node array                                      */
/*----------------------------------------------------------------------------*/
i8 Grid::nNodes(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_GRID,_FOOTER_);
  
  return m_nNodes;
}

/*----------------------------------------------------------------------------*/
/* put node                                                                   */
/*----------------------------------------------------------------------------*/
void Grid::vPutNode(Node* pNode)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_GRID,_FOOTER_);
vTrace("putNode %lld %s\n"_Stz, pNode->iNodeNo(), pNode->szNodeNm()->azChar());
  i8 i_Node = pNode->iNodeNo();
  
  m_pNodes[i_Node] = pNode;
  
  m_nNodes++;
  
//  if (i_Node >= m_nBootNodes)
//    m_nBootNodes++;
}

/*----------------------------------------------------------------------------*/
/* remove given node                                                          */
/*----------------------------------------------------------------------------*/
void Grid::vRemoveNode(Node* pNode)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_GRID,_FOOTER_);
  
  ASSERT(! pNode->pRecvSocket());
  
  i8 i_Node = pNode->iNodeNo();
  
  m_pNodes[i_Node] = nullptr;
  
  m_nBootNodes--;
}

/**
 * @brief Add new node into the cluster/grid
 * @details When a new node comes up, add it to the cluster.
 * <p>
 * @param[in] pszName  name of this node
 * @param[in] pszAddr  Address this node runs at
 * @param[in] iPort   port number this node listens to
 *
 *
 * @return none
 *
 */
void Grid::vAddNode(i8 iNodeNo, Stz* pszName, Stz* pszAddr, i8 iPort)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_GRID,_FOOTER_);

      for (i8 i_Node = 0; i_Node < C_MAX_NODES; i_Node++) {
        //ASSERT(AM::pGrid()->nNodes() == i_Node);
        if (m_pNodes[i_Node] == nullptr) { // Add to this slot
          Node *pNode = new Node(i_Node,
                                 pszName,
                                 AM::nDegOfPar(),
                                 pszAddr,
                                 iPort);

          vPutNode(pNode); // this also incr m_nNodes;
          vIncBootNodes();
          vIncDefinedNodes();

          Article *pNodeArticle = pNode->pArticle();

          pArticle()->vAppend(pNodeArticle);

          break; // done with adding
        }
      }

}

/*----------------------------------------------------------------------------*/
/* get grid node where cluster was started                                    */
/*----------------------------------------------------------------------------*/
i8 Grid::iStartNode(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_GRID,_FOOTER_);
  
  return m_iStartNode;
}

/*----------------------------------------------------------------------------*/
/* set grid node where cluster was started                                    */
/*----------------------------------------------------------------------------*/
void Grid::vStartNode(i8 iStartNode)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_GRID,_FOOTER_);
  
  m_iStartNode = iStartNode;
}

/*----------------------------------------------------------------------------*/
/* set this node number                                                       */
/*----------------------------------------------------------------------------*/
void Grid::vThisNode(i8 iNode)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_GRID,_FOOTER_);
  
  m_iThisNode = iNode;
  m_pThisNode = pGetNode(iNode);
}

/*----------------------------------------------------------------------------*/
/* get this node number                                                       */
/*----------------------------------------------------------------------------*/
i8 Grid::iThisNode(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_GRID,_FOOTER_);
  
  return m_iThisNode;
}

/*----------------------------------------------------------------------------*/
/* get this node                                                              */
/*----------------------------------------------------------------------------*/
Node* Grid::pThisNode(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_GRID,_FOOTER_);
  
  return m_pThisNode;
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Grid::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_GRID);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
