/*============================================================================*/
/* JobEnmSet.hpp -- Ename-Value part for a Job                                */
/*============================================================================*/
#pragma once

namespace Jobs
{
  class JobEnmSet
  {
  public:
    EnameSet  m_oEnameSet;  // ename-value set
  
  public:
    void* operator new(size_t);
  
    JobEnmSet(void);
   ~JobEnmSet(void) noexcept(false);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
