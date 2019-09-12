/*============================================================================*/
/* JobBase.hpp -- base of Job                                                 */
/*============================================================================*/
#pragma once

namespace Jobs
{
  class JobBase : public Focal
  {
  public:
    ReqTuple  m_xReqTuple; // the request tuple
  
    JobClass* m_aJobClass; // job class

  public:
    JobBase(eCN eClassNo);
    JobBase(JobClass* aJobClass, ReqTuple* aReqTuple, i8 iJobId, i8 iForId);
   ~JobBase(void) noexcept(false);
  
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
