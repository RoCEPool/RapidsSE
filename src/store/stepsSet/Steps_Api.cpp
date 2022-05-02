/*============================================================================*/
/* Steps_Api.cpp - Api step group                                             */
/*============================================================================*/

#include "BASE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "Steps.hpp"

#include "eYX.h"

#include "Node.hpp"
#include "Node_I.hpp"
#include "Grid.hpp"
#include "Api.hpp"
#include "Op.hpp"
#include "OpSpec.hpp"
#include "Job.hpp"
#include "JobList.hpp"
#include "LogicUnit.hpp"

#include "Procedure.hpp"

#include "MY.hpp"
#include "TagConstants.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Api(void)
{
  vAddStep("api::forkChild_noWait"_Stz,    &Step::vApiForkChild_noWait);
  vAddStep("api::forkChild_wait"_Stz,      &Step::vApiForkChild_wait);
  vAddStep("api::reapJobs"_Stz,            &Step::vApiReapJobs);
}

/*----------------------------------------------------------------------------*/
/* api::forkChild_noWait -- fork child job (no wait)                          */
/*----------------------------------------------------------------------------*/
void Step::vApiForkChild_noWait(void)
{
  ENTRY();
  
  s48K sCmd;
  m_pDataStack->vPopStr(sCmd,__func__);
   
  s48K sCmdTagStr;
  m_pDataStack->vPopStr(sCmdTagStr,__func__);

  if (AM::TRACE::bVerbose(C_VB_API)) {
    vTrace("SpawnDemon: %s\n"_Stz,sCmd.szStz());
    vTrace("  WithTags: %s\n"_Stz,sCmdTagStr.szStz());
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  TokenVec* pCmdTknVec = TokenVec::pTokeniseSen(sCmd);
    
  TagSet* pCmdTagSet = new TagSet;
  TokenVec* pTagTknVec = TokenVec::pTokeniseSen(sCmdTagStr);
  Api::vPopulateTagSet(pCmdTagSet,pTagTknVec);

  pCmdTagSet->vReplace(C_TAG_NOWAIT,1);

  JobId tChildJobId;
  
  Job* pJob = nullptr;
  eEC eError = Api::eForkJob(tChildJobId,m_pJob,
                           pCmdTknVec,pCmdTagSet,
                           pJob // TO BE REMOVED
                           );
  RASSERT(EC_NONE == eError);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  m_pDataStack->vPushInt(true);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* api::forkChild_wait -- fork child job (wait)                               */
/*----------------------------------------------------------------------------*/
void Step::vApiForkChild_wait(void)
{
  ENTRY();

  // pop command and tag string
  
  s48K sCmd;
  m_pDataStack->vPopStr(sCmd,__func__);

  s48K sCmdTagStr;
  m_pDataStack->vPopStr(sCmdTagStr,__func__);

  if (AM::TRACE::bVerbose(C_VB_API)) {
    vTrace("ExecuteCommand: %s\n"_Stz,sCmd.szStz());
    vTrace("      WithTags: %s\n"_Stz,sCmdTagStr.szStz());
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  
  TokenVec* pCmdTknVec = TokenVec::pTokeniseSen(sCmd);
    
  TagSet* pCmdTagSet = new TagSet;
  TokenVec* pTagTknVec = TokenVec::pTokeniseSen(sCmdTagStr);
  Api::vPopulateTagSet(pCmdTagSet,pTagTknVec);

  pCmdTagSet->vReplace(C_TAG_WAIT,1);
  
  JobId tChildJobId;
  
  Job* pJob = nullptr;
  eEC eError = Api::eForkJob(tChildJobId,m_pJob,
                           pCmdTknVec,pCmdTagSet,
                           pJob // TO BE REMOVED
                           );
  RASSERT(EC_NONE == eError);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  m_pDataStack->vPushInt(true);
  
  EXIT();
}


/*----------------------------------------------------------------------------*/
/* api::reapJobs -- reap retired jobs                                         */
/*----------------------------------------------------------------------------*/
void Step::vApiReapJobs(void)
{
  ENTRY();
  
  i8 iForce = (i8)m_pDataStack->iPopBol(__func__);

  Api::vReapJobsOnce(iForce);

  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
