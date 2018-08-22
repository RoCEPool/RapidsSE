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
JobClass* JobCatalog::pFindJob(Stz* aCommand1, Stz* aCommand2)
{
  for (i8 i_Job = 0; i_Job < m_nClasses; i_Job++) {
    
    if ( ! m_xJobClass[i_Job].m_aCommand1 ||
         ! m_xJobClass[i_Job].m_aCommand1->bSame(aCommand1) )
      continue;
    
    //if (aCommand2->iLength()) {
    if (m_xJobClass[i_Job].m_aCommand2) {
      if ( ! m_xJobClass[i_Job].m_aCommand2->iLength() )
        continue;
      if ( ! m_xJobClass[i_Job].m_aCommand2->bSame(aCommand2) )
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
JobClass* JobCatalog::pFindJob(eCN eClass)
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

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
