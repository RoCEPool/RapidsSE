/*============================================================================*/
/* RejectJob.cpp -- Reject Job                                                */
/*============================================================================*/

#include "BASE.h"

#include "RejectJob.hpp"

#include "eCF.h"
#include "Job.hpp"
#include "AM.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE,RejectJob,CN_REJECTJOB)
__aNewFunction(RejectJob,CN_CHILDEXIT)
__vDeleteFunction(RejectJob,CN_REJECTJOB)
__aFooterFunction(RejectJob)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
RejectJob::RejectJob(void)
: Object(CN_REJECTJOB)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_REJECTJOB,_FOOTER_);
  
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

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
