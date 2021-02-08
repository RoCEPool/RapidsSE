/*============================================================================*/
/* MY.cpp -- My helper                                                        */
/*============================================================================*/

#include "BASE.h"

#include "MY.hpp"

#include "Response.hpp"
#include "Result.hpp"
#include "Job.hpp"
#include "Op.hpp"

#include "CmdLineArgs.hpp"

extern CmdLineArgs   gxCmdLineArgs;

extern AttachMap*    gpAttachMap;
extern BufferPool*   gpBufferPool;
extern Log*          gpLog;
extern Header*       gaSeShmRegion;
extern File*         gpTraceFile;

extern SizeHeap      gxaSizeHeap[];


/*----------------------------------------------------------------------------*/
/* get current job name                                                       */
/*----------------------------------------------------------------------------*/
Stz* MY::szJobNm(void)
{
  Job* pJob = AM::pThisTLSJob();
  
  return aCN[pJob->eGetClassNo()];
}

/*----------------------------------------------------------------------------*/
/* get current word name                                                      */
/*----------------------------------------------------------------------------*/
Stz* MY::szWordNm(void)
{
  Task* pTask = AM::pThisTLSTask();
  
  Word* pWord = pTask->pSenCompiler()->pCurrentWord();
  
  return pWord->szWordNm();
}

/*----------------------------------------------------------------------------*/
/* get current job                                                            */
/*----------------------------------------------------------------------------*/
Job* MY::pJob(void)
{
  Task* pTask = MY::pTask();
  
  Job* pJob = AM::pTaskJob(pTask);
  
  return pJob;
}

/*----------------------------------------------------------------------------*/
/* get current op                                                             */
/*----------------------------------------------------------------------------*/
Op* MY::pOp(void)
{
  Job* pJob = MY::pJob();

  return pJob->m_pOp;
}

/*----------------------------------------------------------------------------*/
/* get current op                                                             */
/*----------------------------------------------------------------------------*/
Pipeline* MY::pPipeline(void)
{
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
  Job* pJob = MY::pJob();
 
  Session* pSession = AM::pJobSession(pJob);

  return pSession;
}

/*----------------------------------------------------------------------------*/
/* get current stage                                                          */
/*----------------------------------------------------------------------------*/
Stage* MY::pStage(void)
{
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
  Task* pTask = AM::pThisTLSTask();

  return pTask;
}

/*----------------------------------------------------------------------------*/
/* get current thread                                                         */
/*----------------------------------------------------------------------------*/
Thread* MY::pThread(void)
{
  Thread* pThread = AM::pThisTLSThread();

  return pThread;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
