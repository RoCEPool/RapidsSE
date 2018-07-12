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
    Stz* m_aComment;  // comment about the job
    Stz* m_aCommand1; // first word of prompt command
    Stz* m_aCommand2; // optional second word of prompt command
    Stz* m_aSeed;     // the task seed for this job
  
    bool (*m_fPre)(TokenVec* pTokenVec);
    void (*m_fPost)(void);
  
    bool m_bEnabled;  // is this job enabled?

  public:
  
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
