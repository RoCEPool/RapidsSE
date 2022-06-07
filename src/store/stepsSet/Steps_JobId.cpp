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
  vAddStep("job(Id).folderPath@"_Stz,    &Step::vJobIdFolderPathFetch);
  vAddStep("job(Id).folderPath!"_Stz,    &Step::vJobIdFolderPathStore);
    
  vAddStep("job(Id).lastJobId@"_Stz,     &Step::vJobIdLastJobIdFetch);
  vAddStep("job(Id).lastJobId!"_Stz,     &Step::vJobIdLastJobIdStore);

  vAddStep("job(Id).pipeToOp0"_Stz,      &Step::vJobIdPipeToOp0);

  vAddStep("job(Id).reapable@"_Stz,      &Step::vJobIdReapableFetch);
  vAddStep("job(Id).reapable!"_Stz,      &Step::vJobIdReapableStore);

  vAddStep("job(Id).systemCloser?"_Stz,  &Step::vJobIdSystemCloserFetch);
  vAddStep("job(Id).systemCloser!"_Stz,  &Step::vJobIdSystemCloserStore);
}

/*----------------------------------------------------------------------------*/
/* job(Id).folderPath@ -- fetch folder path from job                          */
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
/* job(Id).folderPath! -- store folder path into job                          */
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
/* job(Id).lastJobId@ -- fetch last job id from job                           */
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
/* job(Id).lastJobId! -- store last job id into job                           */
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
/* job(Id).systemCloser? -- fetch system closer from job                      */
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

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
