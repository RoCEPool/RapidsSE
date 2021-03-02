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

    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    Jobs(void);
   ~Jobs(void) noexcept(false);
  
    Footer* aFooter(void);
  };
  
  Jobs* _pJobs(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
