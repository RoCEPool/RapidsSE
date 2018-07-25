/*============================================================================*/
/* JobClass.cpp -- a description of a job class                               */
/*============================================================================*/

#include "JobModel.h"
#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

/*----------------------------------------------------------------------------*/
/* end the job by running the post-function                                   */
/*----------------------------------------------------------------------------*/
void JobClass::vEndJob(Interface::SEitf* pSEitf, Prompt::SePrompt* pPrompt, Job* pJob)
{
  (*m_fPost)(pPrompt, pJob);
}

/*----------------------------------------------------------------------------*/
/* create a new job by running the pre-function                                */
/*----------------------------------------------------------------------------*/
Job* JobClass::pNewJob(Interface::SEitf* pSEitf)
{
  Job* pJob = (*m_fPre)(pSEitf);
  
  return pJob;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
