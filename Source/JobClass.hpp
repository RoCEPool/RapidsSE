/*============================================================================*/
/* JobClass.hpp -- a description of a job class                               */
/*============================================================================*/
#pragma once

namespace Jobs
{
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
      eDL  m_eDdlLock;   // ddl lock model
      ePY  m_ePriority;  // job priority
      eJF  m_eFlags;     // job flags
  
      eEC  (*m_fpCheck)(void);
      Job* (*m_fpPre)(JobClass* aJobClass, ReqTuple* aReqTuple, TagSet* pTagSet);
      void (*m_fpPost)(JobClass* aJobClass, Job* pJob);
      Job* (*m_fpNew)(JobClass* aJobClass, ReqTuple* aReqTuple);
      void (*m_fpDelete)(Job* pJob);
      void (*m_fpSend)(Job* pJob);
      void (*m_fpRecv)(Job* pJob);
      void (*m_fpShow)(Job* pJob);
  
      bool m_bEnabled;  // is this job enabled?
  
      Stz* m_aHelp;     // help
      Stz* m_aExample;  // example
  
      bool m_bSeedsSpawned[SS_ENUMS]; // which seeds have been spawned
  
    public:
      eEC  eCheckJob(void);
      void vCmds(void);
      void vDeleteJob(Job* pJob);
      void vDropDdlLock(void);
      void vGrabDdlLock(void);
      Job* pNewJob(ReqTuple* aReqTuple);
      void vPostJob(Job* pJob);
      Job* pPreJob(ReqTuple* aReqTuple, TagSet* pTagSet);
      void vRecvJob(Job* pJob);
      Stz* sSeed(eSS eSeed);
      void vSendJob(Job* pJob);
      void vShowJob(Job* pJob);
  
      // - -
  
      static pJOBCLASSFUN pFirstFun(void);
  
      static void vCmds(eCN eClass);
      static JobClass* pFind(eCN eClass);
      static JobClass* pFind(Stz* aCommand1, Stz* aCommand2);
      static void vVerifySeeds(void);
  };
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  JobClass* _pJobClass(i8 iAddr);
  JobClass* _pJobClass(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
