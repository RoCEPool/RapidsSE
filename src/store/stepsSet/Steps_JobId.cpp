/*============================================================================*/
/* Steps_jobId::cpp - job step group                                         */
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
  vAddStep("jobId::folderPath@"_Stz,    &Step::vJobIdFolderPathFetch);
  vAddStep("jobId::folderPath!"_Stz,    &Step::vJobIdFolderPathStore);
    
  vAddStep("jobId::lastJobId@"_Stz,     &Step::vJobIdLastJobIdFetch);
  vAddStep("jobId::lastJobId!"_Stz,     &Step::vJobIdLastJobIdStore);

  vAddStep("jobId::pipeToOp0"_Stz,      &Step::vJobIdPipeToOp0);

  vAddStep("jobId::systemCloser?"_Stz,  &Step::vJobIdSystemCloserFetch);
  vAddStep("jobId::systemCloser!"_Stz,  &Step::vJobIdSystemCloserStore);

  vAddStep("jobId::isUnknown"_Stz,      &Step::vJobIdIsUnknown);
}

/*----------------------------------------------------------------------------*/
/* jobId::folderPath@ -- fetch folder path from job                          */
/*----------------------------------------------------------------------------*/
void Step::vJobIdFolderPathFetch(void)
{
  ENTRY();

  JobId tJobId = m_pDataStack->iPopInt(__func__);
  Job* pJob = AM::JLIST::pFindJob(tJobId);

  Stz* szFolderPath = pJob->szFolderPath();
  
  m_pDataStack->vPushStz(szFolderPath);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* jobId::folderPath! -- store folder path into job                          */
/*----------------------------------------------------------------------------*/
void Step::vJobIdFolderPathStore(void)
{
  ENTRY();

  JobId tJobId = m_pDataStack->iPopInt(__func__);
  Job* pJob = AM::JLIST::pFindJob(tJobId);

  s256 sFolderPath;
  m_pDataStack->vPopStr(sFolderPath,__func__);

  pJob->vFolderPath(sFolderPath);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* jobId::lastJobId@ -- fetch last job id from job                           */
/*----------------------------------------------------------------------------*/
void Step::vJobIdLastJobIdFetch(void)
{
  ENTRY();

  JobId tJobId = m_pDataStack->iPopInt(__func__);
  Job* pJob = AM::JLIST::pFindJob(tJobId);

  JobId tLastJobId = pJob->tLastJobId();
  
  m_pDataStack->vPushInt(tLastJobId.iAsI8());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* jobId::lastJobId! -- store last job id into job                           */
/*----------------------------------------------------------------------------*/
void Step::vJobIdLastJobIdStore(void)
{
  ENTRY();

  JobId tJobId = m_pDataStack->iPopInt(__func__);
  Job* pJob = AM::JLIST::pFindJob(tJobId);

  i8 iLastJobId = m_pDataStack->iPopInt(__func__);
  
  JobId tLastJobId;
  
  tLastJobId = iLastJobId;
  
  pJob->vLastJobId(tLastJobId);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* jobId::pipeToOp0 -- append operand onto op0 of given job                  */
/*----------------------------------------------------------------------------*/
void Step::vJobIdPipeToOp0(void)
{
  ENTRY();

  JobId tJobId = m_pDataStack->iPopInt(__func__);
  Job* pJob = AM::JLIST::pFindJob(tJobId);

  Operand* pOperand = _pOperand(m_pDataStack->pPopPtr(__func__));

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  Pipeline* pPipeline = pJob->pPipeline();
  Op*       pOp0      = pPipeline->pOp0();

  pOp0->vAppendLeft(pOperand,this);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* jobId::systemCloser? -- fetch system closer from job                      */
/*----------------------------------------------------------------------------*/
void Step::vJobIdSystemCloserFetch(void)
{
  ENTRY();

  JobId tJobId = m_pDataStack->iPopInt(__func__);
  Job* pJob = AM::JLIST::pFindJob(tJobId);

  bool bSystemCloser = pJob->bSystemCloser();
  
  m_pDataStack->vPushInt(bSystemCloser);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* jobId::systemCloser! -- store system closer into job                      */
/*----------------------------------------------------------------------------*/
void Step::vJobIdSystemCloserStore(void)
{
  ENTRY();

  JobId tJobId = m_pDataStack->iPopInt(__func__);
  Job* pJob = AM::JLIST::pFindJob(tJobId);

  i8 iSystemCloser = m_pDataStack->iPopInt(__func__);
    
  pJob->vSystemCloser(iSystemCloser);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* jobId::isUnknown -- is the jobId unknown ?                                */
/*----------------------------------------------------------------------------*/
void Step::vJobIdIsUnknown(void)
{
  ENTRY();

  JobId tJobId = m_pDataStack->iPopInt(__func__);

  m_pDataStack->vPushInt(tJobId.bIsUnknown());

  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
