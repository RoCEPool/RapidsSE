/*============================================================================*/
/* RejectJob.cpp -- Reject Job                                                */
/*============================================================================*/

#include "BASE.h"

#include "RejectJob.hpp"
#include "RejectedJob.hpp"

#include "eCF.h"
#include "Job.hpp"
#include "AM.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE,RejectJob,CN_REJECTJOB)
__aNewFunction(RejectJob,CN_REJECTJOB)
__vDeleteFunction(RejectJob,CN_REJECTJOB)
__aFooterFunction(RejectJob)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
RejectJob::RejectJob(void)
: Object(CN_REJECTJOB)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_REJECTJOB,_FOOTER_);
    
  //m_tOriginId

  m_pRejectedJob = new RejectedJob;

  _checkInitialised_(CN_REJECTJOB);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_REJECTJOB,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
RejectJob::~RejectJob(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_REJECTJOB,_FOOTER_);
 //  vTrace("delete RejectJob(%s) %p @%lld\n"_Stz,
//         aOM[m_eOpRejectJob],this,m_xFooter.iWhenAllocated());
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* post recv function                                                         */
/*----------------------------------------------------------------------------*/
void RejectJob::vPostRecv(void* aReceivedJob)
{
  _static_

  // nothing to do

}

/*----------------------------------------------------------------------------*/
/* get error code                                                             */
/*----------------------------------------------------------------------------*/
eEC RejectJob::eError(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_REJECTJOB,_FOOTER_);

  return m_pRejectedJob->eError();
}

/*----------------------------------------------------------------------------*/
/* set error code                                                             */
/*----------------------------------------------------------------------------*/
void RejectJob::vError(eEC eError)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_REJECTJOB,_FOOTER_);

  m_pRejectedJob->vError(eError);
}

/*----------------------------------------------------------------------------*/
/* get origin job id                                                          */
/*----------------------------------------------------------------------------*/
JobId RejectJob::tOriginId(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_REJECTJOB,_FOOTER_);

  return m_pRejectedJob->tOriginId();
}

/*----------------------------------------------------------------------------*/
/* set origin job id                                                          */
/*----------------------------------------------------------------------------*/
void RejectJob::vOriginId(JobId tOriginId)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_REJECTJOB,_FOOTER_);

  m_pRejectedJob->vOriginId(tOriginId);
}

/*----------------------------------------------------------------------------*/
/* return rejected job                                                        */
/*----------------------------------------------------------------------------*/
RejectedJob* RejectJob::pRejectedJob(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_REJECTJOB,_FOOTER_);

  return m_pRejectedJob;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
