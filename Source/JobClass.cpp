/*============================================================================*/
/* JobClass.cpp -- a description of a job class                               */
/*============================================================================*/

#include "Jobs.h"

extern SizeHeap  gxaSizeHeap[];

extern JobCatalog gxJobCatalog;

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
JobClass* Jobs::_pJobClass(i8 iAddr)
{
  JobClass* pJobClass = reinterpret_cast<JobClass*>(iAddr);
  
  return pJobClass;
}

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
JobClass* Jobs::_pJobClass(void* aAddr)
{
  JobClass* pJobClass = reinterpret_cast<JobClass*>(aAddr);
  
  return pJobClass;
}


/*----------------------------------------------------------------------------*/
/* post-receive fucntion                                                      */
/*----------------------------------------------------------------------------*/
eEC JobClass::eCheckJob(void)
{
  if (! m_fpCheck)
    return EC_NONE;

  eEC eErrorCode = (*m_fpCheck)();

  return eErrorCode;
}

/*----------------------------------------------------------------------------*/
/* create a new job by running the pre-function                               */
/*----------------------------------------------------------------------------*/
Job* JobClass::pPreJob(ReqTuple* aReqTuple, TagSet* pTagSet)
{
  // create the job
  
  Job* pJob = (*m_fpPre)(this,aReqTuple,pTagSet);
  
  //pJob->m_xReqTuple.m_i_Session = -1;
  
  return pJob;
}

/*----------------------------------------------------------------------------*/
/* find job class given class number                                          */
/*----------------------------------------------------------------------------*/
pJOBCLASSFUN JobClass::pFirstFun(void)
{
  return (pJOBCLASSFUN)pJob_Distributed_class;
}

/*----------------------------------------------------------------------------*/
/* find job class given class number                                          */
/*----------------------------------------------------------------------------*/
JobClass* JobClass::pFind(eCN eClass)
{
  JobClass* pJobClass = nullptr;
  
  pJobClass = gxJobCatalog.aFindClass(eClass);
  if (pJobClass)
    return pJobClass;

  for (pJOBCLASSFUN pfJobClass = (pJOBCLASSFUN)pJob_Distributed_class; pfJobClass; ) {
    
    pJobClass = pfJobClass((pFUN**)&pfJobClass);
    //vTrace("class: %s\n"_Stz,aCN[ pJobClass->m_eClass]);
    
    if (pJobClass->m_eClass == eClass)
      return pJobClass;
  }
  
  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* find job class given command                                               */
/*----------------------------------------------------------------------------*/
JobClass* JobClass::pFind(Stz* aCommand1, Stz* aCommand2)
{
  //vTrace("FindJob: %s %s\n"_Stz, aCommand1, aCommand2);
  
  JobClass* pJobClass = nullptr;
  
  pJobClass = gxJobCatalog.aFindClass(aCommand1, aCommand2);
  if (pJobClass)
    return pJobClass;
  

  
  for (pJOBCLASSFUN pfJobClass = (pJOBCLASSFUN)pJob_Distributed_class; pfJobClass; ) {
    
    pJobClass = pfJobClass((pFUN**)&pfJobClass);
    
    if ( ! pJobClass->m_aCommand1 ||
        ! StrLib::bSame(pJobClass->m_aCommand1->azChar(),aCommand1->azChar()) )
      continue;
    
    //if (aCommand2->iLength()) {
    if (pJobClass->m_aCommand2) {
      if ( ! StrLib::nLength(pJobClass->m_aCommand2->azChar()) )
        continue;
      if ( ! StrLib::bSame(pJobClass->m_aCommand2->azChar(),aCommand2->azChar()) )
        continue;
    }
    
    return nullptr;
    
  }
  
  return nullptr;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
