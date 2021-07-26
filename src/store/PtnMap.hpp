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

  class PtnMap : public Object
  {
  private:
    Lock m_okLock;
  
    i8 m_nPtnsPerNode;
  
    i8 m_nKnownNodes;
    i8 m_nPtnsInGrid;

    // which {nodeNo,i_ptn} a logical partition mPtnNo is in
    NodePtn m_xaNodePtn[C_MAX_PTNSINGRID];
  
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    PtnMap(i8 nKnownNodes, i8 nPtnsPerNode);
   ~PtnMap(void) noexcept(false);
  
    static i8   iHashToBucketId(i8 iHash);
    static tPN  tHashToPtnNo(i8 iPtnKeyHash);
    static void vRunningHash(i8* aiPtnKeyHash, i8 iMixIn);
  
    static tPN  tStreamToLglPtnNo(i8 i_Stream);

    static tPN  tRobinToPtnNo(i8 iRobin);
    static i8   i_RobinToPtn(i8 iRobin);

    void vAdjustMap(i8 nKnownNodes);
    i8   nKnownNodes(void);
    i8   nPtnsInGrid(void);
    i8   nPtnsPerNode(void);
    void vSee(void);
    i8   iWhichNodeNo(tPN tPtnNo);
    i8   i_WhichPtn(tPN tPtnNo);
 
  private:
    void vRepartition(void);
    void vResetNodePtnArray(void);

    Footer* aFooter(void);
  };
  
  PtnMap* _pPtnMap(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
