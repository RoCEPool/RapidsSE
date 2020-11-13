/*============================================================================*/
/* Grid.cpp -- Grid                                                           */
/*============================================================================*/

#include "Grids.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Grid* Grids::_pGrid(void* aAddr)
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
  void* pObject = HeapMem::pAllocate(CN_GRID);
  
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
Grid::Grid(i8 nNodes, i8 nTablePtns)
: Object(CN_GRID)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_GRID,_FOOTER_);
  
  m_sGridNm = "SEgrid"_Stz;
  
  m_pArticle = new Article(m_sGridNm,this,nullptr);
  
  vInit(nNodes, nTablePtns);
  
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
/* init                                                                       */
/*----------------------------------------------------------------------------*/
void Grid::vInit(i8 nDefinedNodes, i8 nTablePtns)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_GRID,_FOOTER_);
  
  m_nTablePtns      = nTablePtns;
  
  m_nDefinedNodes   = nDefinedNodes;
  m_nConnectedNodes = 0;
  
  m_iThisNode       = C_UNKNOWN;
  m_pThisNode       = nullptr;
  m_iStartNode      = C_UNKNOWN;
  m_bHasStarted     = false;
      
  // initialize the node array
  for (i8 i_Node = 0; i_Node < C_MAX_NODES; i_Node++) {
    m_pNodes[i_Node] = nullptr;
  }
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
/* add node                                                                   */
/*----------------------------------------------------------------------------*/
void Grid::vAddNode(Node* pNode)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_GRID,_FOOTER_);

  i8 i_Node = pNode->iNodeNo();
  
  m_pNodes[i_Node] = pNode;
  
  if (i_Node >= m_nDefinedNodes)
    m_nDefinedNodes++;
}

/*----------------------------------------------------------------------------*/
/* get article                                                                */
/*----------------------------------------------------------------------------*/
Article* Grid::pArticle(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_GRID,_FOOTER_);

  return m_pArticle;
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
/* get number of defined nodes in the grid                                    */
/*----------------------------------------------------------------------------*/
i8 Grid::nDefinedNodes(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_GRID,_FOOTER_);
  
//  vTrace("/nnDefinedNode()\n"_Stz);
//  
//  vThreadCallStackDump();
  
  return m_nDefinedNodes;
}

/*----------------------------------------------------------------------------*/
/* dump grid                                                                  */
/*----------------------------------------------------------------------------*/
void Grid::vDumpPartitioning(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_GRID,_FOOTER_);
  
  if (nPartitionsInGrid()) {
    for (tPN tPtnNo = 0; tPtnNo < nPartitionsInGrid(); tPtnNo++)
      vTrace("Partition: %lld; Node: %lld\n"_Stz, tPtnNo,
             AM::pPtnMap()->i_WhichNode(tPtnNo));
    vTrace("\n"_Stz);
  }
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
/* find node                                                                  */
/*----------------------------------------------------------------------------*/
Node* Grid::pFindNode(Stz* sName)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_GRID,_FOOTER_);
  
  for (Node_I oNode_I; oNode_I.bCont(); oNode_I.vNext()) {
    Node* pNode = (*oNode_I);
    if (pNode && pNode->szNodeNm()->bSame(sName))
      return pNode;
  }
  
  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* get node                                                                  */
/*----------------------------------------------------------------------------*/
Node* Grid::pGetNode(i8 i_Node)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_GRID,_FOOTER_);

  RASSERT(i_Node >= 0 and i_Node < m_nDefinedNodes);
  
  Node* pNode = m_pNodes[i_Node];
  
  return pNode;
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
/* get number of connected nodes                                              */
/*----------------------------------------------------------------------------*/
void Grid::vIncConnectedNodes(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_GRID,_FOOTER_);
  
  m_nConnectedNodes++;
}

/*----------------------------------------------------------------------------*/
/* get total number of nodes in the connected grid                            */
/*----------------------------------------------------------------------------*/
i8 Grid::nPartitionsInGrid(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_GRID,_FOOTER_);
  
  i8 nPartitions = m_nTablePtns * m_nDefinedNodes;

  return nPartitions;
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
  
  m_nDefinedNodes--;
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
/* set grid node wher ecluster was started                                    */
/*----------------------------------------------------------------------------*/
void Grid::vStartNode(i8 iStartNode)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_GRID,_FOOTER_);
  
  m_iStartNode = iStartNode;
}

/*----------------------------------------------------------------------------*/
/* get number of strands per node                                             */
/*----------------------------------------------------------------------------*/
i8 Grid::nStrandsPerNode(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_GRID,_FOOTER_);
  
  return m_nTablePtns;
}

/*----------------------------------------------------------------------------*/
/* get number of partitions per table in the grid                             */
/*----------------------------------------------------------------------------*/
i8 Grid::nTablePtns(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_GRID,_FOOTER_);
  
  return m_nTablePtns;
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
