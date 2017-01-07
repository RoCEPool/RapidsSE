/*============================================================================*/
/* Task_groups.cpp - task groups                                              */
/*============================================================================*/

#include "StorageEngine.h"
//#include "Tasks.h"
#include "SePrompt.hpp"

extern Arguments       gxArguments;

extern AttachMap*      gpAttachMap;
extern BufferPool*     gpBufferPool;
extern Heap*           gpMediumHeap;
extern BPA*            gpBPA;
extern Dic*            gpMainDic;
extern Word*           gpWord;

extern Grid*           gpGrid;
extern StepArray*      gpStepArray;
extern ThreadList*     gpThreadList;
extern TaskList*       gpTaskList;
extern TxCollection*   gpTxCollection;

extern NP_Task         gnpRootTask;
extern StepBoard*      gpStepBoard;
extern Lock            gxDdlLock;
extern Lock            gxDicLock;
extern SEitfList*      gpSEitfList;

// magnitude: //////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_Magnitude(void)
{
  vAddPrimitive("bytes.magnitude", &vBytesMagnitude,   false);
  vAddPrimitive("number.magnitude", &vNumberMagnitude, false);
}

/*----------------------------------------------------------------------------*/
/* bytes.magnitude -- return magnitude of bytes                               */
/*----------------------------------------------------------------------------*/
void Task::vBytesMagnitude(NNP_Task nnpTask)
{
  ENTRY("vBytesMagnitude");
  
  i8 iBytes;
  if ( ! nnpTask->m_xDataStack.bPop(iBytes) )
    THROW_INVALID_DSTACK("bytes.magnitude");
  
  s256 sMag;
  Magnitudes::vMagnitude(iBytes, sMag.a());
  
  nnpTask->m_xDataStack.vPush(sMag);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* number.magnitude -- get magnitude of number                                */
/*----------------------------------------------------------------------------*/
void Task::vNumberMagnitude(NNP_Task nnpTask)
{
  ENTRY("vNumberMagnitude");
  
  i8 iNumber;
  i8 iScale;
  
  if ( ! nnpTask->m_xDataStack.bPop(iNumber) )
    THROW_INVALID_DSTACK("number.magnitude");
  
  if ( ! nnpTask->m_xDataStack.bPop(iScale) )
    THROW_INVALID_DSTACK("number.magnitude");
  
  s256 s;
  
  Magnitudes::vMagnitude(iNumber, iScale, s.a());
  
  nnpTask->m_xDataStack.vPush(s);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
