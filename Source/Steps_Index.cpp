/*============================================================================*/
/* Steps_Index.cpp - index step group                                         */
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

// index: //////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_Index(void)
{
  vAddPrimitive("index::ibDump",     &vIndexIbDump,     false);
  vAddPrimitive("index::entryTest",  &vIndexEntryTest,  false);
  vAddPrimitive("index::ibNew",      &vIndexIbNew,      false);
}

/*----------------------------------------------------------------------------*/
/* index::ibDump -- dump the index buffer                                     */
/*----------------------------------------------------------------------------*/
void Task::vIndexIbDump(NNP_Task nnpTask)
{
  ENTRY("vIndexIbDump");
  
  Object* pPtrIB;
  if ( ! nnpTask->m_pDataStack->bPop(pPtrIB) )
    THROW_INVALID_DSTACK("index::ibDump");
  
  IndexBuffer* pIndexBuffer = _pIndexBuffer(pPtrIB);
  ASSERT(CN_INDEXBUFFER == pIndexBuffer->eClass());
  
  vTrace("\n");
  
  pIndexBuffer->vShow();
  
  vTrace("\nIndex buffer (%p) dump:\n", pIndexBuffer);
  Entry* pEntry = pIndexBuffer->pFirstEntry();
  
  while (pEntry) {
    pEntry->vContentDump();
    pEntry = pIndexBuffer->pNextEntry(pEntry);
  }
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* index::entryTest -- initial test of index entry                            */
/*----------------------------------------------------------------------------*/
void Task::vIndexEntryTest(NNP_Task nnpTask)
{
  ENTRY("vIndexEntryTest");
  
  Object* pPtrIb = nullptr;
  
  if ( ! nnpTask->m_pDataStack->bPop(pPtrIb) )
    THROW_INVALID_DSTACK("index::entryTest");
  
  IndexBuffer* pIndexBuffer = _pIndexBuffer(pPtrIb);
  ASSERT(CN_INDEXBUFFER == pIndexBuffer->eClass());
  
  s48K sRow;
  if ( ! nnpTask->m_pDataStack->bPop(sRow) )
    THROW_INVALID_DSTACK("index::entryTest");

  vTrace("index::entryTest -- %s\n",sRow.a());
  
  EntWsp xEntWsp;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // set rowId
  RowId xRowId;
  i8 iRowNo = String::iToI8(sRow.a());
  
  xRowId.vBuffer(999);
  xRowId.vRow(iRowNo);
  xEntWsp.vRowId(xRowId);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // add element to key
  KeyWsp xKeyWsp;
  ValWsp xValWsp;
  
  i8 iKey0 = String::iToI8(sRow.a());
  xValWsp = iKey0;
  
  xKeyWsp.vAddElement(xValWsp);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // add key to entry
  xEntWsp.vAddKey(xKeyWsp);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // show
  xEntWsp.vShow();
  
  Entry xEntry(xEntWsp);
  
  // add entry into index buffer
  pIndexBuffer->vAddEntry(xEntry);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* index::ibNew -- new index buffer (temporary?)                              */
/*----------------------------------------------------------------------------*/
void Task::vIndexIbNew(NNP_Task nnpTask)
{
  ENTRY("vIndexIbNew");
  
  IndexBuffer* pIndexBuffer = new IndexBuffer;
  ASSERT(CN_INDEXBUFFER == pIndexBuffer->eClass());
  
  nnpTask->m_pDataStack->vPush(pIndexBuffer);

  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
