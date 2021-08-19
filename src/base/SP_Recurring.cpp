/*============================================================================*/
/* SP_Recurring.cpp - Recurring sprint helper                    (on c stack) */
/*============================================================================*/

#include "BASE.h"

#include "Op.hpp"

#include "SP_Gid.hpp"

#include "SP_Thread.hpp"
#include "SP_Recurring.hpp"
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
  
  vCatf("Task {%p "
        "%s %s %s"_Stz

        ,pTask
        
        ,aTS[pTask->eState()]
        ,aQK[pTask->eQueue()]
        ,aTR[pTask->eTransition()]
  
        );
  
  if (TS_SLEEPING == pTask->eState() && QK_SLEEPING_QUEUE == pTask->eQueue()) {
//    vCatf("wakeup %lld.%06lld "_Stz
//
//          ,(i8)pTask->oWakeup().m_wWakeup.m_uSecond
//          ,(i8)pTask->oWakeup().m_wWakeup.m_uMicro
//          );
        
    SP_Ivl spIvl;
    vCatf(" wakeup %s"_Stz
    
          ,spIvl.sz(pTask->oWakeup().m_wWakeup.m_iAsI8)
          
          );

  }

  vCats("}"_Stz);

  
  
  // - - -
  
  
  
  
  Thread* pThread = pTask->pvThread_DONT_DEREF();
  SP_Thread spThread;
  vCatf(" %s"_Stz,spThread.sz_schedule(pThread));

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // job or op
  
  Job* pJob = pTask->pJob();
  if (pJob) {
    SP_Job spJob;
    vCatf("job: %s"_Stz,spJob.sz_schedule(pJob));
    return (Stz*) &m_caResult[0];
  }

  Op* pOp = pTask->pOp();
  if (pOp) {
    vCats("op: "_Stz);
    
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
