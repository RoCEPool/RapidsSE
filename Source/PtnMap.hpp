/*============================================================================*/
/* PtnMap.hpp -- Partition (to node) Map                                      */
/*============================================================================*/
#pragma once

namespace MetaData
{
  // {i_Node,i_TablePtn} pair
  struct NodeTablePtn {
    i4  m_i_Node;
    i4  m_i_TablePtn;
  };

  class PtnMap : public Object
  {
  private:
    Lock m_okLock;
  
    // which [node,tablePtn] a logical partition is in
    NodeTablePtn m_xaInNodeTablePtn[C_MAX_PTNS];
  
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    PtnMap(i8 nPartitions);
   ~PtnMap(void) noexcept(false);
  
    static i8  i_LglPtnNoToStrand(tPN tPtnNo);
    static tPN tStrandToLglPtnNo(i8 i_Strand);

    static tPN  tPtnNo(i8 iRunningHash, i8 nPartitionsInGrid);
    static void vRunning(i8* aiRunningHash, i8 iVal);

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
