/*============================================================================*/
/* JobMark2.cpp -- a job in the system                                        */
/*============================================================================*/

#include "Jobs.h"
#include "StorageEngine.h"

extern AttachMap* gpAttachMap;
extern Log* gpLog;

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
JobMark2::JobMark2(eCN eClassNo, Session* pSession)
: Job(eClassNo,pSession)
{
  _precondition_(_ADDRESS_,_MAGIC_,eClassNo);
  
  
  _postcondition_(_ADDRESS_,_MAGIC_,eClassNo);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
JobMark2::~JobMark2(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
