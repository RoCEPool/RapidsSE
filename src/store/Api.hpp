/*============================================================================*/
/* Api.hpp - Engine Api                                                       */
/*============================================================================*/
#pragma once

#include "eAC.h"

namespace STORE
{
  class Api
  {
  public:
    
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // external API functions

  static TaskList* pParseCommand(Stz* szCommand, Stz* szCommandTags, Task* pParentTask,
                                 eEC& eErrCode, s2K& sErrStr);
  static void vScheduleTask(Task* pTask);

  static Job* pCreateJob2PC(Job* pParentJob, Stz* sCmd);
  static void vDeleteRootJob(Job*& rpJob);

  static eEC  eForkJobChild(JobId& rtChildJobId,
                           TokenVec* pCmdTknVec, TagSet* pCmdTagSet,
                           JobId tParentId, JobId tOriginId, JobId tDistId,
                           Job* pParentJob,
                           bool bUpdateParent,
                           bool& bIsPipeline);

  static eEC  eForkJob2PC(JobId& rtChildJobId,
                            TokenVec* pCmdTknVec, TagSet* pCmdTagSet,
                            JobId tParentId, JobId tOriginId, JobId tDistId,
                            Job* pParentJob);

  static Job* pNewRootJob(Stz* szJobNm,
                            JobId tParentId, JobId tOriginId, JobId tDistId);

  static void vTriggerChildJob(Job* pParentJob, Job* pChildJob);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
  // internal
  
  static void vCallEngineJob(Stz* szJobNm, Stz* szEntryPt, bool bAddAxioms);
      
  private:


  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
