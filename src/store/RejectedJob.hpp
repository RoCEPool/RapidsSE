/*============================================================================*/
/* RejectedJob.hpp -- Rejected Job                                            */
/*============================================================================*/
#pragma once

#include "eEC.h"

namespace STORE
{
  class RejectedJob : public Object
  {
  protected:

    JobId  m_tOriginId;   // id when job is originator

    eEC    m_eError;      // error


  // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void operator delete(void*);
  
    RejectedJob(void);
   ~RejectedJob(void) noexcept(false);

    static void vPostRecv(void* aJob);

    eEC eError(void);
    void vError(eEC eError);

    JobId tOriginId(void);
    void vOriginId(JobId tOriginId);
  
    bool bRejected(void);

  private:
    Footer* aFooter(void);
  };

  RejectedJob* _pRejectedJob(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
