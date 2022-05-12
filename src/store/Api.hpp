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
  
  static Job* pAllocJob(Stz* szJobNm, JobId tParentId, JobId tOriginId, JobId tDistId);

  static eEC  eForkJob(JobId& rtChildJobId,
                       TokenVec* pCmdTknVec, TagSet* pCmdTagSet,
                       JobId tParentId, JobId tOriginId, JobId tDistId,
                       Job*& rpChildJob_tbr);

  static eEC eNewGenericJob(Stz* szJobNm,
                            JobId tParentId, JobId tOriginId, JobId tDistId,
                            Job*& rpChildJob_tbr);

  static void vReapableJob(Job*& rpJob);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
  // internal
  
  static void vCallEngineJob(Stz* szJobNm, Stz* szEntryPt, bool bAddAxioms);
      
  private:


  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
