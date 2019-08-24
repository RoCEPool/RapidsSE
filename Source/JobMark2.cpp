/*============================================================================*/
/* JobMark2.cpp -- a job in the system                                        */
/*============================================================================*/

#include "Jobs.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
JobMark2* Jobs::_pJobMark2(void* aAddr)
{
  JobMark2* pJobMark2 = reinterpret_cast<JobMark2*>(aAddr);
  
  if (pJobMark2 && ! _bIsValidJob(pJobMark2->eClass()) )
    vTrace("_pJobMark2 got class %s\n"_Stz,aCN[pJobMark2->eClass()]);
  
  RASSERT( ! pJobMark2 || _bIsValidJob(pJobMark2->eClass()) );
  
  return pJobMark2;
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
JobMark2::JobMark2(eCN eClassNo, Session* pSession, i8 iRequestId, ReqTuple* aReqTuple)
: Job(eClassNo,pSession,iRequestId,aReqTuple)
{
  _precondition_(_ADDRESS_,_MAGIC_,eClassNo);
  
  m_bCancel = false;
  m_bEarly  = false;
  m_ixTempBuffers.vCheckInit();
  m_eResUsage = RU_NONE;
  
  _postcondition_(_ADDRESS_,_MAGIC_,eClassNo);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
JobMark2::~JobMark2(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  ASSERT(0 == * m_ixTempBuffers);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
