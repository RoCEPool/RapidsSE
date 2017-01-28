/*============================================================================*/
/* Steps_Magnitude.cpp - magnitude step group                                 */
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
  vAddPrimitive("toDigitName",     &vToDigitName,   false);
  vAddPrimitive("bytes.magnitude", &vBytesMagnitude,   false);
  vAddPrimitive("number.magnitude", &vNumberMagnitude, false);
}

/*----------------------------------------------------------------------------*/
/* >DigitName -- convert to digit name                                        */
/*----------------------------------------------------------------------------*/
void Task::vToDigitName(NNP_Task nnpTask)
{
  ENTRY("vToDigitName");
  
  i8 iDigit;
  
  // get the arguments
  if ( ! nnpTask->m_pDataStack->bPop(iDigit) )
    THROW_INVALID_DSTACK(">DigitName");
  
  s32 sName;
  
  switch (iDigit) {
    case 0: sName = "zero";    break;
    case 1: sName = "one";     break;
    case 2: sName = "two";     break;
    case 3: sName = "three";   break;
    case 4: sName = "four";    break;
    case 5: sName = "five";    break;
    case 6: sName = "six";     break;
    case 7: sName = "seven";   break;
    case 8: sName = "eight";   break;
    case 9: sName = "nine";    break;
    default:sName = "invalid"; break;
  }
  
  nnpTask->m_pDataStack->vPush(sName);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* bytes.magnitude -- return magnitude of bytes                               */
/*----------------------------------------------------------------------------*/
void Task::vBytesMagnitude(NNP_Task nnpTask)
{
  ENTRY("vBytesMagnitude");
  
  i8 iBytes;
  if ( ! nnpTask->m_pDataStack->bPop(iBytes) )
    THROW_INVALID_DSTACK("bytes.magnitude");
  
  s256 sMag;
  Magnitudes::vMagnitude(iBytes, sMag.a());
  
  nnpTask->m_pDataStack->vPush(sMag);
  
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
  
  if ( ! nnpTask->m_pDataStack->bPop(iNumber) )
    THROW_INVALID_DSTACK("number.magnitude");
  
  if ( ! nnpTask->m_pDataStack->bPop(iScale) )
    THROW_INVALID_DSTACK("number.magnitude");
  
  s256 s;
  
  Magnitudes::vMagnitude(iNumber, iScale, s.a());
  
  nnpTask->m_pDataStack->vPush(s);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
