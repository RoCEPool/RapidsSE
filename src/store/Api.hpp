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
  
  static eEC  eForkJob(JobId& rtChildJobId,
                       TokenVec* pCmdTknVec, TagSet* pCmdTagSet,
                       JobId tParentId, JobId tOriginId, JobId tDistId,
                       Job*& rpChildJob_tbr);

  static eEC eNewGenericJob(JobId& rtChildJobId,
                            TokenVec* pCmdTknVec, TagSet* pCmdTagSet,
                            JobId tParentId, JobId tOriginId, JobId tDistId,
                            Job*& rpChildJob_tbr);

  static Job* pNewRootJob(Stz* szJobNm,
                          JobId tParentId, JobId tOriginId, JobId tDistId);
  
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
