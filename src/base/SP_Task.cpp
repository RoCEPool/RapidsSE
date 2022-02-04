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
  
  if (! pTask) {
    vCats("Task {(none)}"_Stz);
    return (Stz*) &m_caResult[0];
  }
  
  vCatf("Task {"
        "%p "
        "%s %s %8s"_Stz

        ,pTask
        
        ,aTS[pTask->eState()]
        ,aQK[pTask->eQueue()]
        ,aTR[pTask->eTransition()]
  
        );
  
  if (TS_SLEEPING == pTask->eState() &&
      QK_SLEEPING_QUEUE == pTask->eQueue()) {
    SP_Ivl spIvl;
    vCatf(" wakeup %s"_Stz
    
          ,spIvl.sz(pTask->oWakeup().m_wWakeup.T.m_iAsI8)
          
          );
  }

  vCats("}"_Stz);

  return (Stz*) &m_caResult[0];
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
__aFooterFunction(SP_Task)

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
