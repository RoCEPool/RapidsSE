/*============================================================================*/
/* Steps_Process.cpp - process step group                                     */
/*============================================================================*/

#include "StorageEngine.h"
//#include "Tasks.h"
#include "SePrompt.hpp"

extern AttachMap*      gpAttachMap;
extern BufferPool*     gpBufferPool;
extern BPA*            gpBPA;
extern Dic*            gpMainDic;
extern Word*           gpWord;

extern Grid*           gpGrid;
extern StepArray*      gpStepArray;
extern ThreadList*     gpThreadList;
extern TaskList*       gpTaskList;

extern NP_Task         gnpBootTask;
extern Lock            gxDdlLock;
extern Lock            gxDicLock;
extern SEitfList*      gpSEitfList;

// process: ////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_Process(void)
{
  vAddPrimitive("process.getPid"_Stz,  &vProcessGetPid,    false);
  vAddPrimitive("process.quit"_Stz,    &vProcessQuit,      false);
  vAddPrimitive("process.testPid"_Stz, &vProcessTestPid,   false);
}

/*----------------------------------------------------------------------------*/
/* process.getpid -- get pid of this process                                  */
/*----------------------------------------------------------------------------*/
void Task::vProcessGetPid(MP_Task mpTask)
{
ENTRY("vProcessGetPid"_Stz);
  
  i8 iPid = Process::iGetPid();
  
  mpTask->m_pDataStack->vPushInt(iPid);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* process.quit -- process exits early                                        */
/*----------------------------------------------------------------------------*/
void Task::vProcessQuit(MP_Task mpTask)
{
  ENTRY("vProcessQuit"_Stz);
  
  vFatal("Process quitting\n"_Stz);
  
  exit(EXIT_FAILURE);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* process.testpid -- test pid on stack                                       */
/*----------------------------------------------------------------------------*/
void Task::vProcessTestPid(MP_Task mpTask)
{
ENTRY("vProcessTestPid"_Stz);

  i8 iPid;
  if ( ! mpTask->m_pDataStack->bPop(iPid) )
    THROW_INVALID_DSTACK("process.testpid"_Stz);
  
  i8 iExists = Process::bTestPid(iPid);
  
  mpTask->m_pDataStack->vPushInt(iExists);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
