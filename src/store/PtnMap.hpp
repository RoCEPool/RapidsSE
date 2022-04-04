/*============================================================================*/
/* PtnMap.hpp -- Partition (to node,tablePtn) Map                             */
/*============================================================================*/
#pragma once

namespace STORE
{
  struct NodePtn
  {
    i4  m_iNodeId;  // node id (in grid)
    i4  m_iPtnNo;   // partition no (within node)
  };


  class PtnMap : public Object
  {
  private:
    Lock m_okLock;
  
    i8 m_iMapChange;
    i8 m_nPtnsPerNode;
  
    i8 m_nKnownNodes;
    i8 m_nPtnsInGrid;

    // which physical {nodeId,i_ptn} a logical partition PtnId is in
    NodePtn m_xaPtnId_to_NodeIdPtnNo[C_MAX_LGLPTNS];

  
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    PtnMap(i8 nKnownNodes, i8 nPtnsPerNode);
   ~PtnMap(void) noexcept(false);
  
    void vAdjustMap(i8 nKnownNodes);

    PtnId tHashToLglPtnId(i8 iHash);
  
    i8 nKnownNodes(void);
  
    i8 iMappedNodeId(PtnId tPtnId);
    i8 i_MappedPtnNo(PtnId tPtnId);

    i8 nPtnsInGrid(void);
    i8 nPtnsPerNode(void);

    PtnId tRobinToDtdPtnStream(i8 iRobin);
    PtnId tRobinToLclPtnStream(i8 iRobin);
    PtnId tRobinToNdePtnNo(i8 iRobin);

    void vSee(void);
  
    PtnId tToPtnNoOfNode(i8 i_Ptn, i8 iNodeNo);
    PtnId tToPtnNoOfThisNode(i8 i_Ptn);

  private:
    void vRepartition(void);
    void vResetNodePtnArray(void);
    void vInitNodeBktArray(void);

    Footer* aFooter(void);
  };
  
  PtnMap* _pPtnMap(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
