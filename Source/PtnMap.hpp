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
  
    // which {node,tablePtn} a logical partition is in
    NodeTablePtn m_xaInNodeTablePtn[C_MAX_PTNS];
  
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    PtnMap(i8 nPartitions);
   ~PtnMap(void) noexcept(false);
  
    static tPN  tHashToPtnNo(i8 iHash, i8 nPartitionsInGrid);
    static i8   i_LglPtnNoToStrand(tPN tPtnNo);
    static void vRunningHash(i8* aiHash, i8 iMixIn);
    static tPN  tStrandToLglPtnNo(i8 i_Strand);


    i8 i_WhichNode(tPN tPtnNo);
    i8 i_WhichTablePtn(tPN tPtnNo);

  private:
    i8 i_LglPtnNoToInitialNode(tPN tPtnNo);
    Footer* aFooter(void);
  };
  
  PtnMap* _pPtnMap(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
