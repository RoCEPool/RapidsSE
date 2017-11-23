/*============================================================================*/
/* GridModel -- Grid Model                                                    */
/*============================================================================*/

#include "GridModel.h"

#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

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
  vFatal("fatal: GridModel class cannot be deleted"_Stz);
  
  NEVER_REACH;
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Grid::Grid(i8 nNodes, i8 nSlots)
: Object(CN_GRID)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_GRID,_FOOTER_);
  
  gpAttachMap->m_pGrid = this;
  
  vInit(nNodes, nSlots);
  
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
void Grid::vInit(i8 nNodes, i8 nSlots)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_GRID,_FOOTER_);
  
  m_nNodes      = nNodes;
  m_iThisNode   = -1;
  m_pThisNode   = nullptr;
  m_iStartNode  = -1;
  m_bHasStarted = false;
  m_nSlots      = nSlots;
  m_nPartitions = m_nSlots * m_nNodes;
    
  // initialize the partition array
  for (i8 i_Partition = 0; i_Partition < C_SLOTS_PER_NODE; i_Partition++) {
    m_iPartition[i_Partition] = -1;
  }
}

/*----------------------------------------------------------------------------*/
/* dump grid                                                                  */
/*----------------------------------------------------------------------------*/
void Grid::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  ASSERT(eClass == CN_GRID);
  ASSERT(pObject->eClass() == CN_GRID);
  
  Grid* pGrid = reinterpret_cast<Grid*>(pObject);
  
  vTrace("\n"_Stz);
}

/*----------------------------------------------------------------------------*/
/* dump grid                                                                  */
/*----------------------------------------------------------------------------*/
void Grid::vDumpPartitioning(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_GRID,_FOOTER_);
  
  for (i8 i_Index = 0; i_Index < m_nPartitions; i_Index++)
    vTrace("Partition: %lld; Node: %lld\n"_Stz, i_Index,
           Partition::iPartitionToNode(i_Index));
  vTrace("\n"_Stz);
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
Node* Grid::pFindNode(Stz* sName)
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
  
  delete pNodeCursor;
  
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
  
  delete pNodeCursor;
  
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
  for (i8 i_Slot = 0; i_Slot < m_nSlots; i_Slot++) {
    m_iPartition[i_Slot] = Partition::iSlotToPartition(i_Slot);
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
Footer* Grid::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_GRID);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
