/*============================================================================*/
/* JobDesc.cpp -- the class which populates the operator catalog               */
/*============================================================================*/

#include "Jobs.h"


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
JobDesc::JobDesc(
                 eCN  eClass,     // enum of the class
                 eJK  eKind,      // job kind
                 eRS  eResponse,  // response kind
                 Stz* aComment,   // comment about the job
                 Stz* aCommand1,  // first word of prompt command
                 Stz* aCommand2,  // optional second word of prompt command
                 
                 Stz* aSeedRun,   // the _run task seed
                 Stz* aSeedReq,   // the _req task seed
                 Stz* aSeedRcv,   // the _rcv task seed
                 Stz* aSeedExe,   // the _exe task seed
                 Stz* aSeedEnd,   // the _end task seed
                 Stz* aSeedCan,   // the _can task seed
                 
                 bool bDetached,  // is this job detached?
                 bool bResources, // check resources?
                 bool bBuffer,    // buffer attached?
                 eDL  eDdlLock,  // ddl lock model
                 ePY  ePriority,  // job priority
                 eJF  eFlags,     // job flags
                 
                 eEC  (*fpCheck)(Session* pSession),
                 Job* (*fpPre)(JobClass* aJobClass, ReqTuple* aReqTuple, i8 iForId, Session* pSession),
                 void (*fpPost)(Job* pJob),
                 Job* (*fpNew)(JobClass* aJobClass),
                 void (*fpDelete)(Job* pJob),
                 void (*fpSend)(Job* pJob),
                 void (*fpRecv)(Job* pJob),
                 void (*fpShow)(Job* pJob),
                 
                 bool bEnabled,  // is this job enabled?
                 
                 Stz* aHelp,     // help
                 Stz* aExample  // example
              )
{
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
