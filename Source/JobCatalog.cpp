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
    
    if ( ! m_xJobClass[i_Job].m_aCommand1->bSame(aCommand1) )
      continue;
    
    if (aCommand2->iLength()) {
      if ( ! m_xJobClass[i_Job].m_aCommand2->iLength() )
        continue;
      if ( ! m_xJobClass[i_Job].m_aCommand2->bSame(aCommand2) )
        continue;
    }
    
    return &m_xJobClass[i_Job];
  }
  
  return nullptr;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
