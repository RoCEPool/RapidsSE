/*============================================================================*/
/* SP_Task.cpp - Task sprint helper                              (on c stack) */
/*============================================================================*/

#include "BASE.h"

#include "SP_Gid.hpp"

#include "SP_Task.hpp"
#include "SP_Ivl.hpp"

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
Stz* SP_Task::sz_log(Task* pTask)
{
  _precondition_(_MAGIC_,CN_SPMEDIUM,_FOOTER_);
  
  SP_Gid spThreadId;
  
  StrLib::vSprint(m_caResult,
                  "{%s %s %s}"_Stz,

                  aTS[pTask->eState()],
                  aQK[pTask->eQueue()],
                  aTR[pTask->eTranstionRequest()],
  
// AY+RH : 29th Apr 2020 : TaskA should never attempt pTaskB->pThread()
//                         TaskA can never assume existence of
//                         some other tasks thread
//
//                  spThreadId.sz(pTask->pThread()
//                                  ? AM::iThreadId(pTask->pThread())
//                                  : C_UNKNOWN),

                  C_VARGS_END);

  return (Stz*) &m_caResult[0];
}


/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
__aFooterFunction(SP_Task)

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
