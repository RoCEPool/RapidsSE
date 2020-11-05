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
  
    i8 i_WhichNode(i8 i_Ptn);
  
  private:
    Footer* aFooter(void);
  };
  
  PtnMap* _pPtnMap(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
