/*============================================================================*/
/* Jobs.hpp -- Jobs                                                           */
/*============================================================================*/
#pragma once

#include "Folder.hpp"

namespace STORE
{
  class Jobs : public Object
  {
  public:
  
    JobList*  m_pDemonJobs;
    JobList*  m_pLocalJobs;
    JobList*  m_pOriginatingJobs;
    JobList*  m_pParticipatingJobs;
    JobList*  m_pRetiredJobs;

    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    Jobs(void);
   ~Jobs(void) noexcept(false);
  
    JobList* pDemonJobs(void);
    JobList* pLocalJobs(void);
    JobList* pOriginatingJobs(void);
    JobList* pParticipatingJobs(void);
    JobList* pRetiredJobs(void);

    Footer* aFooter(void);
  };
  
  Jobs* _pJobs(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
