/*============================================================================*/
/* Steps_Job.cpp - job step group                                             */
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
  vAddStep("job(Id).cascade@"_Stz,       &Step::vJobCascadeFetch);
  vAddStep("job(Id).cascade!"_Stz,       &Step::vJobCascadeStore);
    
  vAddStep("job(Id).folderPath@"_Stz,    &Step::vJobFolderPathFetch);
  vAddStep("job(Id).folderPath!"_Stz,    &Step::vJobFolderPathStore);
  
  vAddStep("job(Id).hasCascade@"_Stz,    &Step::vJobHasCascadeFetch);
  
  vAddStep("job(Id).id@"_Stz,            &Step::vJobIdFetch_other);
  
  vAddStep("job(Id).jobList@"_Stz,       &Step::vJobJobListFetch);
    
  vAddStep("job(Id).lastJobId@"_Stz,     &Step::vJobLastJobIdFetch);
  vAddStep("job(Id).lastJobId!"_Stz,     &Step::vJobLastJobIdStore);
  
  vAddStep("job(Id).masterJob"_Stz,      &Step::vJobMasterJob);
  
  vAddStep("job(Id).name@"_Stz,          &Step::vJobNameFetch);
  
  vAddStep("job(Id).phase@"_Stz,         &Step::vJobPhaseFetch_other);
  vAddStep("job(Id).pipeToOp0"_Stz,      &Step::vJobPipeToOp0);

  vAddStep("job(Id).reapable@"_Stz,      &Step::vJobReapableFetch_other);
  vAddStep("job(Id).reapable!"_Stz,      &Step::vJobReapableStore_other);
  
  vAddStep("job(Id).show"_Stz,           &Step::vJobShow);
  vAddStep("job(Id).systemCloser?"_Stz,  &Step::vJobSystemCloserFetch);
  vAddStep("job(Id).systemCloser!"_Stz,  &Step::vJobSystemCloserStore);

  vAddStep("job(Id).task@"_Stz,          &Step::vJobTaskFetch_other);
}


/*----------------------------------------------------------------------------*/
/* job.cascade@ -- fetch sCascade from job                                    */
/*----------------------------------------------------------------------------*/
void Step::vJobCascadeFetch(void)
{
  ENTRY();
  
  Job* pJob = _pJob(m_pDataStack->pPopPtr(__func__));

  sSTMT sStmt;
  pJob->vStmtListPop(sStmt);
  m_pDataStack->vPushStz(sStmt);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.cascade! -- store sCascade into job                                    */
/*----------------------------------------------------------------------------*/
void Step::vJobCascadeStore(void)
{
  ENTRY();
  
  Job* pJob = _pJob(m_pDataStack->pPopPtr(__func__));

  s16KB sStmt;
  m_pDataStack->vPopStr(sStmt,__func__);
  
  pJob->vStmtListAppend(sStmt);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.folderPath@ -- fetch folder path from job                              */
/*----------------------------------------------------------------------------*/
void Step::vJobFolderPathFetch(void)
{
  ENTRY();
  
  Job* pJob = _pJob(m_pDataStack->pPopPtr(__func__));

  Stz* szFolderPath = pJob->szFolderPath();
  
  m_pDataStack->vPushStz(szFolderPath);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.folderPath! -- store folder path into job                              */
/*----------------------------------------------------------------------------*/
void Step::vJobFolderPathStore(void)
{
  ENTRY();
  
  Job* pJob = _pJob(m_pDataStack->pPopPtr(__func__));

  s256 sFolderPath;
  m_pDataStack->vPopStr(sFolderPath,__func__);

  pJob->vFolderPath(sFolderPath);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.hasCascade@ -- fetch bHasCascade from session                          */
/*----------------------------------------------------------------------------*/
void Step::vJobHasCascadeFetch(void)
{
  ENTRY();
  
  Job* pJob = _pJob(m_pDataStack->pPopPtr(__func__));

  m_pDataStack->vPushInt(pJob->bHasStmtList());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.id@ -- fetch the job id of the job                                     */
/*----------------------------------------------------------------------------*/
void Step::vJobIdFetch_other(void)
{
  ENTRY();
  
  Job* pJob = _pJob(m_pDataStack->pPopPtr(__func__));
  
  m_pDataStack->vPushInt(pJob->tJobId().iAsI8());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.jobList@ -- fetch job list                                             */
/*----------------------------------------------------------------------------*/
void Step::vJobJobListFetch(void)
{
  ENTRY();
  
  Job* pJob = _pJob(m_pDataStack->pPopPtr(__func__));
  
  m_pDataStack->vPushInt(pJob->eJobList());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.lastJobId@ -- fetch last job id from job                               */
/*----------------------------------------------------------------------------*/
void Step::vJobLastJobIdFetch(void)
{
  ENTRY();
  
  Job* pJob = _pJob(m_pDataStack->pPopPtr(__func__));

  JobId tLastJobId = pJob->tLastJobId();
  
  m_pDataStack->vPushInt(tLastJobId.iAsI8());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.lastJobId! -- store last job id into job                               */
/*----------------------------------------------------------------------------*/
void Step::vJobLastJobIdStore(void)
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
/* job.masterJob -- get the master job of this (participant/local) job        */
/*----------------------------------------------------------------------------*/
void Step::vJobMasterJob(void)
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
/* job.name@ -- get name of job                                               */
/*----------------------------------------------------------------------------*/
void Step::vJobNameFetch(void)
{
  ENTRY();
  
  Job* pJob = _pJob(m_pDataStack->pPopPtr(__func__));
  
  s32 szName = pJob->szJobNm();
  m_pDataStack->vPushStr(szName);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.pipeToOp0 -- append operand onto op0 of given job                      */
/*----------------------------------------------------------------------------*/
void Step::vJobPipeToOp0(void)
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
/* job.reapable@ -- fetch the job reapable flag                               */
/*----------------------------------------------------------------------------*/
void Step::vJobReapableFetch_other(void)
{
  ENTRY();
  
  Job* pJob = _pJob(m_pDataStack->pPopPtr(__func__));

  m_pDataStack->vPushInt(pJob->bReapable());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.reapable! -- store the job reapable flag                               */
/*----------------------------------------------------------------------------*/
void Step::vJobReapableStore_other(void)
{
  ENTRY();
  
  Job* pJob = _pJob(m_pDataStack->pPopPtr(__func__));
  
  i8 iJobReapable = m_pDataStack->iPopAsInt(__func__);
  
  pJob->vReapable(iJobReapable);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.show -- show the job                                                   */
/*----------------------------------------------------------------------------*/
void Step::vJobShow(void)
{
  ENTRY();
  
  RASSERT(0);
  
  Job* pJob = _pJob(m_pDataStack->pPopPtr(__func__));
  UNUSED(pJob);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.systemCloser? -- fetch system closer from job                          */
/*----------------------------------------------------------------------------*/
void Step::vJobSystemCloserFetch(void)
{
  ENTRY();
  
  Job* pJob = _pJob(m_pDataStack->pPopPtr(__func__));

  bool bSystemCloser = pJob->bSystemCloser();
  
  m_pDataStack->vPushInt(bSystemCloser);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.systemCloser! -- store system closer into job                          */
/*----------------------------------------------------------------------------*/
void Step::vJobSystemCloserStore(void)
{
  ENTRY();
  
  Job* pJob = _pJob(m_pDataStack->pPopPtr(__func__));

  i8 iSystemCloser = m_pDataStack->iPopInt(__func__);
    
  pJob->vSystemCloser(iSystemCloser);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
