/*============================================================================*/
/* RejectJob.hpp -- Reject Job                                                */
/*============================================================================*/
#pragma once

#include "eEC.h"
#include "RejectedJob.hpp"

namespace STORE
{
  class RejectJob : public Object
  {
  protected:

    RejectedJob* m_pRejectedJob;

  // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void operator delete(void*);
  
    RejectJob(void);
   ~RejectJob(void) noexcept(false);

    static void vPostRecv(void* aJob);

    eEC eError(void);
    void vError(eEC eError);

    JobId tOriginId(void);
    void vOriginId(JobId tOriginId);
  
    RejectedJob* pRejectedJob(void);

  private:
    Footer* aFooter(void);
  };

  RejectJob* _pRejectJob(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
