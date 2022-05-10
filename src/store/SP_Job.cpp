/*============================================================================*/
/* SP_Job.cpp - Job sprint helper                                (on c stack) */
/*============================================================================*/

#include "BASE.h"

#include "SP_Footer.hpp"
#include "SP_Header.hpp"
#include "SP_Job.hpp"
#include "Pipeline.hpp"

#include "Job.hpp"

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
SP_Job::SP_Job(void)
{
  _precondition_(_MAGIC_,CN_SPMEDIUM,_FOOTER_);
  
  _checkInitialised_(CN_SPMEDIUM);
  
  _postcondition_(_MAGIC_,CN_SPMEDIUM,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
SP_Job::~SP_Job(void) noexcept(false)
{
  _precondition_(_MAGIC_,CN_SPMEDIUM,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* sprint Header and return Stz* to result                                    */
/*----------------------------------------------------------------------------*/
Stz* SP_Job::sz(Job* pJob)
{
  _precondition_(_MAGIC_,CN_SPMEDIUM,_FOOTER_);
  
  vInit();

  vCatf("Job {"
        "%p "
        "%s %s %s %s"_Stz

        ,pJob
        
        ,pJob->szJobNm()
        ,aJK[pJob->eJobKind()]
        ,aJP[pJob->eJobPhase()]
        ,aMX[pJob->eMutex()]

        );
  
  if (pJob->pTask()->iAcksRmg_assert_only_()) {
    vCatf(" AcksRmg %lld (out of %lld)"_Stz
           ,pJob->pTask()->iAcksRmg_assert_only_()
           ,pJob->pTask()->iAcksOriginal()
           );
  }
    
  vCats("}"_Stz);

  return (Stz*) &m_caResult[0];
}

/*----------------------------------------------------------------------------*/
/* sprint Header and return Stz* to result                                    */
/*----------------------------------------------------------------------------*/
Stz* SP_Job::sz_recv(Job* pJob)
{
  _precondition_(_MAGIC_,CN_SPMEDIUM,_FOOTER_);
  
  SP_Header   spHeader;
  SP_Footer   spFooter;
  
  StrLib::vSprint(m_caResult,
                  "Header %s ... Footer %s"_Stz,
                  
                  spHeader.sz(pJob),
                   spFooter.sz(pJob)
                  );
  
  return (Stz*) &m_caResult[0];
}


/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
__aFooterFunction(SP_Job)

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
