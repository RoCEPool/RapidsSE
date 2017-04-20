/*============================================================================*/
/* Slots.hpp - table partition slots                                          */
/*============================================================================*/
#pragma once

namespace DBobjects
{
  // allow for maximum 8 partitions per node
  static const i8 C_SLOTS_PER_NODE = 16;
  
  class Slots : public Object
  {
  private:
    Partition m_xPartition[C_SLOTS_PER_NODE];
    
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    Slots(void);
    ~Slots(void);
    
    Partition* pPartition(i8 iSlot);
    
    void vPopulateIndex(i8 iIndexNo);
    void vTidyUp(void);
    void vTidyUpIndex(i8 iIndexNo);
    void vTruncate(void);
    void vTruncateIndex(i8 iIndexNo);
    
    Footer* pFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
