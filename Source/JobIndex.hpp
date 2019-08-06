/*============================================================================*/
/* JobIndex.hpp -- Index part for a Job                                       */
/*============================================================================*/
#pragma once

namespace Jobs
{
  class JobIndex
  {
  public:
    Index* m_pIndex; // index pointer
  
  public:
    void* operator new(size_t);
  
    JobIndex(void);
   ~JobIndex(void) noexcept(false);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
