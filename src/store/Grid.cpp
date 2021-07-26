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
        
  // initialize the node list

#ifdef WITH_NODE_SKIPLIST

  m_pNodes_stdList = new std::list<Node*>;
  m_pNodes_stdList->clear();

  m_pNodes_skipList = new SkipListNL(true,STORE::bNodeCompare);
  
#else
  
  m_pNodes_stdList = new std::list<Node*>;
  m_pNodes_stdList->clear();

#endif
  
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
  
#ifdef WITH_NODE_SKIPLIST

#else
  
#endif

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
  
#ifdef WITH_NODE_SKIPLIST
  
  Node *pNode = new Node(iNodeNo,
                         szNodeNm,
                         AM::SYSARGS::nPtnsPerNode(),
                         szIPAddr,
                         iPortNo,
                         szRegionNm);

  if (! m_pNodes_skipList->pFind(pNode)) {
    bool b = m_pNodes_skipList->bInsert(pNode);
    RASSERT(b);
  }
  else {
    delete pNode;
  }

#else

  // Search List using Lambda function as comparator
  std::list<Node*>::iterator itFound = std::find_if(m_pNodes_stdList->begin(), m_pNodes_stdList->end(),
               [&] (Node* pNode) { return pNode->iNodeNo() == iNodeNo; });

  if (itFound == m_pNodes_stdList->end()) { // not found, let's add
    //vTrace("NOT FOUND\n"_Stz);

    CheckSum ChecksumValue((unsigned char *)szIPAddr->azChar(),
                           szIPAddr->nLength());
    
    i8 iNodeNo = (ChecksumValue.iGetBernstein() + iPortNo);


    Node *pNode = new Node(iNodeNo,
                           szNodeNm,
                           AM::SYSARGS::nPtnsPerNode(),
                           szIPAddr,
                           iPortNo,
                           szRegionNm);

    vAddNode(pNode); // this also incr m_nNodes;

    // Sorting List using Lambda function as comparator
    m_pNodes_stdList->sort([](Node* pNode1, Node* pNode2)
                  {
                    if(pNode1->iNodeNo() == pNode2->iNodeNo())
                      return pNode1 < pNode2;
                    return pNode1->iNodeNo() < pNode2->iNodeNo();
                  });

  }

#endif
  
}

/*----------------------------------------------------------------------------*/
/* add node                                                                   */
/*----------------------------------------------------------------------------*/
void Grid::vAddNode(Node* pNode)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_GRID,_FOOTER_);

#ifdef WITH_NODE_SKIPLIST

  bool b = m_pNodes_skipList->bInsert(pNode);
  RASSERT(b);
  
#else
  
  m_pNodes_stdList->push_back(pNode);
  
#endif

  if (AM::TRACE::bVerbose(C_VB_NODE)) {
    SP_Node spNode;
    vTrace("Node Added: %s\n"_Stz,spNode.sz(pNode));
  }
  
}


/*----------------------------------------------------------------------------*/
/* increment number of connected nodes                                        */
/*----------------------------------------------------------------------------*/
void Grid::vAdjustPtnMap(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_GRID,_FOOTER_);
  
  // We have to readjust partition map when new connection or dropped connection
  PtnMap* pPtnMap = AM::pPtnMap();

  //pPtnMap->vDumpDataBuffers(); // debug dump
  pPtnMap->vAdjustMap(AM::GRID::nKnownNodes(),AM::SYSARGS::nPtnsPerNode());
  pPtnMap->vDumpDataBuffers();

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

  return m_pNodes_stdList->size();
}

/*----------------------------------------------------------------------------*/
/* remove given node                                                          */
/*----------------------------------------------------------------------------*/
void Grid::vRemoveNode(Node* pNode)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_GRID,_FOOTER_);

  m_pNodes_stdList->remove(pNode);
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
         ,AM::PTNMAP::nPtnsPerNode()
         ,AM::PTNMAP::nPtnsInGrid()
         );
  
  SP_Node spNode;
  
  for (Node_I oNode_I; oNode_I.bMore(); oNode_I.vNext()) {
    Node* pNode = *oNode_I;
    vTrace("  %s\n"_Stz,spNode.sz(pNode));
  }
  
  vTrace("\n"_Stz);
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
    m_pParcel = new Parcel(nullptr,0,0,0,PX_NONE);

    // Set the state that this is created as a parcel
    m_pParcel->vSetOriginState(true);
  }

  m_pParcel->vAddObject((Object *)this);

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

#ifdef WITH_NODE_SKIPLIST

std::list<Node*>::const_iterator Grid::itBeginNodes(void)
{
  return m_pNodes_stdList->begin();
}

std::list<Node*>::const_iterator Grid::itEndNodes(void)
{
  return m_pNodes_stdList->end();
}

#else

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
