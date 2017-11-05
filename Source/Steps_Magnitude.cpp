/*============================================================================*/
/* Steps_Magnitude.cpp - magnitude step group                                 */
/*============================================================================*/

#include "StorageEngine.h"
//#include "Tasks.h"
#include "SePrompt.hpp"

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

extern NP_Task         gnpBootTask;
extern Lock            gxDdlLock;
extern Lock            gxDicLock;
extern SEitfList*      gpSEitfList;

// magnitude: //////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_Magnitude(void)
{
  vAddPrimitive("toDigitName"_Stz,     &vToDigitName,   false);
  vAddPrimitive("bytes.magnitude"_Stz, &vBytesMagnitude,   false);
  vAddPrimitive("number.magnitude"_Stz, &vNumberMagnitude, false);
}

/*----------------------------------------------------------------------------*/
/* >DigitName -- convert to digit name                                        */
/*----------------------------------------------------------------------------*/
void Task::vToDigitName(MP_Task mpTask)
{
  ENTRY("vToDigitName"_Stz);
  
  i8 iDigit;
  
  // get the arguments
  if ( ! mpTask->m_pDataStack->bPop(iDigit) )
    THROW_INVALID_DSTACK(">DigitName"_Stz);
  
  s32 sName;
  
  switch (iDigit) {
    case 0: sName = "zero"_Stz;    break;
    case 1: sName = "one"_Stz;     break;
    case 2: sName = "two"_Stz;     break;
    case 3: sName = "three"_Stz;   break;
    case 4: sName = "four"_Stz;    break;
    case 5: sName = "five"_Stz;    break;
    case 6: sName = "six"_Stz;     break;
    case 7: sName = "seven"_Stz;   break;
    case 8: sName = "eight"_Stz;   break;
    case 9: sName = "nine"_Stz;    break;
    default:sName = "invalid"_Stz; break;
  }
  
  mpTask->m_pDataStack->vPushS32(sName);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* bytes.magnitude -- return magnitude of bytes                               */
/*----------------------------------------------------------------------------*/
void Task::vBytesMagnitude(MP_Task mpTask)
{
  ENTRY("vBytesMagnitude"_Stz);
  
  i8 iBytes;
  if ( ! mpTask->m_pDataStack->bPop(iBytes) )
    THROW_INVALID_DSTACK("bytes.magnitude"_Stz);
  
  s256 sMag;
  Magnitudes::vMagnitude(iBytes, sMag);
  
  mpTask->m_pDataStack->vPushS256(sMag);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* number.magnitude -- get magnitude of number                                */
/*----------------------------------------------------------------------------*/
void Task::vNumberMagnitude(MP_Task mpTask)
{
  ENTRY("vNumberMagnitude"_Stz);
  
  i8 iNumber;
  i8 iScale;
  
  if ( ! mpTask->m_pDataStack->bPop(iNumber) )
    THROW_INVALID_DSTACK("number.magnitude"_Stz);
  
  if ( ! mpTask->m_pDataStack->bPop(iScale) )
    THROW_INVALID_DSTACK("number.magnitude"_Stz);
  
  s256 s;
  
  Magnitudes::vMagnitude(iNumber, iScale, s);
  
  mpTask->m_pDataStack->vPushS256(s);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
