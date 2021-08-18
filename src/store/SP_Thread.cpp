/*============================================================================*/
/* SP_Thread.cpp - Thread sprint helper                          (on c stack) */
/*============================================================================*/

#include "BASE.h"

#include "SP_Thread.hpp"
#include "SP_Gid.hpp"


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
SP_Thread::SP_Thread(void)
{
  _precondition_(_MAGIC_,CN_SPMEDIUM,_FOOTER_);
  
  _checkInitialised_(CN_SPMEDIUM);
  
  _postcondition_(_MAGIC_,CN_SPMEDIUM,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
SP_Thread::~SP_Thread(void) noexcept(false)
{
  _precondition_(_MAGIC_,CN_SPMEDIUM,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* sprint Header and return Stz* to result                                    */
/*----------------------------------------------------------------------------*/
Stz* SP_Thread::sz(Thread* pThread)
{
  _precondition_(_MAGIC_,CN_SPMEDIUM,_FOOTER_);
  UNUSED(pThread);
  
  return (Stz*) &m_caResult[0];
}

/*----------------------------------------------------------------------------*/
/* sprint Header and return Stz* to result                                    */
/*----------------------------------------------------------------------------*/
Stz* SP_Thread::sz_log(Thread* pThread)
{
  _precondition_(_MAGIC_,CN_SPMEDIUM,_FOOTER_);
  
  SP_Gid spTaskId;
  
  StrLib::vSprint(m_caResult,
                  "{%s %s%s}"_Stz,
                  
                  aHS[pThread->eThreadState()],
                  
                  spTaskId.sz(pThread->pTask()
                                ? pThread->pTask()->iTaskId()
                                : C_UNKNOWN),
                  
                  CN_NONE == pThread->eDedicated()
                    ? ""_Stz
                    : " dedicated"_Stz,

                  C_VARGS_END);
  
  return (Stz*) &m_caResult[0];
}

/*----------------------------------------------------------------------------*/
/* sprint Header and return Stz* to result                                    */
/*----------------------------------------------------------------------------*/
Stz* SP_Thread::sz_schedule(Thread* pThread)
{
  _precondition_(_MAGIC_,CN_SPMEDIUM,_FOOTER_);
  
  vInit();
  
  vCatf("Thread %p: "
        "%s %s"
        ""_Stz
              
        ,pThread
        ,aHS[pThread->eThreadState()]
                                  
        ,CN_NONE == pThread->eDedicated()
           ? "general"_Stz
           : " dedicated"_Stz

        );
  
  return (Stz*) &m_caResult[0];
}


/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
__aFooterFunction(SP_Thread)

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
