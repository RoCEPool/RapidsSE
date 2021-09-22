/*============================================================================*/
/* PtnMap.hpp -- Partition (to node,tablePtn) Map                             */
/*============================================================================*/
#pragma once

namespace STORE
{
  struct NodePtn
  {
    i4  m_iNodeNo;  // node number
    i4  m_i_Ptn;    // node partition (aka partition of node)
  };

  struct NodeBkt
  {
    i4  m_iNodeNo;  // node number
    i4  m_i_Ptn;    // node partition (aka partition of node)
    i8  m_i_Bkt;    // Bucket number
  };

  class PtnMap : public Object
  {
  private:
    Lock m_okLock;
  
    i8 m_iMapChange;
    i8 m_nPtnsPerNode;
  
    i8 m_nKnownNodes;
    i8 m_nPtnsInGrid;

    // which {nodeNo,i_ptn} a logical partition mPtnNo is in
    NodePtn m_xaNodePtn[C_MAX_PTNSINGRID];

    // Temp bucket array
    NodeBkt m_xaBkt[C_MAX_PTNSINGRID];
  
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    PtnMap(i8 nKnownNodes, i8 nPtnsPerNode);
   ~PtnMap(void) noexcept(false);
  
    static i8   iHashToBucketId(i8 iHash);
  
    void vAdjustMap(i8 nKnownNodes);
    void vSee(void);
#ifdef WITH_PTNID

    PtnId tHashToDtdPtnId(i8 iHash);
    i8 iToBkt(PtnId tPtnNo);
    PtnId tToPtn(i8 iBktNo);
    i8 nKnownNodes(void);
    i8 nPtnsInGrid(void);
    i8 nPtnsPerNode(void);

    PtnId tRobinToDtdPtnNo(i8 iRobin);
    PtnId tRobinToLclPtnNo(i8 iRobin);
    PtnId tRobinToNdePtnNo(i8 iRobin, i8 iNodeNo);
    i8 iWhichNodeNo(PtnId tPtnNo);
    i8 i_WhichPtn(PtnId tPtnNo);
    PtnId tToPtnNoOfNode(i8 i_Ptn, i8 iNodeNo);
    PtnId tToPtnNoOfThisNode(i8 i_Ptn);

#else
    tPN  tHashToDtdPtnNo(i8 iHash);
    i8   iToBkt(tPN tPtnNo);
    tPN  tToPtn(i8 iBktNo);
    i8   nKnownNodes(void);
    i8   nPtnsInGrid(void);
    i8   nPtnsPerNode(void);
    tPN  tRobinToDtdPtnNo(i8 iRobin);
    tPN  tRobinToLclPtnNo(i8 iRobin);
    tPN  tRobinToNdePtnNo(i8 iRobin, i8 iNodeNo);
    i8   iWhichNodeNo(tPN tPtnNo);
    i8   i_WhichPtn(tPN tPtnNo);

    tPN  tToPtnNoOfNode(i8 i_Ptn, i8 iNodeNo);
    tPN  tToPtnNoOfThisNode(i8 i_Ptn);
#endif


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
