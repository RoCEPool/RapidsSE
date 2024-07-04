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
    JobId  m_tDistId;     // distributed job id
    eEC    m_eError;      // error

  // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void operator delete(void*);
  
    RejectJob(void);
   ~RejectJob(void) noexcept(false);

    static bool bObjRecv(Socket* pSocket, Object* pObjBase, SysCall& roSysCall,
                         Object** ppObject, void* pvAuxBuf, i8 iAuxSize);
    static void vPostRecv(void* aJob);

    JobId tDistId(void);
    void vDistId(JobId tDistId);

    eEC eError(void);
    void vError(eEC eError);

    JobId tOriginId(void);
    void vOriginId(JobId tOriginId);

  private:
    Footer* aFooter(void);
  };

  RejectJob* _pRejectJob(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
