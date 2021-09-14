/*============================================================================*/
/* MY.cpp -- My helper                                                        */
/*============================================================================*/

#include "BASE.h"

#include "MY.hpp"

#include "Job.hpp"
#include "LogicUnit.hpp"
#include "Op.hpp"
#include "Grid.hpp"
#include "Pipeline.hpp"

/*----------------------------------------------------------------------------*/
/* get elapsed time since boot in nanoseconds                                 */
/*----------------------------------------------------------------------------*/
i8 MY::nNanoseconds(void)
{
  _static_
  
  return 0;
}

/*----------------------------------------------------------------------------*/
/* get current partition number                                               */
/*----------------------------------------------------------------------------*/
i8 MY::iPtn(void)
{
  _static_
  
  Op* pOp = MY::pOp();
  
  i8 iPtn = pOp ? pOp->tPtnNo() : -1;
  
  return iPtn;
}

/*----------------------------------------------------------------------------*/
/* get current caller name                                                    */
/*----------------------------------------------------------------------------*/
Stz* MY::szCallerNm(void)
{
  _static_
    
  Stz* szCallerNm;
  
  if (MY::pTask())
    szCallerNm = MY::pTask()->szTaskNm();
  else
  if (MY::pThread())
    szCallerNm = MY::pThread()->szThreadNm();
  else
    szCallerNm = "PmyThread"_Stz;

  return szCallerNm;
}

/*----------------------------------------------------------------------------*/
/* get current catalog name                                                   */
/*----------------------------------------------------------------------------*/
Stz* MY::szCatalogNm(void)
{
  _static_
  
  Dic* pCatalogDic = MY::pCatalogDic();
  
  if (! pCatalogDic)
    return ""_Stz;
  
  Stz* szCatalogNm = pCatalogDic->szDicNm();
  
  return szCatalogNm;
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
/* get current job name                                                       */
/*----------------------------------------------------------------------------*/
Stz* MY::szJobNm(void)
{
  _static_
  
  Job* pJob = MY::pJob();
  
  if (! pJob)
    return "(no-job)"_Stz;
  
  Stz* szJobNm = pJob->szJobNm();
  
  return szJobNm;
}

/*----------------------------------------------------------------------------*/
/* get current node name                                                      */
/*----------------------------------------------------------------------------*/
Stz* MY::szNodeNm(void)
{
  _static_
  
  Stz* szNodeNm = pNode()->szNodeNm();

  return szNodeNm;
}

/*----------------------------------------------------------------------------*/
/* get current operator name                                                  */
/*----------------------------------------------------------------------------*/
Stz* MY::szOpNm(void)
{
  _static_
  
  Op* pOp = MY::pOp();
  
  if (! pOp)
    return "(no-op)"_Stz;
  
  Stz* szOpNm = pOp->szOpNm();
  
  return szOpNm;
}

/*----------------------------------------------------------------------------*/
/* get current pipeline name                                                  */
/*----------------------------------------------------------------------------*/
Stz* MY::szPipelineNm(void)
{
  _static_
  
  Pipeline* pPipeline = MY::pPipeline();
  
  if (! pPipeline)
    return ""_Stz;
  
  Stz* szPipelineNm = pPipeline->szPipelineNm();
  
  return szPipelineNm;
}

/*----------------------------------------------------------------------------*/
/* get current schema name                                                    */
/*----------------------------------------------------------------------------*/
Stz* MY::szSchemaNm(void)
{
  _static_
  
  Dic* pSchemaDic = MY::pSchemaDic();
  
  if (! pSchemaDic)
    return ""_Stz;
  
  Stz* szSchemaNm = pSchemaDic->szDicNm();
  
  return szSchemaNm;
}

/*----------------------------------------------------------------------------*/
/* get current table name                                                     */
/*----------------------------------------------------------------------------*/
Stz* MY::szTableNm(void)
{
  _static_
  
  Dic* pTableDic = MY::pDicStack()->pTop();
  
  if (! pTableDic)
    return ""_Stz;
  
  Stz* szTableNm = pTableDic->szDicNm();
  
  return szTableNm;
}

/*----------------------------------------------------------------------------*/
/* get current task name                                                      */
/*----------------------------------------------------------------------------*/
Stz* MY::szTaskNm(void)
{
  _static_
  
  Task* pTask = MY::pTask();
  
  if (! pTask)
    return "(no-task)"_Stz;
  
  Stz* szTaskNm = pTask->szTaskNm();
  
  return szTaskNm;
}

/*----------------------------------------------------------------------------*/
/* get current tenant name                                                    */
/*----------------------------------------------------------------------------*/
Stz* MY::szTenantNm(void)
{
  _static_
  
  Dic* pTenantDic = MY::pTenantDic();
  
  if (! pTenantDic)
    return ""_Stz;
  
  Stz* szTenantNm = pTenantDic->szDicNm();
  
  return szTenantNm;
}

/*----------------------------------------------------------------------------*/
/* get current thread name                                                    */
/*----------------------------------------------------------------------------*/
Stz* MY::szThreadNm(void)
{
  _static_
  
  Stz* szThreadNm;
  if (MY::pThread())
    szThreadNm = MY::pThread()->szThreadNm();
  else
    szThreadNm = "PmyThread"_Stz;

  return szThreadNm;
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
/* get current catalog dictionary                                             */
/*----------------------------------------------------------------------------*/
Dic* MY::pCatalogDic(void)
{
  _static_

  DicStack* pDicStack = MY::pDicStack();

  if (! pDicStack)
    return nullptr;
  
  Dic* pCatalogDic = pDicStack->pCatalogDic();
  
  return pCatalogDic;
}

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

  // revised job/op model

  Task* pTask = MY::pTask();
  if (! pTask)
    return nullptr;
  
  Job* pJob = pTask->pJob_task();
  if (pJob)
    return pJob;

  Op* pOp = pTask->pOp();
  if (pOp) {
    pJob = pOp->pPipelineJob();
    return pJob;
  }
  
  return nullptr;

}

/*----------------------------------------------------------------------------*/
/* get current logic unit                                                     */
/*----------------------------------------------------------------------------*/
LogicUnit* MY::pLogicUnit(void)
{
  _static_
  
  // revised job/op model
  
  Task* pTask = MY::pTask();

  if (!pTask)
    return nullptr;

  Job* pJob = pTask->pJob_task();
  if (pJob)
    return pJob->pJobLogicUnit();

  Op* pOp = pTask->pOp();
  if (pOp)
    return pOp->pOpLogicUnit();

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

  // revised job/op model
  
  Task* pTask = MY::pTask();
  if (! pTask)
    return nullptr;
  
  Op* pOp = pTask->pOp();
  
  return pOp;
}

/*----------------------------------------------------------------------------*/
/* get current operand kind                                                   */
/*----------------------------------------------------------------------------*/
eZK MY::eOperand(void)
{
  _static_

  LogicUnit* pLogicUnit = MY::pLogicUnit();

  if (pLogicUnit)
    return pLogicUnit->m_ePipeInOperand;
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

  if (! pOp)
    return nullptr;
  
  return pOp->pPipeline_op();
}

/*----------------------------------------------------------------------------*/
/* get current schema dictionary                                              */
/*----------------------------------------------------------------------------*/
Dic* MY::pSchemaDic(void)
{
  _static_

  DicStack* pDicStack = MY::pDicStack();

  if (! pDicStack)
    return nullptr;
  
  Dic* pSchemaDic = pDicStack->pSchemaDic();
  
  return pSchemaDic;
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
/* get current table dictionary                                               */
/*----------------------------------------------------------------------------*/
Dic* MY::pTableDic(void)
{
  _static_

  DicStack* pDicStack = MY::pDicStack();

  if (! pDicStack)
    return nullptr;
  
  Dic* pTableDic = pDicStack->pTableDic();
  
  return pTableDic;
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
/* get current tenant dictionary                                              */
/*----------------------------------------------------------------------------*/
Dic* MY::pTenantDic(void)
{
  _static_

  DicStack* pDicStack = MY::pDicStack();

  if (! pDicStack)
    return nullptr;
  
  Dic* pTenantDic = pDicStack->pTenantDic();
  
  return pTenantDic;
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

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get current path                                                           */
/*----------------------------------------------------------------------------*/
Stz* MY::szPath(Stz* szPathBuffer)
{
  _static_
  
  *szPathBuffer = ""_Stz;

  DicStack* pDicStack = MY::pDicStack();

  if (! pDicStack)
    return szPathBuffer;
  
  pDicStack->vToPathStr(szPathBuffer);
  
  return szPathBuffer;
}

/*----------------------------------------------------------------------------*/
/* get current path                                                           */
/*----------------------------------------------------------------------------*/
void MY::vPath(Stz* szPathBuffer)
{
  _static_

  szPathBuffer = ""_Stz;
  
  DicStack* pDicStack = MY::pDicStack();

  if (! pDicStack)
    return;
  
  pDicStack->vToPathStr(szPathBuffer);

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
