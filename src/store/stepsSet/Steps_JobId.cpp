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
#include "ExtendedText.hpp"
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
  vAddStep("jobId::folderPath!"_Stz,    &Step::vJobIdFolderPathStore);
  vAddStep("jobId::isUnknown"_Stz,      &Step::vJobIdIsUnknown);
  vAddStep("jobId::lastJobId@"_Stz,     &Step::vJobIdLastJobIdFetch);
  vAddStep("jobId::pipeToOp0"_Stz,      &Step::vJobIdPipeToOp0);
  vAddStep("jobId::systemCloser!"_Stz,  &Step::vJobIdSystemCloserStore);
}

/*----------------------------------------------------------------------------*/
/* jobId::folderPath! -- store folder path into job                           */
/*----------------------------------------------------------------------------*/
void Step::vJobIdFolderPathStore(void)
{
  ENTRY();

  JobId tJobId = m_pDataStack->iPopInt(__func__);

  s256 sFolderPath;
  m_pDataStack->vPopStr(sFolderPath,__func__);

  eEC eError = AM::JLIST::eFolderPathSet(tJobId, sFolderPath);

  if (eError == EC_NONE) {
    m_pDataStack->vPushInt(true);
  }
  else {
    m_pDataStack->vPushInt(false);
  }

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* jobId::lastJobId@ -- fetch last job id from job                           */
/*----------------------------------------------------------------------------*/
void Step::vJobIdLastJobIdFetch(void)
{
  ENTRY();

  JobId tLastJobId;

  JobId tJobId = m_pDataStack->iPopInt(__func__);

  eEC eError = AM::JLIST::eLastJobId(tJobId, tLastJobId);

  if (eError == EC_NONE) {
    m_pDataStack->vPushInt(true);
    m_pDataStack->vPushInt(tLastJobId.iAsI8());
  }
  else {
    m_pDataStack->vPushInt(false);
  }

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* jobId::pipeToOp0 -- append operand onto op0 of given job                  */
/*----------------------------------------------------------------------------*/
void Step::vJobIdPipeToOp0(void)
{
  ENTRY();

  JobId tJobId = m_pDataStack->iPopInt(__func__);

  Operand* pOperand = _pOperand(m_pDataStack->pPopPtr(__func__));

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  eEC eError = AM::JLIST::ePipeToOp0(tJobId, pOperand, this);

  if (eError == EC_NONE) {
    m_pDataStack->vPushInt(true);
  }
  else {
    m_pDataStack->vPushInt(false);
  }


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

/*----------------------------------------------------------------------------*/
/* jobId::systemCloser! -- store system closer into job                      */
/*----------------------------------------------------------------------------*/
void Step::vJobIdSystemCloserStore(void)
{
  ENTRY();

  JobId tJobId = m_pDataStack->iPopInt(__func__);

  i8 iSystemCloser = m_pDataStack->iPopInt(__func__);

  eEC eError = AM::JLIST::eSystemCloserSet(tJobId, iSystemCloser);

  if (eError == EC_NONE) {
    m_pDataStack->vPushInt(true);
  }
  else {
    m_pDataStack->vPushInt(false);
  }
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
