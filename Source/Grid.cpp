/*============================================================================*/
/* Grid.cpp -- Grid                                                           */
/*============================================================================*/

#include "Grids.h"

extern Args gxArgs;

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
/* find node                                                                  */
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
/* get node                                                                  */
/*----------------------------------------------------------------------------*/
Node* Grid::pGetNode(i8 i_Node)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_GRID,_FOOTER_);

  RASSERT(i_Node >= 0 and i_Node < m_nBootNodes);
  
  Node* pNode = m_pNodes[i_Node];
  
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
/* increment number of connected nodes                                        */
/*----------------------------------------------------------------------------*/
void Grid::vIncConnectedNodes(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_GRID,_FOOTER_);
  
  m_nConnectedNodes++;
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
