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
  

  static void vDeleteRootJob(Job*& rpJob);

  static eEC  eForkJobChild(JobId& rtChildJobId,
                           TokenVec* pCmdTknVec, TagSet* pCmdTagSet,
                           JobId tParentId, JobId tOriginId, JobId tDistId,
                           Job* pParentJob,
                           bool bUpdateParent,
                           bool& bIsPipeline,
                           bool& bIsBackground);

  static eEC  eForkJob2PC(JobId& rtChildJobId,
                            TokenVec* pCmdTknVec, TagSet* pCmdTagSet,
                            JobId tParentId, JobId tOriginId, JobId tDistId,
                            Job* pParentJob);

  static Job* pNewRootJob(Stz* szJobNm,
                            JobId tParentId, JobId tOriginId, JobId tDistId);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
  // internal
  
  static void vCallEngineJob(Stz* szJobNm, Stz* szEntryPt, bool bAddAxioms);
      
  private:


  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
