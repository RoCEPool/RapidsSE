/*============================================================================*/
/* Steps_job(Id).cpp - job step group                                         */
/*============================================================================*/

#include "BASE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "Steps.hpp"

#include "eYX.h"

#include "Grid.hpp"
#include "Api.hpp"
#include "Stream.hpp"
#include "Op.hpp"
#include "Stage.hpp"
#include "Pipeline.hpp"
#include "Vertex.hpp"
#include "Job.hpp"
#include "Node_I.hpp"
#include "LogicUnit.hpp"

#include "JobList.hpp"

#include "TagConstants.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_JobId(void)
{
  vAddStep("job(Id).cascade@"_Stz,       &Step::vJobIdCascadeFetch);
  vAddStep("job(Id).cascade!"_Stz,       &Step::vJobIdCascadeStore);
    
  vAddStep("job(Id).folderPath@"_Stz,    &Step::vJobIdFolderPathFetch);
  vAddStep("job(Id).folderPath!"_Stz,    &Step::vJobIdFolderPathStore);
  
  vAddStep("job(Id).hasCascade@"_Stz,    &Step::vJobIdHasCascadeFetch);
  
  vAddStep("job(Id).id@"_Stz,            &Step::vJobIdIdFetch);
  
  vAddStep("job(Id).jobList@"_Stz,       &Step::vJobIdJobListFetch);
    
  vAddStep("job(Id).lastJobId@"_Stz,     &Step::vJobIdLastJobIdFetch);
  vAddStep("job(Id).lastJobId!"_Stz,     &Step::vJobIdLastJobIdStore);
  
  vAddStep("job(Id).masterJob"_Stz,      &Step::vJobIdMasterJob);
  
  vAddStep("job(Id).name@"_Stz,          &Step::vJobIdNameFetch);
  
  vAddStep("job(Id).phase@"_Stz,         &Step::vJobIdPhaseFetch);
  vAddStep("job(Id).pipeToOp0"_Stz,      &Step::vJobIdPipeToOp0);

  vAddStep("job(Id).reapable@"_Stz,      &Step::vJobIdReapableFetch);
  vAddStep("job(Id).reapable!"_Stz,      &Step::vJobIdReapableStore);
  
  vAddStep("job(Id).show"_Stz,           &Step::vJobIdShow);
  vAddStep("job(Id).systemCloser?"_Stz,  &Step::vJobIdSystemCloserFetch);
  vAddStep("job(Id).systemCloser!"_Stz,  &Step::vJobIdSystemCloserStore);

  vAddStep("job(Id).task@"_Stz,          &Step::vJobIdTaskFetch);
}


/*----------------------------------------------------------------------------*/
/* job(Id).cascade@ -- fetch sCascade from job                                */
/*----------------------------------------------------------------------------*/
void Step::vJobIdCascadeFetch(void)
{
  ENTRY();
  
  Job* pJob = _pJob(m_pDataStack->pPopPtr(__func__));

  sSTMT sStmt;
  pJob->vStmtListPop(sStmt);
  m_pDataStack->vPushStz(sStmt);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job(Id).cascade! -- store sCascade into job                                */
/*----------------------------------------------------------------------------*/
void Step::vJobIdCascadeStore(void)
{
  ENTRY();
  
  Job* pJob = _pJob(m_pDataStack->pPopPtr(__func__));

  s16KB sStmt;
  m_pDataStack->vPopStr(sStmt,__func__);
  
  pJob->vStmtListAppend(sStmt);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job(Id).folderPath@ -- fetch folder path from job                          */
/*----------------------------------------------------------------------------*/
void Step::vJobIdFolderPathFetch(void)
{
  ENTRY();
  
  Job* pJob = _pJob(m_pDataStack->pPopPtr(__func__));

  Stz* szFolderPath = pJob->szFolderPath();
  
  m_pDataStack->vPushStz(szFolderPath);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job(Id).folderPath! -- store folder path into job                          */
/*----------------------------------------------------------------------------*/
void Step::vJobIdFolderPathStore(void)
{
  ENTRY();
  
  Job* pJob = _pJob(m_pDataStack->pPopPtr(__func__));

  s256 sFolderPath;
  m_pDataStack->vPopStr(sFolderPath,__func__);

  pJob->vFolderPath(sFolderPath);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job(Id).hasCascade@ -- fetch bHasCascade from session                      */
/*----------------------------------------------------------------------------*/
void Step::vJobIdHasCascadeFetch(void)
{
  ENTRY();
  
  Job* pJob = _pJob(m_pDataStack->pPopPtr(__func__));

  m_pDataStack->vPushInt(pJob->bHasStmtList());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job(Id).id@ -- fetch the job id of the job                                 */
/*----------------------------------------------------------------------------*/
void Step::vJobIdIdFetch(void)
{
  ENTRY();
  
  Job* pJob = _pJob(m_pDataStack->pPopPtr(__func__));
  
  m_pDataStack->vPushInt(pJob->tJobId().iAsI8());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job(Id).jobList@ -- fetch job list                                         */
/*----------------------------------------------------------------------------*/
void Step::vJobIdJobListFetch(void)
{
  ENTRY();
  
  Job* pJob = _pJob(m_pDataStack->pPopPtr(__func__));
  
  m_pDataStack->vPushInt(pJob->eJobList());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job(Id).lastJobId@ -- fetch last job id from job                           */
/*----------------------------------------------------------------------------*/
void Step::vJobIdLastJobIdFetch(void)
{
  ENTRY();
  
  Job* pJob = _pJob(m_pDataStack->pPopPtr(__func__));

  JobId tLastJobId = pJob->tLastJobId();
  
  m_pDataStack->vPushInt(tLastJobId.iAsI8());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job(Id).lastJobId! -- store last job id into job                           */
/*----------------------------------------------------------------------------*/
void Step::vJobIdLastJobIdStore(void)
{
  ENTRY();
  
  Job* pJob = _pJob(m_pDataStack->pPopPtr(__func__));

  i8 iLastJobId = m_pDataStack->iPopInt(__func__);
  
  JobId tLastJobId;
  
  tLastJobId = iLastJobId;
  
  pJob->vLastJobId(tLastJobId);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job(Id).masterJob -- get the master job of this (participant/local) job    */
/*----------------------------------------------------------------------------*/
void Step::vJobIdMasterJob(void)
{
  ENTRY();
  
  Job* pJob = _pJob(m_pDataStack->pPopPtr(__func__));

  Job* pOriginJob = pJob->pOriginatingParentJob();
  if (pOriginJob) {
    m_pDataStack->vPushPtr(pOriginJob);
  }
  else {
    Job* pLocalJob = pJob;
    m_pDataStack->vPushPtr(pLocalJob);
  }
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job(Id).name@ -- get name of job                                           */
/*----------------------------------------------------------------------------*/
void Step::vJobIdNameFetch(void)
{
  ENTRY();
  
  Job* pJob = _pJob(m_pDataStack->pPopPtr(__func__));
  
  s32 szName = pJob->szJobNm();
  m_pDataStack->vPushStr(szName);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* jobI(Id).phase@ -- fetch the job phase                                     */
/*----------------------------------------------------------------------------*/
void Step::vJobIdPhaseFetch(void)
{
  ENTRY();
  
  Job* pJob = _pJob(m_pDataStack->pPopPtr(__func__));

  m_pDataStack->vPushInt(pJob->eJobPhase());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job(Id).pipeToOp0 -- append operand onto op0 of given job                  */
/*----------------------------------------------------------------------------*/
void Step::vJobIdPipeToOp0(void)
{
  ENTRY();

  Job* pJob = _pJob(m_pDataStack->pPopPtr(__func__));
  
  Operand* pOperand = _pOperand(m_pDataStack->pPopPtr(__func__));
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  Pipeline* pPipeline = pJob->pPipeline();
  Op*       pOp0      = pPipeline->pOp0();

  pOp0->vAppendLeft(pOperand,this);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job(Id).reapable@ -- fetch the job reapable flag                           */
/*----------------------------------------------------------------------------*/
void Step::vJobIdReapableFetch(void)
{
  ENTRY();
  
  Job* pJob = _pJob(m_pDataStack->pPopPtr(__func__));

  m_pDataStack->vPushInt(pJob->bReapable());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job(Id).reapable! -- store the job reapable flag                           */
/*----------------------------------------------------------------------------*/
void Step::vJobIdReapableStore(void)
{
  ENTRY();
  
  Job* pJob = _pJob(m_pDataStack->pPopPtr(__func__));
  
  i8 iJobReapable = m_pDataStack->iPopAsInt(__func__);
  
  pJob->vReapable(iJobReapable);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job(Id).show -- show the job                                               */
/*----------------------------------------------------------------------------*/
void Step::vJobIdShow(void)
{
  ENTRY();
  
  RASSERT(0);
  
  Job* pJob = _pJob(m_pDataStack->pPopPtr(__func__));
  UNUSED(pJob);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job(Id).systemCloser? -- fetch system closer from job                      */
/*----------------------------------------------------------------------------*/
void Step::vJobIdSystemCloserFetch(void)
{
  ENTRY();
  
  Job* pJob = _pJob(m_pDataStack->pPopPtr(__func__));

  bool bSystemCloser = pJob->bSystemCloser();
  
  m_pDataStack->vPushInt(bSystemCloser);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job(Id).systemCloser! -- store system closer into job                      */
/*----------------------------------------------------------------------------*/
void Step::vJobIdSystemCloserStore(void)
{
  ENTRY();
  
  Job* pJob = _pJob(m_pDataStack->pPopPtr(__func__));

  i8 iSystemCloser = m_pDataStack->iPopInt(__func__);
    
  pJob->vSystemCloser(iSystemCloser);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job(Id).task@ -- fetch the task executing the job                              */
/*----------------------------------------------------------------------------*/
void Step::vJobIdTaskFetch(void)
{
  ENTRY();
  
  Job* pJob = _pJob(m_pDataStack->pPopPtr(__func__));

  m_pDataStack->vPushPtr(pJob->pTask());
  
  EXIT();
}



/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
