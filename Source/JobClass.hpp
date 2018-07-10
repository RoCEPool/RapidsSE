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
    s32  m_sName;    // name of the job class
    s256 m_sComment; // comment about the job
    s32  m_sCommand; // the prompt command
    s32  m_sSeed;    // the task seed for this job
    bool (*m_fPre)(TokenVec* pTokenVec);
      void (*m_fPost)(void);

  public:
  
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
