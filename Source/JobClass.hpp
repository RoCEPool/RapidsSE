/*============================================================================*/
/* JobClass.hpp -- a description of a job class                               */
/*============================================================================*/
#pragma once

namespace Jobs
{

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  struct JobClass
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
  
      Job* (*m_fPre)(Interface::Session* pSession);
      void (*m_fPost)(Interface::Session* pSession, Job* pJob);
      Job* (*m_fNew)(Job* pArg);
      void (*m_fDelete)(Interface::Session* pSession, Job* pJob);
      void (*m_fSend)(Job* pJob);
      void (*m_fRecv)(Job* pJob);
      void (*m_fShow)(Job* pJob);
  
      bool m_bEnabled;  // is this job enabled?
  
      Stz* m_aHelp;     // help
      Stz* m_aExample;  // example
  
      bool m_bSeedsSpawned[SS_ENUMS]; // which seeds have been spawned
  
    public:
      Job* pNewJob(Job* pJob);
      void vDeleteJob(Interface::Session* pSession, Job* pJob);
      void vPostJob(Interface::Session* pSession, Job* pJob);
      Job* pPreJob(Interface::Session* pSession);
      Stz* sSeed(eSS eSeed);
      void vSendJob(Job* pJob);
      void vRecvJob(Job* pJob);
      void vShowJob(Job* pJob);
  
      void vJobCat(void);
      static void vJobCatHeader(void);
  
      // - -
  
      static pJOBCLASSFUN pFirstFun(void);
  
      static JobClass* pFind(eCN eClass);
      static JobClass* pFind(Stz* aCommand1, Stz* aCommand2);
      static void vTraceJobs(eCN eClass);
  };
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  JobClass* _pJobClass(i8 iAddr);
  JobClass* _pJobClass(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
