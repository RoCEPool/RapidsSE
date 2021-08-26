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
Grid::Grid(Stz* szGridNm)
: Operand(CN_GRID)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_GRID,_FOOTER_);
  
  m_sGridNm   = szGridNm; //"SEgrid"_Stz;
  m_pThisNode = nullptr;
  m_pNodes = new SkipListNL(true,STORE::bNodeCompare);
    
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
/* repair the grid                                                            */
/*----------------------------------------------------------------------------*/
void Grid::vRepair(Object* pObject, eRF eRepair)
{
  _static_

  Grid* pGrid = _pGrid(pObject);

  // delete the grid
  delete pGrid;
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
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
void Grid::vAddDetectedNode(i8 iNodeNo, Stz* szNodeNm,
                            Stz* szIPAddr, i8 iPortNo, Stz* szRegionNm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_GRID,_FOOTER_);
  
  Node *pNode = new Node(iNodeNo,
                         szNodeNm,
                         AM::SYSARGS::nPtnsPerNode(),
                         szIPAddr,
                         iPortNo,
                         szRegionNm);

  if (! m_pNodes->pFind(pNode)) {
    vAddNode(pNode);
  }
  else {
    delete pNode;
  }
}

/*----------------------------------------------------------------------------*/
/* add node                                                                   */
/*----------------------------------------------------------------------------*/
void Grid::vAddNode(Node* pNode)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_GRID,_FOOTER_);

  bool b = m_pNodes->bInsert(pNode);
  RASSERT(b);

  if (AM::TRACE::bVerbose(C_VB_NODE)) {
    SP_Node spNode;
    vTrace("Node Added: %s\n"_Stz,spNode.sz(pNode));
  }
}

/*----------------------------------------------------------------------------*/
/* adjust partition map after node added/removed                              */
/*----------------------------------------------------------------------------*/
void Grid::vAdjustPtnMap(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_GRID,_FOOTER_);
  
  // We have to readjust partition map when new connection or dropped connection
  PtnMap* pPtnMap = AM::pPtnMap();

  pPtnMap->vAdjustMap(AM::GRID::nKnownNodes());
  pPtnMap->vSee();
}

/*----------------------------------------------------------------------------*/
/* dump distributed state vector                                              */
/*----------------------------------------------------------------------------*/
void Grid::vDsv(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_GRID,_FOOTER_);
  
  vTrace("\nDsv of Grid: %s\n"_Stz,
         m_sGridNm.szStz());
  
  vTrace("  this node %lld %s\n"_Stz,
         iThisNodeNo(),
         m_pThisNode ? m_pThisNode->szNodeNm() : "no m_pNode"_Stz);
  
  vTrace("nodes %lld \n"_Stz,
         nKnownNodes());


  for (Node_I oNode_I; oNode_I.bMore(); oNode_I.vNext()) {
    Node* pNode = (*oNode_I);
    if (pNode) pNode->vDsv();
  }
}

/*----------------------------------------------------------------------------*/
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

  sDumpBuffer.vCatSprint("Dsv of Grid: %s (\n"_Stz,
         m_sGridNm.szStz());

  sDumpBuffer.vCatSprint("  this node %lld %s\n"_Stz,
                         iThisNodeNo(),
         m_pThisNode ? m_pThisNode->szNodeNm() : "no m_pNode"_Stz);

  sDumpBuffer.vCatSprint("nodes %lld \n"_Stz,
                         nKnownNodes());



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

/*----------------------------------------------------------------------------*/
/* get node by node ID                                                        */
/*----------------------------------------------------------------------------*/
Node* Grid::pFindNode(i8 iNodeNo)
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
/* get node by its position in the node list                                  */
/*----------------------------------------------------------------------------*/
Node* Grid::pGetNthNode(i8 i_Node)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_GRID,_FOOTER_);

  i8 iSeq = 0;
  for (Node_I oNode_I; oNode_I.bMore(); oNode_I.vNext()) {
    Node* pNode = (*oNode_I);
    
    if (pNode && i_Node == iSeq++)
      return pNode;
  }

  return nullptr;
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
/* get number of nodes in the node array                                      */
/*----------------------------------------------------------------------------*/
i8 Grid::nKnownNodes(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_GRID,_FOOTER_);

  return m_pNodes->nItems();
}

/*----------------------------------------------------------------------------*/
/* remove given node                                                          */
/*----------------------------------------------------------------------------*/
void Grid::vRemoveNode(Node* pNode)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_GRID,_FOOTER_);

  bool b = m_pNodes->bRemove(pNode);
  RASSERT(b);
}

/*----------------------------------------------------------------------------*/
/* see nodes of the grid                                                      */
/*----------------------------------------------------------------------------*/
void Grid::vSeeNodes(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_GRID,_FOOTER_);
  
  vTrace("See Grid::\n"_Stz);

  vTrace("  ptns per node %lld\n"_Stz,AM::PTNMAP::nPtnsPerNode());
  vTrace("  ptns in grid  %lld\n"_Stz,AM::PTNMAP::nPtnsInGrid());

  SP_Node spNode;
  
  for (Node_I oNode_I; oNode_I.bMore(); oNode_I.vNext()) {
    Node* pNode = *oNode_I;
    vTrace("  %s\n"_Stz,spNode.sz(pNode));
  }
  
  vTrace(";;\n\n"_Stz);
}

/*----------------------------------------------------------------------------*/
/* get this node number                                                       */
/*----------------------------------------------------------------------------*/
i8 Grid::iThisNodeNo(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_GRID,_FOOTER_);
  
  return m_pThisNode->iNodeNo();
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
/* set this node                                                              */
/*----------------------------------------------------------------------------*/
void Grid::vThisNode(Node* pNode)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_GRID,_FOOTER_);

  m_pThisNode = pNode;
}

/*----------------------------------------------------------------------------*/
/* returns number of object parts includes the main                           */
/* put each object part in a parcel                                           */
/*----------------------------------------------------------------------------*/
i8 Grid::iPrepareParcel(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_GRID, _FOOTER_);

  if (!m_pParcel) {
    m_pParcel = new Parcel;

    // Set the state that this is created as a parcel
    m_pParcel->vOriginalParcel(true);
  }

  m_pParcel->vAddItem((Object *)this);

  return m_pParcel->nItems();
}
  
/*----------------------------------------------------------------------------*/
/* returns pParcel                                                            */
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

  return C_UNKNOWN;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
