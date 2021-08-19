/*============================================================================*/
/* SP_Task.cpp - Task sprint helper                              (on c stack) */
/*============================================================================*/

#include "BASE.h"

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
SP_Task::SP_Task(void)
{
  _precondition_(_MAGIC_,CN_SPMEDIUM,_FOOTER_);
  
  _checkInitialised_(CN_SPID);
  
  _postcondition_(_MAGIC_,CN_SPMEDIUM,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
SP_Task::~SP_Task(void) noexcept(false)
{
  _precondition_(_MAGIC_,CN_SPMEDIUM,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* sprint Header and return Stz* to result                                    */
/*----------------------------------------------------------------------------*/
Stz* SP_Task::sz(Task* pTask)
{
  _precondition_(_MAGIC_,CN_SPMEDIUM,_FOOTER_);
  
  vInit();
  
  vCatf("Task %p "
        "%s %s %s "_Stz

        ,pTask
        
        ,aTS[pTask->eState()]
        ,aQK[pTask->eQueue()]
        ,aTR[pTask->eTranstionRequest()]
  
        );
  
  if (TS_SLEEPING == pTask->eState() && QK_SLEEPING_QUEUE == pTask->eQueue()) {
//    vCatf("wakeup %lld.%06lld "_Stz
//
//          ,(i8)pTask->oWakeup().m_wWakeup.m_uSecond
//          ,(i8)pTask->oWakeup().m_wWakeup.m_uMicro
//          );
        
    SP_Ivl spIvl;
    vCatf("wakeup %s "_Stz
    
          ,spIvl.sz(pTask->oWakeup().m_wWakeup.m_iAsI8)
          
          );

  }
  
  return (Stz*) &m_caResult[0];
}

/*----------------------------------------------------------------------------*/
/* sprint Header and return Stz* to result                                    */
/*----------------------------------------------------------------------------*/
Stz* SP_Task::szJobOp(Task* pTask)
{
  _precondition_(_MAGIC_,CN_SPMEDIUM,_FOOTER_);
  
  IGNORE_RETURN( sz(pTask) );
  
  Thread* pThread = pTask->pvThread_DONT_DEREF();
  if (pThread) {
    SP_Thread spThread;
    vCatf("%s "_Stz,spThread.sz_schedule(pThread));
  }
  else {
    vCats("(no thread) "_Stz);
  }

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
__aFooterFunction(SP_Task)

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
