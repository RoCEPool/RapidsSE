/*============================================================================*/
/* JobEnmSet.hpp -- Ename-Value part for a Job                                */
/*============================================================================*/
#pragma once

namespace Jobs
{
  class JobEnmSet
  {
  public:
    EnmSet  m_oEnmSet; // ename-value
  public:
    void* operator new(size_t);
  
    JobEnmSet(void);
   ~JobEnmSet(void) noexcept(false);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
