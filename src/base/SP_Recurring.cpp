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
  
  if (! pTask) {
    vCats("Task {(none)}"_Stz);
    return (Stz*) &m_caResult[0];
  }

  Thread* pThread = pTask->pvThread_DONT_DEREF();
  Job* pJob = pTask->pJob();
  Op* pOp = pTask->pOp();
  
  vInit();
  
  s32 sMnemonic;
  
  sMnemonic = "TSK"_Stz;
  if (pThread)
    sMnemonic.vConcat("/THD"_Stz);
  else
    sMnemonic.vConcat("/thd"_Stz);
  if (pJob)
    sMnemonic.vConcat("*JOB"_Stz);
  else
  if (pOp)
    sMnemonic.vConcat("*OPR"_Stz);
  else
    sMnemonic.vConcat("*???"_Stz);

  vCatf("%-11s"_Stz,sMnemonic.szStz());

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // task
  SP_Task spTask;
  vCatf(" %s"_Stz,spTask.sz(pTask));

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // thread
  
  SP_Thread spThread;
  vCatf(" %s"_Stz,spThread.sz(pThread));

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // job or op
  
  if (pJob) {
    SP_Job spJob;
    vCatf(" %s"_Stz,spJob.sz(pJob));
  }
  else
  if (pOp) {
    SP_Op spOp;
    vCatf(" %s"_Stz,spOp.sz(pOp));
  }
  else {
    vCats(" NON-DECODABLE"_Stz);
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
