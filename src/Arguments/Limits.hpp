/*============================================================================*/
/* Limits.hpp - Enforced Limits of Args                                              */
/*============================================================================*/
#pragma once

namespace Arguments
{
  class Limits : public Object
  {
  private:
    
    i8   m_nMaxMemory;     // maximum amount of memory
    i8   m_nMaxNodes;      // maximum number of nodes
    i8   m_nMaxPartitions; // maximum number of partitions per node
    i8   m_nMaxSessions;   // maximum number of sessions
    i8   m_nMaxThreads;    // maximum number of threads

    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Limits(void);
   ~Limits(void) noexcept(false);
    
    Footer* aFooter(void);
  };

  Limits* _pLimits(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
