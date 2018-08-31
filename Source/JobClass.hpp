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
    char m_aName[32];     // name of the job class
    eCN  m_eClass;        // enum of the class
    eJK  m_eKind;         // job kind
    char m_aComment[128]; // comment about the job
    char m_aCommand1[32]; // first word of prompt command
    char m_aCommand2[32]; // optional second word of prompt command
    char m_aSeedRun[32];  // the _run task seed
    char m_aSeedReq[32];  // the _req task seed
    char m_aSeedRcv[32];  // the _rcv task seed
    char m_aSeedExe[32];  // the _exe task seed
    char m_aSeedEnd[32];  // the _end task seed
    char m_aSeedCan[32];  // the _can task seed
    bool m_bDetached;     // is this job detached?
    bool m_bResources;    // check resources?
    bool m_bBuffer;       // buffer attached?
    eJT  m_eTrace;        // job trace?
  
    Job* (*m_fPre)(Interface::SEitf* pSEitf);
    void (*m_fPost)(Interface::SEitf* pSEitf, Job* pJob);
    Job* (*m_fNew)(Job* pArg);
    void (*m_fDelete)(Interface::SEitf* pSEitf, Job* pJob);
    void (*m_fSend)(Job* pJob);
    void (*m_fRecv)(Job* pJob);
    void (*m_fShow)(Job* pJob);
  
    bool m_bEnabled;  // is this job enabled?

    char m_aHelp[128];     // help
    char m_aExample[32];  // example
  
    bool m_bSeedsSpawned[SS_ENUMS]; // which seeds have been spawned
  
  public:
    Job* pNewJob(Job* pJob);
    void vDeleteJob(Interface::SEitf* pSEitf, Job* pJob);
    void vPostJob(Interface::SEitf* pSEitf, Job* pJob);
    Job* pPreJob(Interface::SEitf* pSEitf);
    Stz* sSeed(eSS eSeed);
    void vSendJob(Job* pJob);
    void vRecvJob(Job* pJob);
    void vShowJob(Job* pJob);
  
    void vJobCat(void);
    static void vJobCatHeader(void);
  };
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  JobClass* _pJobClass(i8 iAddr);
  JobClass* _pJobClass(void* aAddr);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  struct JobClass_s
  {
    public:
      Stz* m_aName;     // name of the job class
      eCN  m_eClass;     // enum of the class
      eJK  m_eKind;      // job kind
      Stz* m_aComment;   // comment about the job
      Stz* m_aCommand1;  // first word of prompt command
      Stz* m_aCommand2;  // optional second word of prompt command
      Stz* m_aSeedRun;   // the _run task seed
      Stz* m_aSeedReq;   // the _req task seed
      Stz* m_aSeedRcv;   // the _rcv task seed
      Stz* m_aSeedExe;   // the _exe task seed
      Stz* m_aSeedEnd;   // the _end task seed
      Stz* m_aSeedCan;   // the _can task seed
      bool m_bDetached;  // is this job detached?
      bool m_bResources; // check resources?
      bool m_bBuffer;    // buffer attached?
      eJT  m_eTrace;     // job trace?
  
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
  
      bool m_bSeedsSpawned[SS_ENUMS]; // which seeds have been spawned
  
    public:
      Job* pNewJob(Job* pJob);
      void vDeleteJob(Interface::SEitf* pSEitf, Job* pJob);
      void vPostJob(Interface::SEitf* pSEitf, Job* pJob);
      Job* pPreJob(Interface::SEitf* pSEitf);
      Stz* sSeed(eSS eSeed);
      void vSendJob(Job* pJob);
      void vRecvJob(Job* pJob);
      void vShowJob(Job* pJob);
  
      void vJobCat(void);
      static void vJobCatHeader(void);
  };
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  JobClass_s* _pJobClass_s(i8 iAddr);
  JobClass_s* _pJobClass_s(void* aAddr);

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
