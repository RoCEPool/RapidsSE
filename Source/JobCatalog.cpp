/*============================================================================*/
/* JobCatalog.hpp -- the catalog of job classes                               */
/*============================================================================*/

#include "JobModel.h"
#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

JobCatalog gxJobCatalog;

/*----------------------------------------------------------------------------*/
/* find job class given class number                                          */
/*----------------------------------------------------------------------------*/
pJOBCLASSFUN JobCatalog::pFirstFun(void)
{
  return (pJOBCLASSFUN)pJob__Begun_class;
}

/*----------------------------------------------------------------------------*/
/* find job class given class number                                          */
/*----------------------------------------------------------------------------*/
JobClass_s* JobCatalog::pFindJobClass_s(eCN eClass)
{
  JobClass_s* pJobClass_s = nullptr;
  
  for (pJOBCLASSFUN pfJobClass = (pJOBCLASSFUN)pJob__Begun_class; pfJobClass; ) {
    
    pJobClass_s = pfJobClass((pFUN**)&pfJobClass);

    if (pJobClass_s->m_eClass == eClass)
      return pJobClass_s;
  }

  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* find job class given command                                               */
/*----------------------------------------------------------------------------*/
JobClass_s* JobCatalog::pFindJobClass_s(Stz* aCommand1, Stz* aCommand2)
{
  //vTrace("FindJob: %s %s\n"_Stz, aCommand1, aCommand2);
  
  JobClass_s* pJobClass_s = nullptr;
  
  for (pJOBCLASSFUN pfJobClass = (pJOBCLASSFUN)pJob__Begun_class; pfJobClass; ) {
  
    pJobClass_s = pfJobClass((pFUN**)&pfJobClass);

    if ( ! pJobClass_s->m_aCommand1 ||
        ! StrLib::bSame(pJobClass_s->m_aCommand1->aChar(),aCommand1->aChar()) )
      continue;
    
    //if (aCommand2->iLength()) {
    if (pJobClass_s->m_aCommand2) {
      if ( ! StrLib::iLength(pJobClass_s->m_aCommand2->aChar()) )
        continue;
      if ( ! StrLib::bSame(pJobClass_s->m_aCommand2->aChar(),aCommand2->aChar()) )
        continue;
    }
    
    if (pJobClass_s->m_bEnabled)
      return pJobClass_s;
    else
      return nullptr;
    
  }

  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* trace the job for the given class                                          */
/*----------------------------------------------------------------------------*/
void JobCatalog::vTraceJobs_s(eCN eClass)
{
  JobClass_s* pJobClass_s = nullptr;

  for (pJOBCLASSFUN pfJobClass = pFirstFun(); pfJobClass; ) {
    
    pJobClass_s = pfJobClass((pFUN**)&pfJobClass);

    if (pJobClass_s->m_eClass == eClass)
      pJobClass_s->vJobCat();
  }
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
