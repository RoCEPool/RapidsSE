/*============================================================================*/
/* HeadsUp.cpp -- Heads-Up helper                                (on c stack) */
/*============================================================================*/

#include "BASE.h"

#include "HeadsUp.hpp"

#include "MY.hpp"
#include "PsxThd.hpp"

extern pthread_key_t giTLS_ThreadKey;
extern pthread_key_t giTLS_TaskKey;
extern pthread_key_t giTLS_HandlerKey;
extern pthread_key_t giTLS_StepKey;

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
HeadsUp::HeadsUp(void)
{
  _precondition_(_MAGIC_,CN_SPMEDIUM,_FOOTER_);
  
  _checkInitialised_(CN_SPID);
  
  _postcondition_(_MAGIC_,CN_SPMEDIUM,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
HeadsUp::HeadsUp(Stz* szFunc)
{
  _precondition_(_MAGIC_,CN_SPMEDIUM,_FOOTER_);
  
  _checkInitialised_(CN_SPID);
  
  _postcondition_(_MAGIC_,CN_SPMEDIUM,_FOOTER_);
  
  vTrace("HeadsUp @ %s: %s\n"_Stz
         ,szFunc
         ,sz()
         );
  
  Task* pTask = MY::pTask();
  if (pTask)
    pTask->vSeeFrameStack();

}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
HeadsUp::~HeadsUp(void) noexcept(false)
{
  _precondition_(_MAGIC_,CN_SPMEDIUM,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* sprint heads-up into result                                                */
/*----------------------------------------------------------------------------*/
Stz* HeadsUp::sz(void)
{
  _precondition_(_MAGIC_,CN_SPMEDIUM,_FOOTER_);
  
  vInit();
  
  // thread/task/job/op/eventhandler/step

  Stz* szThreadNm = MY::szThreadNm();
  Stz* szTaskNm   = MY::szTaskNm();
  Stz* szJobNm    = MY::szJobNm();
  Stz* szOpNm     = MY::szOpNm();
  
  Stz* szEvNm     = (Stz*)PsxThd::pTlsGet(giTLS_HandlerKey);
  Stz* szStepNm   = (Stz*)PsxThd::pTlsGet(giTLS_StepKey);
 
  Thread* pThread = MY::pThread();
  Stz* szThreadInfo = pThread
                      ? (pThread->eDedicated()
                          ? "dedicated"_Stz : "general"_Stz)
                      : ("unknown"_Stz)
                     ;
  
  Task* pTask = MY::pTask();
  Stz* szLockInfo = pTask
                      ? (pTask->bIsLocked()
                          ? "locked"_Stz : "not-locked"_Stz)
                      : ("unknown"_Stz)
                     ;
  
  vCatf("H:%s (%s) T:%s (%s) J:%s O:%s E:%s S:%s"
        ""_Stz

        ,szThreadNm
          ,szThreadInfo
        ,szTaskNm
          ,szLockInfo
        ,szJobNm
        ,szOpNm
        ,szEvNm
        ,szStepNm

        );
  
  return (Stz*) &m_caResult[0];
}

/*----------------------------------------------------------------------------*/
/* trace out heads-up info                                                    */
/*----------------------------------------------------------------------------*/
void HeadsUp::vShow(void)
{
  
  vTrace("%s\n"_Stz,sz());
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
__aFooterFunction(HeadsUp)

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
