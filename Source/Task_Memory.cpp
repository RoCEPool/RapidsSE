/*============================================================================*/
/* Task_Memory.cpp - task memory group                                        */
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

// memory: /////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_Memory(void)
{
  vAddPrimitive("memory.byteDump", &vMemoryByteDump,   false);
  vAddPrimitive("memory.wordDump", &vMemoryWordDump,   false);
}

/*----------------------------------------------------------------------------*/
/* memory.bytedump -- byte-oriented dump                                      */
/*----------------------------------------------------------------------------*/
void Task::vMemoryByteDump(NNP_Task nnpTask)
{
ENTRY("vMemoryByteDump");
  
  i8 iSize;
  if ( ! nnpTask->m_pDataStack->bPop(iSize) )
    THROW_INVALID_DSTACK("memory.bytedump");
  
  Object* pWordObject;
  if ( ! nnpTask->m_pDataStack->bPop(pWordObject) )
    THROW_INVALID_DSTACK("memory.bytedump");
  Word* pWord = _pWord(pWordObject);
  
  //pWord->vByteDump(iSize);
  Memory::vHexDump(pWord,iSize);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* memory.worddump -- word-oriented dump                                      */
/*----------------------------------------------------------------------------*/
void Task::vMemoryWordDump(NNP_Task nnpTask)
{
ENTRY("vMemoryWordDump");
  
  i8 iSize;
  if ( ! nnpTask->m_pDataStack->bPop(iSize) )
    THROW_INVALID_DSTACK("memory.worddump");
  
  Object* pWordObject;
  if ( ! nnpTask->m_pDataStack->bPop(pWordObject) )
    THROW_INVALID_DSTACK("memory.worddump");
  Word* pWord = _pWord(pWordObject);
  
  //pWord->vWordDump(iSize);
  Memory::vWordDump(pWord,iSize);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
