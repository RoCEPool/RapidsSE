/*============================================================================*/
/* JobClass.hpp -- a description of a job class                               */
/*============================================================================*/
#pragma once

namespace Jobs
{
  enum eDL // ddl lock
  {
  /* 0 */ DL_NONE=0,
  /* 1 */ DL_GRAB_READ,
  /* 2 */ DL_GRAB_WRITE,
  /* 3 */ DL_ENUMS
  };
  
  static Stz* aDL[]
  {
  /* 0 */ "none"_Stz,
  /* 1 */ "grab_read"_Stz,
  /* 2 */ "grab_write"_Stz,
  /* 3 */ "enums"_Stz
  };

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  struct JobClass
  {
    public:
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
      eDL  m_eDdlLock;   // ddl lock model
      ePY  m_ePriority;  // job priority
      eJF  m_eFlags;     // job flags
  
      eEC  (*m_fpCheck)(Interface::Session* pSession);
      Job* (*m_fpPre)(Interface::Session* pSession);
      void (*m_fpPost)(Interface::Session* pSession, Job* pJob);
      Job* (*m_fpNew)(Job* pArg);
      void (*m_fpDelete)(Interface::Session* pSession, Job* pJob);
      void (*m_fpSend)(Job* pJob);
      void (*m_fpRecv)(Job* pJob);
      void (*m_fpShow)(Job* pJob);
  
      bool m_bEnabled;  // is this job enabled?
  
      Stz* m_aHelp;     // help
      Stz* m_aExample;  // example
  
      bool m_bSeedsSpawned[SS_ENUMS]; // which seeds have been spawned
  
    public:
      eEC  eCheckJob(Interface::Session* pSession);
      void vDeleteJob(Interface::Session* pSession, Job* pJob);
      void vDropDdlLock(void);
      void vGrabDdlLock(void);
      Job* pNewJob(Job* pJob);
      void vPostJob(Interface::Session* pSession, Job* pJob);
      Job* pPreJob(Interface::Session* pSession);
      void vRecvJob(Job* pJob);
      Stz* sSeed(eSS eSeed);
      void vSendJob(Job* pJob);
      void vShowJob(Job* pJob);
  
      void vJobCat(void);
      static void vJobCatHeader(void);
  
      // - -
  
      static pJOBCLASSFUN pFirstFun(void);
  
      static JobClass* pFind(eCN eClass);
      static JobClass* pFind(Stz* aCommand1, Stz* aCommand2);
      static void vJobCat(eCN eClass);
      static void vVerifySeeds(void);
  };
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  JobClass* _pJobClass(i8 iAddr);
  JobClass* _pJobClass(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
