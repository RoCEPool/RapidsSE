/*============================================================================*/
/* JobMark2.hpp -- a job in the system                                        */
/*============================================================================*/
#pragma once

namespace Jobs
{
  class JobMark2 : public Job,
                   public JobTable,
                   public JobIndex
  {
  public:
    bool   m_bCancel; // cancel job
    bool   m_bEarly;  // early exit
    Atomic m_ixTempBuffers; // temp buffers in flight
    eRU    m_eResUsage;     // resource usage
  
  public:
    JobMark2(eCN eClassNo, ReqTuple* aReqTuple, Session* pSession);
   ~JobMark2(void) noexcept(false);
  };
  
  JobMark2* _pJobMark2(void* aAddr);
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
