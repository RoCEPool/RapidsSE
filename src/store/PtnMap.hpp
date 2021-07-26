/*============================================================================*/
/* PtnMap.hpp -- Partition (to node,tablePtn) Map                             */
/*============================================================================*/
#pragma once

namespace STORE
{
  // {i_Node,i_NodePtn} pair
  struct NodePtn
  {
    i4  m_i_Node;     // node
    i4  m_i_NodePtn;  // node partition
  };

  class PtnMap : public Object
  {
  private:
    Lock m_okLock;
  
    i8 m_nKnownNodes;
    i8 m_nPtnsPerNode;
    i8 m_nPtnsInGrid;

    // which {node,tablePtn} a logical partition is in
    NodePtn m_xaNodePtn[C_MAX_PTNSINGRID];
  
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    PtnMap(i8 nKnownNodes, i8 nPtnsPerNode);
   ~PtnMap(void) noexcept(false);
  
    static i8   iHashToBucketId(i8 iHash);
    static tPN  tHashToPtnNo(i8 iHash, i8 nPtnsInGrid);
    static void vRunningHash(i8* aiHash, i8 iMixIn);
    static tPN  tStreamToLglPtnNo(i8 i_Stream);

    void vAdjustMap(i8 nKnownNodes, i8 nPtnsPerNode);
    void vDumpDataBuffers(void);
    i8   nPtnsInGrid(void);
    i8   nPtnsPerNode(void);
    void vSee(void);
    i8   i_WhichNode(tPN tPtnNo);
    i8   i_WhichStream(tPN tPtnNo);
    i8   i_WhichTablePtn(tPN tPtnNo);

  private:
    i8 i_LglPtnNoToInitialNode(tPN tPtnNo);
    i8 i_LglPtnNoToInitialStream(tPN tPtnNo);
    void vRepartition(i8 nKnownNodes);
    void vResetNodePtnArray(void);

    Footer* aFooter(void);
  };
  
  PtnMap* _pPtnMap(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
