/*============================================================================*/
/* GridModel -- Grid Model                                                    */
/*============================================================================*/

#include "GridModel.h"

extern Grid* gpGrid;

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Grid* GridModel::_pGrid(void* pAddr)
{
  Grid* pGrid = reinterpret_cast<Grid*>(pAddr);
  
  RASSERT( !pGrid || CN_GRID == pGrid->eClass() );
  
  return pGrid;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* Grid::operator new(size_t iSize, Grid* pGrid)
{
  RASSERT(iSize);
  return pGrid;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void Grid::operator delete(void* pBufferAddress)
{
  RASSERT(pBufferAddress);
  vFatal("fatal: GridModel class cannot be deleted");
  
  RASSERT(0);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Grid::Grid(void)
: Object(CN_GRID)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_GRID,_FOOTER_);
  
  gpGrid = this;
  
  vInit();
  
  _checkInitialised_(CN_GRID);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_GRID,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Grid::~Grid(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_GRID,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* init                                                                       */
/*----------------------------------------------------------------------------*/
void Grid::vInit(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_GRID,_FOOTER_);
  
  m_iThisNode = -1;
  m_bHasStarted = false;
}

/*----------------------------------------------------------------------------*/
/* dump grid                                                                  */
/*----------------------------------------------------------------------------*/
void Grid::vDump(eCN eClass, Object* pObject, const char* pTab)
{
  ASSERT(eClass == CN_GRID);
  ASSERT(pObject->eClass() == CN_GRID);
  
  Grid* pGrid = reinterpret_cast<Grid*>(pObject);
  
  vTrace("\n");
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
/* get grid started                                                           */
/*----------------------------------------------------------------------------*/
bool Grid::bHasStarted(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_GRID,_FOOTER_);
  
  return m_bHasStarted;
}

/*----------------------------------------------------------------------------*/
/* find node                                                                  */
/*----------------------------------------------------------------------------*/
Node* Grid::pFindNode(char* pName)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_GRID,_FOOTER_);
  
  NodeCursor* pNodeCursor = new NodeCursor(this);
  pNodeCursor->vBegin();
  pNodeCursor->vFirst();
  Node* pNode = pNodeCursor->pObject();
  while (pNode) {
    if (String::bEqualTo(pNode->pName(), pName)) {
      break;
    }
    pNodeCursor->vNext();
    pNode = pNodeCursor->pObject();
  }
  pNodeCursor->vEnd();
  
  return pNode;
}

/*----------------------------------------------------------------------------*/
/* find node                                                                  */
/*----------------------------------------------------------------------------*/
Node* Grid::pFindNode(i8 iNodeNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_GRID,_FOOTER_);
  
  NodeCursor* pNodeCursor = new NodeCursor(this);
  pNodeCursor->vBegin();
  pNodeCursor->vFirst();
  Node* pNode = pNodeCursor->pObject();
  while (pNode) {
    if (pNode->iNodeNo() == iNodeNo) {
      break;
    }
    pNodeCursor->vNext();
    pNode = pNodeCursor->pObject();
  }
  pNodeCursor->vEnd();
  
  return pNode;
}

/*----------------------------------------------------------------------------*/
/* set this node number                                                       */
/*----------------------------------------------------------------------------*/
void Grid::vThisNode(i8 iNode)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_GRID,_FOOTER_);
  
  m_iThisNode = iNode;
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
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Grid::pFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_GRID);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
