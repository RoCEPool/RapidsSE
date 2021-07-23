/*============================================================================*/
/* Grid.cpp -- Grid                                                           */
/*============================================================================*/

#include "BASE.h"
#include <algorithm>    // std::find_if  NEED TO REPLACE this std::
#include "Node.hpp"
#include "Node_I.hpp"
#include "Grid.hpp"
#include "PtnMap.hpp"
#include "CmdLineArgs.hpp"

#include "SP_Node.hpp"

#include "TagConstants.h"

extern CmdLineArgs gxCmdLineArgs;
extern AttachMap*   gpAttachMap;

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE,Grid,CN_GRID)
__aNewFunction(Grid,CN_GRID)
__vDeleteFunction(Grid,CN_GRID)
__aFooterFunction(Grid)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Grid::Grid(Stz* szGridNm, i8 iConnectCode)
: Operand(CN_GRID)
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


  m_pNodes_stdList = new std::list<Node*>;
  m_pNodes_stdList->clear();

  m_pParcel = nullptr;

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
         nNodes());


  for (Node_I oNode_I; oNode_I.bMore(); oNode_I.vNext()) {
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

  for (Node_I oNode_I; oNode_I.bMore(); oNode_I.vNext()) {
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


  for (Node_I oNode_I; oNode_I.bMore(); oNode_I.vNext()) {
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
 * @brief returns a string buffer contains Grid Info
 * @details returns a string buffer contains Grid Info:
 * Node ID, Node Name, Node Address and Node Port Number
 * <p>
 *
 * @return s4K
 *
 */
s16KB Grid::szDumpInfo(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_GRID,_FOOTER_);

  s16KB sDumpBuffer = "\n\n\n==== DumpGrid: =======================================\n"_Stz;

  sDumpBuffer.vCatSprint("Dsv of Grid: %s (connect code %lld)\n"_Stz,
         m_sGridNm.szStz(),
         m_iConnectCode);

  sDumpBuffer.vCatSprint("  this node %lld %s\n"_Stz,
         m_iThisNode,
         m_pThisNode ? m_pThisNode->szNodeNm() : "no m_pNode"_Stz);

  sDumpBuffer.vCatSprint("  %s started; start node %lld\n"_Stz,
         m_bHasStarted ? "has"_Stz : "not"_Stz,
         m_iStartNodeNo  );

  sDumpBuffer.vCatSprint("  bootNodes %lld definedNodes %lld "
         "connectedNodes %lld nodes %lld \n"_Stz,
         m_nBootNodes,
         m_nDefinedNodes,
         m_nConnectedNodes,
         nNodes());



  for (Node_I oNode_I; oNode_I.bMore(); oNode_I.vNext()) {
    Node* pNode = (*oNode_I);
    if (pNode) {
      sDumpBuffer = pNode->szDumpInfo(sDumpBuffer);
    }
  }

  sDumpBuffer.vCatSprint("%s"_Stz,
                         "======================================================\n\n"_Stz);
  return sDumpBuffer;
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

  for (Node_I oNode_I; oNode_I.bMore(); oNode_I.vNext()) {
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
  for (Node_I oNode_I; oNode_I.bMore(); oNode_I.vNext()) {
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
  Node_I oNode_I;
  if (oNode_I.bMore()) {
    if ((*oNode_I)->iNodeNo() == iNodeNo)
      return true;
 }

  return false;
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
/* get number of nodes in the node array                                      */
/*----------------------------------------------------------------------------*/
i8 Grid::nNodes(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_GRID,_FOOTER_);

  return m_pNodes_stdList->size();
}

/*----------------------------------------------------------------------------*/
/* put node                                                                   */
/*----------------------------------------------------------------------------*/
void Grid::vPutNode(Node* pNode)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_GRID,_FOOTER_);

  m_pNodes_stdList->push_back(pNode);

  if (AM::TRACE::bVerbose(C_VB_NODE)) {
    SP_Node spNode;
    vTrace("Node Added: %s\n"_Stz,spNode.sz(pNode));
  }
}

/*----------------------------------------------------------------------------*/
/* remove given node                                                          */
/*----------------------------------------------------------------------------*/
void Grid::vRemoveNode(Node* pNode)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_GRID,_FOOTER_);

  m_pNodes_stdList->remove(pNode);
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
void Grid::vAddNode(i8 iNodeNo, Stz* pszName, Stz* pszAddr, i8 iPort, Stz* szRegionNm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_GRID,_FOOTER_);


  // Search List using Lambda function as comparator
  std::list<Node*>::iterator itFound = std::find_if(m_pNodes_stdList->begin(), m_pNodes_stdList->end(),
               [&] (Node* pNode) { return pNode->iNodeNo() == iNodeNo; });

  if (itFound == m_pNodes_stdList->end()) { // not found, let's add
    //vTrace("NOT FOUND\n"_Stz);

    CheckSum ChecksumValue((unsigned char *)pszAddr->azChar(), pszAddr->nLength());
    i8 l_iNodeNo = (ChecksumValue.iGetBernstein() + iPort);


    Node *pNode = new Node(l_iNodeNo,
                           pszName,
                           AM::nDegOfPar(),
                           pszAddr,
                           iPort,
                           szRegionNm);

    vPutNode(pNode); // this also incr m_nNodes;

    // Sorting List using Lambda function as comparator
    m_pNodes_stdList->sort([](Node* pNode1, Node* pNode2)
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
/* see nodes of the grid                                                      */
/*----------------------------------------------------------------------------*/
void Grid::vSeeNodes(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_GRID,_FOOTER_);
  
  vTrace("Grid::\n"_Stz);

  vTrace("  %lld partitions per table = "
         "%lld total partitions in grid "
         "\n"_Stz
         ,AM::nPtnsPerTable()
         ,AM::nPtnsInGrid()
         );
  
  SP_Node spNode;
  
  for (Node_I oNode_I; oNode_I.bMore(); oNode_I.vNext()) {
    Node* pNode = *oNode_I;
    vTrace("  %s\n"_Stz,spNode.sz(pNode));
  }
  
  vTrace("\n"_Stz);
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
/* returns number of object parts includes the main                           */
/* put each object part in a parcel                                           */
/*----------------------------------------------------------------------------*/

i8 Grid::iPrepareParcel(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_GRID, _FOOTER_);

  if (!m_pParcel) {
    m_pParcel = new Parcel(nullptr,0,0,0,PX_NONE);

    // Set the state that this is created as a parcel
    m_pParcel->vSetOriginState(true);
  }

  m_pParcel->vAddObject((Object *)this);

  //std::list<Node*> * m_pNodes_stdList = &m_lNodes;
  m_pParcel->vAddObject((Object *)m_pNodes_stdList);

  return m_pParcel->nObjects();
}

/*----------------------------------------------------------------------------*/
/* returns pParcel                                                            */
/*                                                                            */
/*----------------------------------------------------------------------------*/

Parcel* Grid::pParcel(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_GRID,_FOOTER_);
  return m_pParcel;
}


/*----------------------------------------------------------------------------*/
/* reassemble object parts from the received parcel                           */
/*                                                                            */
/*----------------------------------------------------------------------------*/
i8  Grid::iReassembleFromParcel(Parcel* pParcel) {
  _precondition_(_ADDRESS_, _MAGIC_, CN_GRID, _FOOTER_);

  vTrace(">>>>>>>>> iReassembleFromParcel with %lld parts\n"_Stz, pParcel->nObjects());

  if (!pParcel) return 0;
  i8 nSize = pParcel->nObjects();

  for (i8 i_Obj = 0; i_Obj < nSize; i_Obj++) {
    Object *pObject = pParcel->pGetObject(i_Obj);
    eCN eClass = pObject ? pObject->eGetClassNo() : CN_NONE;

    switch (eClass) {
      case CN_GRID:
        // do nothing for now
        break;

      //case CN_IDXVEC: {
        //m_pIdxVec = (IdxVec *) pObject;
      //  break;
      //}
      default:
        m_pNodes_stdList = (std::list<Node *> *) pObject;
        //vTrace("iReassembleObj %ld Error\n"_Stz,eClass);
    }
  }

  if (m_pNodes_stdList) {
    vTrace(">>>>>>>>> iReassembleFromParcel grid with %lld nodes\n"_Stz, m_pNodes_stdList ? m_pNodes_stdList->size() : 0);

    for (Node_I oNode_I; oNode_I.bMore(); oNode_I.vNext()) {
      Node* pNode = (*oNode_I);
      if (pNode) pNode->vDsv();
    }
  }

  // We are done with the received parcel, let's delete the received parcel
  delete pParcel;
  // Null the parcel pointer for this table
  m_pParcel = nullptr;

  return nSize;
}

#ifndef WITH_NODE_SKIPLIST

std::list<Node*>::const_iterator Grid::itBeginNodes(void)
{
  return m_pNodes_stdList->begin();
}

std::list<Node*>::const_iterator Grid::itEndNodes(void)
{
  return m_pNodes_stdList->end();
}

#endif

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
