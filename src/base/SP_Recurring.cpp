/*============================================================================*/
/* SP_Recurring.cpp - Recurring sprint helper                    (on c stack) */
/*============================================================================*/

#include "BASE.h"

#include "SP_Recurring.hpp"

#include "Op.hpp"

#include "SP_Gid.hpp"

#include "SP_Thread.hpp"
#include "SP_Task.hpp"
#include "SP_Ivl.hpp"
#include "SP_Job.hpp"
#include "SP_Op.hpp"


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
SP_Recurring::SP_Recurring(void)
{
  _precondition_(_MAGIC_,CN_SPMEDIUM,_FOOTER_);
  
  _checkInitialised_(CN_SPID);
  
  _postcondition_(_MAGIC_,CN_SPMEDIUM,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
SP_Recurring::~SP_Recurring(void) noexcept(false)
{
  _precondition_(_MAGIC_,CN_SPMEDIUM,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* sprint extended task to result                                             */
/*----------------------------------------------------------------------------*/
Stz* SP_Recurring::sz(Task* pTask)
{
  _precondition_(_MAGIC_,CN_SPMEDIUM,_FOOTER_);
  
  vInit();
  
  SP_Task spTask;
  vCatf("%s"_Stz,spTask.sz(pTask));

  // - - -
  
  Thread* pThread = pTask->pvThread_DONT_DEREF();
  SP_Thread spThread;
  vCatf(" %s"_Stz,spThread.sz(pThread));

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // job or op
  
  Job* pJob = pTask->pJob();
  if (pJob) {
    SP_Job spJob;
    vCatf(" %s"_Stz,spJob.sz(pJob));
    return (Stz*) &m_caResult[0];
  }

  Op* pOp = pTask->pOp();
  if (pOp) {
    vCats(" op: "_Stz);
    
    i8 nQueuedOperands = pOp->nQueuedOperands();
    vCatf("operands %lld "_Stz,nQueuedOperands);

    SP_Op spOp;
    vCatf("%s "_Stz,spOp.szSchedule(pOp));
    return (Stz*) &m_caResult[0];
  }

  return (Stz*) &m_caResult[0];
}


/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
__aFooterFunction(SP_Recurring)

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
