/*============================================================================*/
/* JobDicPath.hpp -- Dictionary Path part for a Job                           */
/*============================================================================*/
#pragma once

namespace Jobs
{
  class JobDicPath
  {
  public:
    DicPath m_oDicPath; // dictionary path
    
  public:
    void* operator new(size_t);
  
    JobDicPath(void);
    JobDicPath(Job* pJob);
    JobDicPath(Session* pSession);
   ~JobDicPath(void) noexcept(false);
  
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
