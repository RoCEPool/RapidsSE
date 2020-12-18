/*============================================================================*/
/* SP_Job.cpp - Job sprint helper                                (on c stack) */
/*============================================================================*/

#include "Plans.h"

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
SP_Job::SP_Job(void)
: SP(CN_SPID)
{
  _precondition_(_MAGIC_,CN_SPID,_FOOTER_);
  
  _checkInitialised_(CN_SPID);
  
  _postcondition_(_MAGIC_,CN_SPID,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
SP_Job::~SP_Job(void) noexcept(false)
{
  _precondition_(_MAGIC_,CN_SPID,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* sprint Header and return Stz* to result                                    */
/*----------------------------------------------------------------------------*/
Stz* SP_Job::sz_api(Job* pJob)
{
  _precondition_(_MAGIC_,CN_SPID,_FOOTER_);
    
  StrLib::vSprint(m_caResult,
                  "Job %p: name %s "_Stz,
                  
                  pJob,
                  pJob->szJobNm()
                  
                  );
  
  return (Stz*) &m_caResult[0];
}

/*----------------------------------------------------------------------------*/
/* sprint Header and return Stz* to result                                    */
/*----------------------------------------------------------------------------*/
Stz* SP_Job::sz_log(Job* pJob)
{
  _precondition_(_MAGIC_,CN_SPID,_FOOTER_);
  
  SP_ReqTuple spReqTuple;
  
  StrLib::vSprint(m_caResult,
                  "{%s %lld %lld %s %s %s %s%s%s}%s"_Stz,
                  
                  aCN[pJob->eClass()],
                  pJob->i_TablePtn(),
                  pJob->iAcksRmg_printOnlyValue(),

                  aSS[pJob->eSeed()],
                  aJL[pJob->eJobList()],
                  aJP[pJob->eJobPhase()],
                  aJS[pJob->eJobState()],
                  
                  pJob->bHasTag_noWait()   ? ""_Stz       : " wait"_Stz,
                  pJob->bWasReceived() ? " recvd"_Stz : ""_Stz,

                  spReqTuple.sz(&pJob->m_xReqTuple),
                  
                  C_VARGS_END);
  
  return (Stz*) &m_caResult[0];
}

/*----------------------------------------------------------------------------*/
/* sprint Header and return Stz* to result                                    */
/*----------------------------------------------------------------------------*/
Stz* SP_Job::sz_recv(Job* pJob)
{
  _precondition_(_MAGIC_,CN_SPID,_FOOTER_);
  
  SP_Header   spHeader;
  SP_ReqTuple spReqTuple;
  SP_Footer   spFooter;
  
  StrLib::vSprint(m_caResult,
                  "Header %s ReqTuple %s ... Footer %s"_Stz,
                  
                  spHeader.sz(pJob),
                  spReqTuple.sz(&pJob->m_xReqTuple),
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
