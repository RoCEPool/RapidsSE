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

    LglPtn tHashToLglPtnId(i8 iHash);
  
    i8 nKnownNodes(void);
  
    i8 iMappedNodeId(LglPtn tLglPtn);
    i8 iMappedPtnNo(LglPtn tLglPtn);

    i8 nPtnsInGrid(void);
    i8 nPtnsPerNode(void);

    LglPtn tRobinToDtdPtnStream(i8 iRobin);
    LglPtn tRobinToLclPtnStream(i8 iRobin);
    LglPtn tRobinToNdePtnNo(i8 iRobin);

    void vSee(void);
  
    LglPtn tToPtnNoOfNode(i8 i_Ptn, i8 iNodeId);
    LglPtn tToPtnNoOfThisNode(i8 i_Ptn);

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
