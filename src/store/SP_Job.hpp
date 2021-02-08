/*============================================================================*/
/* SP_Job.hpp - Job sprint helper                                             */
/*============================================================================*/
#pragma once

namespace STORE
{
  class SP_Job : public SP
  {
  public:

    // must be last
    Footer m_xFooter;
    
  public:
    SP_Job(void);
   ~SP_Job(void) noexcept(false);
  
    Stz* sz_api(Job* pJob);
    Stz* sz_log(Job* pJob);
    Stz* sz_recv(Job* pJob);
    Stz* sz_detail(Job* pJob);

    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
