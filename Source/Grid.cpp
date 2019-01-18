/*============================================================================*/
/* Grids -- Grid Model                                                    */
/*============================================================================*/

#include "Grids.h"

#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

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
  vFatal("fatal: Grids class cannot be deleted"_Stz);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Grid::Grid(i8 nNodes, i8 nSlots)
: Object(CN_GRID)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_GRID,_FOOTER_);
  
  vInit(nNodes, nSlots);
  
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
void Grid::vInit(i8 nNodes, i8 nSlots)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_GRID,_FOOTER_);
  
  m_nNodes      = nNodes;
  m_iThisNode   = C_UNKNOWN;
  m_pThisNode   = nullptr;
  m_iStartNode  = C_UNKNOWN;
  m_bHasStarted = false;
  m_nSlots      = nSlots;
  m_nPartitions = m_nSlots * m_nNodes;
    
  // initialize the partition array
  for (i8 i_Partition = 0; i_Partition < C_SLOTS_PER_NODE; i_Partition++) {
    m_iPartition[i_Partition] = C_UNKNOWN;
  }
    
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
/* dump grid                                                                  */
/*----------------------------------------------------------------------------*/
void Grid::vAddNode(Node* pNode)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_GRID,_FOOTER_);

  m_pNodes[pNode->iNodeNo()] = pNode;
}

/*----------------------------------------------------------------------------*/
/* dump grid                                                                  */
/*----------------------------------------------------------------------------*/
void Grid::vDumpPartitioning(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_GRID,_FOOTER_);
  
  for (i8 i_Index = 0; i_Index < m_nPartitions; i_Index++)
    vTrace("Partition: %lld; Node: %lld\n"_Stz, i_Index,
           TablePtn::iPartitionToNode(i_Index));
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
  
  for (i8 i_Node = 0; i_Node < m_nNodes; i_Node++) {
    Node* pNode = m_pNodes[i_Node];
    if (pNode && pNode->aNodeName()->bSame(sName))
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

  RASSERT(i_Node >= 0 and i_Node < m_nNodes);
  
  Node* pNode = m_pNodes[i_Node];
  
  return pNode;
}

/*----------------------------------------------------------------------------*/
/* get number of nodes in the grid                                            */
/*----------------------------------------------------------------------------*/
i8 Grid::nNodes(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_GRID,_FOOTER_);
  
  return m_nNodes;
}

/*----------------------------------------------------------------------------*/
/* get number of partition slots per node in the grid                         */
/*----------------------------------------------------------------------------*/
i8 Grid::nSlots(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_GRID,_FOOTER_);
  
  return m_nSlots;
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
  
  // now initialize the array of partition numbers on this node
  for (i8 i_Slot = 0; i_Slot < m_nSlots; i_Slot++) {
    m_iPartition[i_Slot] = TablePtn::iSlotToPartition(i_Slot);
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
