/*============================================================================*/
/* Grid.cpp -- Grid                                                           */
/*============================================================================*/

#include "BASE.h"

#include "Node.hpp"
#include "Node_I.hpp"
#include "Grid.hpp"
#include "PtnMap.hpp"
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
  m_iConnectCode    = iConnectCode;
  
  m_iThisNode       = C_UNKNOWN;

  m_pThisNode       = nullptr;
  
  m_iStartNodeNo    = C_UNKNOWN;
  m_bHasStarted     = false;
      
  // initialize the node array
  m_nBootNodes      = 0;
  m_nDefinedNodes   = 0;
  m_nConnectedNodes = 0;
  m_nNodes          = 0;
  //for (i8 i_Node = 0; i_Node < C_MAX_NODES; i_Node++) {
  //  m_pNodes[i_Node] = nullptr;
 // }

  m_lNodes.clear();

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
  UNUSED(eClass);
  UNUSED(pObject);
  UNUSED(aTab);

  //Grid* pGrid = _pGrid(pObject);
  
  //vTrace("\n"_Stz);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

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
         m_iStartNodeNo
         );
  
  vTrace("  bootNodes %lld definedNodes %lld "
         "connectedNodes %lld nodes %lld \n"_Stz,
         m_nBootNodes,
         m_nDefinedNodes,
         m_nConnectedNodes,
         m_nNodes);


  for (Node_I oNode_I(0,C_ALL); oNode_I.bMore(); oNode_I.vNext()) {
    Node* pNode = (*oNode_I);
    if (pNode) pNode->vDsv();
  }
}

/*----------------------------------------------------------------------------*/
/* find node by name                                                          */
/*----------------------------------------------------------------------------*/
Node* Grid::pFindNode(Stz* sName)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_GRID,_FOOTER_);

  for (Node_I oNode_I(0,C_ALL); oNode_I.bMore(); oNode_I.vNext()) {
    Node* pNode = (*oNode_I);
    if (pNode && pNode->szNodeNm()->bSame(sName))
      return pNode;
  }
  
  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* get node by node ID                                                        */
/*----------------------------------------------------------------------------*/
Node* Grid::pFindNodeByNodeNo(i8 iNodeNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_GRID,_FOOTER_);
  //vTrace("%lld % lld % lld %lld\n"_Stz, iNodeNo, m_nBootNodes, m_nConnectedNodes, m_nDefinedNodes);
  // OK, we got recursive call between pFindNode and pFindNodeBYNodeNo, from the pNode = (*oNode_I)


  for (Node_I oNode_I(0,C_ALL); oNode_I.bMore(); oNode_I.vNext()) {
    Node* pNode = (*oNode_I);
    if (pNode) vTrace("pFindNodeByNodeNo found %s"_Stz, pNode->szNodeNm()->azChar());
    if (pNode && pNode->iNodeNo() == iNodeNo) {

      //vTrace("pFindNodeByNodeNo found %s"_Stz, pNode->szNodeNm()->azChar());
      return pNode;
    }
  }

  return nullptr;
}

/**
 * @brief returns a node object matched the node ID
 * @details returns a node object matched the node ID
 * <p>
 * @param[in] i_NodeNo - node number
 *
 * @return Node*  if found or nullptr
 *
 */
Node* Grid::pGetNode(i8 iNodeNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_GRID,_FOOTER_);

  for (Node_I oNode_I(0,C_ALL); oNode_I.bMore(); oNode_I.vNext()) {
    Node* pNode = (*oNode_I);
    if (pNode && pNode->iNodeNo() == iNodeNo)
      return pNode;
  }

  return nullptr;
}


/*----------------------------------------------------------------------------*/
/* get node ID                                                                */
/*----------------------------------------------------------------------------*/
i8 Grid::iGetNodeNo(i8 i_Node)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_GRID,_FOOTER_);
  //vThreadCallStackDump();

  // called from  PtnMap::vPartition()

  //vTrace("Grid::iGetNodeNo for this index: %lld  "_Stz, i_Node);
  i8 iSeq = 0;
  for (Node_I oNode_I(0,C_ALL); oNode_I.bMore(); oNode_I.vNext()) {
    Node* pNode = (*oNode_I);
    if (pNode && i_Node == iSeq++) {
      //vTrace("iGetNodeNo %lld\n"_Stz, pNode->iNodeNo());
      return pNode->iNodeNo();
    }
  }

  return 0;
}

/*----------------------------------------------------------------------------*/
/* is this node number is the first node in the grid                          */
/*----------------------------------------------------------------------------*/
bool Grid::bFirstNode(i8 iNodeNo)
{
  // Get first node, check if the same
  Node_I oNode_I(0,C_ALL);
  if (oNode_I.bMore()) {
    if ((*oNode_I)->iNodeNo() == iNodeNo)
      return true;
 }

  return false;
}

std::list<Node*>::const_iterator Grid::itBeginNodes(void)
{
  return m_lNodes.begin();
}

std::list<Node*>::const_iterator Grid::itEndNodes(void)
{
  return m_lNodes.end();
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
/* increment number of boot nodes                                              */
/*----------------------------------------------------------------------------*/
void Grid::vIncBootNodes(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_GRID,_FOOTER_);
  
  m_nBootNodes++;
}

/*----------------------------------------------------------------------------*/
/* decrement number of boot nodes                                              */
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
  
  ++m_nConnectedNodes;

  // We have to readjust partition map when new connection or dropped connection
  PtnMap* pPtnMap = AM::pPtnMap();

  //pPtnMap->vDumpDataBuffers(); // debug dump
  pPtnMap->vAdjustDataBuffers(AM::nConnectedNodes(),AM::nDegOfPar());
  pPtnMap->vDumpDataBuffers();

}

/*----------------------------------------------------------------------------*/
/* decrement number of connected nodes                                        */
/*----------------------------------------------------------------------------*/
void Grid::vDecConnectedNodes(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_GRID,_FOOTER_);

  --m_nConnectedNodes;
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

  // newcode
  return m_lNodes.size();

  /* oldcode
  return m_nNodes; */
}

/*----------------------------------------------------------------------------*/
/* put node                                                                   */
/*----------------------------------------------------------------------------*/
void Grid::vPutNode(Node* pNode)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_GRID,_FOOTER_);

  // newcode
  m_lNodes.push_back(pNode);
  m_nNodes++;

}

/*----------------------------------------------------------------------------*/
/* put node Reverse                                                           */
/*----------------------------------------------------------------------------*/
void Grid::vPutNodeR(Node* pNode)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_GRID,_FOOTER_);

  m_lNodes.push_front(pNode);
  m_nNodes++;

}

/*----------------------------------------------------------------------------*/
/* remove given node                                                          */
/*----------------------------------------------------------------------------*/
void Grid::vRemoveNode(Node* pNode)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_GRID,_FOOTER_);

  // newcode
  m_lNodes.remove(pNode);
  m_nBootNodes--;

  /* oldcode
  ASSERT(! pNode->pRecvSocket());
  i8 i_Node = pNode->iNodeNo();
  m_pNodes[i_Node] = nullptr;
  m_nBootNodes--; */
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


  // Search List using Lambda function as comparator
  std::list<Node*>::iterator itFound = std::find_if(m_lNodes.begin(), m_lNodes.end(),
               [&] (Node* pNode) { return pNode->iNodeNo() == iNodeNo; });

  if (itFound == m_lNodes.end()) { // not found, let's add
    //vTrace("NOT FOUND\n"_Stz);

    CheckSum ChecksumValue((unsigned char *)pszAddr->azChar(), pszAddr->nLength());
    i8 l_iNodeNo = (ChecksumValue.iGetBernstein() + iPort);


    Node *pNode = new Node(l_iNodeNo,
                           pszName,
                           AM::nDegOfPar(),
                           pszAddr,
                           iPort);

    vPutNode(pNode); // this also incr m_nNodes;

    // Sorting List using Lambda function as comparator
    m_lNodes.sort([](Node* pNode1, Node* pNode2)
                  {
                    if(pNode1->iNodeNo() == pNode2->iNodeNo())
                      return pNode1 < pNode2;
                    return pNode1->iNodeNo() < pNode2->iNodeNo();
                  });

    vIncBootNodes();
    vIncDefinedNodes();

  } //else { vTrace("FOUND\n"_Stz);}

  // Found, nothing to do


}

/*----------------------------------------------------------------------------*/
/* get grid node where cluster was started                                    */
/*----------------------------------------------------------------------------*/
i8 Grid::iStartNodeNo(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_GRID,_FOOTER_);
  
  return m_iStartNodeNo;
}

/*----------------------------------------------------------------------------*/
/* set grid node where cluster was started                                    */
/*----------------------------------------------------------------------------*/
void Grid::vStartNodeNo(i8 iStartNodeNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_GRID,_FOOTER_);
  
  m_iStartNodeNo = iStartNodeNo;
}

/*----------------------------------------------------------------------------*/
/* set this node number                                                       */
/*----------------------------------------------------------------------------*/
void Grid::vThisNodeNo(i8 iNode)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_GRID,_FOOTER_);
  // iNode is no longer an index
  m_iThisNode = iNode;
  //m_pThisNode = pGetNode(iNode); oldcode
}

/*----------------------------------------------------------------------------*/
/* set this node object                                                       */
/*----------------------------------------------------------------------------*/
void Grid::vSetThisNode(Node* pNode)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_GRID,_FOOTER_);

  m_pThisNode = pNode;
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
