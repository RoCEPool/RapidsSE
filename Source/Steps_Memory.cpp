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

extern NP_Task         gnpRootTask;
extern Lock            gxDdlLock;
extern Lock            gxDicLock;
extern SEitfList*      gpSEitfList;

extern i8 giMemCopyBytes;
extern i8 giMemMoveBytes;
extern i8 giMemSetBytes;
extern i8 giMemZeroBytes;


// memory: /////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_Memory(void)
{
  vAddPrimitive("memory.byteDump"_Str,   &vMemoryByteDump,    false);
  vAddPrimitive("memory.list"_Str,       &vMemoryList,        false);
  vAddPrimitive("memory.reset"_Str,      &vMemoryReset,       false);
  vAddPrimitive("memory.wordDump"_Str,   &vMemoryWordDump,    false);
}

/*----------------------------------------------------------------------------*/
/* memory.bytedump -- byte-oriented dump                                      */
/*----------------------------------------------------------------------------*/
void Task::vMemoryByteDump(MP_Task mpTask)
{
ENTRY("vMemoryByteDump"_Str);
  
  i8 iSize;
  if ( ! mpTask->m_pDataStack->bPop(iSize) )
    THROW_INVALID_DSTACK("memory.bytedump"_Str);
  
  Object* pWordObject;
  if ( ! mpTask->m_pDataStack->bPop(pWordObject) )
    THROW_INVALID_DSTACK("memory.bytedump"_Str);
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
  ENTRY("vMemoryReset"_Str);
  
  giMemCopyBytes = 0;
  giMemMoveBytes = 0;
  giMemSetBytes  = 0;
  giMemZeroBytes = 0;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* memory.list-- trace memory statistics                                      */
/*----------------------------------------------------------------------------*/
void Task::vMemoryList(MP_Task mpTask)
{
  ENTRY("vMemoryList"_Str);

  s32 sMagnitude;
  
  Magnitudes::vMagnitude(giMemCopyBytes, sMagnitude);
  vTrace("MemCopyBytes %s\n"_Str,sMagnitude.aStr());
  
  Magnitudes::vMagnitude(giMemMoveBytes, sMagnitude);
  vTrace("MemMoveBytes %s\n"_Str,sMagnitude.aStr());

  Magnitudes::vMagnitude(giMemSetBytes, sMagnitude);
  vTrace("MemSetBytes  %s\n"_Str,sMagnitude.aStr());
  
  Magnitudes::vMagnitude(giMemZeroBytes, sMagnitude);
  vTrace("MemZeroBytes %s\n"_Str,sMagnitude.aStr());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* memory.worddump -- word-oriented dump                                      */
/*----------------------------------------------------------------------------*/
void Task::vMemoryWordDump(MP_Task mpTask)
{
ENTRY("vMemoryWordDump"_Str);
  
  i8 iSize;
  if ( ! mpTask->m_pDataStack->bPop(iSize) )
    THROW_INVALID_DSTACK("memory.worddump"_Str);
  
  Object* pWordObject;
  if ( ! mpTask->m_pDataStack->bPop(pWordObject) )
    THROW_INVALID_DSTACK("memory.worddump"_Str);
  Word* pWord = _pWord(pWordObject);
  
  //pWord->vWordDump(iSize);
  MemLib::vWordDump(pWord,iSize);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
