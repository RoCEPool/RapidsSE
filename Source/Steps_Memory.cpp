/*============================================================================*/
/* Steps_Memory.cpp - memory step group                                       */
/*============================================================================*/

#include "StorageEngine.h"
//#include "Tasks.h"
#include "SePrompt.hpp"

extern AttachMap*      gpAttachMap;

// memory: /////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Memory(void)
{
  vAddStep("memory.byteDump"_Stz,   &Task::vMemoryByteDump,    false);
  vAddStep("memory::list"_Stz,       &Task::vMemoryList,        false);
  vAddStep("memory::reset"_Stz,      &Task::vMemoryReset,       false);
  vAddStep("memory.wordDump"_Stz,   &Task::vMemoryWordDump,    false);
}

/*----------------------------------------------------------------------------*/
/* memory.bytedump -- byte-oriented dump                                      */
/*----------------------------------------------------------------------------*/
void Task::vMemoryByteDump(MP_Task mpTask)
{
ENTRY("vMemoryByteDump"_Stz);
  
  i8 iSize;
  if ( ! mpTask->m_pDataStack->bPop(iSize) )
    THROW_INVALID_DSTACK("memory.bytedump"_Stz);
  
  Object* pWordObject;
  if ( ! mpTask->m_pDataStack->bPop(pWordObject) )
    THROW_INVALID_DSTACK("memory.bytedump"_Stz);
  Word* pWord = _pWord(pWordObject);
  
  //pWord->vByteDump(iSize);
  MemLib::vHexDump(pWord,iSize);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* memory.reset -- reset memory statistics                                    */
/*----------------------------------------------------------------------------*/
void Task::vMemoryReset(MP_Task mpTask)
{
  ENTRY("vMemoryReset"_Stz);
  
  gpAttachMap->m_iMemCopyBytes = 0;
  gpAttachMap->m_iMemMoveBytes = 0;
  gpAttachMap->m_iMemScribbleBytes = 0;
  gpAttachMap->m_iMemSetBytes  = 0;
  gpAttachMap->m_iMemZeroBytes = 0;
  
  for (i8 i_Class = 0; i_Class < CN_ENUMS; i_Class++) {
    gpAttachMap->m_xMemCopy[i_Class] = 0;
    gpAttachMap->m_xMemMove[i_Class] = 0;
    gpAttachMap->m_xMemScribble[i_Class] = 0;
    gpAttachMap->m_xMemSet[i_Class] = 0;
    gpAttachMap->m_xMemZero[i_Class] = 0;
  }
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* memory.list-- trace memory statistics                                      */
/*----------------------------------------------------------------------------*/
void Task::vMemoryList(MP_Task mpTask)
{
  ENTRY("vMemoryList"_Stz);

  s32 sMagnitude;
  
  Magnitudes::vMagnitude(gpAttachMap->m_iMemCopyBytes, sMagnitude);
  vTrace("MemCopyBytes %s\n"_Stz,sMagnitude.aStr());
  for (i8 i_Class = 0; i_Class < CN_ENUMS; i_Class++) {
    if (gpAttachMap->m_xMemCopy[i_Class]) {
      Magnitudes::vMagnitude(gpAttachMap->m_xMemCopy[i_Class], sMagnitude);
      vTrace("  %s %s\n"_Stz,aCN[i_Class],sMagnitude.aStr());
    }
  }
  
  Magnitudes::vMagnitude(gpAttachMap->m_iMemMoveBytes, sMagnitude);
  vTrace("MemMoveBytes %s\n"_Stz,sMagnitude.aStr());
  for (i8 i_Class = 0; i_Class < CN_ENUMS; i_Class++) {
    if (gpAttachMap->m_xMemMove[i_Class]) {
      Magnitudes::vMagnitude(gpAttachMap->m_xMemMove[i_Class], sMagnitude);
      vTrace("  %s %s\n"_Stz,aCN[i_Class],sMagnitude.aStr());
    }
  }

  Magnitudes::vMagnitude(gpAttachMap->m_iMemScribbleBytes, sMagnitude);
  vTrace("MemScribbleBytes %s\n"_Stz,sMagnitude.aStr());
  for (i8 i_Class = 0; i_Class < CN_ENUMS; i_Class++) {
    if (gpAttachMap->m_xMemScribble[i_Class]) {
      Magnitudes::vMagnitude(gpAttachMap->m_xMemScribble[i_Class], sMagnitude);
      vTrace("  %s %s\n"_Stz,aCN[i_Class],sMagnitude.aStr());
    }
  }
  
  Magnitudes::vMagnitude(gpAttachMap->m_iMemSetBytes, sMagnitude);
  vTrace("MemSetBytes %s\n"_Stz,sMagnitude.aStr());
  for (i8 i_Class = 0; i_Class < CN_ENUMS; i_Class++) {
    if (gpAttachMap->m_xMemSet[i_Class]) {
      Magnitudes::vMagnitude(gpAttachMap->m_xMemSet[i_Class], sMagnitude);
      vTrace("  %s %s\n"_Stz,aCN[i_Class],sMagnitude.aStr());
    }
  }
  
  Magnitudes::vMagnitude(gpAttachMap->m_iMemZeroBytes, sMagnitude);
  vTrace("MemZeroBytes %s\n"_Stz,sMagnitude.aStr());
  for (i8 i_Class = 0; i_Class < CN_ENUMS; i_Class++) {
    if (gpAttachMap->m_xMemZero[i_Class]) {
      Magnitudes::vMagnitude(gpAttachMap->m_xMemZero[i_Class], sMagnitude);
      vTrace("  %s %s\n"_Stz,aCN[i_Class],sMagnitude.aStr());
    }
  }
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* memory.worddump -- word-oriented dump                                      */
/*----------------------------------------------------------------------------*/
void Task::vMemoryWordDump(MP_Task mpTask)
{
ENTRY("vMemoryWordDump"_Stz);
  
  i8 iSize;
  if ( ! mpTask->m_pDataStack->bPop(iSize) )
    THROW_INVALID_DSTACK("memory.worddump"_Stz);
  
  Object* pWordObject;
  if ( ! mpTask->m_pDataStack->bPop(pWordObject) )
    THROW_INVALID_DSTACK("memory.worddump"_Stz);
  Word* pWord = _pWord(pWordObject);
  
  //pWord->vWordDump(iSize);
  MemLib::vWordDump(pWord,iSize);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
