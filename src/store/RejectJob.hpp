/*============================================================================*/
/* RejectJob.hpp -- Reject Job                                                */
/*============================================================================*/
#pragma once

#include "eEC.h"

namespace STORE
{
  class RejectJob : public Object
  {
  protected:

    JobId  m_tOriginId;   // id when job is originator
  
    eEC    m_eError;      // error


  // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void operator delete(void*);
  
    RejectJob(void);
   ~RejectJob(void) noexcept(false);
 
    eEC eError(void);
    void vError(eEC eError);

    JobId tOriginId(void);
    void vOriginId(JobId tOriginId);
  
    bool bReject(void);

  private:
    Footer* aFooter(void);
  };

  RejectJob* _pRejectJob(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
