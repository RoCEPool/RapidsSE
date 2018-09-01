/*============================================================================*/
/* JobCatalog.hpp -- the catalog of job classes                               */
/*============================================================================*/

#include "JobModel.h"
#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

JobCatalog gxJobCatalog;

/*----------------------------------------------------------------------------*/
/* find job class given command                                               */
/*----------------------------------------------------------------------------*/
JobClass_o* JobCatalog::pFindJobClass_o_(Stz* aCommand1, Stz* aCommand2)
{
  return gxJobCatalog.pFindJobClass_o(aCommand1,aCommand2);
}

/*----------------------------------------------------------------------------*/
/* find job class given command                                               */
/*----------------------------------------------------------------------------*/
JobClass_o* JobCatalog::pFindJobClass_o_(eCN eClass)
{
  return gxJobCatalog.pFindJobClass_o(eClass);
}

/*----------------------------------------------------------------------------*/
/* trace the job for the given class                                          */
/*----------------------------------------------------------------------------*/
void JobCatalog::vTraceJobs_(eCN eClass)
{
  gxJobCatalog.vTraceJobs(eClass);
}

/*----------------------------------------------------------------------------*/
/* find job class given command                                               */
/*----------------------------------------------------------------------------*/
JobClass_o* JobCatalog::pFindJobClass_o(Stz* aCommand1, Stz* aCommand2)
{
  //vTrace("FindJob: %s %s\n"_Stz, aCommand1, aCommand2);
  
  for (i8 i_Job = 0; i_Job < m_nClasses; i_Job++) {
    
    if ( ! m_xJobClass[i_Job].m_aCommand1[0] ||
        ! StrLib::bSame(m_xJobClass[i_Job].m_aCommand1,aCommand1->aChar()) )
      continue;
    
    //if (aCommand2->iLength()) {
    if (m_xJobClass[i_Job].m_aCommand2[0]) {
      if ( ! StrLib::iLength(m_xJobClass[i_Job].m_aCommand2) )
        continue;
    if ( ! StrLib::bSame(m_xJobClass[i_Job].m_aCommand2,aCommand2->aChar()) )
        continue;
    }
    
    if (m_xJobClass[i_Job].m_bEnabled)
      return &m_xJobClass[i_Job];
    else
      return nullptr;
  }
  
  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* find job class given command                                               */
/*----------------------------------------------------------------------------*/
JobClass_o* JobCatalog::pFindJobClass_o(eCN eClass)
{
  for (i8 i_Job = 0; i_Job < m_nClasses; i_Job++) {
    
    if (m_xJobClass[i_Job].m_eClass == eClass)
      return &m_xJobClass[i_Job];
  }
  
  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* trace the job for the given class                                          */
/*----------------------------------------------------------------------------*/
void JobCatalog::vTraceJobs(eCN eClass)
{
  for (i8 i_Job = 0; i_Job < m_nClasses; i_Job++)
    if (m_xJobClass[i_Job].m_eClass == eClass)
      m_xJobClass[i_Job].vJobCat();
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

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
JobClass_s* JobCatalog::pFindJobClass_JC2(eCN eClass)
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
JobClass_s* JobCatalog::pFindJobClass_JC2(Stz* aCommand1, Stz* aCommand2)
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
void JobCatalog::vTraceJobs_JC2(eCN eClass)
{
  JobClass_s* pJobClass_s = nullptr;

  for (pJOBCLASSFUN pfJobClass = pFirstFun(); pfJobClass; ) {
    
    pJobClass_s = pfJobClass((pFUN**)&pfJobClass);

    pJobClass_s->vJobCat();
  }
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
