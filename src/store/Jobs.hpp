/*============================================================================*/
/* Jobs.hpp -- Jobs                                                           */
/*============================================================================*/
#pragma once

#include "Folder.hpp"

namespace STORE
{
  class Jobs : public Object
  {
  private:
    JobList*  m_pJobsList;          // list of all jobs
  
    JobList*  m_pDemonJobs;         // jobs belonging to demons
    JobList*  m_pLocalJobs;         // jobs per local pipelines
    JobList*  m_pOriginatingJobs;   // jobs per pipelinee on origin
    JobList*  m_pParticipatingJobs; // jobs per pipelines on participating nodes
    JobList*  m_pExitedJobs;        // jobs of the above that have exited
                                    //   which are to be finally deleted

    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    Jobs(void);
   ~Jobs(void) noexcept(false);

    bool bAnyExitedJobs(void);
    void vCreateJobLists(void);
  
    JobList* pJobsList(void);

    JobList* pDemonJobs(void);
    JobList* pLocalJobs(void);
    JobList* pOriginatingJobs(void);
    JobList* pParticipatingJobs(void);

    Job* pReapableJob(void);
    bool bReapJob(void);

    JobList* pExitedJobs(void);

    Footer* aFooter(void);
  };
  
  Jobs* _pJobs(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
