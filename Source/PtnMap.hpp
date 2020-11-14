/*============================================================================*/
/* PtnMap.hpp -- Partition (to node,tablePtn) Map                             */
/*============================================================================*/
#pragma once

namespace MetaData
{
  // {i_Node,i_TablePtn} pair
  struct NodeTablePtn {
    i4  m_i_Node;     // node
    i4  m_i_TablePtn; // table partition
  };

  class PtnMap : public Object
  {
  private:
    Lock m_okLock;
  
    i8 m_nPtnsInGrid;
    i8 m_nPtnsPerTable;

    // which {node,tablePtn} a logical partition is in
    NodeTablePtn m_xaInNodeTablePtn[C_MAX_PTNS];
  
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    PtnMap(i8 nBootNodes, i8 nDegOfPar);
   ~PtnMap(void) noexcept(false);
  
    static i8   iHashToBucketId(i8 iHash);
    static tPN  tHashToPtnNo(i8 iHash, i8 nPtnsInGrid);
    static void vRunningHash(i8* aiHash, i8 iMixIn);
    static tPN  tStrandToLglPtnNo(i8 i_Strand);

    void vDumpPartitioning(void);

    i8 nPtnsInGrid(void);
    i8 nPtnsPerTable(void);
    i8 i_WhichNode(tPN tPtnNo);
    i8 i_WhichStrand(tPN tPtnNo);
    i8 i_WhichTablePtn(tPN tPtnNo);

  private:
    i8 i_LglPtnNoToInitialNode(tPN tPtnNo);
    i8 i_LglPtnNoToInitialStrand(tPN tPtnNo);
    Footer* aFooter(void);
  };
  
  PtnMap* _pPtnMap(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
