/*============================================================================*/
/* MY.cpp -- My helper                                                        */
/*============================================================================*/

#include "BASE.h"
#include "STORE.h"

#include "MY.hpp"

#include "Response.hpp"
#include "Result.hpp"
#include "Job.hpp"

extern CmdLineArgs          gxArgs;

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
  
  Word* pWord = pTask->pCompiler()->pCurrentWord();
  
  return pWord->szWordNm();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
