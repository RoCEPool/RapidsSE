/*============================================================================*/
/* Steps_IndexBuffer.cpp - index buffer step group                            */
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
extern StepBoard*      gpStepBoard;
extern Lock            gxDdlLock;
extern Lock            gxDicLock;
extern SEitfList*      gpSEitfList;

// indexbuffer: ////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_IndexBuffer(void)
{
  vAddPrimitive("indexBuffer.firstEntry"_Str, &vIndexBufferFirstEntry, false);
  vAddPrimitive("indexBuffer.nextEntry"_Str,  &vIndexBufferNextEntry,  false);
  vAddPrimitive("indexBuffer.nextBuffer"_Str, &vIndexBufferNextBuffer, false);
}

/*----------------------------------------------------------------------------*/
/* indexBuffer.nextBuffer -- get next buffer of this index buffer             */
/*----------------------------------------------------------------------------*/
void Task::vIndexBufferNextBuffer(NNP_Task nnpTask)
{
  ENTRY("vIndexBufferNextBuffer"_Str);
  
  Object* pPtrEntry = nullptr;
  Object* pPtrIndexBuffer;
  
  if ( ! nnpTask->m_pDataStack->bPop(pPtrIndexBuffer) )
    THROW_INVALID_DSTACK("indexBuffer.nextBuffer"_Str);
  
  IndexBuffer* pIndexBuffer = _pIndexBuffer(pPtrIndexBuffer);
  ASSERT(nullptr == pIndexBuffer || CN_INDEXBUFFER == pIndexBuffer->eClass());
  
  if (pIndexBuffer->pNextBuffer()) {
    bn nBuffer = pIndexBuffer->pNextBuffer()->nBufferNo();
  }
  
  nnpTask->m_pDataStack->vPushObject(pIndexBuffer->pNextBuffer());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* indexBuffer.firstEntry -- get first entry from index buffer                */
/*----------------------------------------------------------------------------*/
void Task::vIndexBufferFirstEntry(NNP_Task nnpTask)
{
  ENTRY("vIndexBufferFirstEntry"_Str);
  
  Object* pPtrEntry = nullptr;
  Object* pPtrIndexBuffer;
  
  if ( ! nnpTask->m_pDataStack->bPop(pPtrIndexBuffer) )
    THROW_INVALID_DSTACK("indexBuffer.firstEntry!"_Str);
  
  IndexBuffer* pIndexBuffer = _pIndexBuffer(pPtrIndexBuffer);
  ASSERT(nullptr == pIndexBuffer || CN_INDEXBUFFER == pIndexBuffer->eClass());
  
  nnpTask->m_pDataStack->vPushObject(pIndexBuffer->aNextEntry(pIndexBuffer->aHead()));
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* indexBuffer.nextEntry -- get next entry from index buffer                  */
/*----------------------------------------------------------------------------*/
void Task::vIndexBufferNextEntry(NNP_Task nnpTask)
{
  ENTRY("vIndexBufferNextEntry"_Str);
  
  Object* pPtrEntry = nullptr;
  Object* pPtrIndexBuffer;
  
  if ( ! nnpTask->m_pDataStack->bPop(pPtrIndexBuffer) )
    THROW_INVALID_DSTACK("indexBuffer.nextEntry"_Str);
  
  IndexBuffer* pIndexBuffer = _pIndexBuffer(pPtrIndexBuffer);
  ASSERT(nullptr == pIndexBuffer || CN_INDEXBUFFER == pIndexBuffer->eClass());
  
  if ( ! nnpTask->m_pDataStack->bPop(pPtrEntry) )
    THROW_INVALID_DSTACK("indexBuffer.nextEntry"_Str);
  
  Entry* aEntry = _aEntry(pPtrEntry);
  
  Entry *pNextEntry;
  
  if ( pPtrEntry ) {
    ASSERT(CN_ENTRY == aEntry->eClass());
    pNextEntry = pIndexBuffer->aNextEntry(aEntry);
  } else {
    pNextEntry = pIndexBuffer->aNextEntry(pIndexBuffer->aHead());
  }
  
  // assert that key of the next entry is >= entry              //ALKA:TEMPORARY
//  if (pEntry && pNextEntry) {
//    Key* aEntryKey = pEntry->aKey();
//    Key* aNextEntryKey = pNextEntry->aKey();
//    ASSERT(aEntryKey->bLessEqual(aNextEntryKey,pIndexBuffer->pIndex()->pIdxColVec()));
//  }
  
  nnpTask->m_pDataStack->vPushObject(pNextEntry);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
