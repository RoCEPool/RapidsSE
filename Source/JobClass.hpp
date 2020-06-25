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
  
      eEC  (*m_fpCheck)(void);
      Job* (*m_fpPre)(JobClass* aJobClass, ReqTuple* aReqTuple, TagSet* pTagSet);
      void (*m_fpPost)(JobClass* aJobClass, Job* pJob);
      Job* (*m_fpNew)(JobClass* aJobClass, ReqTuple* aReqTuple);
      void (*m_fpDelete)(Job* pJob);
      void (*m_fpSend)(Job* pJob);
      void (*m_fpRecv)(Job* pJob);
      void (*m_fpShow)(Job* pJob);
  
      bool m_bSeedsSpawned[SS_ENUMS]; // which seeds have been spawned
  
    public:
      eEC  eCheckJob(void);
      void vDeleteJob(Job* pJob);
      Job* pNewJob(ReqTuple* aReqTuple);
      void vPostJob(Job* pJob);
      Job* pPreJob(ReqTuple* aReqTuple, TagSet* pTagSet);
      void vRecvJob(Job* pJob);
      void vSendJob(Job* pJob);
      void vShowJob(Job* pJob);
  
      // - -
  
      static pJOBCLASSFUN pFirstFun(void);
  
      static JobClass* pFind(eCN eClass);
      static JobClass* pFind(Stz* aCommand1, Stz* aCommand2);
  };
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  JobClass* _pJobClass(i8 iAddr);
  JobClass* _pJobClass(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
