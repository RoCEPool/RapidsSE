/*============================================================================*/
/* GridModel -- Grid Model                                                    */
/*============================================================================*/

#include "GridModel.h"

#include "StorageEngine.h"

extern Grid* gpGrid;
extern Arguments gxArguments;

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Grid* GridModel::_pGrid(void* aAddr)
{
  Grid* pGrid = reinterpret_cast<Grid*>(aAddr);
  
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
  vFatal("fatal: GridModel class cannot be deleted"_Str);
  
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
  
  m_nNodes      = gxArguments.m_nNodes;
  m_iThisNode   = -1;
  m_pThisNode   = nullptr;
  m_iStartNode  = -1;
  m_bHasStarted = false;
  m_nSlots      = gxArguments.m_nSlots;
  m_nPartitions = m_nSlots * m_nNodes;
  m_bTraceapi   = gxArguments.m_bTraceapi;
  
  // initialize the partition array
  for (i8 iPartition = 0; iPartition < C_SLOTS_PER_NODE; iPartition++) {
    m_iPartition[iPartition] = -1;
  }
}

/*----------------------------------------------------------------------------*/
/* dump grid                                                                  */
/*----------------------------------------------------------------------------*/
void Grid::vDump(eCN eClass, Object* pObject, Str* aTab)
{
  ASSERT(eClass == CN_GRID);
  ASSERT(pObject->eClass() == CN_GRID);
  
  Grid* pGrid = reinterpret_cast<Grid*>(pObject);
  
  vTrace("\n"_Str);
}

/*----------------------------------------------------------------------------*/
/* dump grid                                                                  */
/*----------------------------------------------------------------------------*/
void Grid::vDumpPartitioning(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_GRID,_FOOTER_);
  
  for (i8 iIndex = 0; iIndex < m_nPartitions; iIndex++)
    vTrace("Partition: %lld; Node: %lld\n"_Str, iIndex,
           Partition::iPartitionToNode(iIndex));
  vTrace("\n"_Str);
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
Node* Grid::pFindNode(Str* sName)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_GRID,_FOOTER_);
  
  NodeCursor* pNodeCursor = new NodeCursor(this);
  pNodeCursor->vBegin();
  pNodeCursor->vFirst();
  Node* pNode = pNodeCursor->pObject();
  while (pNode) {
    if (pNode->aNodeName()->bSame(sName))
      break;
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
/* get number of nodes in the grid                                            */
/*----------------------------------------------------------------------------*/
i8 Grid::iNodes(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_GRID,_FOOTER_);
  
  return m_nNodes;
}

/*----------------------------------------------------------------------------*/
/* get number of partition slots per node in the grid                         */
/*----------------------------------------------------------------------------*/
i8 Grid::iSlots(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_GRID,_FOOTER_);
  
  return m_nSlots;
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
/* get grid node where cluster was started                                    */
/*----------------------------------------------------------------------------*/
i8 Grid::iStartNode(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_GRID,_FOOTER_);
  
  return m_iStartNode;
}

/*----------------------------------------------------------------------------*/
/* set this node number                                                       */
/*----------------------------------------------------------------------------*/
void Grid::vThisNode(i8 iNode)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_GRID,_FOOTER_);
  
  m_iThisNode = iNode;
  m_pThisNode = pFindNode(iNode);
  
  // now initialize the array of partition numbers on this node
  for (i8 iSlot = 0; iSlot < gxArguments.m_nSlots; iSlot++) {
    m_iPartition[iSlot] = Partition::iSlotToPartition(iSlot);
  }

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
Footer* Grid::pFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_GRID);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
