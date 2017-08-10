/*============================================================================*/
/* Steps_Memory.cpp - memory step group                                       */
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
extern TxCollection*   gpTxCollection;

extern NP_Task         gnpBootTask;
extern Lock            gxDdlLock;
extern Lock            gxDicLock;
extern SEitfList*      gpSEitfList;

extern i8 giMemCopyBytes;
extern i8 giMemMoveBytes;
extern i8 giMemScribbleBytes;
extern i8 giMemSetBytes;
extern i8 giMemZeroBytes;

extern i8 gxMemCopy[CN_ENUMS];
extern i8 gxMemMove[CN_ENUMS];
extern i8 gxMemScribble[CN_ENUMS];
extern i8 gxMemSet[CN_ENUMS];
extern i8 gxMemZero[CN_ENUMS];

// memory: /////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_Memory(void)
{
  vAddPrimitive("memory.byteDump"_Stz,   &vMemoryByteDump,    false);
  vAddPrimitive("memory::list"_Stz,       &vMemoryList,        false);
  vAddPrimitive("memory::reset"_Stz,      &vMemoryReset,       false);
  vAddPrimitive("memory.wordDump"_Stz,   &vMemoryWordDump,    false);
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
  
  giMemCopyBytes = 0;
  giMemMoveBytes = 0;
  giMemScribbleBytes = 0;
  giMemSetBytes  = 0;
  giMemZeroBytes = 0;
  
  for (i8 i_Class = 0; i_Class < CN_ENUMS; i_Class++) {
    gxMemCopy[i_Class] = 0;
    gxMemMove[i_Class] = 0;
    gxMemScribble[i_Class] = 0;
    gxMemSet[i_Class] = 0;
    gxMemZero[i_Class] = 0;
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
  
  Magnitudes::vMagnitude(giMemCopyBytes, sMagnitude);
  vTrace("MemCopyBytes %s\n"_Stz,sMagnitude.aStr());
  for (i8 i_Class = 0; i_Class < CN_ENUMS; i_Class++) {
    if (gxMemCopy[i_Class]) {
      Magnitudes::vMagnitude(gxMemCopy[i_Class], sMagnitude);
      vTrace("  %s %s\n"_Stz,aCN[i_Class],sMagnitude.aStr());
    }
  }
  
  Magnitudes::vMagnitude(giMemMoveBytes, sMagnitude);
  vTrace("MemMoveBytes %s\n"_Stz,sMagnitude.aStr());
  for (i8 i_Class = 0; i_Class < CN_ENUMS; i_Class++) {
    if (gxMemMove[i_Class]) {
      Magnitudes::vMagnitude(gxMemMove[i_Class], sMagnitude);
      vTrace("  %s %s\n"_Stz,aCN[i_Class],sMagnitude.aStr());
    }
  }

  Magnitudes::vMagnitude(giMemScribbleBytes, sMagnitude);
  vTrace("MemScribbleBytes %s\n"_Stz,sMagnitude.aStr());
  for (i8 i_Class = 0; i_Class < CN_ENUMS; i_Class++) {
    if (gxMemScribble[i_Class]) {
      Magnitudes::vMagnitude(gxMemScribble[i_Class], sMagnitude);
      vTrace("  %s %s\n"_Stz,aCN[i_Class],sMagnitude.aStr());
    }
  }
  
  Magnitudes::vMagnitude(giMemSetBytes, sMagnitude);
  vTrace("MemSetBytes %s\n"_Stz,sMagnitude.aStr());
  for (i8 i_Class = 0; i_Class < CN_ENUMS; i_Class++) {
    if (gxMemSet[i_Class]) {
      Magnitudes::vMagnitude(gxMemSet[i_Class], sMagnitude);
      vTrace("  %s %s\n"_Stz,aCN[i_Class],sMagnitude.aStr());
    }
  }
  
  Magnitudes::vMagnitude(giMemZeroBytes, sMagnitude);
  vTrace("MemZeroBytes %s\n"_Stz,sMagnitude.aStr());
  for (i8 i_Class = 0; i_Class < CN_ENUMS; i_Class++) {
    if (gxMemZero[i_Class]) {
      Magnitudes::vMagnitude(gxMemZero[i_Class], sMagnitude);
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
