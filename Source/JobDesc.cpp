/*============================================================================*/
/* JobDesc.cpp -- the class which populates the operator catalog               */
/*============================================================================*/

#include "Jobs.h"

extern JobCatalog gxJobCatalog;

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
JobDesc::JobDesc(
                 eCN  eClass,     // enum of the class
                 eJK  eKind,      // job kind
                 Stz* aComment,   // comment about the job
                 Stz* aCommand1,  // first word of prompt command
                 Stz* aCommand2,  // optional second word of prompt command
                 
                 eDL  eDdlLock,  // ddl lock model
                 ePY  ePriority,  // job priority
                 eJF  eFlags,     // job flags
                 
                 eEC  (*fpCheck)(void),
                 Job* (*fpPre)(JobClass* aJobClass, ReqTuple* aReqTuple, TagSet* pTagSet),
                 void (*fpPost)(JobClass* aJobClass, Job* pJob),
                 Job* (*fpNew)(JobClass* aJobClass, ReqTuple* aReqTuple),
                 void (*fpDelete)(Job* pJob),
                 void (*fpSend)(Job* pJob),
                 void (*fpRecv)(Job* pJob),
                 void (*fpShow)(Job* pJob),
                 
                 bool bEnabled,  // is this job enabled?
                 
                 Stz* aHelp,     // help
                 Stz* aExample  // example
              )
{
  JobClass* pJobClass = & gxJobCatalog.m_xJobClass[gxJobCatalog.m_nJobClasses++];

  pJobClass->m_eClass     = eClass;
  pJobClass->m_eKind      = eKind;
  pJobClass->m_aComment   = aComment;
  pJobClass->m_aCommand1  = aCommand1;
  pJobClass->m_aCommand2  = aCommand2;
  
  pJobClass->m_eDdlLock   = eDdlLock;
  pJobClass->m_ePriority  = ePriority;
  pJobClass->m_eFlags     = eFlags;
  
  pJobClass->m_fpCheck    = fpCheck;
  pJobClass->m_fpPre      = fpPre;
  pJobClass->m_fpPost     = fpPost;
  pJobClass->m_fpNew      = fpNew;
  pJobClass->m_fpDelete   = fpDelete;
  pJobClass->m_fpSend     = fpSend;
  pJobClass->m_fpRecv     = fpRecv;
  pJobClass->m_fpShow     = fpShow;
  
  pJobClass->m_bEnabled   = bEnabled;
  
  pJobClass->m_aHelp      = aHelp;
  pJobClass->m_aExample   = aExample;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
