/*============================================================================*/
/* JobMark2.hpp -- a job in the system                                        */
/*============================================================================*/
#pragma once

namespace Jobs
{
  class JobMark2 : public Job,
                   public JobTable
  {
  public:

  public:
    JobMark2(eCN eClassNo, Interface::Session* pSession);
   ~JobMark2(void) noexcept(false);
  };
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
