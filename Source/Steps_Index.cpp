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
  vAddPrimitive("index.indexDefn@",  &vIndexDefnStore,  false);
  vAddPrimitive("index.init",        &vIndexInit,       false);
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
  
  while (pIndexBuffer) {
    pIndexBuffer->vShow();
    
    Entry* pEntry = pIndexBuffer->pFirstEntry();
    
    while (pEntry) {
      pEntry->vContentDump();
      pEntry = pIndexBuffer->pNextEntry(pEntry);
    }
    pIndexBuffer = pIndexBuffer->pNextBuffer();
  }
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* index.indexDefn@ -- store the indexDefn in the index                       */
/*----------------------------------------------------------------------------*/
void Task::vIndexDefnStore(NNP_Task nnpTask)
{
  ENTRY("vIndexDefnStore");
  Object* pPtrIndex = nullptr;
  Object* pPtrIndexDefn = nullptr;
  
  if ( ! nnpTask->m_pDataStack->bPop(pPtrIndex) )
    THROW_INVALID_DSTACK("index.indexDefn@");
  
  Index* pIndex = _pIndex(pPtrIndex);
  ASSERT(CN_INDEX == pIndex->eClass());
  
  if ( ! nnpTask->m_pDataStack->bPop(pPtrIndexDefn) )
    THROW_INVALID_DSTACK("index.indexDefnStore");
  
  IndexDefn* pIndexDefn = _pIndexDefn(pPtrIndexDefn);
  ASSERT(CN_INDEXDEFN == pIndexDefn->eClass());
  
  pIndex->vIndexDefn(pIndexDefn);
  
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

  vTrace("index::entryTest -- %s\n",sRow.aStr());
  
  EntWsp xEntWsp;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // set rowId
  RowId xRowId;
  i8 iRowNo = String::iToI8(sRow.aStr());
  
  xRowId.vBuffer(999);
  xRowId.vRow(iRowNo);
  xEntWsp.vRowId(xRowId);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // add element to key
  KeyWsp xKeyWsp;
  ValWsp xValWsp;
  

  //i8 iKey0 = String::iToI8(sRow.aStr());
  i8 iKey0 = Random::iRandom(0,1000);

  xValWsp = iKey0;
  
  xKeyWsp.vAddElement(xValWsp);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // add key to entry
  xEntWsp.vAddKey(xKeyWsp);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // show
  //xEntWsp.vShow();
 
  // insert entry into index buffer
  //pIndexBuffer->vInsertEntry(xEntWsp);

  // verify index buffer
  //pIndexBuffer->vVerify();
  

  bool bResult = false;
  IndexBuffer* pFirstBuffer = pIndexBuffer;
  
  while (!bResult) {
    pIndexBuffer = pFirstBuffer->pFindBuffer(xEntWsp);
  
    // insert entry into index buffer
    bResult = pIndexBuffer->bInsertEntry(xEntWsp);
  }
 
  pIndexBuffer = pFirstBuffer;
  while (pIndexBuffer) {
    pIndexBuffer->vVerify();
    
    pIndexBuffer = pIndexBuffer->pNextBuffer();
  }

  
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

/*----------------------------------------------------------------------------*/
/* index::init -- initialize the index                                        */
/*----------------------------------------------------------------------------*/
void Task::vIndexInit(NNP_Task nnpTask)
{
  ENTRY("vIndexInit");
  Object* pPtrIndex = nullptr;
  
  if ( ! nnpTask->m_pDataStack->bPop(pPtrIndex) )
    THROW_INVALID_DSTACK("index::vIndexInit");
  
  Index* pIndex = _pIndex(pPtrIndex);
  ASSERT(CN_INDEX == pIndex->eClass());
  
  new(pIndex) Index;
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
