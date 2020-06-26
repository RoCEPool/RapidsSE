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
  
      Job* (*m_fpPre)(JobClass* aJobClass, ReqTuple* aReqTuple, TagSet* pTagSet);
  
    public:
      Job* pPreJob(ReqTuple* aReqTuple, TagSet* pTagSet);
  
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
