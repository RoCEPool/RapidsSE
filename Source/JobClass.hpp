/*============================================================================*/
/* JobClass.hpp -- a description of a job class                               */
/*============================================================================*/
#pragma once

namespace JobsModel
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class JobClass
  {
  public:
    Stz* m_aName;     // name of the job class
    eCN  m_eClass;    // enum of the class
    Stz* m_aComment;  // comment about the job
    Stz* m_aCommand1; // first word of prompt command
    Stz* m_aCommand2; // optional second word of prompt command
    Stz* m_aSeed;     // the task seed for this job
    bool m_bDetached; // is this job detached?

    Job* (*m_fPre)(Interface::SEitf* pSEitf);
    void (*m_fPost)(Interface::SEitf* pSEitf, Job* pJob);
  
    bool m_bEnabled;  // is this job enabled?

  public:
    void vEndJob(Interface::SEitf* pSEitf, Job* pJob);
    Job* pNewJob(Interface::SEitf* pSEitf);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
