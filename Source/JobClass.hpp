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
    eJK  m_eKind;     // job kind
    Stz* m_aComment;  // comment about the job
    Stz* m_aCommand1; // first word of prompt command
    Stz* m_aCommand2; // optional second word of prompt command
    Stz* m_aSeedRun;  // the _run task seed
    Stz* m_aSeedReq;  // the _req task seed
    Stz* m_aSeedRcv;  // the _rcv task seed
    Stz* m_aSeedExe;  // the _exe task seed
    Stz* m_aSeedEnd;  // the _end task seed
    Stz* m_aSeedCan;  // the _can task seed
    bool m_bDetached; // is this job detached?
    bool m_bResources;// check resources?
    bool m_bBuffer;   // buffer attached?
    eJT  m_eTrace;    // job trace?
  
    Job* (*m_fPre)(Interface::SEitf* pSEitf);
    void (*m_fPost)(Interface::SEitf* pSEitf, Job* pJob);
    Job* (*m_fNew)(Job* pArg);
    void (*m_fDelete)(Interface::SEitf* pSEitf, Job* pJob);
    void (*m_fSend)(Job* pJob);
    void (*m_fRecv)(Job* pJob);
    void (*m_fShow)(Job* pJob);
  
    bool m_bEnabled;  // is this job enabled?

    Stz* m_aHelp;     // help
    Stz* m_aExample;  // example
  
    bool m_bSeedsRun[SS_ENUMS]; // which seeds where runs
  
  public:
    Job* pNewJob(Job* pJob);
    void vDeleteJob(Interface::SEitf* pSEitf, Job* pJob);
    void vPostJob(Interface::SEitf* pSEitf, Job* pJob);
    Job* pPreJob(Interface::SEitf* pSEitf);
    void vSendJob(Job* pJob);
    void vRecvJob(Job* pJob);
    void vShowJob(Job* pJob);
  
    void vJobCat(void);
    static void vJobCatHeader(void);
  };
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  JobClass* _pJobClass(i8 iAddr);
  JobClass* _pJobClass(void* aAddr);

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
