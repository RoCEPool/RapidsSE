/*============================================================================*/
/* RejectedJob.cpp -- Rejected Job                                                */
/*============================================================================*/

#include "BASE.h"

#include "RejectedJob.hpp"

#include "eCF.h"
#include "Job.hpp"
#include "AM.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE,RejectedJob,CN_REJECTEDJOB)
__aNewFunction(RejectedJob,CN_REJECTEDJOB)
__vDeleteFunction(RejectedJob,CN_REJECTEDJOB)
__aFooterFunction(RejectedJob)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
RejectedJob::RejectedJob(void)
: Object(CN_REJECTEDJOB)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_REJECTEDJOB,_FOOTER_);
    
  //m_tOriginId
  m_eError = EC_NONE;

  _checkInitialised_(CN_REJECTEDJOB);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_REJECTEDJOB,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
RejectedJob::~RejectedJob(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_REJECTEDJOB,_FOOTER_);
 //  vTrace("delete RejectedJob(%s) %p @%lld\n"_Stz,
//         aOM[m_eOpRejectedJob],this,m_xFooter.iWhenAllocated());
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* post recv function                                                         */
/*----------------------------------------------------------------------------*/
void RejectedJob::vPostRecv(void* aReceivedJob)
{
  _static_

  // nothing to do

}

/*----------------------------------------------------------------------------*/
/* get error code                                                             */
/*----------------------------------------------------------------------------*/
eEC RejectedJob::eError(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_REJECTEDJOB,_FOOTER_);

  return m_eError;
}

/*----------------------------------------------------------------------------*/
/* set error code                                                             */
/*----------------------------------------------------------------------------*/
void RejectedJob::vError(eEC eError)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_REJECTEDJOB,_FOOTER_);

  m_eError = eError;
}

/*----------------------------------------------------------------------------*/
/* get origin job id                                                          */
/*----------------------------------------------------------------------------*/
JobId RejectedJob::tOriginId(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_REJECTEDJOB,_FOOTER_);

  return m_tOriginId;
}

/*----------------------------------------------------------------------------*/
/* set origin job id                                                          */
/*----------------------------------------------------------------------------*/
void RejectedJob::vOriginId(JobId tOriginId)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_REJECTEDJOB,_FOOTER_);

  m_tOriginId = tOriginId;
}

/*----------------------------------------------------------------------------*/
/* Rejected job                                                                 */
/*----------------------------------------------------------------------------*/
bool RejectedJob::bRejected(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_REJECTEDJOB,_FOOTER_);

  return false;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
