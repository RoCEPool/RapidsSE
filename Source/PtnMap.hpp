/*============================================================================*/
/* PtnMap.hpp -- Partition (to node) Map                                      */
/*============================================================================*/
#pragma once

namespace Partitioning
{
  class PtnMap : public Object
  {
  private:
    Lock m_okLock;
  
    i8   m_iaInNode[C_MAX_PTNS]; // which node is a partition in
  
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    PtnMap(i8 nPartitions);
   ~PtnMap(void) noexcept(false);
  
    static i8  i_PtnNoToStrand(tPN tPtnNo);
    static tPN tStrandToPtnNo(i8 i_Strand);

    i8 i_WhichNode(tPN tPtnNo);
  
  private:
    i8 i_PtnNoToNode(tPN tPtnNo);
    Footer* aFooter(void);
  };
  
  PtnMap* _pPtnMap(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
