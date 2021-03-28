/*============================================================================*/
/* MY.cpp -- My helper                                                        */
/*============================================================================*/

#include "BASE.h"

#include "MY.hpp"

#include "Job.hpp"
#include "LogicUnit.hpp"
#include "Op.hpp"
#include "Grid.hpp"

/*----------------------------------------------------------------------------*/
/* get current job name                                                       */
/*----------------------------------------------------------------------------*/
Stz* MY::szJobNm(void)
{
  _static_
  
  Job* pJob = AM::pThisTLSJob();
  
  return aCN[pJob->eGetClassNo()];
}

/*----------------------------------------------------------------------------*/
/* get current host name                                                      */
/*----------------------------------------------------------------------------*/
Stz* MY::szHostNm(void)
{
  _static_

  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* get current word name                                                      */
/*----------------------------------------------------------------------------*/
Stz* MY::szWordNm(void)
{
  _static_

  Task* pTask = AM::pThisTLSTask();
  
  Word* pWord = pTask->pSenCompiler()->pCurrentWord();
  
  return pWord->szWordNm();
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get current dictionary                                                     */
/*----------------------------------------------------------------------------*/
Dic* MY::pDic(void)
{
  _static_

  DicStack* pDicStack = MY::pDicStack();

  if (pDicStack)
    return pDicStack->pTop();
  else
    return nullptr;
}

/*----------------------------------------------------------------------------*/
/* get current dictionary stack                                               */
/*----------------------------------------------------------------------------*/
DicStack* MY::pDicStack(void)
{
  _static_

  Task* pTask = MY::pTask();

  if (pTask)
    return pTask->pDicStack();
  else
    return nullptr;
}

/*----------------------------------------------------------------------------*/
/* get current grid                                                           */
/*----------------------------------------------------------------------------*/
Grid* MY::pGrid(void)
{
  _static_

  Grid* pGrid = AM::pGrid();

  return pGrid;
}

/*----------------------------------------------------------------------------*/
/* get current job                                                            */
/*----------------------------------------------------------------------------*/
Job* MY::pJob(void)
{
  _static_

  Task* pTask = MY::pTask();
  
  Job* pJob = AM::pTaskJob(pTask);
  
  return pJob;
}

/*----------------------------------------------------------------------------*/
/* get current logic unit                                                     */
/*----------------------------------------------------------------------------*/
LogicUnit* MY::pLogicUnit(void)
{
  _static_

  Job* pJob = MY::pJob();

  if (pJob)
    return pJob->m_pLogicUnit_job;
  else
    return nullptr;
}

/*----------------------------------------------------------------------------*/
/* get current node                                                           */
/*----------------------------------------------------------------------------*/
Node* MY::pNode(void)
{
  _static_

  Grid* pGrid = MY::pGrid();
  
  if (! pGrid)
    return nullptr;

  Node* pNode = pGrid->pThisNode();

  return pNode;
}

/*----------------------------------------------------------------------------*/
/* get current op                                                             */
/*----------------------------------------------------------------------------*/
Op* MY::pOp(void)
{
  _static_

  Job* pJob = MY::pJob();

  if (pJob)
    return pJob->m_pOp;
  else
    return nullptr;
}

/*----------------------------------------------------------------------------*/
/* get current operand kind                                                   */
/*----------------------------------------------------------------------------*/
eZK MY::eOperand(void)
{
  _static_

  LogicUnit* pLogicUnit = MY::pLogicUnit();

  if (pLogicUnit)
    return pLogicUnit->m_eOperand;
  else
    return ZK_NONE;
}

/*----------------------------------------------------------------------------*/
/* get current op                                                             */
/*----------------------------------------------------------------------------*/
Pipeline* MY::pPipeline(void)
{
  _static_

  Op* pOp = MY::pOp();

  if (pOp)
    return pOp->pPipeline();
  else
    return nullptr;
}

/*----------------------------------------------------------------------------*/
/* get current session                                                        */
/*----------------------------------------------------------------------------*/
Session* MY::pSession(void)
{
  _static_

  Job* pJob = MY::pJob();
 
  Session* pSession = AM::pJobSession(pJob);

  return pSession;
}

/*----------------------------------------------------------------------------*/
/* get current stage                                                          */
/*----------------------------------------------------------------------------*/
Stage* MY::pStage(void)
{
  _static_

  Op* pOp = MY::pOp();

  if (pOp)
    return pOp->pStage();
  else
    return nullptr;
}

/*----------------------------------------------------------------------------*/
/* get current stream                                                         */
/*----------------------------------------------------------------------------*/
Stream* MY::pStream(void)
{
  _static_

  Op* pOp = MY::pOp();

  if (pOp)
    return pOp->pStream();
  else
    return nullptr;
}

/*----------------------------------------------------------------------------*/
/* get current task                                                           */
/*----------------------------------------------------------------------------*/
Task* MY::pTask(void)
{
  _static_

  Task* pTask = AM::pThisTLSTask();

  return pTask;
}

/*----------------------------------------------------------------------------*/
/* get current thread                                                         */
/*----------------------------------------------------------------------------*/
Thread* MY::pThread(void)
{
  _static_

  Thread* pThread = AM::pThisTLSThread();

  return pThread;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
