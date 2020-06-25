/*============================================================================*/
/* JobDesc.hpp -- the class which populates the job catalog                   */
/*============================================================================*/
#pragma once

namespace Jobs
{
  class JobDesc
  {
  public:
    JobDesc(
            eCN  eClass,     // enum of the class
            eJK  eKind,      // job kind
            Stz* aComment,   // comment about the job
            Stz* aCommand1,  // first word of prompt command
            Stz* aCommand2,  // optional second word of prompt command
            
            bool bDetached,  // is this job detached?
            bool bResources, // check resources?
            bool bBuffer,    // buffer attached?
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
           );
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
