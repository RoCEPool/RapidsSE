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

  public:
    JobBase(eCN eClassNo, ReqTuple* aReqTuple, i8 iJobId, Focal* pFor);
   ~JobBase(void) noexcept(false);
  
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
