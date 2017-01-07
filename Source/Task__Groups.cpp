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

// loader: /////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_Loader(void)
{
  vAddPrimitive("loader.delete",      &vLoaderDelete,      false);
  vAddPrimitive("loader.findChunk",   &vLoaderFindChunk,   false);
  vAddPrimitive("loader.init",        &vLoaderInit,        false);
  vAddPrimitive("loader.new",         &vLoaderNew,         false);
  vAddPrimitive("loader.newSub",      &vLoaderNewSub,      false);
  vAddPrimitive("loader.path!",       &vLoaderPathStore,   false);
  vAddPrimitive("loader.table!",      &vLoaderTableStore,  false);
  vAddPrimitive("loader.txref!",      &vLoaderTxRefStore,  false);
}

/*----------------------------------------------------------------------------*/
/* loader.delete -- delete a table loader                                     */
/*----------------------------------------------------------------------------*/
void Task::vLoaderDelete(NNP_Task nnpTask)
{
  ENTRY("vLoade3Delete");
  
  Object* pPtrLoader;
  if ( ! nnpTask->m_xDataStack.bPop(pPtrLoader) )
    THROW_INVALID_DSTACK("loader.delete");
  
  Loader* pLoader = _pLoader(pPtrLoader);
  ASSERT(CN_LOADER == pLoader->eClass());
  
  delete pLoader;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* loader.findChunk -- find next chunk                                        */
/*----------------------------------------------------------------------------*/
void Task::vLoaderFindChunk(NNP_Task nnpTask)
{
  ENTRY("vLoaderFindChunk");
  
  Object* pPtrLoader;
  if ( ! nnpTask->m_xDataStack.bPop(pPtrLoader) )
    THROW_INVALID_DSTACK("loader.findChunk");
  
  Loader* pLoader = _pLoader(pPtrLoader);
  ASSERT(CN_LOADER == pLoader->eClass());
  
  bool bFoundChunk = pLoader->bFindChunk();
  
  nnpTask->m_xDataStack.vPush(bFoundChunk);
 
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* loader.init -- init a table loader                                         */
/*----------------------------------------------------------------------------*/
void Task::vLoaderInit(NNP_Task nnpTask)
{
  ENTRY("vLoaderInit");
  
  Object* pPtrLoader;
  if ( ! nnpTask->m_xDataStack.bPop(pPtrLoader) )
    THROW_INVALID_DSTACK("loader.init");
  
  Loader* pLoader = _pLoader(pPtrLoader);
  ASSERT(CN_LOADER == pLoader->eClass());
  
  new(pLoader) Loader;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* loader.new -- new a table loader                                           */
/*----------------------------------------------------------------------------*/
void Task::vLoaderNew(NNP_Task nnpTask)
{
  ENTRY("vLoaderNew");
  
  Loader* pLoader = new Loader;
  ASSERT(CN_LOADER == pLoader->eClass());
  
  nnpTask->m_xDataStack.vPush(pLoader);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* loader.newSub -- new a loader sub                                          */
/*----------------------------------------------------------------------------*/
void Task::vLoaderNewSub(NNP_Task nnpTask)
{
  ENTRY("vLoaderNewSub");
  
  Object* pPtrLoader;
  if ( ! nnpTask->m_xDataStack.bPop(pPtrLoader) )
    THROW_INVALID_DSTACK("loader.init");
  
  Loader* pLoader = _pLoader(pPtrLoader);
  ASSERT(CN_LOADER == pLoader->eClass());
  
  LoaderSub* pLoaderSub = new LoaderSub;
  ASSERT(CN_LOADERSUB == pLoaderSub->eClass());
  
  pLoaderSub->vLoader(pLoader);
  
  nnpTask->m_xDataStack.vPush(pLoaderSub);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* loader.path! -- store file path into loader                                */
/*----------------------------------------------------------------------------*/
void Task::vLoaderPathStore(NNP_Task nnpTask)
{
  ENTRY("vLoaderPathStore");
  
  ValWsp xWspPath;
  Object* pPtrLoader;
  if ( ! nnpTask->m_xDataStack.bPop(pPtrLoader,xWspPath) )
    THROW_INVALID_DSTACK("loader.path!");
  
  Loader* pLoader = _pLoader(pPtrLoader);
  ASSERT(CN_LOADER == pLoader->eClass());
  
  s256 sPath;
  sPath = xWspPath.sStr();
  
  pLoader->vPath(sPath);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* loader.table! -- store table into loader                                   */
/*----------------------------------------------------------------------------*/
void Task::vLoaderTableStore(NNP_Task nnpTask)
{
  ENTRY("vLoaderTableStore");
  
  Object* pPtrTable;
  Object* pPtrLoader;
  if ( ! nnpTask->m_xDataStack.bPop(pPtrLoader,pPtrTable) )
    THROW_INVALID_DSTACK("loader.tableStore");
  
  Loader* pLoader = _pLoader(pPtrLoader);
  ASSERT(CN_LOADER == pLoader->eClass());
  
  Table* pTable = _pTable(pPtrTable);
  
  ASSERT(CN_TABLE == pTable->eClass());
  
  pLoader->vTable(pTable);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* loader.txref! -- store txref into loader                                   */
/*----------------------------------------------------------------------------*/
void Task::vLoaderTxRefStore(NNP_Task nnpTask)
{
  ENTRY("vLoaderTxRefStore");
  
  Object* pPtrLoader;
  Object* pPtrTxRef;
  if ( ! nnpTask->m_xDataStack.bPop(pPtrLoader,pPtrTxRef) )
    THROW_INVALID_DSTACK("loader.txref!");
  
  Loader* pLoader = _pLoader(pPtrLoader);
  ASSERT(CN_LOADER == pLoader->eClass());
  
  TxRef* pTxRef = _pTxRef(pPtrTxRef);
  ASSERT(CN_TXREF == pTxRef->eClass());
  
  pLoader->vTxRef(pTxRef);
  
  EXIT();
}

// loaderSub: //////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_LoaderSub(void)
{
  vAddPrimitive("loaderSub.appendBuffer",&vLoaderSubAppendBuffer,false);
  vAddPrimitive("loaderSub.begin",       &vLoaderSubBegin,       false);
  vAddPrimitive("loaderSub.delete",      &vLoaderSubDelete,      false);
  vAddPrimitive("loaderSub.end",         &vLoaderSubEnd,         false);
  vAddPrimitive("loaderSub.fetchRecord", &vLoaderSubFetchRecord, false);
  vAddPrimitive("loaderSub.hashPartition",&vLoaderSubHashPartition, false);
  vAddPrimitive("loaderSub.hashRecord",  &vLoaderSubHashRecord,  false);
  vAddPrimitive("loaderSub.init",        &vLoaderSubInit,        false);
  vAddPrimitive("loaderSub.insertRow",   &vLoaderSubInsertRow,   false);
  vAddPrimitive("loaderSub.insertRow?",  &vLoaderSubInsertRow2,  false);
  vAddPrimitive("loaderSub.loader!",     &vLoaderSubLoaderStore, false);
  vAddPrimitive("loaderSub.new",         &vLoaderSubNew,         false);
  vAddPrimitive("loaderSub.partition",   &vLoaderSubPartition,   false);
  vAddPrimitive("loaderSub.partition!",  &vLoaderSubPartitionStore, false);
  vAddPrimitive("loaderSub.rowBuffer",   &vLoaderSubRowBuffer,   false);
  vAddPrimitive("loaderSub.rowBuffer!",  &vLoaderSubRowBufferStore, false);
  vAddPrimitive("loaderSub.table",       &vLoaderSubTable,       false);
}

/*----------------------------------------------------------------------------*/
/* loaderSub.appendBuffer -- append buffer into table                         */
/*----------------------------------------------------------------------------*/
void Task::vLoaderSubAppendBuffer(NNP_Task nnpTask)
{
  ENTRY("vLoaderSubAppendBuffer");
  
  LoaderSub* pLoaderSub = _pLoaderSub(nnpTask->m_pRegFrame->pValReg(0)->pPtr());
  ASSERT(CN_LOADERSUB == pLoaderSub->eClass());
  
  pLoaderSub->vAppendBuffer();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* loaderSub.begin -- begin loading                                           */
/*----------------------------------------------------------------------------*/
void Task::vLoaderSubBegin(NNP_Task nnpTask)
{
  ENTRY("vLoaderSubBegin");
  
  LoaderSub* pLoaderSub = _pLoaderSub(nnpTask->m_pRegFrame->pValReg(0)->pPtr());
  ASSERT(CN_LOADERSUB == pLoaderSub->eClass());
  
  pLoaderSub->vBegin();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* loaderSub.delete -- delete a table loader                                  */
/*----------------------------------------------------------------------------*/
void Task::vLoaderSubDelete(NNP_Task nnpTask)
{
  ENTRY("vLoade3SubDelete");
  
  LoaderSub* pLoaderSub = _pLoaderSub(nnpTask->m_pRegFrame->pValReg(0)->pPtr());
  ASSERT(CN_LOADERSUB == pLoaderSub->eClass());
  
  delete pLoaderSub;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* loaderSub.end -- end loading                                               */
/*----------------------------------------------------------------------------*/
void Task::vLoaderSubEnd(NNP_Task nnpTask)
{
  ENTRY("vLoaderSubEnd");
  
  LoaderSub* pLoaderSub = _pLoaderSub(nnpTask->m_pRegFrame->pValReg(0)->pPtr());
  ASSERT(CN_LOADERSUB == pLoaderSub->eClass());
  
  pLoaderSub->vEnd();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* loaderSub.fetchRecord -- fetch next record from file being loaded          */
/*----------------------------------------------------------------------------*/
void Task::vLoaderSubFetchRecord(NNP_Task nnpTask)
{
  ENTRY("vLoaderSubFetchRecord");
  
  LoaderSub* pLoaderSub = _pLoaderSub(nnpTask->m_pRegFrame->pValReg(0)->pPtr());
  ASSERT(CN_LOADERSUB == pLoaderSub->eClass());
  
  bool bFetched = pLoaderSub->bFetchRecord();
  
  nnpTask->m_xDataStack.vPush(bFetched);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* loaderSub.hashPartition -- hash partition & get node where partition lives */
/*----------------------------------------------------------------------------*/
void Task::vLoaderSubHashPartition(NNP_Task nnpTask)
{
  ENTRY("vLoaderSubHashRecord");
  
  LoaderSub* pLoaderSub = _pLoaderSub(nnpTask->m_pRegFrame->pValReg(0)->pPtr());
  ASSERT(CN_LOADERSUB == pLoaderSub->eClass());

  i8 iNode = pLoaderSub->iPartition() % gxArguments.m_nNodes;
  
  nnpTask->m_xDataStack.vPush(iNode);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* loaderSub.hashRecord -- hash record being loaded                           */
/*----------------------------------------------------------------------------*/
void Task::vLoaderSubHashRecord(NNP_Task nnpTask)
{
  ENTRY("vLoaderSubHashRecord");
  
  LoaderSub* pLoaderSub = _pLoaderSub(nnpTask->m_pRegFrame->pValReg(0)->pPtr());
  ASSERT(CN_LOADERSUB == pLoaderSub->eClass());
  
  pLoaderSub->vHashRecord();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* loaderSub.init -- init a table loader                                      */
/*----------------------------------------------------------------------------*/
void Task::vLoaderSubInit(NNP_Task nnpTask)
{
  ENTRY("vLoaderInit");
  
  Object* pPtrLoaderSub;
  if ( ! nnpTask->m_xDataStack.bPop(pPtrLoaderSub) )
    THROW_INVALID_DSTACK("loaderSub.init");
  
  LoaderSub* pLoaderSub = _pLoaderSub(pPtrLoaderSub);
  ASSERT(CN_LOADERSUB == pLoaderSub->eClass());
  
  new(pLoaderSub) LoaderSub;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* loaderSub.insertRow -- insert row into buffer (buffer into table when full)*/
/*----------------------------------------------------------------------------*/
void Task::vLoaderSubInsertRow(NNP_Task nnpTask)
{
  ENTRY("vLoaderSubInsertRow");
  
  LoaderSub* pLoaderSub = _pLoaderSub(nnpTask->m_pRegFrame->pValReg(0)->pPtr());
  ASSERT(CN_LOADERSUB == pLoaderSub->eClass());
  
  bool bInserted = pLoaderSub->bInsertRow();
  
  nnpTask->m_xDataStack.vPush(bInserted);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* loaderSub.insertRow2 -- insert row into buffer                             */
/*----------------------------------------------------------------------------*/
void Task::vLoaderSubInsertRow2(NNP_Task nnpTask)
{
  ENTRY("vLoaderSubInsertRow?");
  
  LoaderSub* pLoaderSub = _pLoaderSub(nnpTask->m_pRegFrame->pValReg(0)->pPtr());
  ASSERT(CN_LOADERSUB == pLoaderSub->eClass());
  
  bool bInserted = pLoaderSub->bInsertRow2();
  
  nnpTask->m_xDataStack.vPush(bInserted);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* loaderSub.loader! -- store loader                                          */
/*----------------------------------------------------------------------------*/
void Task::vLoaderSubLoaderStore(NNP_Task nnpTask)
{
  ENTRY("vLoaderSubLoaderStore");
  
  Object* pPtrLoaderSub;
  if ( ! nnpTask->m_xDataStack.bPop(pPtrLoaderSub) )
    THROW_INVALID_DSTACK("loaderSub.loader!");
  
  LoaderSub* pLoaderSub = _pLoaderSub(pPtrLoaderSub);
  ASSERT(CN_LOADERSUB == pLoaderSub->eClass());
  
  Object* pPtrLoader;
  if ( ! nnpTask->m_xDataStack.bPop(pPtrLoader) )
    THROW_INVALID_DSTACK("loader.openfile");
  
  Loader* pLoader = _pLoader(pPtrLoader);
  ASSERT(CN_LOADER == pLoader->eClass());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* loaderSub.new -- new a table loader subordinate                            */
/*----------------------------------------------------------------------------*/
void Task::vLoaderSubNew(NNP_Task nnpTask)
{
  ENTRY("vLoaderSubNew");
  
  LoaderSub* pLoaderSub = new LoaderSub;
  ASSERT(CN_LOADERSUB == pLoaderSub->eClass());
  
  nnpTask->m_xDataStack.vPush(pLoaderSub);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* loaderSub.partition -- get partition number derived from hash of record    */
/*----------------------------------------------------------------------------*/
void Task::vLoaderSubPartition(NNP_Task nnpTask)
{
  ENTRY("vLoaderSubPartition");
  
  LoaderSub* pLoaderSub = _pLoaderSub(nnpTask->m_pRegFrame->pValReg(0)->pPtr());
  ASSERT(CN_LOADERSUB == pLoaderSub->eClass());
  
  i8 iPartition = pLoaderSub->iPartition();
  
  nnpTask->m_xDataStack.vPush(iPartition);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* loaderSub.partition! -- store partition number                             */
/*----------------------------------------------------------------------------*/
void Task::vLoaderSubPartitionStore(NNP_Task nnpTask)
{
  ENTRY("vLoaderSubPartitionStore");
  
  i8 iPartition;
  if ( ! nnpTask->m_xDataStack.bPop(iPartition) )
    THROW_INVALID_DSTACK("loaderSub.partition!");
 
  LoaderSub* pLoaderSub = _pLoaderSub(nnpTask->m_pRegFrame->pValReg(0)->pPtr());
  ASSERT(CN_LOADERSUB == pLoaderSub->eClass());

  pLoaderSub->vPartition(iPartition);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* loaderSub.rowBuffer -- get row buffer of given partition                   */
/*----------------------------------------------------------------------------*/
void Task::vLoaderSubRowBuffer(NNP_Task nnpTask)
{
  ENTRY("vLoaderSubRowBuffer");
  
  LoaderSub* pLoaderSub = _pLoaderSub(nnpTask->m_pRegFrame->pValReg(0)->pPtr());
  ASSERT(CN_LOADERSUB == pLoaderSub->eClass());
  
  RowBuffer* pRowBuffer = pLoaderSub->pRowBuffer();
  
  nnpTask->m_xDataStack.vPush(pRowBuffer);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* loaderSub.rowBuffer! -- store row buffer                                   */
/*----------------------------------------------------------------------------*/
void Task::vLoaderSubRowBufferStore(NNP_Task nnpTask)
{
  ENTRY("vLoaderSubRowBufferStore");
  
  Object* pPtrRowBuffer;
  if ( ! nnpTask->m_xDataStack.bPop(pPtrRowBuffer) )
    THROW_INVALID_DSTACK("loaderSub.rowBuffer!");
  
  RowBuffer* pRowBuffer = _pRowBuffer(pPtrRowBuffer);
  ASSERT( !pRowBuffer || CN_ROWBUFFER == pRowBuffer->eClass() );
  
  LoaderSub* pLoaderSub = _pLoaderSub(nnpTask->m_pRegFrame->pValReg(0)->pPtr());
  ASSERT(CN_LOADERSUB == pLoaderSub->eClass());
  
  pLoaderSub->vRowBuffer(pRowBuffer);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* loaderSub.table -- get table being loaded                                  */
/*----------------------------------------------------------------------------*/
void Task::vLoaderSubTable(NNP_Task nnpTask)
{
  ENTRY("vLoaderSubTable");
  
  LoaderSub* pLoaderSub = _pLoaderSub(nnpTask->m_pRegFrame->pValReg(0)->pPtr());
  ASSERT(CN_LOADERSUB == pLoaderSub->eClass());
  
  Table* pTable = pLoaderSub->pTable();
  
  nnpTask->m_xDataStack.vPush(pTable);
  
  EXIT();
}

// local: //////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_Local(void)
{
  vAddPrimitive("local.fetch",     &vLocalFetch,       false);
  vAddPrimitive("local.store",     &vLocalStore,       false);
  vAddPrimitive("locals|",         &vLocalsBar,        true);
  vAddPrimitive("locals.pop",      &vLocalsPop,        false);
  vAddPrimitive("locals.push",     &vLocalsPush,       false);
  vAddPrimitive("to",              &vLocalsTo,         true);
}

/*----------------------------------------------------------------------------*/
/* local.fetch -- push locals[n] onto dstack                                  */
/*----------------------------------------------------------------------------*/
void Task::vLocalFetch(NNP_Task nnpTask)
{
  ENTRY("vLocalFetch");
  
  if ( ! nnpTask->m_pLocalsFrame )
    THROW_INVALID_DSTACK("local.fetch");
  
  i8 iLocal;
  if ( ! nnpTask->m_xDataStack.bPop(iLocal) )
    THROW_INVALID_DSTACK("local.fetch");
  
  ValReg* pValReg = nnpTask->m_pLocalsFrame->pValReg(iLocal);
  
  nnpTask->m_xDataStack.vPush(pValReg);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* localStore -- pop dstack into locals[n]                                    */
/*----------------------------------------------------------------------------*/
void Task::vLocalStore(NNP_Task nnpTask)
{
  ENTRY("vLocalStore");
  
  if ( ! nnpTask->m_pLocalsFrame )
    THROW_INVALID_DSTACK("localStore");
  
  i8 iLocal;
  if ( ! nnpTask->m_xDataStack.bPop(iLocal) )
    THROW_INVALID_DSTACK("localStore");
  
  ValReg* pValReg = nnpTask->m_pLocalsFrame->pValReg(iLocal);
  
  nnpTask->m_xDataStack.bPop(pValReg);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* locals| -- declare set of locals                                immediate  */
/*----------------------------------------------------------------------------*/
void Task::vLocalsBar(NNP_Task nnpTask)
{
ENTRY("vLocalsBar");
  
  Word* pCurrentWord = nnpTask->pCurrentWord();
  
  Dic* pDic = nnpTask->pDicPeek();

  ValWsp xValWsp;
  
  for (;;) {
    
    // get name of local
    nnpTask->vTokenNext();
    
    // end of locals declaration?
    if (String::bEqualTo("|",nnpTask->m_sToken.a()))
      break;

    // test for first use of locals|
    if ( ! nnpTask->m_nLocals ) {
      
      // create a locals dictionary
      pDic = new Dic;
      nnpTask->vDicPush(pDic);
      
      // enocde locals frame push
      pCurrentWord->vEncode("locals.push");
    }
    
    // create new word for local in current dictionary
    Word* pWordLocal = new(pDic) Word(nnpTask->m_sToken.a());
    // ** do not set as current word **
    
    // set word as local variable
    pWordLocal->vWordType(WT_LOCAL);
    pWordLocal->vLocal(nnpTask->m_nLocals);
    
    // encode the word
    xValWsp = nnpTask->m_nLocals;
    pWordLocal->vEncode("pushValue",&xValWsp);
    pWordLocal->vEncode("local.fetch");
    pWordLocal->vEncode("exit");

    // initialise the local from dstack
    pCurrentWord->vEncode("pushValue",&xValWsp);
    pCurrentWord->vEncode("local.store");
    
    nnpTask->m_nLocals++;
  }
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* locals.pop -- pop locals frame                                             */
/*----------------------------------------------------------------------------*/
void Task::vLocalsPop(NNP_Task nnpTask)
{
ENTRY("vLocalsPop");
  
  RegFrame* pLocalsFrame = nnpTask->m_pLocalsFrame;
  
  if ( ! pLocalsFrame )
    THROW_INVALID_DSTACK("locals.pop");
  
  nnpTask->m_pLocalsFrame = nnpTask->m_pLocalsFrame->pNext();
  
  delete pLocalsFrame;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* locals.push -- push locals frame                                           */
/*----------------------------------------------------------------------------*/
void Task::vLocalsPush(NNP_Task nnpTask)
{
ENTRY("vLocalsPush");
  
  RegFrame* pLocalsFrame = new RegFrame;
  
  pLocalsFrame->vStackPtr(nnpTask->m_xReturnStack.pTopValue());
  
  pLocalsFrame->vNext(nnpTask->m_pLocalsFrame);
  
  nnpTask->m_pLocalsFrame = pLocalsFrame;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* to -- compile store value into local variable                    immediate */
/*----------------------------------------------------------------------------*/
void Task::vLocalsTo(NNP_Task nnpTask)
{
ENTRY("vLocalsTo");
  
  Word* pCurrentWord = nnpTask->pCurrentWord();
  
  // get name of local
  nnpTask->vTokenNext();

  Word* pWordLocal = nnpTask->pFindWord(nnpTask->m_sToken.a());
  ASSERT(pWordLocal);
 
  ValWsp xValWsp;
  xValWsp = pWordLocal->iLocal();
  
  pCurrentWord->vEncode("pushValue",&xValWsp);
  pCurrentWord->vEncode("local.store");

  EXIT();
}

// lock: ///////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_Lock(void)
{
  vAddPrimitive("lock.dropRead",   &vLockDropRead,     false);
  vAddPrimitive("lock.dropWrite",  &vLockDropWrite,    false);
  vAddPrimitive("lock.grabRead",   &vLockGrabRead,     false);
  vAddPrimitive("lock.grabWrite",  &vLockGrabWrite,    false);
  vAddPrimitive("lock.init",       &vLockInit,         false);
}

/*----------------------------------------------------------------------------*/
/* lock.dropread -- drop read lock                                            */
/*----------------------------------------------------------------------------*/
void Task::vLockDropRead(NNP_Task nnpTask)
{
ENTRY("vLockDropRead");
  
  Object* pPtrLock;;
  if ( ! nnpTask->m_xDataStack.bPop(pPtrLock) )
    THROW_INVALID_DSTACK("lock.dropread");
  
  Lock* pLock = _pLock(pPtrLock);
  ASSERT(pLock);
  ASSERT(CN_LOCK == pLock->eClass());
  
  pLock->vDropRead();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* lock.dropwrite -- drop write lock                                          */
/*----------------------------------------------------------------------------*/
void Task::vLockDropWrite(NNP_Task nnpTask)
{
ENTRY("vLockDropWrite");

  Object* pPtrLock;;
  if ( ! nnpTask->m_xDataStack.bPop(pPtrLock) )
    THROW_INVALID_DSTACK("lock.dropwrite");
  
  Lock* pLock = _pLock(pPtrLock);
  ASSERT(pLock);
  ASSERT(CN_LOCK == pLock->eClass());

  pLock->vDropWrite();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* lock.grabread -- grab read lock                                            */
/*----------------------------------------------------------------------------*/
void Task::vLockGrabRead(NNP_Task nnpTask)
{
ENTRY("vLockGrabRead");

  Object* pPtrLock;;
  if ( ! nnpTask->m_xDataStack.bPop(pPtrLock) )
    THROW_INVALID_DSTACK("lock.grabread");
  
  Lock* pLock = _pLock(pPtrLock);
  ASSERT(pLock);
  ASSERT(CN_LOCK == pLock->eClass());

  pLock->vGrabRead();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* lock.grabwrite -- grab write lock                                          */
/*----------------------------------------------------------------------------*/
void Task::vLockGrabWrite(NNP_Task nnpTask)
{
ENTRY("vLockGrabWrite");

  Object* pPtrLock;;
  if ( ! nnpTask->m_xDataStack.bPop(pPtrLock) )
    THROW_INVALID_DSTACK("lock.grabwrite");
  
  Lock* pLock = _pLock(pPtrLock);
  ASSERT(pLock);
  ASSERT(CN_LOCK == pLock->eClass());
  
  pLock->vGrabWrite();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* lock.init -- initialise lock                                               */
/*----------------------------------------------------------------------------*/
void Task::vLockInit(NNP_Task nnpTask)
{
ENTRY("vLockInit");
  
  Object* pPtrLock;;
  if ( ! nnpTask->m_xDataStack.bPop(pPtrLock) )
    THROW_INVALID_DSTACK("lock.init");
  
  Lock* pLock = _pLock(pPtrLock);
  
  new(pLock) Lock;
  
  ASSERT(pLock);
  ASSERT(CN_LOCK == pLock->eClass());
  
  EXIT();
}

// logic: //////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_Logic(void)
{
  vAddPrimitive("and",             &vLogicAnd,         false);
  vAddPrimitive("not",             &vLogicNot,         false);
  vAddPrimitive("or",              &vLogicOr,          false);
}

/*----------------------------------------------------------------------------*/
/* and -- logical and of top two values                                       */
/*----------------------------------------------------------------------------*/
void Task::vLogicAnd(NNP_Task nnpTask)
{
  ENTRY("vLogicAnd");
  
  i8 iArgB;
  if ( ! nnpTask->m_xDataStack.bPopParamInt(iArgB) )
    THROW_INVALID_DSTACK("and");
  
  i8 iArgA;
  if ( ! nnpTask->m_xDataStack.bPopParamInt(iArgA) )
    THROW_INVALID_DSTACK("and");
  
  i8 iArgR = (iArgA && iArgB) ? 1 : 0;
  
  nnpTask->m_xDataStack.vPush(iArgR);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* not -- not of top value                                                    */
/*----------------------------------------------------------------------------*/
void Task::vLogicNot(NNP_Task nnpTask)
{
  ENTRY("vLogicNot");
  
  i8 iValA;
  if ( ! nnpTask->m_xDataStack.bPopParamInt(iValA) )
    THROW_INVALID_DSTACK("not");
  
  i8 iNotA = iValA ? 0 : 1;
  
  nnpTask->m_xDataStack.vPush(iNotA);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* or -- logical or of top two values                                         */
/*----------------------------------------------------------------------------*/
void Task::vLogicOr(NNP_Task nnpTask)
{
  ENTRY("vLogicOr");
  
  i8 iArgB;
  if ( ! nnpTask->m_xDataStack.bPopParamInt(iArgB) )
    THROW_INVALID_DSTACK("or");
  
  i8 iArgA;
  if ( ! nnpTask->m_xDataStack.bPopParamInt(iArgA) )
    THROW_INVALID_DSTACK("or");
  
  i8 iArgR = (iArgA || iArgB) ? 1 : 0;
  
  nnpTask->m_xDataStack.vPush(iArgR);
  
  EXIT();
}

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
  if ( ! nnpTask->m_xDataStack.bPop(iSize) )
    THROW_INVALID_DSTACK("memory.bytedump");
  
  Object* pWordObject;
  if ( ! nnpTask->m_xDataStack.bPop(pWordObject) )
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
  if ( ! nnpTask->m_xDataStack.bPop(iSize) )
    THROW_INVALID_DSTACK("memory.worddump");
  
  Object* pWordObject;
  if ( ! nnpTask->m_xDataStack.bPop(pWordObject) )
    THROW_INVALID_DSTACK("memory.worddump");
  Word* pWord = _pWord(pWordObject);
  
  //pWord->vWordDump(iSize);
  Memory::vWordDump(pWord,iSize);
  
  EXIT();
}

// node: ///////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_Node(void)
{
  vAddPrimitive("node.init",       &vNodeInit,         false);
  vAddPrimitive("node.new",        &vNodeNew,          false);
  vAddPrimitive("node.no",         &vNodeNo,           false);
  vAddPrimitive("node.status",     &vNodeStatus,       false);
}

/*----------------------------------------------------------------------------*/
/* node.init -- initialise a node (of a grid)                                 */
/*----------------------------------------------------------------------------*/
void Task::vNodeInit(NNP_Task nnpTask)
{
ENTRY("vNodeInit");
  
  Object* pPtrNode;
  
  if ( ! nnpTask->m_xDataStack.bPop(pPtrNode) )
    THROW_INVALID_DSTACK("node.init");
  
  Node* pNode = _pNode(pPtrNode);
  ASSERT(CN_NODE == pNode->eClass());
  
  new(pNode) Node;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* node.new -- new a node (of a grid)                                         */
/*----------------------------------------------------------------------------*/
void Task::vNodeNew(NNP_Task nnpTask)
{
ENTRY("vNodeNew");
  
  i8 iNodeNo;
  
  if ( ! nnpTask->m_xDataStack.bPop(iNodeNo) )
    THROW_INVALID_DSTACK("node.new");
  
  Node* pNode = new Node;
  ASSERT(CN_NODE == pNode->eClass());

  pNode->vNodeNo(iNodeNo);
  
  nnpTask->m_xDataStack.vPush(pNode);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* node.no -- push node number of node (of a grid)                            */
/*----------------------------------------------------------------------------*/
void Task::vNodeNo(NNP_Task nnpTask)
{
ENTRY("vNodeNo");
  
  Object* pPtrNode;
  
  if ( ! nnpTask->m_xDataStack.bPop(pPtrNode) )
    THROW_INVALID_DSTACK("node.no");
  
  Node* pNode = _pNode(pPtrNode);
  ASSERT(CN_NODE == pNode->eClass());
  
  nnpTask->m_xDataStack.vPush(pNode->iNodeNo());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* node.status -- get status of this node (of a grid)                         */
/*----------------------------------------------------------------------------*/
void Task::vNodeStatus(NNP_Task nnpTask)
{
  ENTRY("vNodeStatus");

  s256 s;
  
  if (Region::bTest(gxArguments.m_sRegionName))
    String::vSprint(s.a(),"RapidsSE instance %s running", gxArguments.m_sRegionName.a());
  else
    String::vSprint(s.a(),"RapidsSE instance %s not running",gxArguments.m_sRegionName.a());
  
  nnpTask->m_xDataStack.vPush(s);
  
  EXIT();
}

// nodecursor: /////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_NodeCursor(void)
{
  vAddPrimitive("nodeCursor.begin",  &vNodeCursorBegin,  false);
  vAddPrimitive("nodeCursor.delete", &vNodeCursorDelete, false);
  vAddPrimitive("nodeCursor.first",  &vNodeCursorFirst,  false);
  vAddPrimitive("nodeCursor.get",    &vNodeCursorGet,    false);
  vAddPrimitive("nodeCursor.init",   &vNodeCursorInit,   false);
  vAddPrimitive("nodeCursor.new",    &vNodeCursorNew,    false);
  vAddPrimitive("nodeCursor.next",   &vNodeCursorNext,   false);
}

/*----------------------------------------------------------------------------*/
/* nodecursor.begin -- begin using node (of grid) cursor                      */
/*----------------------------------------------------------------------------*/
void Task::vNodeCursorBegin(NNP_Task nnpTask)
{
ENTRY("vNodeCursorBegin");
  
  Object* pPtrNodeCursor;
  if ( ! nnpTask->m_xDataStack.bPop(pPtrNodeCursor) )
    THROW_INVALID_DSTACK("nodeCursor.begin");
  
  NodeCursor* pNodeCursor = _pNodeCursor(pPtrNodeCursor);
  ASSERT(CN_NODECURSOR == pNodeCursor->eClass());
  
  pNodeCursor->vBegin();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* nodecursor.delete -- delete a node (of grid) cursor                        */
/*----------------------------------------------------------------------------*/
void Task::vNodeCursorDelete(NNP_Task nnpTask)
{
ENTRY("vNodeCursorDelete");
  
  Object* pPtrNodeCursor;
  if ( ! nnpTask->m_xDataStack.bPop(pPtrNodeCursor) )
    THROW_INVALID_DSTACK("nodeCursor.delete");
  
  NodeCursor* pNodeCursor = _pNodeCursor(pPtrNodeCursor);
  ASSERT(CN_NODECURSOR == pNodeCursor->eClass());
  
  delete pNodeCursor;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* nodecursor.end -- end using node (of grid) cursor                          */
/*----------------------------------------------------------------------------*/
void Task::vNodeCursorEnd(NNP_Task nnpTask)
{
ENTRY("vNodeCursorEnd");
  
  Object* pPtrNodeCursor;
  if ( ! nnpTask->m_xDataStack.bPop(pPtrNodeCursor) )
    THROW_INVALID_DSTACK("nodecursor.end");
  
  NodeCursor* pNodeCursor = _pNodeCursor(pPtrNodeCursor);
  ASSERT(CN_NODECURSOR == pNodeCursor->eClass());
  
  pNodeCursor->vEnd();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* nodeCursor.first -- set to first and push node (of grid)                   */
/*----------------------------------------------------------------------------*/
void Task::vNodeCursorFirst(NNP_Task nnpTask)
{
ENTRY("vNodeCursorFirst");
  
  Object* pPtrNodeCursor;
  if ( ! nnpTask->m_xDataStack.bPop(pPtrNodeCursor) )
    THROW_INVALID_DSTACK("nodeCursor.first");
  
  NodeCursor* pNodeCursor = _pNodeCursor(pPtrNodeCursor);
  ASSERT(CN_NODECURSOR == pNodeCursor->eClass());
  
  pNodeCursor->vFirst();
  
  Object* pObject = pNodeCursor->pObject();
  
  nnpTask->m_xDataStack.vPush(pObject);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* nodeCursor.get -- push current node (of grid)                              */
/*----------------------------------------------------------------------------*/
void Task::vNodeCursorGet(NNP_Task nnpTask)
{
ENTRY("vNodeCursorGet");
  
  Object* pPtrNodeCursor;
  if ( ! nnpTask->m_xDataStack.bPop(pPtrNodeCursor) )
    THROW_INVALID_DSTACK("nodeCursor.get");
  
  NodeCursor* pNodeCursor = _pNodeCursor(pPtrNodeCursor);
  ASSERT(CN_NODECURSOR == pNodeCursor->eClass());
  
  Object* pObject = pNodeCursor->pObject();
  
  nnpTask->m_xDataStack.vPush(pObject);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* nodeCursor.init -- init a node cursor                                      */
/*----------------------------------------------------------------------------*/
void Task::vNodeCursorInit(NNP_Task nnpTask)
{
ENTRY("vNodeCursorInit");
  
  Object* pPtrNodeCursor;
  if ( ! nnpTask->m_xDataStack.bPop(pPtrNodeCursor) )
    THROW_INVALID_DSTACK("nodeCursor.init");
  
  NodeCursor* pNodeCursor = _pNodeCursor(pPtrNodeCursor);
  ASSERT(CN_NODECURSOR == pNodeCursor->eClass());
  
  new(pNodeCursor) NodeCursor(gpGrid);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* nodeCursor.new -- new a node (of grid) cursor                              */
/*----------------------------------------------------------------------------*/
void Task::vNodeCursorNew(NNP_Task nnpTask)
{
ENTRY("vNodeCursorNew");
  
  NodeCursor* pNodeCursor = new NodeCursor(gpGrid);
  ASSERT(CN_NODECURSOR == pNodeCursor->eClass());
  
  nnpTask->m_xDataStack.vPush(pNodeCursor);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* nodeCursor.next -- advance to next and get node (of grid)                  */
/*----------------------------------------------------------------------------*/
void Task::vNodeCursorNext(NNP_Task nnpTask)
{
ENTRY("vNodeCursorNext");
  
  Object* pPtrNodeCursor;
  if ( ! nnpTask->m_xDataStack.bPop(pPtrNodeCursor) )
    THROW_INVALID_DSTACK("nodeCursor.next");
  
  NodeCursor* pNodeCursor = _pNodeCursor(pPtrNodeCursor);
  ASSERT(CN_NODECURSOR == pNodeCursor->eClass());
  
  pNodeCursor->vNext();
  
  Object* pObject = pNodeCursor->pObject();
  
  nnpTask->m_xDataStack.vPush(pObject);
  
  EXIT();
}

// object: /////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_Object(void)
{
  vAddPrimitive("object.class@",     &vObjectClassFetch, false);
  vAddPrimitive("object.class!",     &vObjectClassStore, false);
  vAddPrimitive("object@",           &vObjectFetch,      false);
  vAddPrimitive("object.magic!!",    &vObjectMagicMark,  false);
  vAddPrimitive("object!",           &vObjectStore,      false);
  vAddPrimitive("object.wordbody!",  &vObjectWordBodySet,false);
}

/*----------------------------------------------------------------------------*/
/* object.class@ -- fetch class number from object                            */
/*----------------------------------------------------------------------------*/
void Task::vObjectClassFetch(NNP_Task nnpTask)
{
  ENTRY("vObjectClassFetch");
  
  Object* pObject;
  if ( ! nnpTask->m_xDataStack.bPop(pObject) )
    THROW_INVALID_DSTACK("object.class@");
  
  eCN eClassNo;
  eClassNo = pObject->eClass();
  
  nnpTask->m_xDataStack.vPush(eClassNo);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* object.class! -- store class number into object                            */
/*----------------------------------------------------------------------------*/
void Task::vObjectClassStore(NNP_Task nnpTask)
{
  ENTRY("vObjectClassStore");
  
  Object* pObject;
  if ( ! nnpTask->m_xDataStack.bPop(pObject) )
    THROW_INVALID_DSTACK("object.object.class!");
  
  eCN eClassNo;
  if ( ! nnpTask->m_xDataStack.bPop(eClassNo) )
    THROW_INVALID_DSTACK("object.object.class!");
  
  pObject->vClass(eClassNo);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* object.dump -- dump the given object                                       */
/*----------------------------------------------------------------------------*/
void Task::vObjectDump(NNP_Task nnpTask)
{
  ENTRY("vObjectDump");
  
  Object* pObject;
  if ( ! nnpTask->m_xDataStack.bPop(pObject) )
    THROW_INVALID_DSTACK("object.dump");
  
  eCN eClass = pObject->eClass();
  
  pDUMPFUN* pDumpFun =gpDump[eClass];
  
  (pDumpFun)(pObject->eClass(), pObject, pSingleTab());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* object@ -- fetch object from return stack onto param stack                 */
/*----------------------------------------------------------------------------*/
void Task::vObjectFetch(NNP_Task nnpTask)
{
  ENTRY("vObjectFetch");
  
  i8 iPick = 0;
  
  ValWsp xObjectWsp;
  
  nnpTask->m_xReturnStack.vPeek(iPick,&xObjectWsp);
  
  nnpTask->m_xDataStack.vPush(xObjectWsp);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* object.magic!! -- put magic number into object                             */
/*----------------------------------------------------------------------------*/
void Task::vObjectMagicMark(NNP_Task nnpTask)
{
  ENTRY("vObjectMagicMark");
  
  Object* pObject;
  if ( ! nnpTask->m_xDataStack.bPop(pObject) )
    THROW_INVALID_RSTACK("object.magic!!");
  
  pObject->vSetMagic();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* object! -- replace object on return stack from param stack                 */
/*----------------------------------------------------------------------------*/
void Task::vObjectStore(NNP_Task nnpTask)
{
  ENTRY("vObjectStore");
  
  i8 iPeek = 0;
  
  ValWsp xObjectWsp;
  
  if ( ! nnpTask->m_xDataStack.bPop(xObjectWsp) )
    THROW_INVALID_DSTACK("object!");
  
  nnpTask->m_xReturnStack.vPoke(iPeek,&xObjectWsp);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* object.wordbody! -- mark object as a part of a wordbody                    */
/*----------------------------------------------------------------------------*/
void Task::vObjectWordBodySet(NNP_Task nnpTask)
{
  ENTRY("vObjectWordBodySet");
  
  Object* pObject;
  if ( ! nnpTask->m_xDataStack.bPop(pObject) )
    THROW_INVALID_DSTACK("object.wordbody!");
  
  i8 iIsBody;
  if ( ! nnpTask->m_xDataStack.bPop(iIsBody) )
    THROW_INVALID_DSTACK("object.object.class!");
  
  pObject->vWordBody(iIsBody);
  
  EXIT();
}

// process: ////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_Process(void)
{
  vAddPrimitive("process.getPid",  &vProcessGetPid,    false);
  vAddPrimitive("process.quit",    &vProcessQuit,      false);
  vAddPrimitive("process.testPid", &vProcessTestPid,   false);
}

/*----------------------------------------------------------------------------*/
/* process.getpid -- get pid of this process                                  */
/*----------------------------------------------------------------------------*/
void Task::vProcessGetPid(NNP_Task nnpTask)
{
ENTRY("vProcessGetPid");
  
  i8 iPid = Process::iGetPid();
  
  nnpTask->m_xDataStack.vPush(iPid);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* process.quit -- process exits early                                        */
/*----------------------------------------------------------------------------*/
void Task::vProcessQuit(NNP_Task nnpTask)
{
  ENTRY("vProcessQuit");
  
  exit(EXIT_FAILURE);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* process.testpid -- test pid on stack                                       */
/*----------------------------------------------------------------------------*/
void Task::vProcessTestPid(NNP_Task nnpTask)
{
ENTRY("vProcessTestPid");

  i8 iPid;
  if ( ! nnpTask->m_xDataStack.bPop(iPid) )
    THROW_INVALID_DSTACK("process.testpid");
  
  i8 iExists = Process::bTestPid(iPid);
  
  nnpTask->m_xDataStack.vPush(iExists);
  
  EXIT();
}

// dstack: /////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_Dstack(void)
{
  vAddPrimitive("stack.class",     &vDstackClass,      false);
  vAddPrimitive("depth",           &vDstackDepth,      false);
  vAddPrimitive("drop",            &vDstackDrop,       false);
  vAddPrimitive("2drop",           &vDstackDropTwo,    false);
  vAddPrimitive("dump",            &vDstackDump,       false);
  vAddPrimitive("dup",             &vDstackDup,        false);
  vAddPrimitive("?dup",            &vDstackDupNonZero, false);
  vAddPrimitive("nip",             &vDstackNip,        false);
  vAddPrimitive("over",            &vDstackOver,       false);
  vAddPrimitive("pick",            &vDstackPick,       false);
  vAddPrimitive("reverse",         &vDstackReverse,    false);
  vAddPrimitive("roll",            &vDstackRoll,       false);
  vAddPrimitive("-roll",           &vDstackRollInv,    false);
  vAddPrimitive("rot",             &vDstackRot,        false);
  vAddPrimitive("-rot",            &vDstackRotInv,     false);
  vAddPrimitive("swap",            &vDstackSwap,       false);
  vAddPrimitive("valset>",         &vDstackToValues,   false);
  vAddPrimitive(">valset",         &vDstackToValSet,   false);
  vAddPrimitive("tuck",            &vDstackTuck,       false);
}

/*----------------------------------------------------------------------------*/
/* stack.class -- get class of nth value on stack                             */
/*----------------------------------------------------------------------------*/
void Task::vDstackClass(NNP_Task nnpTask)
{
  ENTRY("vDstackClass");
  
  i8 nTh;
  if ( ! nnpTask->m_xDataStack.bPop(nTh) )
    THROW_INVALID_DSTACK("stack.class");
  
  ValWsp xValWsp;
  
  nnpTask->m_xDataStack.vPeek(nTh,&xValWsp);
  
  eCN eClass = xValWsp.eClass();
  
  nnpTask->m_xDataStack.vPush(eClass);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* depth -- push depth of parameters stack onto stack                         */
/*----------------------------------------------------------------------------*/
void Task::vDstackDepth(NNP_Task nnpTask)
{
  ENTRY("vDstackDepth");
  
  i8 iDepth = nnpTask->m_xDataStack.iValues();
  
  nnpTask->m_xDataStack.vPush(iDepth);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* drop -- discard top value off stack                                        */
/*----------------------------------------------------------------------------*/
void Task::vDstackDrop(NNP_Task nnpTask)
{
  ENTRY("vDstackDrop");
  
  // should just discard on stack
  
  ValWsp xValA;
  if ( ! nnpTask->m_xDataStack.bPop(xValA) )
    THROW_INVALID_DSTACK("drop");
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* 2drop -- discard top two values off stack                                  */
/*----------------------------------------------------------------------------*/
void Task::vDstackDropTwo(NNP_Task nnpTask)
{
  ENTRY("vDstackDrop");
  
  // should just discard on stack
  
  ValWsp xValA;
  ValWsp xValB;
  if ( ! nnpTask->m_xDataStack.bPop(xValA,xValB) )
    THROW_INVALID_DSTACK("2drop");
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dup -- dup top of param stack                                              */
/*----------------------------------------------------------------------------*/
void Task::vDstackDup(NNP_Task nnpTask)
{
  ENTRY("vDstackDup");
  
  // could just copy from stack back into stack
  
  ValWsp xValA;
  if ( ! nnpTask->m_xDataStack.bPop(xValA) )
    THROW_INVALID_DSTACK("dup");
  
  nnpTask->m_xDataStack.vPush(xValA);
  nnpTask->m_xDataStack.vPush(xValA);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* ?dup -- dup top of param stack if non zero                                 */
/*----------------------------------------------------------------------------*/
void Task::vDstackDupNonZero(NNP_Task nnpTask)
{
  ENTRY("vDstackDupNonZero");
  
  // could just copy from stack back into stack
  
  ValWsp xValA;
  if ( ! nnpTask->m_xDataStack.bPop(xValA) )
    THROW_INVALID_DSTACK("?dup");
  
  if (xValA.iAsInt()) {
    nnpTask->m_xDataStack.vPush(xValA);
    nnpTask->m_xDataStack.vPush(xValA);
  }
  else
    nnpTask->m_xDataStack.vPush((i8)0);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dump -- dump top of param stack                                            */
/*----------------------------------------------------------------------------*/
void Task::vDstackDump(NNP_Task nnpTask)
{
  ENTRY("vDstackDump");
  
  s8K sWorkspace;
  
  ValWsp xValWsp;
  if ( ! nnpTask->m_xDataStack.bPop(xValWsp) )
    THROW_INVALID_DSTACK("dump");
  
  xValWsp.vToString(sWorkspace.a());
  char* aClass = xValWsp.aClass();
  
  eCN eClass = xValWsp.eClass();
  
  if (CN_PTRVAL == eClass) {
    PtrVal* pPtrVal = xValWsp.pAsPtrVal();
    Object* pObject = pPtrVal->pObject();
    if (pObject) {
      eCN ePtdClass   = pObject->eClass();
      vTrace("[%s] %s (%s %s)\n",
             nnpTask->m_sName.a(),sWorkspace.a(),aClass,pCN[ePtdClass]);
    }
    else {
      vTrace("[%s] nullptr\n",nnpTask->m_sName.a());
    }
  }
  else {
    vTrace("[%s] %s (%s)\n",
           nnpTask->m_sName.a(),sWorkspace.a(),aClass);
  }
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* nip -- drop the 2nd value from the stack ( B A -- A )                      */
/*----------------------------------------------------------------------------*/
void Task::vDstackNip(NNP_Task nnpTask)
{
  ENTRY("vDstackNip");
  
  ValWsp xValA;
  ValWsp xValB;
  
  if ( ! nnpTask->m_xDataStack.bPop(xValA,xValB) )
    THROW_INVALID_DSTACK("nip");
  
  nnpTask->m_xDataStack.vPush(&xValA);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* over -- place copy of 2nd value on top stack                               */
/*----------------------------------------------------------------------------*/
void Task::vDstackOver(NNP_Task nnpTask)
{
  ENTRY("vDstackOver");
  
  // could do this directly on stack
  
  ValWsp xValWsp;
  
  nnpTask->m_xDataStack.vPeek(1,&xValWsp);
  
  nnpTask->m_xDataStack.vPush(xValWsp);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* pick -- place copy of nth value in stack onto stack                        */
/*----------------------------------------------------------------------------*/
void Task::vDstackPick(NNP_Task nnpTask)
{
  ENTRY("vDstackPick");
  
  i8 iPick;
  if ( ! nnpTask->m_xDataStack.bPopParamInt(iPick) )
    THROW_INVALID_DSTACK("pick");
  
  // could do this directly on stack
  
  ValWsp xValWsp;
  
  nnpTask->m_xDataStack.vPeek(iPick,&xValWsp);
  
  nnpTask->m_xDataStack.vPush(xValWsp);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* reverse -- reverse stack values ( ... D C B A n -- A B C D ... )           */
/*----------------------------------------------------------------------------*/
void Task::vDstackReverse(NNP_Task nnpTask)
{
  ENTRY("vDstackReverse");
  
  ValWsp xVal0;
  ValWsp xVal1;
  ValWsp xVal2;
  ValWsp xVal3;
  ValWsp xVal4;
  ValWsp xVal5;
  ValWsp xVal6;
  ValWsp xVal7;
  
  ValWsp* apValWsp[8] = {
    &xVal0, &xVal1, &xVal2, &xVal3, &xVal4, &xVal5, &xVal6, &xVal7
  };
  
  i8 nValues;
  if ( ! nnpTask->m_xDataStack.bPop(nValues) )
    THROW_INVALID_DSTACK("reverse");
  
  for (i8 i = 0; i < nValues; i++) {
    if ( ! nnpTask->m_xDataStack.bPop(apValWsp[i]) )
      THROW_INVALID_DSTACK("reverse");
  }

  for (i8 i = 0; i < nValues; i++) {
    nnpTask->m_xDataStack.vPush(apValWsp[i]);
  }
}

/*----------------------------------------------------------------------------*/
/* roll -- roll top stack values at n ( ... D C B A n -- A ... D C B )        */
/*----------------------------------------------------------------------------*/
void Task::vDstackRoll(NNP_Task nnpTask)
{
  ENTRY("vDstackRoll");
  
  ValWsp xValA;
  ValWsp xValX;
  
  i8 nValues;
  if ( ! nnpTask->m_xDataStack.bPop(nValues) )
    THROW_INVALID_DSTACK("roll");
  
  if ( ! nnpTask->m_xDataStack.bPop(xValA) )
    THROW_INVALID_DSTACK("roll");
  
  for (i8 iCount = 0; iCount < nValues; iCount++) {
    if ( ! nnpTask->m_xDataStack.bPop(xValX) )
      THROW_INVALID_DSTACK("roll");
    
    nnpTask->m_xReturnStack.vPush(&xValX);
  }
  
  nnpTask->m_xDataStack.vPush(&xValA);
  
  for (i8 iCount = 0; iCount < nValues; iCount++) {
    if ( ! nnpTask->m_xReturnStack.bPop(xValX) )
      THROW_INVALID_DSTACK("roll");
    
    nnpTask->m_xDataStack.vPush(&xValX);
  }
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* -roll -- inverse roll top stack values at n ( Z ... B A n -- ... B A Z )   */
/*----------------------------------------------------------------------------*/
void Task::vDstackRollInv(NNP_Task nnpTask)
{
  ENTRY("vDstackRollInv");
  
  ValWsp xValZ;
  ValWsp xValX;
  
  i8 nValues;
  if ( ! nnpTask->m_xDataStack.bPop(nValues) )
    THROW_INVALID_DSTACK("-roll");
  
  for (i8 iCount = 0; iCount < nValues; iCount++) {
    if ( ! nnpTask->m_xDataStack.bPop(xValX) )
      THROW_INVALID_DSTACK("-roll");
    
    nnpTask->m_xReturnStack.vPush(&xValX);
  }
  
  if ( ! nnpTask->m_xDataStack.bPop(xValZ) )
    THROW_INVALID_DSTACK("-roll");
  
  
  for (i8 iCount = 0; iCount < nValues; iCount++) {
    if ( ! nnpTask->m_xReturnStack.bPop(xValX) )
      THROW_INVALID_DSTACK("-roll");
    
    nnpTask->m_xDataStack.vPush(&xValX);
  }
  
  nnpTask->m_xDataStack.vPush(&xValZ);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* rot -- rotate top 3 values ( C B A -- A C B )                              */
/*----------------------------------------------------------------------------*/
void Task::vDstackRot(NNP_Task nnpTask)
{
  ENTRY("vDstackRot");
  
  ValWsp xValA;
  ValWsp xValB;
  ValWsp xValC;
  
  if ( ! nnpTask->m_xDataStack.bPop(xValA,xValB,xValC) )
    THROW_INVALID_DSTACK("rot");
  
  nnpTask->m_xDataStack.vPush(&xValA);
  nnpTask->m_xDataStack.vPush(&xValC);
  nnpTask->m_xDataStack.vPush(&xValB);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* -rot -- inverse rotate top 3 values ( C B A -- B A C )                     */
/*----------------------------------------------------------------------------*/
void Task::vDstackRotInv(NNP_Task nnpTask)
{
  ENTRY("vDstackRotInv");
  
  ValWsp xValA;
  ValWsp xValB;
  ValWsp xValC;
  
  if ( ! nnpTask->m_xDataStack.bPop(xValA,xValB,xValC) )
    THROW_INVALID_DSTACK("rot");
  
  nnpTask->m_xDataStack.vPush(&xValB);
  nnpTask->m_xDataStack.vPush(&xValA);
  nnpTask->m_xDataStack.vPush(&xValC);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* swap -- swap top two values on param stack                                 */
/*----------------------------------------------------------------------------*/
void Task::vDstackSwap(NNP_Task nnpTask)
{
  ENTRY("vDstackSwap");
  
  ValWsp xValA;
  ValWsp xValB;
  
  if ( ! nnpTask->m_xDataStack.bPop(xValA,xValB) )
    THROW_INVALID_DSTACK("swap");
  
  nnpTask->m_xDataStack.vPush(&xValA);
  nnpTask->m_xDataStack.vPush(&xValB);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* >valset -- collapse values on dstack to a valset                           */
/*----------------------------------------------------------------------------*/
void Task::vDstackToValSet(NNP_Task nnpTask)
{
  ENTRY("vDstackToValSet");
  
  i8 nVal;
  
  if ( ! nnpTask->m_xDataStack.bPop(nVal) )
  THROW_INVALID_DSTACK(">valset");
  
  if ( ! nnpTask->m_xDataStack.bGuard(nVal) )
  THROW_INVALID_DSTACK(">valset");
  
  char* pCursor = nullptr;
  ValSet xValSet;
  
  ValWsp xVal;
  for (i8 iVal = 0; iVal < nVal; iVal++) {
    
    if ( ! nnpTask->m_xDataStack.bPop(xVal) )
    THROW_INVALID_DSTACK(">valset");
    
    xValSet.vAppend(pCursor,xVal);
  }
  
  //  vTrace("ValSet %lld bytes\n",xValSet.iSize());
  
  nnpTask->m_xDataStack.vPush(xValSet);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* >values -- expand valset to values on dstack                               */
/*----------------------------------------------------------------------------*/
void Task::vDstackToValues(NNP_Task nnpTask)
{
  ENTRY("vDstackToValues");
  
  ValSet xValSet;
  
  if ( ! nnpTask->m_xDataStack.bPop(xValSet) )
  THROW_INVALID_DSTACK(">values");
  
  i8 nVal = 0;
  ValWsp* pValWsp[128];
  
  char* pCursor = nullptr;
  while (xValSet.bExtract(pCursor,pValWsp[nVal]))
  nVal++;
  
  for (i8 iVal = nVal - 1; iVal >= 0; iVal--)
  nnpTask->m_xDataStack.vPush(pValWsp[iVal]);
  
  nnpTask->m_xDataStack.vPush(nVal);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* tuck -- copy the first stack value below second ( B A -- A B A )           */
/*----------------------------------------------------------------------------*/
void Task::vDstackTuck(NNP_Task nnpTask)
{
  ENTRY("vDstackTuck");
  
  ValWsp xValA;
  ValWsp xValB;
  
  if ( ! nnpTask->m_xDataStack.bPop(xValA,xValB) )
    THROW_INVALID_DSTACK("tuck");
  
  nnpTask->m_xDataStack.vPush(&xValA);
  nnpTask->m_xDataStack.vPush(&xValB);
  nnpTask->m_xDataStack.vPush(&xValA);
  
  EXIT();
}

// push: ///////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_Push(void)
{
  vAddPrimitive("i",               &vPushI,            false);
  vAddPrimitive("j",               &vPushJ,            false);
  vAddPrimitive("nullPtr",         &vPushNullPtr,      false);
  vAddPrimitive("pushValue",       &vPushValue,        false);
}

/*----------------------------------------------------------------------------*/
/* i -- copy loop index from return stack onto param stack                    */
/*----------------------------------------------------------------------------*/
void Task::vPushI(NNP_Task nnpTask)
{
  ENTRY("vPushI");
  
  Value* pI = nnpTask->m_xReturnStack.pPeek();
  
  nnpTask->m_xDataStack.vPush(pI);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* j -- copy 2nd loop index from return stack onto param stack                */
/*----------------------------------------------------------------------------*/
void Task::vPushJ(NNP_Task nnpTask)
{
  ENTRY("vPushJ");
  
  Value* pI = nnpTask->m_xReturnStack.pPeek(3);
  
  nnpTask->m_xDataStack.vPush(pI);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* nullPtr -- push a nullptr                                                  */
/*----------------------------------------------------------------------------*/
void Task::vPushNullPtr(NNP_Task nnpTask)
{
  ENTRY("vPushNullPtr");
  
  PtrVal xPtrVal;
  xPtrVal = nullptr;
  
  nnpTask->m_xDataStack.vPush(xPtrVal);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* pushValue -- push value arg onto stack                                     */
/*----------------------------------------------------------------------------*/
void Task::vPushValue(NNP_Task nnpTask)
{
  ENTRY("vPushValue");
  
  Value* pValue = nnpTask->IP.m_pValue;
  
  nnpTask->m_xDataStack.vPush(pValue);
  
  nnpTask->vAdjustNext(pValue->iSize());
  
  EXIT();
}

// queue: //////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_Queue(void)
{
  vAddPrimitive("queue.append",    &vQueueAppend,      false);
  vAddPrimitive("queue.dump",      &vQueueDump,        false);
  vAddPrimitive("queue.init",      &vQueueInit,        false);
  vAddPrimitive("queue.pop",       &vQueuePop,         false);
}

/*----------------------------------------------------------------------------*/
/* queue.append -- append onto queue                                          */
/*----------------------------------------------------------------------------*/
void Task::vQueueAppend(NNP_Task nnpTask)
{
ENTRY("vQueueAppend");
  
  Object* pObject;
  ValWsp xValWsp;
  
  if ( ! nnpTask->m_xDataStack.bPop(pObject,xValWsp) )
    THROW_INVALID_DSTACK("queue.append");
  
  Queue* pQueue = static_cast<Queue*>(pObject);
  ASSERT(pQueue);
  ASSERT(CN_QUEUE == pObject->eClass());
  
  pQueue->bAppend(&xValWsp);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* q.dump -- queue dump                                                       */
/*----------------------------------------------------------------------------*/
void Task::vQueueDump(NNP_Task nnpTask)
{
ENTRY("vQueueDump");
  
  Object* pObject;
  
  if ( ! nnpTask->m_xDataStack.bPop(pObject) )
    THROW_INVALID_DSTACK("q.dump");
  
  ASSERT(CN_QUEUE == pObject->eClass());
  
  Queue* pQueue = static_cast<Queue*>(pObject);
  
  pQueue->vDump(pSingleTab());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* queue.init -- queue init                                                   */
/*----------------------------------------------------------------------------*/
void Task::vQueueInit(NNP_Task nnpTask)
{
ENTRY("vQueueInit");
  
  Object* pObject;
  
  if ( ! nnpTask->m_xDataStack.bPop(pObject) )
    THROW_INVALID_DSTACK("queue.init");
    
  ASSERT(CN_QUEUE == pObject->eClass());
  
  Queue* pQueue = static_cast<Queue*>(pObject);
  
  new(pQueue) Queue;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* queue.pop -- queue pop                                                     */
/*----------------------------------------------------------------------------*/
void Task::vQueuePop(NNP_Task nnpTask)
{
ENTRY("vQueuePop");
  
  Object* pObject;
  ValWsp xValWsp;
  
  if ( ! nnpTask->m_xDataStack.bPop(pObject) )
    THROW_INVALID_DSTACK("queue.pop");
  
  ASSERT(CN_QUEUE == pObject->eClass());
  
  Queue* pQueue = static_cast<Queue*>(pObject);
  ASSERT(pQueue);
  
  if (pQueue->bPop(&xValWsp)) {
    nnpTask->m_xDataStack.vPush(&xValWsp);
    nnpTask->m_xDataStack.vPush((i8)1);
  }
  else
    nnpTask->m_xDataStack.vPush((i8)0);
  
  
  EXIT();
}

// registers: //////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_Reg(void)
{
  vAddPrimitive("reg.pop",          &vRegPop,          false);
  vAddPrimitive("reg.push",         &vRegPush,         false);
  vAddPrimitive("reg0",             &vRegReg0,         false);
  vAddPrimitive("reg1",             &vRegReg1,         false);
  vAddPrimitive("reg2",             &vRegReg2,         false);
  vAddPrimitive("reg3",             &vRegReg3,         false);
  vAddPrimitive("reg4",             &vRegReg4,         false);
  vAddPrimitive("reg5",             &vRegReg5,         false);
  vAddPrimitive("reg6",             &vRegReg6,         false);
  vAddPrimitive("reg7",             &vRegReg7,         false);
  vAddPrimitive("reg8",             &vRegReg8,         false);
  vAddPrimitive("reg9",             &vRegReg9,         false);
  vAddPrimitive("regA",             &vRegRegA,         false);
  vAddPrimitive("regB",             &vRegRegB,         false);
  vAddPrimitive("regC",             &vRegRegC,         false);
  vAddPrimitive("regD",             &vRegRegD,         false);
  vAddPrimitive("regE",             &vRegRegE,         false);
  vAddPrimitive("regF",             &vRegRegF,         false);
  vAddPrimitive("regN",             &vRegRegNth,       false);
  vAddPrimitive(">reg0",            &vRegToReg0,       false);
  vAddPrimitive(">reg1",            &vRegToReg1,       false);
  vAddPrimitive(">reg2",            &vRegToReg2,       false);
  vAddPrimitive(">reg3",            &vRegToReg3,       false);
  vAddPrimitive(">reg4",            &vRegToReg4,       false);
  vAddPrimitive(">reg5",            &vRegToReg5,       false);
  vAddPrimitive(">reg6",            &vRegToReg6,       false);
  vAddPrimitive(">reg7",            &vRegToReg7,       false);
  vAddPrimitive(">reg8",            &vRegToReg8,       false);
  vAddPrimitive(">reg9",            &vRegToReg9,       false);
  vAddPrimitive(">regA",            &vRegToRegA,       false);
  vAddPrimitive(">regB",            &vRegToRegB,       false);
  vAddPrimitive(">regC",            &vRegToRegC,       false);
  vAddPrimitive(">regD",            &vRegToRegD,       false);
  vAddPrimitive(">regE",            &vRegToRegE,       false);
  vAddPrimitive(">regF",            &vRegToRegF,       false);
  vAddPrimitive(">regN",            &vRegToRegNth,     false);
}

/*----------------------------------------------------------------------------*/
/* reg.pop -- pop register set                                                */
/*----------------------------------------------------------------------------*/
void Task::vRegPop(NNP_Task nnpTask)
{
  ENTRY("vRegPop");
  
  RegFrame* pRegFrame = nnpTask->m_pRegFrame;
  
  if ( ! pRegFrame )
    THROW_INVALID_DSTACK("reg.pop");
  
  nnpTask->m_pRegFrame = nnpTask->m_pRegFrame->pNext();
  
  delete pRegFrame;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* reg.push -- push a register set                                            */
/*----------------------------------------------------------------------------*/
void Task::vRegPush(NNP_Task nnpTask)
{
ENTRY("vRegPush");
  
  RegFrame* pRegFrame = new RegFrame;
  
  pRegFrame->vNext(nnpTask->m_pRegFrame);
  
  nnpTask->m_pRegFrame = pRegFrame;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* reg0 -- push reg0 onto dstack                                              */
/*----------------------------------------------------------------------------*/
void Task::vRegReg0(NNP_Task nnpTask)
{
ENTRY("vRegReg0");
  
  if ( ! nnpTask->m_pRegFrame )
    THROW_INVALID_DSTACK("reg0");
  
  ValReg* pValReg = nnpTask->m_pRegFrame->pValReg(0);
  
  nnpTask->m_xDataStack.vPush(pValReg);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* reg1 -- push reg1 onto dstack                                              */
/*----------------------------------------------------------------------------*/
void Task::vRegReg1(NNP_Task nnpTask)
{
  ENTRY("vRegReg1");
  
  if ( ! nnpTask->m_pRegFrame )
    THROW_INVALID_DSTACK("reg1");
  
  ValReg* pValReg = nnpTask->m_pRegFrame->pValReg(1);
  
  nnpTask->m_xDataStack.vPush(pValReg);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* reg2 -- push reg2 onto dstack                                              */
/*----------------------------------------------------------------------------*/
void Task::vRegReg2(NNP_Task nnpTask)
{
  ENTRY("vRegReg2");
  
  if ( ! nnpTask->m_pRegFrame )
    THROW_INVALID_DSTACK("reg2");
  
  ValReg* pValReg = nnpTask->m_pRegFrame->pValReg(2);
  
  nnpTask->m_xDataStack.vPush(pValReg);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* reg3 -- push reg3 onto dstack                                              */
/*----------------------------------------------------------------------------*/
void Task::vRegReg3(NNP_Task nnpTask)
{
  ENTRY("vRegReg3");
  
  if ( ! nnpTask->m_pRegFrame )
    THROW_INVALID_DSTACK("reg3");
  
  ValReg* pValReg = nnpTask->m_pRegFrame->pValReg(3);
  
  nnpTask->m_xDataStack.vPush(pValReg);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* reg4 -- push reg4 onto dstack                                              */
/*----------------------------------------------------------------------------*/
void Task::vRegReg4(NNP_Task nnpTask)
{
  ENTRY("vRegReg4");
  
  if ( ! nnpTask->m_pRegFrame )
    THROW_INVALID_DSTACK("reg4");
  
  ValReg* pValReg = nnpTask->m_pRegFrame->pValReg(4);
  
  nnpTask->m_xDataStack.vPush(pValReg);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* reg5 -- push reg5 onto dstack                                              */
/*----------------------------------------------------------------------------*/
void Task::vRegReg5(NNP_Task nnpTask)
{
  ENTRY("vRegReg5");
  
  if ( ! nnpTask->m_pRegFrame )
    THROW_INVALID_DSTACK("reg5");
  
  ValReg* pValReg = nnpTask->m_pRegFrame->pValReg(5);
  
  nnpTask->m_xDataStack.vPush(pValReg);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* reg6 -- push reg6 onto dstack                                              */
/*----------------------------------------------------------------------------*/
void Task::vRegReg6(NNP_Task nnpTask)
{
  ENTRY("vRegReg6");
  
  if ( ! nnpTask->m_pRegFrame )
    THROW_INVALID_DSTACK("reg6");
  
  ValReg* pValReg = nnpTask->m_pRegFrame->pValReg(6);
  
  nnpTask->m_xDataStack.vPush(pValReg);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* reg7 -- push reg7 onto dstack                                              */
/*----------------------------------------------------------------------------*/
void Task::vRegReg7(NNP_Task nnpTask)
{
  ENTRY("vRegReg7");
  
  if ( ! nnpTask->m_pRegFrame )
    THROW_INVALID_DSTACK("reg7");
  
  ValReg* pValReg = nnpTask->m_pRegFrame->pValReg(7);
  
  nnpTask->m_xDataStack.vPush(pValReg);
  
  EXIT();
}

// object: /////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* reg8 -- push reg8 onto dstack                                              */
/*----------------------------------------------------------------------------*/
void Task::vRegReg8(NNP_Task nnpTask)
{
  ENTRY("vRegReg8");
  
  if ( ! nnpTask->m_pRegFrame )
    THROW_INVALID_DSTACK("reg8");
  
  ValReg* pValReg = nnpTask->m_pRegFrame->pValReg(8);
  
  nnpTask->m_xDataStack.vPush(pValReg);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* reg9 -- push reg9 onto dstack                                              */
/*----------------------------------------------------------------------------*/
void Task::vRegReg9(NNP_Task nnpTask)
{
  ENTRY("vRegReg9");
  
  if ( ! nnpTask->m_pRegFrame )
    THROW_INVALID_DSTACK("reg9");
  
  ValReg* pValReg = nnpTask->m_pRegFrame->pValReg(9);
  
  nnpTask->m_xDataStack.vPush(pValReg);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* regA -- push regA onto dstack                                              */
/*----------------------------------------------------------------------------*/
void Task::vRegRegA(NNP_Task nnpTask)
{
  ENTRY("vRegRegA");
  
  if ( ! nnpTask->m_pRegFrame )
    THROW_INVALID_DSTACK("regA");
  
  ValReg* pValReg = nnpTask->m_pRegFrame->pValReg(10);
  
  nnpTask->m_xDataStack.vPush(pValReg);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* regB -- push regB onto dstack                                              */
/*----------------------------------------------------------------------------*/
void Task::vRegRegB(NNP_Task nnpTask)
{
  ENTRY("vRegRegB");
  
  if ( ! nnpTask->m_pRegFrame )
    THROW_INVALID_DSTACK("regB");
  
  ValReg* pValReg = nnpTask->m_pRegFrame->pValReg(11);
  
  nnpTask->m_xDataStack.vPush(pValReg);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* regC -- push regC onto dstack                                              */
/*----------------------------------------------------------------------------*/
void Task::vRegRegC(NNP_Task nnpTask)
{
  ENTRY("vRegRegC");
  
  if ( ! nnpTask->m_pRegFrame )
    THROW_INVALID_DSTACK("regC");
  
  ValReg* pValReg = nnpTask->m_pRegFrame->pValReg(12);
  
  nnpTask->m_xDataStack.vPush(pValReg);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* regD -- push regD onto dstack                                              */
/*----------------------------------------------------------------------------*/
void Task::vRegRegD(NNP_Task nnpTask)
{
  ENTRY("vRegRegD");
  
  if ( ! nnpTask->m_pRegFrame )
    THROW_INVALID_DSTACK("regD");
  
  ValReg* pValReg = nnpTask->m_pRegFrame->pValReg(13);
  
  nnpTask->m_xDataStack.vPush(pValReg);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* regE -- push regE onto dstack                                              */
/*----------------------------------------------------------------------------*/
void Task::vRegRegE(NNP_Task nnpTask)
{
  ENTRY("vRegRegE");
  
  if ( ! nnpTask->m_pRegFrame )
    THROW_INVALID_DSTACK("regE");
  
  ValReg* pValReg = nnpTask->m_pRegFrame->pValReg(14);
  
  nnpTask->m_xDataStack.vPush(pValReg);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* regF -- push regF onto dstack                                              */
/*----------------------------------------------------------------------------*/
void Task::vRegRegF(NNP_Task nnpTask)
{
  ENTRY("vRegRegF");
  
  if ( ! nnpTask->m_pRegFrame )
    THROW_INVALID_DSTACK("regF");
  
  ValReg* pValReg = nnpTask->m_pRegFrame->pValReg(15);
  
  nnpTask->m_xDataStack.vPush(pValReg);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* regN -- push reg[n] onto dstack                                            */
/*----------------------------------------------------------------------------*/
void Task::vRegRegNth(NNP_Task nnpTask)
{
  ENTRY("vRegRegNth");
  
  if ( ! nnpTask->m_pRegFrame )
    THROW_INVALID_DSTACK("regN");
  
  i8 iReg;
  if ( ! nnpTask->m_xDataStack.bPop(iReg) )
    THROW_INVALID_DSTACK("regN");
  
  ValReg* pValReg = nnpTask->m_pRegFrame->pValReg(iReg);
  
  nnpTask->m_xDataStack.vPush(pValReg);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* >reg0 -- pop dstack into reg0                                              */
/*----------------------------------------------------------------------------*/
void Task::vRegToReg0(NNP_Task nnpTask)
{
ENTRY("vRegToReg0");
  
  if ( ! nnpTask->m_pRegFrame )
    THROW_INVALID_DSTACK(">reg0");
  
  ValReg* pValReg = nnpTask->m_pRegFrame->pValReg(0);
  
  nnpTask->m_xDataStack.bPop(pValReg);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* >reg1 -- pop dstack into reg1                                              */
/*----------------------------------------------------------------------------*/
void Task::vRegToReg1(NNP_Task nnpTask)
{
  ENTRY("vRegToReg1");
  
  if ( ! nnpTask->m_pRegFrame )
    THROW_INVALID_DSTACK(">reg1");
  
  ValReg* pValReg = nnpTask->m_pRegFrame->pValReg(1);
  
  nnpTask->m_xDataStack.bPop(pValReg);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* >reg2 -- pop dstack into reg2                                              */
/*----------------------------------------------------------------------------*/
void Task::vRegToReg2(NNP_Task nnpTask)
{
  ENTRY("vRegToReg2");
  
  if ( ! nnpTask->m_pRegFrame )
    THROW_INVALID_DSTACK(">reg2");
  
  ValReg* pValReg = nnpTask->m_pRegFrame->pValReg(2);
  
  nnpTask->m_xDataStack.bPop(pValReg);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* >reg3 -- pop dstack into reg3                                              */
/*----------------------------------------------------------------------------*/
void Task::vRegToReg3(NNP_Task nnpTask)
{
  ENTRY("vRegToReg3");
  
  if ( ! nnpTask->m_pRegFrame )
    THROW_INVALID_DSTACK(">reg3");
  
  ValReg* pValReg = nnpTask->m_pRegFrame->pValReg(3);
  
  nnpTask->m_xDataStack.bPop(pValReg);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* >reg4 -- pop dstack into reg4                                              */
/*----------------------------------------------------------------------------*/
void Task::vRegToReg4(NNP_Task nnpTask)
{
  ENTRY("vRegToReg4");
  
  if ( ! nnpTask->m_pRegFrame )
    THROW_INVALID_DSTACK(">reg4");
  
  ValReg* pValReg = nnpTask->m_pRegFrame->pValReg(4);
  
  nnpTask->m_xDataStack.bPop(pValReg);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* >reg5 -- pop dstack into reg5                                              */
/*----------------------------------------------------------------------------*/
void Task::vRegToReg5(NNP_Task nnpTask)
{
  ENTRY("vRegToReg5");
  
  if ( ! nnpTask->m_pRegFrame )
    THROW_INVALID_DSTACK(">reg5");
  
  ValReg* pValReg = nnpTask->m_pRegFrame->pValReg(5);
  
  nnpTask->m_xDataStack.bPop(pValReg);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* >reg6 -- pop dstack into reg6                                              */
/*----------------------------------------------------------------------------*/
void Task::vRegToReg6(NNP_Task nnpTask)
{
  ENTRY("vRegToReg6");
  
  if ( ! nnpTask->m_pRegFrame )
    THROW_INVALID_DSTACK(">reg6");
  
  ValReg* pValReg = nnpTask->m_pRegFrame->pValReg(6);
  
  nnpTask->m_xDataStack.bPop(pValReg);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* >reg7 -- pop dstack into reg7                                              */
/*----------------------------------------------------------------------------*/
void Task::vRegToReg7(NNP_Task nnpTask)
{
  ENTRY("vRegToReg7");
  
  if ( ! nnpTask->m_pRegFrame )
    THROW_INVALID_DSTACK(">reg7");
  
  ValReg* pValReg = nnpTask->m_pRegFrame->pValReg(7);
  
  nnpTask->m_xDataStack.bPop(pValReg);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* >reg8 -- pop dstack into reg8                                              */
/*----------------------------------------------------------------------------*/
void Task::vRegToReg8(NNP_Task nnpTask)
{
  ENTRY("vRegToReg8");
  
  if ( ! nnpTask->m_pRegFrame )
    THROW_INVALID_DSTACK(">reg8");
  
  ValReg* pValReg = nnpTask->m_pRegFrame->pValReg(8);
  
  nnpTask->m_xDataStack.bPop(pValReg);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* >reg9 -- pop dstack into reg9                                              */
/*----------------------------------------------------------------------------*/
void Task::vRegToReg9(NNP_Task nnpTask)
{
  ENTRY("vRegToReg9");
  
  if ( ! nnpTask->m_pRegFrame )
    THROW_INVALID_DSTACK(">reg9");
  
  ValReg* pValReg = nnpTask->m_pRegFrame->pValReg(9);
  
  nnpTask->m_xDataStack.bPop(pValReg);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* >regA -- pop dstack into regA                                              */
/*----------------------------------------------------------------------------*/
void Task::vRegToRegA(NNP_Task nnpTask)
{
  ENTRY("vRegToRegA");
  
  if ( ! nnpTask->m_pRegFrame )
    THROW_INVALID_DSTACK(">regA");
  
  ValReg* pValReg = nnpTask->m_pRegFrame->pValReg(10);
  
  nnpTask->m_xDataStack.bPop(pValReg);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* >regB -- pop dstack into regB                                              */
/*----------------------------------------------------------------------------*/
void Task::vRegToRegB(NNP_Task nnpTask)
{
  ENTRY("vRegToRegB");
  
  if ( ! nnpTask->m_pRegFrame )
    THROW_INVALID_DSTACK(">regB");
  
  ValReg* pValReg = nnpTask->m_pRegFrame->pValReg(11);
  
  nnpTask->m_xDataStack.bPop(pValReg);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* >regC -- pop dstack into regC                                              */
/*----------------------------------------------------------------------------*/
void Task::vRegToRegC(NNP_Task nnpTask)
{
  ENTRY("vRegToRegC");
  
  if ( ! nnpTask->m_pRegFrame )
    THROW_INVALID_DSTACK(">regC");
  
  ValReg* pValReg = nnpTask->m_pRegFrame->pValReg(12);
  
  nnpTask->m_xDataStack.bPop(pValReg);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* >regD -- pop dstack into regD                                              */
/*----------------------------------------------------------------------------*/
void Task::vRegToRegD(NNP_Task nnpTask)
{
  ENTRY("vRegToRegD");
  
  if ( ! nnpTask->m_pRegFrame )
    THROW_INVALID_DSTACK(">regD");
  
  ValReg* pValReg = nnpTask->m_pRegFrame->pValReg(13);
  
  nnpTask->m_xDataStack.bPop(pValReg);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* >regE -- pop dstack into regE                                              */
/*----------------------------------------------------------------------------*/
void Task::vRegToRegE(NNP_Task nnpTask)
{
  ENTRY("vRegToRegE");
  
  if ( ! nnpTask->m_pRegFrame )
    THROW_INVALID_DSTACK(">regE");
  
  ValReg* pValReg = nnpTask->m_pRegFrame->pValReg(14);
  
  nnpTask->m_xDataStack.bPop(pValReg);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* >regF -- pop dstack into regF                                              */
/*----------------------------------------------------------------------------*/
void Task::vRegToRegF(NNP_Task nnpTask)
{
  ENTRY("vRegToRegF");
  
  if ( ! nnpTask->m_pRegFrame )
    THROW_INVALID_DSTACK(">regF");
  
  ValReg* pValReg = nnpTask->m_pRegFrame->pValReg(15);
  
  nnpTask->m_xDataStack.bPop(pValReg);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* >regN -- pop dstack into regN                                              */
/*----------------------------------------------------------------------------*/
void Task::vRegToRegNth(NNP_Task nnpTask)
{
  ENTRY("vRegToRegNth");
  
  if ( ! nnpTask->m_pRegFrame )
    THROW_INVALID_DSTACK(">regN");
  
  i8 iReg;
  if ( ! nnpTask->m_xDataStack.bPop(iReg) )
    THROW_INVALID_DSTACK(">regNth");
  
  ValReg* pValReg = nnpTask->m_pRegFrame->pValReg(iReg);
  
  nnpTask->m_xDataStack.bPop(pValReg);
  
  EXIT();
}

// rowbuffer: //////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_RowBuffer(void)
{
  vAddPrimitive("rowBuffer.append", &vRowBufferAppend, false);
  vAddPrimitive("rowBuffer.appendRecord", &vRowBufferAppendRecord, false);
  vAddPrimitive("rowBuffer.dump",   &vRowBufferDump,    false);
  vAddPrimitive("rowBuffer.new",    &vRowBufferNew,    false);
}

/*----------------------------------------------------------------------------*/
/* rowbuffer.append -- attempt to append row onto row buffer                  */
/*----------------------------------------------------------------------------*/
void Task::vRowBufferAppend(NNP_Task nnpTask)
{
ENTRY("vRowBufferAppend");
  
  Object* pPtrRowBuffer;
  if ( ! nnpTask->m_xDataStack.bPop(pPtrRowBuffer) )
    THROW_INVALID_DSTACK("rowbuffer.append");
  RowBuffer* pRowBuffer = _pRowBuffer(pPtrRowBuffer);
  ASSERT(CN_ROWBUFFER == pRowBuffer->eClass());

  Object* pPtrTxRef;
  if ( ! nnpTask->m_xDataStack.bPop(pPtrTxRef) )
    THROW_INVALID_DSTACK("rowbuffer.append");
  TxRef* pTxRef = _pTxRef(pPtrTxRef);
  ASSERT(CN_TXREF == pTxRef->eClass());
  
  s8K sRecord;
  if ( ! nnpTask->m_xDataStack.bPop(sRecord) )
    THROW_INVALID_DSTACK("rowbuffer.append");

  i8 iLength = sRecord.iLength();
  
  bool b = pRowBuffer->bInsertRow(pTxRef,sRecord.a(),iLength);
  
  if (b) {
    nnpTask->m_xDataStack.vPush((i8)1);
  }
  else {
    nnpTask->m_xDataStack.vPush(sRecord);
    nnpTask->m_xDataStack.vPush((i8)0);
  }
    
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* rowbuffer.appendRecord -- attempt to append record onto row buffer         */
/*----------------------------------------------------------------------------*/
void Task::vRowBufferAppendRecord(NNP_Task nnpTask)
{
  ENTRY("vRowBufferAppendRecord");
  
  Object* pPtrRowBuffer;
  if ( ! nnpTask->m_xDataStack.bPop(pPtrRowBuffer) )
    THROW_INVALID_DSTACK("rowBuffer.appendRecord");
  RowBuffer* pRowBuffer = _pRowBuffer(pPtrRowBuffer);
  ASSERT(CN_ROWBUFFER == pRowBuffer->eClass());
  
  Object* pPtrTxRef;
  if ( ! nnpTask->m_xDataStack.bPop(pPtrTxRef) )
    THROW_INVALID_DSTACK("rowBuffer.appendRecord");
  TxRef* pTxRef = _pTxRef(pPtrTxRef);
  ASSERT(CN_TXREF == pTxRef->eClass());
  
  Object* pPtrFile;
  if ( ! nnpTask->m_xDataStack.bPop(pPtrFile) )
    THROW_INVALID_DSTACK("rowBuffer.appendRecord");
  
  File* pFile = _pFile(pPtrFile);
  
  char* aRecord = pFile->pRecord()->pWorkArea();
  
//  i8 iLength = String::iLength(aRecord);
  i8 iLength = pFile->iRecordLength();
  
  bool b = pRowBuffer->bInsertRow(pTxRef,aRecord,iLength);
  
  if (b) {
    nnpTask->m_xDataStack.vPush((i8)1);
  }
  else {
    nnpTask->m_xDataStack.vPush((i8)0);
  }
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* rowbuffer.dump -- dump the row buffer                                      */
/*----------------------------------------------------------------------------*/
void Task::vRowBufferDump(NNP_Task nnpTask)
{
  ENTRY("vRowBufferDump");
  
  Object* pPtrRowBuffer;
  if ( ! nnpTask->m_xDataStack.bPop(pPtrRowBuffer) )
    THROW_INVALID_DSTACK("rowbuffer.dump");
  RowBuffer* pRowBuffer = _pRowBuffer(pPtrRowBuffer);
  ASSERT(CN_ROWBUFFER == pRowBuffer->eClass());

  Row* pRow = pRowBuffer->pFirstRow();
  
  while (pRow) {
    pRow->vDump();
    pRow = pRowBuffer->pNextRow(pRow);
  }
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* rowbuffer.new -- new a row buffer                                          */
/*----------------------------------------------------------------------------*/
void Task::vRowBufferNew(NNP_Task nnpTask)
{
  ENTRY("vRowBufferNew");
  
  RowBuffer* pRowBuffer = new RowBuffer;
  ASSERT(CN_ROWBUFFER == pRowBuffer->eClass());
  
  nnpTask->m_xDataStack.vPush(pRowBuffer);
  
  EXIT();
}

// rowcursor: //////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_RowCursor(void)
{
  vAddPrimitive("rowCursor.begin",  &vRowCursorBegin,  false);
  vAddPrimitive("rowCursor.delete", &vRowCursorDelete, false);
  vAddPrimitive("rowCursor.end",    &vRowCursorEnd,    false);
  vAddPrimitive("rowCursor.first",  &vRowCursorFirst,  false);
  vAddPrimitive("rowCursor.get",    &vRowCursorGet,    false);
  vAddPrimitive("rowCursor.init",   &vRowCursorInit,   false);
  vAddPrimitive("rowCursor.new",    &vRowCursorNew,    false);
  vAddPrimitive("rowCursor.next",   &vRowCursorNext,   false);
  vAddPrimitive("rowCursor.rowBuffer", &vRowCursorRowBuffer,  false);
  vAddPrimitive("rowCursor.table@", &vRowCursorTableFetch, false);
  vAddPrimitive("rowCursor.table!", &vRowCursorTableStore, false);
}

/*----------------------------------------------------------------------------*/
/* rowcursor.begin -- begin using row cursor                                  */
/*----------------------------------------------------------------------------*/
void Task::vRowCursorBegin(NNP_Task nnpTask)
{
ENTRY("vRowCursorBegin");
  
  Object* pPtrRowCursor;
  if ( ! nnpTask->m_xDataStack.bPop(pPtrRowCursor) )
    THROW_INVALID_DSTACK("rowcursor.begin");
  
  RowCursor* pRowCursor = _pRowCursor(pPtrRowCursor);
  ASSERT(CN_ROWCURSOR == pRowCursor->eClass());
  
  i8 iPartn;
  if ( ! nnpTask->m_xDataStack.bPop(iPartn) )
    THROW_INVALID_DSTACK("rowcursor.begin");
  
  pRowCursor->vBegin(iPartn);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* rowcursor.delete -- delete a row cursor                                    */
/*----------------------------------------------------------------------------*/
void Task::vRowCursorDelete(NNP_Task nnpTask)
{
ENTRY("vRowCursorDelete");
  
  Object* pPtrRowCursor;
  if ( ! nnpTask->m_xDataStack.bPop(pPtrRowCursor) )
    THROW_INVALID_DSTACK("rowcursor.delete");
  
  RowCursor* pRowCursor = _pRowCursor(pPtrRowCursor);
  ASSERT(CN_ROWCURSOR == pRowCursor->eClass());
  
  Object* pObject = reinterpret_cast<Object*>(pRowCursor);
  delete pRowCursor;
  
  Heap::vFree(pObject);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* rowcursor.end -- end using row cursor                                      */
/*----------------------------------------------------------------------------*/
void Task::vRowCursorEnd(NNP_Task nnpTask)
{
ENTRY("vRowCursorEnd");
  
  Object* pPtrRowCursor;
  if ( ! nnpTask->m_xDataStack.bPop(pPtrRowCursor) )
    THROW_INVALID_DSTACK("rowcursor.end");
  
  RowCursor* pRowCursor = _pRowCursor(pPtrRowCursor);
  ASSERT(CN_ROWCURSOR == pRowCursor->eClass());
  
  pRowCursor->vEnd();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* rowcursor.first -- set to first and get row                                */
/*----------------------------------------------------------------------------*/
void Task::vRowCursorFirst(NNP_Task nnpTask)
{
ENTRY("vRowCursorFirst");
  
  Object* pPtrRowCursor;
  if ( ! nnpTask->m_xDataStack.bPop(pPtrRowCursor) )
    THROW_INVALID_DSTACK("rowcursor.first");
  
  RowCursor* pRowCursor = _pRowCursor(pPtrRowCursor);
  ASSERT(CN_ROWCURSOR == pRowCursor->eClass());
  
  pRowCursor->vFirst();
  
  i8 bRow = pRowCursor->pRowAsStr() ? 1 : 0;
  
  nnpTask->m_xDataStack.vPush(bRow);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* rowcursor.get -- get current row                                           */
/*----------------------------------------------------------------------------*/
void Task::vRowCursorGet(NNP_Task nnpTask)
{
ENTRY("vRowCursorGet");
  
  Object* pPtrRowCursor;
  if ( ! nnpTask->m_xDataStack.bPop(pPtrRowCursor) )
    THROW_INVALID_DSTACK("rowcursor.get");
  
  RowCursor* pRowCursor = _pRowCursor(pPtrRowCursor);
  ASSERT(CN_ROWCURSOR == pRowCursor->eClass());
  
  s8K xRow;
  xRow = pRowCursor->pRowAsStr();
  
  nnpTask->m_xDataStack.vPush(xRow);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* rowcursor.init -- init a row cursor                                        */
/*----------------------------------------------------------------------------*/
void Task::vRowCursorInit(NNP_Task nnpTask)
{
ENTRY("vRowCursorInit");
  
  Object* pPtrRowCursor;
  if ( ! nnpTask->m_xDataStack.bPop(pPtrRowCursor) )
    THROW_INVALID_DSTACK("rowcursor.init");
  
  RowCursor* pRowCursor = _pRowCursor(pPtrRowCursor);
  ASSERT(CN_ROWCURSOR == pRowCursor->eClass());
  
  new(pRowCursor) RowCursor;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* rowcursor.new -- new an row cursor                                         */
/*----------------------------------------------------------------------------*/
void Task::vRowCursorNew(NNP_Task nnpTask)
{
ENTRY("vRowCursorNew");
  
  RowCursor* pRowCursor = new RowCursor;
  ASSERT(CN_ROWCURSOR == pRowCursor->eClass());
  
  nnpTask->m_xDataStack.vPush(pRowCursor);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* rowcursor.next -- advance to next and get row                              */
/*----------------------------------------------------------------------------*/
void Task::vRowCursorNext(NNP_Task nnpTask)
{
ENTRY("vRowCursorNext");
  
  Object* pPtrRowCursor;
  if ( ! nnpTask->m_xDataStack.bPop(pPtrRowCursor) )
    THROW_INVALID_DSTACK("rowcursor.next");
  
  RowCursor* pRowCursor = _pRowCursor(pPtrRowCursor);
  ASSERT(CN_ROWCURSOR == pRowCursor->eClass());
  
  pRowCursor->vNext();
  
  i8 bRow = pRowCursor->pRowAsStr() ? 1 : 0;
  
  nnpTask->m_xDataStack.vPush(bRow);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* rowcursor.rowBuffer -- get current rowBuffer                               */
/*----------------------------------------------------------------------------*/
void Task::vRowCursorRowBuffer(NNP_Task nnpTask)
{
  ENTRY("vRowCursorRowBuffer");
  
  Object* pPtrRowCursor;
  if ( ! nnpTask->m_xDataStack.bPop(pPtrRowCursor) )
  THROW_INVALID_DSTACK("rowcursor.rowBuffer");
  
  RowCursor* pRowCursor = _pRowCursor(pPtrRowCursor);
  ASSERT(CN_ROWCURSOR == pRowCursor->eClass());
  
  RowBuffer* pRowBuffer;
  pRowBuffer = pRowCursor->pRowBuffer();
  
  nnpTask->m_xDataStack.vPush(pRowBuffer);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* rowcursor.table@ -- fetch table from row cursor                            */
/*----------------------------------------------------------------------------*/
void Task::vRowCursorTableFetch(NNP_Task nnpTask)
{
ENTRY("vRowCursorTableFetch");
  
  Object* pPtrRowCursor;
  if ( ! nnpTask->m_xDataStack.bPop(pPtrRowCursor) )
    THROW_INVALID_DSTACK("rowcursor.table@");
  
  RowCursor* pRowCursor = _pRowCursor(pPtrRowCursor);
  ASSERT(CN_ROWCURSOR == pRowCursor->eClass());
  
  Object* pPtrTable = pRowCursor->pTable();
  
  nnpTask->m_xDataStack.vPush(pPtrTable);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* rowcursor.table! -- store table into row cursor                            */
/*----------------------------------------------------------------------------*/
void Task::vRowCursorTableStore(NNP_Task nnpTask)
{
ENTRY("vRowCursorTableStore");
  
  Object* pPtrRowCursor;
  if ( ! nnpTask->m_xDataStack.bPop(pPtrRowCursor) )
    THROW_INVALID_DSTACK("rowcursor.table!");
  
  RowCursor* pRowCursor = _pRowCursor(pPtrRowCursor);
  ASSERT(CN_ROWCURSOR == pRowCursor->eClass());
  
  Object* pPtrTable;
  if ( ! nnpTask->m_xDataStack.bPop(pPtrTable) )
    THROW_INVALID_DSTACK("rowcursor.table!");
  
  Table* pTable = _pTable(pPtrTable);
  ASSERT(CN_TABLE == pTable->eClass());
  
  pRowCursor->vTable(pTable);
  
  EXIT();
}

// rstack: /////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_Rstack(void)
{
  vAddPrimitive("cs-pick",         &vRstackCsPick,     false);
  vAddPrimitive("cs-roll",         &vRstackCsRoll,     false);
  vAddPrimitive("rdepth",          &vRstackDepth,      false);
  vAddPrimitive("rdrop",           &vRstackDrop,       false);
  vAddPrimitive("r@",              &vRstackFetch,      false);
  vAddPrimitive("2r@",             &vRstackFetchTwo,   false);
  vAddPrimitive("rpick",           &vRstackPick,       false);
  vAddPrimitive("rreverse",        &vRstackReverse,    false);
  vAddPrimitive("rroll",           &vRstackRoll,       false);
  vAddPrimitive("-rroll",          &vRstackRollInv,    false);
  vAddPrimitive("rswap",           &vRstackSwap,       false);
  vAddPrimitive("r>",              &vRstackToP,        false);
  vAddPrimitive(">r",              &vRstackToR,        false);
  vAddPrimitive("2>r",             &vRstackToRTwo,     false);
  vAddPrimitive("r0",              &vRstack0,          false);
  vAddPrimitive("r1",              &vRstack1,          false);
  vAddPrimitive("r2",              &vRstack2,          false);
  vAddPrimitive("r3",              &vRstack3,          false);
  vAddPrimitive("r4",              &vRstack4,          false);
  vAddPrimitive("r5",              &vRstack5,          false);
  vAddPrimitive("r6",              &vRstack6,          false);
  vAddPrimitive("r7",              &vRstack7,          false);
  vAddPrimitive("r8",              &vRstack8,          false);
  vAddPrimitive("r9",              &vRstack9,          false);
}

/*----------------------------------------------------------------------------*/
/* cs-pick -- place copy of nth value in control stack onto control stack     */
/*----------------------------------------------------------------------------*/
void Task::vRstackCsPick(NNP_Task nnpTask)
{
  ENTRY("vRstackCsPick");
  
  // could do this directly on stack
  
  // pop which value
  i8 nThValue;
  if ( ! nnpTask->m_xDataStack.bPopParamInt(nThValue) )
    THROW_INVALID_DSTACK("cs-pick");
  
  // pop one return address
//  ValWsp xRtnWsp;
//  bool b = nnpTask->m_xReturnStack.bPop(&xRtnWsp);
//  ASSERT(b);
  
  // copy n-th value onto top
  ValWsp xValWsp;
  nnpTask->m_xReturnStack.vPeek(nThValue,&xValWsp);
  nnpTask->m_xReturnStack.vPush(xValWsp);
  
  // push back the one return address
//  nnpTask->m_xReturnStack.vPush(xRtnWsp);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* cs-roll -- roll top control values at n ( Z Y ... B A n -- Y ... B A Z )   */
/*----------------------------------------------------------------------------*/
void Task::vRstackCsRoll(NNP_Task nnpTask)
{
  ENTRY("vRstackCsRoll");
  
  // pop n values
  i8 nValues;
  if ( ! nnpTask->m_xDataStack.bPop(nValues) )
    THROW_INVALID_DSTACK("cs-roll");
  
  ValWsp xValMid;
  for (i8 iCount = 0; iCount < nValues; iCount++) {
    if ( ! nnpTask->m_xDataStack.bPop(xValMid) )
      THROW_INVALID_DSTACK("cs-roll");
    
    nnpTask->m_xReturnStack.vPush(&xValMid);
  }
  
  Cell* pCellZ;
  if ( ! nnpTask->m_xDataStack.bPop(pCellZ) )
    THROW_INVALID_DSTACK("cs-roll");
  
  for (i8 iCount = 0; iCount < nValues; iCount++) {
    if ( ! nnpTask->m_xReturnStack.bPop(xValMid) )
      THROW_INVALID_RSTACK("cs-roll");
    
    nnpTask->m_xDataStack.vPush(&xValMid);
  }

  nnpTask->m_xDataStack.vPush(pCellZ);
  
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* rdepth -- put number of values in return stack onto param stack            */
/*----------------------------------------------------------------------------*/
void Task::vRstackDepth(NNP_Task nnpTask)
{
  ENTRY("vRstackDepth");
  
  i8 iDepth = nnpTask->m_xReturnStack.iValues();
  
  nnpTask->m_xDataStack.vPush(iDepth);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* rdrop -- discard top value off rstack                                      */
/*----------------------------------------------------------------------------*/
void Task::vRstackDrop(NNP_Task nnpTask)
{
  ENTRY("vRstackDrop");
  
  // should just discard on stack
  
  ValWsp xValA;
  if ( ! nnpTask->m_xReturnStack.bPop(xValA) )
    THROW_INVALID_RSTACK("rdrop");
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* r@ -- place copy of top value in return stack onto param stack             */
/*----------------------------------------------------------------------------*/
void Task::vRstackFetch(NNP_Task nnpTask)
{
  ENTRY("vRstackFetch");
  
  // could do this directly on stack
  
  ValWsp xValWsp;
  
  nnpTask->m_xReturnStack.vPeek(0,&xValWsp);
  
  nnpTask->m_xDataStack.vPush(xValWsp);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* 2r@ -- place copy of two top values in return stack onto param stack       */
/*----------------------------------------------------------------------------*/
void Task::vRstackFetchTwo(NNP_Task nnpTask)
{
  ENTRY("vRstackFetchTwo");
  
  // could do this directly on stack
  
  ValWsp xValX0;
  ValWsp xValX1;
  
  nnpTask->m_xReturnStack.vPeek(0,&xValX0);
  nnpTask->m_xReturnStack.vPeek(1,&xValX1);
  
  nnpTask->m_xDataStack.vPush(xValX1);
  nnpTask->m_xDataStack.vPush(xValX0);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* rpick -- place copy of nth value in return stack onto param stack          */
/*----------------------------------------------------------------------------*/
void Task::vRstackPick(NNP_Task nnpTask)
{
  ENTRY("vRstackPick");
  
  i8 iPick;
  if ( ! nnpTask->m_xDataStack.bPopParamInt(iPick) )
    THROW_INVALID_DSTACK("rpick");
  
  // could do this directly on stack
  
  ValWsp xValWsp;
  
  nnpTask->m_xReturnStack.vPeek(iPick,&xValWsp);
  
  nnpTask->m_xDataStack.vPush(xValWsp);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* rreverse -- reverse rstack values ( ... D C B A n -- A B C D ... )         */
/*----------------------------------------------------------------------------*/
void Task::vRstackReverse(NNP_Task nnpTask)
{
  ENTRY("vRstackReverse");
  
  ValWsp xVal0;
  ValWsp xVal1;
  ValWsp xVal2;
  ValWsp xVal3;
  ValWsp xVal4;
  ValWsp xVal5;
  ValWsp xVal6;
  ValWsp xVal7;
  
  ValWsp* apValWsp[8] = {
    &xVal0, &xVal1, &xVal2, &xVal3, &xVal4, &xVal5, &xVal6, &xVal7
  };
  
  i8 nValues;
  if ( ! nnpTask->m_xDataStack.bPop(nValues) )
    THROW_INVALID_DSTACK("areverse");
  
  for (i8 i = 0; i < nValues; i++) {
    if ( ! nnpTask->m_xReturnStack.bPop(apValWsp[i]) )
      THROW_INVALID_DSTACK("areverse");
  }
  
  for (i8 i = 0; i < nValues; i++) {
    nnpTask->m_xReturnStack.vPush(apValWsp[i]);
  }
}

/*----------------------------------------------------------------------------*/
/* rroll -- roll top rstack values at n ( ... D C B A n -- A ... D C B )      */
/*----------------------------------------------------------------------------*/
void Task::vRstackRoll(NNP_Task nnpTask)
{
  ENTRY("vRstackRoll");
  
  ValWsp xValA;
  ValWsp xValX;
  
  i8 nValues;
  if ( ! nnpTask->m_xDataStack.bPop(nValues) )
    THROW_INVALID_DSTACK("rroll");
  
  if ( ! nnpTask->m_xReturnStack.bPop(xValA) )
    THROW_INVALID_RSTACK("rroll");
  
  for (i8 iCount = 0; iCount < nValues; iCount++) {
    if ( ! nnpTask->m_xReturnStack.bPop(xValX) )
      THROW_INVALID_RSTACK("rroll");
    
    nnpTask->m_xDataStack.vPush(&xValX);
  }
  
  nnpTask->m_xReturnStack.vPush(&xValA);
  
  for (i8 iCount = 0; iCount < nValues; iCount++) {
    if ( ! nnpTask->m_xDataStack.bPop(xValX) )
      THROW_INVALID_DSTACK("rroll");
    
    nnpTask->m_xReturnStack.vPush(&xValX);
  }
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* -rroll -- inverse roll top rstack values at n ( Z ... B A n -- ... B A Z ) */
/*----------------------------------------------------------------------------*/
void Task::vRstackRollInv(NNP_Task nnpTask)
{
  ENTRY("vRstackRollInv");
  
  ValWsp xValZ;
  ValWsp xValX;
  
  i8 nValues;
  if ( ! nnpTask->m_xDataStack.bPop(nValues) )
    THROW_INVALID_DSTACK("-rroll");
  
  for (i8 iCount = 0; iCount < nValues; iCount++) {
    if ( ! nnpTask->m_xReturnStack.bPop(xValX) )
      THROW_INVALID_RSTACK("-rroll");
    
    nnpTask->m_xDataStack.vPush(&xValX);
  }
  
  if ( ! nnpTask->m_xReturnStack.bPop(xValZ) )
    THROW_INVALID_DSTACK("-rroll");
  
  
  for (i8 iCount = 0; iCount < nValues; iCount++) {
    if ( ! nnpTask->m_xDataStack.bPop(xValX) )
      THROW_INVALID_DSTACK("-rroll");
    
    nnpTask->m_xReturnStack.vPush(&xValX);
  }
  
  nnpTask->m_xReturnStack.vPush(&xValZ);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* rswap -- swap top two values on return stack                               */
/*----------------------------------------------------------------------------*/
void Task::vRstackSwap(NNP_Task nnpTask)
{
  ENTRY("vRstackSwap");
  
  ValWsp xValA;
  ValWsp xValB;
  
  if ( ! nnpTask->m_xReturnStack.bPop(xValA,xValB) )
    THROW_INVALID_DSTACK("swap");
  
  nnpTask->m_xReturnStack.vPush(&xValA);
  nnpTask->m_xReturnStack.vPush(&xValB);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* r> -- move value from rstack to dstack                                     */
/*----------------------------------------------------------------------------*/
void Task::vRstackToP(NNP_Task nnpTask)
{
  ENTRY("vRstackToP");
  
  ValWsp xVal;
  if ( ! nnpTask->m_xReturnStack.bPop(xVal) )
    THROW_INVALID_DSTACK("r>");
  
  nnpTask->m_xDataStack.vPush(xVal);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* >r -- move value from dstack to rstack                                     */
/*----------------------------------------------------------------------------*/
void Task::vRstackToR(NNP_Task nnpTask)
{
  ENTRY("vRstackToR");
  
  ValWsp xValOpaque;
  if ( ! nnpTask->m_xDataStack.bPop(xValOpaque) )
    THROW_INVALID_DSTACK(">r");
  
  nnpTask->m_xReturnStack.vPush(xValOpaque);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* 2>r -- move two values from dstack to rstack preserving order              */
/*----------------------------------------------------------------------------*/
void Task::vRstackToRTwo(NNP_Task nnpTask)
{
  ENTRY("vRstackToRTwo");
  
  ValWsp xValX0;
  ValWsp xValX1;
  if ( ! nnpTask->m_xDataStack.bPop(xValX0,xValX1) )
    THROW_INVALID_DSTACK("2>r");
  
  nnpTask->m_xReturnStack.vPush(xValX1);
  nnpTask->m_xReturnStack.vPush(xValX0);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* r0 -- place copy of 0th value in return stack onto param stack             */
/*----------------------------------------------------------------------------*/
void Task::vRstack0(NNP_Task nnpTask)
{
  ENTRY("vRstack0");
  
  // could do this directly on stack
  
  ValWsp xValWsp;
  
  nnpTask->m_xReturnStack.vPeek(0,&xValWsp);
  
  nnpTask->m_xDataStack.vPush(xValWsp);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* r1 -- place copy of 1st value in return stack onto param stack             */
/*----------------------------------------------------------------------------*/
void Task::vRstack1(NNP_Task nnpTask)
{
  ENTRY("vRstack1");
  
  // could do this directly on stack
  
  ValWsp xValWsp;
  
  nnpTask->m_xReturnStack.vPeek(1,&xValWsp);
  
  nnpTask->m_xDataStack.vPush(xValWsp);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* r2 -- place copy of 2nd value in return stack onto param stack             */
/*----------------------------------------------------------------------------*/
void Task::vRstack2(NNP_Task nnpTask)
{
  ENTRY("vRstack2");
  
  // could do this directly on stack
  
  ValWsp xValWsp;
  
  nnpTask->m_xReturnStack.vPeek(2,&xValWsp);
  
  nnpTask->m_xDataStack.vPush(xValWsp);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* r3 -- place copy of 3rd value in return stack onto param stack             */
/*----------------------------------------------------------------------------*/
void Task::vRstack3(NNP_Task nnpTask)
{
  ENTRY("vRstack3");
  
  // could do this directly on stack
  
  ValWsp xValWsp;
  
  nnpTask->m_xReturnStack.vPeek(3,&xValWsp);
  
  nnpTask->m_xDataStack.vPush(xValWsp);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* r4 -- place copy of 4th value in return stack onto param stack             */
/*----------------------------------------------------------------------------*/
void Task::vRstack4(NNP_Task nnpTask)
{
  ENTRY("vRstack4");
  
  // could do this directly on stack
  
  ValWsp xValWsp;
  
  nnpTask->m_xReturnStack.vPeek(4,&xValWsp);
  
  nnpTask->m_xDataStack.vPush(xValWsp);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* r5 -- place copy of 0th value in return stack onto param stack             */
/*----------------------------------------------------------------------------*/
void Task::vRstack5(NNP_Task nnpTask)
{
  ENTRY("vRstack5");
  
  // could do this directly on stack
  
  ValWsp xValWsp;
  
  nnpTask->m_xReturnStack.vPeek(5,&xValWsp);
  
  nnpTask->m_xDataStack.vPush(xValWsp);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* r6 -- place copy of 6th value in return stack onto param stack             */
/*----------------------------------------------------------------------------*/
void Task::vRstack6(NNP_Task nnpTask)
{
  ENTRY("vRstack6");
  
  // could do this directly on stack
  
  ValWsp xValWsp;
  
  nnpTask->m_xReturnStack.vPeek(6,&xValWsp);
  
  nnpTask->m_xDataStack.vPush(xValWsp);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* r7 -- place copy of 7th value in return stack onto param stack             */
/*----------------------------------------------------------------------------*/
void Task::vRstack7(NNP_Task nnpTask)
{
  ENTRY("vRstack7");
  
  // could do this directly on stack
  
  ValWsp xValWsp;
  
  nnpTask->m_xReturnStack.vPeek(7,&xValWsp);
  
  nnpTask->m_xDataStack.vPush(xValWsp);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* r8 -- place copy of 8th value in return stack onto param stack             */
/*----------------------------------------------------------------------------*/
void Task::vRstack8(NNP_Task nnpTask)
{
  ENTRY("vRstack8");
  
  // could do this directly on stack
  
  ValWsp xValWsp;
  
  nnpTask->m_xReturnStack.vPeek(8,&xValWsp);
  
  nnpTask->m_xDataStack.vPush(xValWsp);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* r9 -- place copy of 9th value in return stack onto param stack             */
/*----------------------------------------------------------------------------*/
void Task::vRstack9(NNP_Task nnpTask)
{
  ENTRY("vRstack9");
  
  // could do this directly on stack
  
  ValWsp xValWsp;
  
  nnpTask->m_xReturnStack.vPeek(9,&xValWsp);
  
  nnpTask->m_xDataStack.vPush(xValWsp);
  
  EXIT();
}

// seitf: //////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_SEitf(void)
{
  vAddPrimitive("seitf.count",     &vSEitfCount,       false);
  vAddPrimitive("seitf.delete",    &vSEitfDelete,      false);
  vAddPrimitive("seitf.dump",      &vSEitfDump,        false);
  vAddPrimitive("seitf.req@",      &vSEitfReqFetch,    false);
  vAddPrimitive("seitf.req!",      &vSEitfReqStore,    false);
  vAddPrimitive("seitf.rsp@",      &vSEitfRspFetch,    false);
  vAddPrimitive("seitf.rsp!",      &vSEitfRspStore,    false);
  vAddPrimitive("seitf.table@",    &vSEitfTableFetch,  false);
  vAddPrimitive("seitf.tableRef",  &vSEitfTableRef,    false);
  vAddPrimitive("seitf.tableDeref",&vSEitfTableDeref,  false);
}

/*----------------------------------------------------------------------------*/
/* seitf.count -- get count of SE interface's                                 */
/*----------------------------------------------------------------------------*/
void Task::vSEitfCount(NNP_Task nnpTask)
{
  ENTRY("vSEitfCount");
  
  Object* pPtrSEitf;
  
  nnpTask->m_xDataStack.vPush(gpSEitfList->iCount());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* seitf.delete -- delete SE interface                                        */
/*----------------------------------------------------------------------------*/
void Task::vSEitfDelete(NNP_Task nnpTask)
{
ENTRY("vSEitfDelete");
  
  Object* pPtrSEitf;
  
  if ( ! nnpTask->m_xDataStack.bPop(pPtrSEitf) )
    THROW_INVALID_DSTACK("seitf.delete");

  SEitf* pSEitf = _pSEitf(pPtrSEitf);
  ASSERT(CN_SEITF == pSEitf->eClass());
  
  delete pSEitf;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* seitf.dump -- dump of SE interface                                         */
/*----------------------------------------------------------------------------*/
void Task::vSEitfDump(NNP_Task nnpTask)
{
ENTRY("vSEitfDump");
  
  Object* pPtrSEitf;
  if ( ! nnpTask->m_xDataStack.bPop(pPtrSEitf) )
    THROW_INVALID_DSTACK("seitf.dump");
  
  SEitf* pSEitf = _pSEitf(pPtrSEitf);
  ASSERT(CN_SEITF == pSEitf->eClass());
  
  pSEitf->vDump();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* seitf.req@ -- fetch SEitf request queue                                    */
/*----------------------------------------------------------------------------*/
void Task::vSEitfReqFetch(NNP_Task nnpTask)
{
ENTRY("vSEitfReqFetch");
  
  Object* pPtrSEitf;
  if ( ! nnpTask->m_xDataStack.bPop(pPtrSEitf) )
    THROW_INVALID_DSTACK("seitf.req@");
  
  SEitf* pSEitf = _pSEitf(pPtrSEitf);
  ASSERT(CN_SEITF == pSEitf->eClass());
  
  Object* pPtrQueue = pSEitf->pRequestQueue();
  
  nnpTask->m_xDataStack.vPush(pPtrQueue);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* seitf.req! -- store SEitf request queue                                    */
/*----------------------------------------------------------------------------*/
void Task::vSEitfReqStore(NNP_Task nnpTask)
{
ENTRY("vSEitfReqStore");
  
  Object* pPtrSEitf;
  if ( ! nnpTask->m_xDataStack.bPop(pPtrSEitf) )
    THROW_INVALID_DSTACK("seitf.req!");
  
  SEitf* pSEitf = _pSEitf(pPtrSEitf);
  ASSERT(CN_SEITF == pSEitf->eClass());
  
  Object* pPtrQueue;
  
  if ( ! nnpTask->m_xDataStack.bPop(pPtrQueue) )
    THROW_INVALID_DSTACK("seitf.req!");
  
  Queue* pQueue = _pQueue(pPtrQueue);
  ASSERT(CN_QUEUE == pQueue->eClass());
  
  pSEitf->vRequestQueue(pQueue);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* seitf.rsp@ -- fetch SEitf response queue                                   */
/*----------------------------------------------------------------------------*/
void Task::vSEitfRspFetch(NNP_Task nnpTask)
{
ENTRY("vSEitfRspFetch");
  
  Object* pPtrSEitf;
  if ( ! nnpTask->m_xDataStack.bPop(pPtrSEitf) )
    THROW_INVALID_DSTACK("seitf.rsp@");
  
  SEitf* pSEitf = _pSEitf(pPtrSEitf);
  ASSERT(CN_SEITF == pSEitf->eClass());
  
  Object* pPtrQueue = pSEitf->pResponseQueue();
  
  nnpTask->m_xDataStack.vPush(pPtrQueue);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* seitf.rsp! -- store SEitf response queue                                   */
/*----------------------------------------------------------------------------*/
void Task::vSEitfRspStore(NNP_Task nnpTask)
{
ENTRY("vSEitfRspStore");
  
  Object* pPtrSEitf;
  if ( ! nnpTask->m_xDataStack.bPop(pPtrSEitf) )
    THROW_INVALID_DSTACK("seitf.rsp!");
  
  SEitf* pSEitf = _pSEitf(pPtrSEitf);
  ASSERT(CN_SEITF == pSEitf->eClass());
  
  Object* pPtrQueue;
  
  if ( ! nnpTask->m_xDataStack.bPop(pPtrQueue) )
    THROW_INVALID_DSTACK("seitf.rsp!");
  
  Queue* pQueue = _pQueue(pPtrQueue);
  ASSERT(CN_QUEUE == pQueue->eClass());
  
  pSEitf->vResponseQueue(pQueue);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* seitf.table@ -- fetch table from SEitf                                     */
/*----------------------------------------------------------------------------*/
void Task::vSEitfTableFetch(NNP_Task nnpTask)
{
  ENTRY("vSEitfTableFetch");
  
  Object* pPtrSEitf;
  if ( ! nnpTask->m_xDataStack.bPop(pPtrSEitf) )
    THROW_INVALID_DSTACK("seitf.rsp!");
  
  SEitf* pSEitf = _pSEitf(pPtrSEitf);
  ASSERT(CN_SEITF == pSEitf->eClass());
  
  nnpTask->m_xDataStack.vPush(pSEitf->pCurrentTable());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* seitf.tableDeref -- store table in SEitf and inc reference                 */
/*----------------------------------------------------------------------------*/
void Task::vSEitfTableDeref(NNP_Task nnpTask)
{
  ENTRY("vSEitfRspStore");
  
  Object* pPtrSEitf;
  if ( ! nnpTask->m_xDataStack.bPop(pPtrSEitf) )
    THROW_INVALID_DSTACK("seitf.tableDeref");
  
  SEitf* pSEitf = _pSEitf(pPtrSEitf);
  ASSERT(CN_SEITF == pSEitf->eClass());
  
  Table* pTable = pSEitf->pCurrentTable();
  ASSERT(CN_TABLE == pTable->eClass());
  
  pTable->vRefDec();

  pSEitf->vCurrentTable(nullptr);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* seitf.tableRef -- store table in SEitf and inc reference                   */
/*----------------------------------------------------------------------------*/
void Task::vSEitfTableRef(NNP_Task nnpTask)
{
  ENTRY("vSEitfTableRef");
  
  Object* pPtrSEitf;
  if ( ! nnpTask->m_xDataStack.bPop(pPtrSEitf) )
    THROW_INVALID_DSTACK("seitf.tableRef");
  
  SEitf* pSEitf = _pSEitf(pPtrSEitf);
  ASSERT(CN_SEITF == pSEitf->eClass());
  
  Object* pPtrTable;
  
  if ( ! nnpTask->m_xDataStack.bPop(pPtrTable) )
    THROW_INVALID_DSTACK("seitf.tableRef");
  
  Table* pTable = _pTable(pPtrTable);
  ASSERT(CN_TABLE == pTable->eClass());
  
  pTable->vRefInc();
  
  pSEitf->vCurrentTable(pTable);
  
  EXIT();
}

// seitfcursor: ////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_SEitfCursor(void)
{
  vAddPrimitive("seitfCursor.begin",  &vSEitfCursorBegin,  false);
  vAddPrimitive("seitfCursor.delete", &vSEitfCursorDelete, false);
  vAddPrimitive("seitfCursor.end",    &vSEitfCursorEnd,    false);
  vAddPrimitive("seitfCursor.first",  &vSEitfCursorFirst,  false);
  vAddPrimitive("seitfCursor.get",    &vSEitfCursorGet,    false);
  vAddPrimitive("seitfCursor.init",   &vSEitfCursorInit,   false);
  vAddPrimitive("seitfCursor.new",    &vSEitfCursorNew,    false);
  vAddPrimitive("seitfCursor.next",   &vSEitfCursorNext,   false);
}

/*----------------------------------------------------------------------------*/
/* seitfcursor.begin -- begin using seitf currsor                             */
/*----------------------------------------------------------------------------*/
void Task::vSEitfCursorBegin(NNP_Task nnpTask)
{
ENTRY("vSEitfCursorBegin");
  
  Object* pPtrSEitfCursor;
  if ( ! nnpTask->m_xDataStack.bPop(pPtrSEitfCursor) )
    THROW_INVALID_DSTACK("seitfcursor.begin");
  
  SEitfCursor* pSEitfCursor = _pSEitfCursor(pPtrSEitfCursor);
  ASSERT(CN_SEITFCURSOR == pSEitfCursor->eClass());

  pSEitfCursor->vBegin();
  pSEitfCursor->vFirst();
  
  Object* pObject = pSEitfCursor->pObject();
  
  nnpTask->m_xDataStack.vPush(pObject);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* seitfcursor.delete -- delete an seitf cursor                               */
/*----------------------------------------------------------------------------*/
void Task::vSEitfCursorDelete(NNP_Task nnpTask)
{
ENTRY("vSEitfCursorDelete");
  
  Object* pPtrSEitfCursor;
  if ( ! nnpTask->m_xDataStack.bPop(pPtrSEitfCursor) )
    THROW_INVALID_DSTACK("seitfcursor.delete");
  
  SEitfCursor* pSEitfCursor = _pSEitfCursor(pPtrSEitfCursor);
  ASSERT(CN_SEITFCURSOR == pSEitfCursor->eClass());
  
  Object* pObject = reinterpret_cast<Object*>(pSEitfCursor);
  delete pSEitfCursor;
  
  Heap::vFree(pObject);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* seitfcursor.end -- end using seitf cursor                                  */
/*----------------------------------------------------------------------------*/
void Task::vSEitfCursorEnd(NNP_Task nnpTask)
{
ENTRY("vSEitfCursorEnd");
  
  Object* pPtrSEitfCursor;
  if ( ! nnpTask->m_xDataStack.bPop(pPtrSEitfCursor) )
    THROW_INVALID_DSTACK("seitfcursor.end");
  
  SEitfCursor* pSEitfCursor = _pSEitfCursor(pPtrSEitfCursor);
  ASSERT(CN_SEITFCURSOR == pSEitfCursor->eClass());
  
  pSEitfCursor->vEnd();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* seitfcursor.first -- set to first and get seitf                            */
/*----------------------------------------------------------------------------*/
void Task::vSEitfCursorFirst(NNP_Task nnpTask)
{
ENTRY("vSEitfCursorFirst");
  
  Object* pPtrSEitfCursor;
  if ( ! nnpTask->m_xDataStack.bPop(pPtrSEitfCursor) )
    THROW_INVALID_DSTACK("seitfcursor.first");
  
  SEitfCursor* pSEitfCursor = _pSEitfCursor(pPtrSEitfCursor);
  ASSERT(CN_SEITFCURSOR == pSEitfCursor->eClass());
  
  pSEitfCursor->vFirst();
  
  Object* pObject = pSEitfCursor->pObject();
  
  nnpTask->m_xDataStack.vPush(pObject);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* seitfcursor.get -- get current seitf                                       */
/*----------------------------------------------------------------------------*/
void Task::vSEitfCursorGet(NNP_Task nnpTask)
{
ENTRY("vSEitfCursorGet");
  
  Object* pPtrSEitfCursor;
  if ( ! nnpTask->m_xDataStack.bPop(pPtrSEitfCursor) )
    THROW_INVALID_DSTACK("seitfcursor.get");
  
  SEitfCursor* pSEitfCursor = _pSEitfCursor(pPtrSEitfCursor);
  ASSERT(CN_SEITFCURSOR == pSEitfCursor->eClass());
  
  Object* pObject = pSEitfCursor->pObject();
  
  nnpTask->m_xDataStack.vPush(pObject);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* seitfcursor.init -- init an seitf cursor                                   */
/*----------------------------------------------------------------------------*/
void Task::vSEitfCursorInit(NNP_Task nnpTask)
{
ENTRY("vSEitfCursorInit");
  
  Object* pPtrSEitfCursor;
  if ( ! nnpTask->m_xDataStack.bPop(pPtrSEitfCursor) )
    THROW_INVALID_DSTACK("seitfcursor.init");
  
  SEitfCursor* pSEitfCursor = _pSEitfCursor(pPtrSEitfCursor);
  ASSERT(CN_SEITFCURSOR == pSEitfCursor->eClass());
  
  s32 sName;
  sName = "seitflist";
  Word* pWord = nnpTask->pFindWord(sName);
  ASSERT(pWord);
  SEitfList* pSEitfList = reinterpret_cast<SEitfList*>(pWord->pDataArea());
  
  
  new(pSEitfCursor) SEitfCursor(pSEitfList);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* seitfcursor.new -- new an seitf cursor                                     */
/*----------------------------------------------------------------------------*/
void Task::vSEitfCursorNew(NNP_Task nnpTask)
{
ENTRY("vSEitfCursorNew");
  
  Object* pPtrSEitfList;
  if ( ! nnpTask->m_xDataStack.bPop(pPtrSEitfList) )
    THROW_INVALID_DSTACK("seitfcursor.new");
  
  SEitfList* pSEitfList = _pSEitfList(pPtrSEitfList);
  ASSERT(CN_SEITFLIST == pSEitfList->eClass());
  
  SEitfCursor* pSEitfCursor = _pSEitfCursor(Heap::pAllocate(CN_SEITFCURSOR));

  new(pSEitfCursor) SEitfCursor(pSEitfList);
  
  Object* pPtrSEitfCursor = pSEitfCursor;
  
  nnpTask->m_xDataStack.vPush(pPtrSEitfCursor);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* seitfcursor.next -- advance to next and get seitf                          */
/*----------------------------------------------------------------------------*/
void Task::vSEitfCursorNext(NNP_Task nnpTask)
{
ENTRY("vSEitfCursorNext");
  
  Object* pPtrSEitfCursor;
  if ( ! nnpTask->m_xDataStack.bPop(pPtrSEitfCursor) )
    THROW_INVALID_DSTACK("seitfcursor.next");
  
  SEitfCursor* pSEitfCursor = _pSEitfCursor(pPtrSEitfCursor);
  ASSERT(CN_SEITFCURSOR == pSEitfCursor->eClass());
  
  pSEitfCursor->vNext();
  
  Object* pObject = pSEitfCursor->pObject();
  
  nnpTask->m_xDataStack.vPush(pObject);
  
  EXIT();
}

// seitflist: //////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_SEitfList(void)
{
  vAddPrimitive("seitfList.dump",  &vSEitfListDump,    false);
  vAddPrimitive("seitfList.init",  &vSEitfListInit,    false);
}

/*----------------------------------------------------------------------------*/
/* seitflist.dump -- dump of seitfList on param stack                         */
/*----------------------------------------------------------------------------*/
void Task::vSEitfListDump(NNP_Task nnpTask)
{
ENTRY("vSEitfListDump");
  
  Object* pPtrSEitfList;
  if ( ! nnpTask->m_xDataStack.bPop(pPtrSEitfList) )
    THROW_INVALID_DSTACK("seitflist.dump");
  
  SEitfList* pSEitfList = _pSEitfList(pPtrSEitfList);
  ASSERT(CN_SEITFLIST == pSEitfList->eClass());
  
  pSEitfList->vDump("SEitfList:", pSingleTab());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* seitflist.init -- init of seitfList on param stack                         */
/*----------------------------------------------------------------------------*/
void Task::vSEitfListInit(NNP_Task nnpTask)
{
ENTRY("vSEitfListInit");
  
  Object* pPtrSEitfList;
  if ( ! nnpTask->m_xDataStack.bPop(pPtrSEitfList) )
    THROW_INVALID_DSTACK("seitflist.init");
  
  SEitfList* pSEitfList = _pSEitfList(pPtrSEitfList);
  ASSERT(CN_SEITFLIST == pSEitfList->eClass());
  
  new(pSEitfList) SEitfList;
  
  EXIT();
}

// shell: //////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_Shell(void)
{
  vAddPrimitive("shell.execute",  &vShellExecute,  false);
}

/*----------------------------------------------------------------------------*/
/* shell.execute -- execute given shell command                               */
/*----------------------------------------------------------------------------*/
void Task::vShellExecute(NNP_Task nnpTask)
{
  ENTRY("vShellExecute");
  
  ValWsp xCommand;
  
  if ( ! nnpTask->m_xDataStack.bPop(xCommand) )
    THROW_INVALID_DSTACK("shell.execute");
  
  SysCall xSysCall;
  
  i8 iExitStatus = Shell::iExecute(xCommand.sStr(),xSysCall);
  
  nnpTask->m_xDataStack.vPush(iExitStatus);
  
  EXIT();
}

// sockets: ////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_Socket(void)
{
  vAddPrimitive("socket.accept",   &vSocketAccept,     false);
  vAddPrimitive("socket.close",    &vSocketClose,      false);
  vAddPrimitive("socket.connect",  &vSocketConnect,    false);
  vAddPrimitive("socket.listen",   &vSocketListen,     false);
  vAddPrimitive("socket.open",     &vSocketOpen,       false);
  vAddPrimitive("socket.recv",     &vSocketRecv,       false);
  vAddPrimitive("socket.send",     &vSocketSend,       false);
}

/*----------------------------------------------------------------------------*/
/* socket.accept: accept a socket connection                                  */
/*----------------------------------------------------------------------------*/
void Task::vSocketAccept(NNP_Task nnpTask)
{
  SysCall xSysCall;
  Socket* pSocket;
  int     iSocketId;
  Object* pObject;
  
  ENTRY("vSocketAccept");
  
  if ( ! nnpTask->m_xDataStack.bPop(pObject) )
    THROW_INVALID_DSTACK("socket.accept");
  
  // get pSocket
  pSocket = reinterpret_cast<Socket*>(pObject);
  
  // accept a connection
  bool bResult = pSocket->bAcceptClient(iSocketId, xSysCall);
  
  if ( ! bResult )
    THROW("Unable to accept connection: Errno %d!", xSysCall.iError());
  
  Task* pTask = (Task*)0x208f500c8;
  
  // return the new socket
  nnpTask->m_xDataStack.vPush(new Socket(iSocketId));
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* open a socket                                                              */
/*----------------------------------------------------------------------------*/
void Task::vSocketClose(NNP_Task nnpTask)
{
  SysCall xSysCall;
  Socket* pSocket;
  Object* pObject;
  
  ENTRY("vSocketClose");
  
  if ( ! nnpTask->m_xDataStack.bPop(pObject) )
    THROW_INVALID_DSTACK("socketclose");
  
  // get pSocket
  pSocket = reinterpret_cast<Socket*>(pObject);
  
  // close the socket
  bool bResult = pSocket->bClose(xSysCall);
  
  if ( ! bResult )
    THROW("Unable to close socket: Errno %d!", xSysCall.iError());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* socket.connect: connect to a server                                        */
/*----------------------------------------------------------------------------*/
void Task::vSocketConnect(NNP_Task nnpTask)
{
  SysCall xSysCall;
  Socket* pSocket;
  Object* pObject;
  ValWsp xNode;
  
  i8 iPort;
  
  ENTRY("vSocketConnect");
  
  if ( ! nnpTask->m_xDataStack.bPop(pObject) )
    THROW_INVALID_DSTACK("socket.connect");
  
  if ( ! nnpTask->m_xDataStack.bPop(xNode) )
    THROW_INVALID_DSTACK("socket.connect");
  
  if ( ! nnpTask->m_xDataStack.bPop(iPort) )
    THROW_INVALID_DSTACK("socket.connect");
  
  // get address of node
  const char* sAddr = reinterpret_cast<char*>(xNode.sStr());
  
  // get pSocket
  pSocket = reinterpret_cast<Socket*>(pObject);
  
  bool bResult = false;
  
  // try connecting in a loop if timeout occurs
  // possibly other node not back to accepting connections
  
  i8 iTimedOutCount = 0;
  i8 iConnRefusedCount = 0;
  
  while (!bResult) {
    bResult = pSocket->bConnectServer(sAddr, (int)iPort, xSysCall);
    if ( ! bResult ) {
      i8 iError = xSysCall.iError();
      
      if (iError == ETIMEDOUT) {
        if (0 == iTimedOutCount++)
          vTrace("Connect timedout... retrying...\n");
        continue;
      }
      
      if (iError == ECONNREFUSED) {
        if (0 == iConnRefusedCount++)
          vTrace("Connect refused... retrying...\n");
        continue;
      }
      
      THROW("Unable to connect: Errno %d!", iError);
    }
  }
  
  // return the socket object
  Object* pPtrSocket = pSocket;
  nnpTask->m_xDataStack.vPush(pPtrSocket);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* socket.listen: listen in for socket connections                            */
/*----------------------------------------------------------------------------*/
void Task::vSocketListen(NNP_Task nnpTask)
{
  SysCall xSysCall;
  Socket* pSocket;
  i8      iPort;
  
  ENTRY("vSocketListen");
  
  if ( ! nnpTask->m_xDataStack.bPop(iPort) )
    THROW_INVALID_DSTACK("socketlisten");
  
  pSocket = new Socket;
  bool bResult = pSocket->bCreateServer((int)iPort, xSysCall);
  
  if ( ! bResult )
    THROW("Unable to listen for connections: Errno %d!", xSysCall.iError());
  
  // return new socket
  Object* pPtrSocket = pSocket;
  nnpTask->m_xDataStack.vPush(pPtrSocket);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* socket.open: open a socket                                                 */
/*----------------------------------------------------------------------------*/
void Task::vSocketOpen(NNP_Task nnpTask)
{
  SysCall xSysCall;
  Socket* pSocket;
  
  ENTRY("vSocketOpen");
  
  pSocket = new Socket;
  bool bResult = pSocket->bSocket(xSysCall);
  
  if ( ! bResult )
    THROW("Unable to create socker: Errno %d!", xSysCall.iError());
  
  // return pSocket
  Object* pPtrSocket = pSocket;
  nnpTask->m_xDataStack.vPush(pPtrSocket);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* socket.send: send to a socket ( valset socket -- 1 )                       */
/*----------------------------------------------------------------------------*/
void Task::vSocketSend(NNP_Task nnpTask)
{
  ENTRY("vSocketSend");
  
  SysCall xSysCall;
  Socket* pSocket;
  Object* pObject;
  ValSet  xValSet;
  bool    bResult;
  
  if ( ! nnpTask->m_xDataStack.bPop(pObject) )
    THROW_INVALID_DSTACK("socket.send");
  
  if ( ! nnpTask->m_xDataStack.bPop(xValSet) )
    THROW_INVALID_DSTACK("socket.send");
  
  pSocket = reinterpret_cast<Socket*>(pObject);
  
  //******************************************************************
  // Send the size of valset
  //******************************************************************
  i8 iBytes = xValSet.iSize();
  bResult = pSocket->bSendTo(sizeof(iBytes), &iBytes, xSysCall);
  
  if ( ! bResult ) {
    //THROW("Unable to sendTo socket: Errno %d!", xSysCall.iError());
    nnpTask->m_xDataStack.vPush((i8)0);
    
    //goto exit;
    EXIT();
  }
  
  //******************************************************************
  // now send the actual value set
  //******************************************************************
  bResult = pSocket->bSendTo(iBytes, &xValSet, xSysCall);
  
  if ( ! bResult ) {
    //THROW("Unable to sendTo socket: Errno %d!", xSysCall.iError());
    nnpTask->m_xDataStack.vPush((i8)0);
  }
  
  //******************************************************************
  // get rowBuffer if any in valset
  //******************************************************************
  RowBuffer* pRowBuffer = xValSet.pGetRowBuffer();
  
  if (pRowBuffer) {
    
    //******************************************************************
    // send rowBuffer size
    //******************************************************************
    i8 iBytes = pRowBuffer->iBytesUsed();
    bResult = pSocket->bSendTo(sizeof(iBytes), &iBytes, xSysCall);
    
    if ( ! bResult ) {
      //THROW("Unable to sendTo socket: Errno %d!", xSysCall.iError());
      nnpTask->m_xDataStack.vPush((i8)0);
      
      //goto exit;
      EXIT();
    }
    
    //******************************************************************
    // send rowBuffer itself
    //******************************************************************
    bResult = pSocket->bSendTo(iBytes, pRowBuffer->pDataArea(), xSysCall);
    
    if ( ! bResult ) {
      //THROW("Unable to sendTo socket: Errno %d!", xSysCall.iError());
      nnpTask->m_xDataStack.vPush((i8)0);
      
      //goto exit;
      EXIT();
    }
    
  }
  
  nnpTask->m_xDataStack.vPush((i8)1);
  
exit:
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* socket.recv: recv from a socket                                            */
/*----------------------------------------------------------------------------*/
void Task::vSocketRecv(NNP_Task nnpTask)
{
  ENTRY("vSocketRecv");
  
  SysCall xSysCall;
  Socket* pSocket;
  ValSet xValSet;
  //s8K   sWorkspace;
  Object* pObject;
  i8 iBytes;
  
  if ( ! nnpTask->m_xDataStack.bPop(pObject) )
    THROW_INVALID_DSTACK("socketrecv");
  
  pSocket = reinterpret_cast<Socket*>(pObject);
  
  if (gxArguments.m_bTracepoint)
    vTrace("[N%lld:%8s\\%lld %-11s] receiving on %lld\n",
           gxArguments.m_iNode,
           nnpTask->m_sName.a(),
           *nnpTask->m_zAcks,
           nnpTask->m_sComment.a(),
           
           pSocket->m_iSocketId);
  
  //******************************************************************
  // Receive the size of valset
  //******************************************************************
  bool bResult = pSocket->bRecvBytes(sizeof(iBytes), reinterpret_cast<void*>(&iBytes), xSysCall);
  
  if ( ! bResult )
    THROW("Unable to recvFrom socket: Errno %d!", xSysCall.iError());
  
  //******************************************************************
  // Receive the valset
  //******************************************************************
  bResult = pSocket->bRecvBytes(iBytes, reinterpret_cast<void*>(&xValSet), xSysCall);
  
  if ( ! bResult )
    THROW("Unable to recvFrom socket: Errno %d!", xSysCall.iError());
  
  //******************************************************************
  // Check if valset has a row buffer
  //******************************************************************
  bool bHasRowBuffer = xValSet.bHasRowBuffer();
  
  if (bHasRowBuffer) {
    
    //******************************************************************
    // receive rowBuffer size
    //******************************************************************
    bResult = pSocket->bRecvBytes(sizeof(iBytes), reinterpret_cast<void*>(&iBytes), xSysCall);
    
    if ( ! bResult )
      THROW("Unable to recvFrom socket: Errno %d!", xSysCall.iError());
    
    //******************************************************************
    // receive rowBuffer
    //******************************************************************
    RowBuffer* pRowBuffer = new RowBuffer(iBytes);
    bResult = pSocket->bRecvBytes(iBytes, reinterpret_cast<void*>(pRowBuffer->pDataArea()), xSysCall);
    
    if ( ! bResult )
      THROW("Unable to recvFrom socket: Errno %d!", xSysCall.iError());
    
    
    //******************************************************************
    // set rowBuffer in valset
    //******************************************************************
    bool bSet = xValSet.bSetRowBuffer(pRowBuffer);
    ASSERT(bSet);
  }
  
  nnpTask->m_xDataStack.vPush(xValSet);
  
  EXIT();
}

// steps: //////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_Steps(void)
{
  vAddPrimitive("steps.list",  &vStepsList,  false);
  vAddPrimitive("steps.reset", &vStepsReset, false);
}

/*----------------------------------------------------------------------------*/
/* steps.list -- list steps                                                   */
/*----------------------------------------------------------------------------*/
void Task::vStepsList(NNP_Task nnpTask)
{
ENTRY("vStepsList");

  gpStepArray->vList();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* steps.reset -- reset steps                                                 */
/*----------------------------------------------------------------------------*/
void Task::vStepsReset(NNP_Task nnpTask)
{
  ENTRY("vStepsReset");
  
  gpStepArray->vReset();
  
  EXIT();
}

// string: /////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_Str(void)
{
  vAddPrimitive("strpad",      &vStringPad,   false);
  vAddPrimitive("streql",      &vStrEql,      false);
  vAddPrimitive("strlen",      &vStrLen,      false);
  vAddPrimitive("subStr",      &vSubStr,      false);
  vAddPrimitive("concat",      &vConcat,      false);
  vAddPrimitive("clip",        &vClip,        false);
  vAddPrimitive("prepend",     &vPrepend,     false);
  vAddPrimitive("toDigitName", &vToDigitName, false);
  vAddPrimitive("rndStr",      &vRndStr,      false);
  vAddPrimitive("sprint",      &vSprint,      false);
  vAddPrimitive("s256.init",   &vS256Init,    false);
  vAddPrimitive("csv.firstValue",  &vCsvFirstValue,    false);
}

/*----------------------------------------------------------------------------*/
/* csv.firstValue -- get first value from csv string                          */
/*----------------------------------------------------------------------------*/
void Task::vCsvFirstValue(NNP_Task nnpTask)
{
  ENTRY("vCsvFirstValue");
  
  ValWsp xStr;
  if ( ! nnpTask->m_xDataStack.bPop(xStr) )
    THROW_INVALID_DSTACK("csv.firstValue");
  
  s256 sFirstValue;
  
  char* z = xStr.sStr();
  
  if (! String::bExtractItem(&z,sFirstValue.a()))
    sFirstValue = "";
  
  nnpTask->m_xDataStack.vPush(sFirstValue);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* strpad -- pad string to given length                                       */
/*----------------------------------------------------------------------------*/
void Task::vStringPad(NNP_Task nnpTask)
{
ENTRY("vStringPad");
  
  i8 iLength;

  ValWsp xStringA;
  
  // get the arguments
  if ( ! nnpTask->m_xDataStack.bPop(iLength,xStringA) )
    THROW_INVALID_DSTACK("strpad");
  
  String::vPad(xStringA.sStr(),iLength,' ');
  xStringA.pAsStrVal()->vSize();
  
  nnpTask->m_xDataStack.vPush(xStringA);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* streql -- test string equality                                             */
/*----------------------------------------------------------------------------*/
void Task::vStrEql(NNP_Task nnpTask)
{
ENTRY("vStrEql");
  
  ValWsp xStringA;
  ValWsp xStringB;
  
  // get the arguments
  if ( ! nnpTask->m_xDataStack.bPop(xStringB,xStringA) )
    THROW_INVALID_DSTACK("streql");
  
  i8 iEqual = String::bEqualTo(xStringA.sStr(),xStringB.sStr());
  
  nnpTask->m_xDataStack.vPush(iEqual);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* strlen -- string length                                                    */
/*----------------------------------------------------------------------------*/
void Task::vStrLen(NNP_Task nnpTask)
{
ENTRY("vStrLen");
  
  i8 iLength;
  s8K sString;
  
  // get the arguments
  if ( ! nnpTask->m_xDataStack.bPop(sString) )
    THROW_INVALID_DSTACK("strlen");
  
  iLength = String::iLength(sString.a());
  
  nnpTask->m_xDataStack.vPush(iLength);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* substr -- substring ( start length str == substr )                         */
/*----------------------------------------------------------------------------*/
//void Task::vSubStr(NNP_Task nnpTask)
//{
//ENTRY("vSubStr");
//  
//  i8 iLength;
//  i8 iStart;
//  ValWsp xString;
//  ValWsp xSubStr;
//  
//  StrVal* pStrVal = xString.pAsStrVal();
//  
//  // get the arguments
//  if ( ! nnpTask->m_xDataStack.bPop(iStart,iLength,xString) )
//    THROW_INVALID_DSTACK("substr");
//  
//  // validate start index
//  if ( iStart < 0 || iStart >= pStrVal->iChars())
//    THROW("Invalid start index for: subStr");
//  
//  // handle to end of string case
//  if (-1 == iLength)
//    iLength = pStrVal->iChars() - iStart;
//  
//  if ( iLength <= 0 || iStart + iLength > pStrVal->iChars())
//    THROW("Invalid length for: substr");
//  
//  // compute substring
//  //xSubStr.vCopyString(xString.sStr() + iStartIndex,iLength);
//  
//  char* pStart = xString.sStr() + iStart;
//  
//  xSubStr.pAsStrVal()->vCopyString(pStart,iLength);
//  
//  nnpTask->m_xDataStack.vPush(&xSubStr);
//  
//  EXIT();
//}
void Task::vSubStr(NNP_Task nnpTask)
{
  ENTRY("vSubStr");
  
  i8 iLength;
  i8 iStart;
  s8K sString;
  s8K sSubStr;
  
  // get the arguments
  if ( ! nnpTask->m_xDataStack.bPop(iStart,iLength,sString) )
    THROW_INVALID_DSTACK("substr");
  
  i8 iHasLength = String::iLength(sString.a());
  
  // validate start index
  if ( iStart < 0 || iStart >= iHasLength)
    THROW("Invalid start index for: subStr");
  
  // handle to end of string case
  if (-1 == iLength)
    iLength = iHasLength - iStart;
  
  if ( iLength <= 0 || iStart + iLength > iHasLength)
    THROW("Invalid length for: substr");
  
  char* pStart = sString.a() + iStart;
  
  String::vCopyLen(sSubStr.a(),pStart,iLength);
  
  nnpTask->m_xDataStack.vPush(sSubStr);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* concat -- concat two strings                                               */
/*----------------------------------------------------------------------------*/
void Task::vConcat(NNP_Task nnpTask)
{
ENTRY("vConcat");
  
  ValWsp xStrR;
  ValWsp xStrB;
  ValWsp xStrA;
  
  if ( ! nnpTask->m_xDataStack.bPop(xStrB,xStrA) )
    THROW_INVALID_DSTACK("concat");
  
  xStrR = xStrA;
  
  String::vConcat(xStrR.sStr(),xStrB.sStr());
  xStrR.pAsStrVal()->vSize();
  
  nnpTask->m_xDataStack.vPush(xStrR);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* clip -- clip string ( length str -- str )                                  */
/*----------------------------------------------------------------------------*/
//void Task::vClip(NNP_Task nnpTask)
//{
//ENTRY("vClip");
//  
//  i8 iLength;
//  ValWsp xString;
//  ValWsp xSubStr;
//  
//  StrVal* pStrVal = xString.pAsStrVal();
//  
//  // get the arguments
//  if ( ! nnpTask->m_xDataStack.bPop(iLength,xString) )
//    THROW_INVALID_DSTACK("clip");
//  
//  // validate length
//  i8 iStrLength = pStrVal->iChars();
//  
//  if ( iLength == 0 || llabs(iLength) >= iStrLength)
//    THROW("Invalid length for: clip");
//  
//  char* pStart;
//  
//  if (iLength > 0)
//    pStart = xString.sStr();
//  else
//    pStart = xString.sStr() + iStrLength - 1 - llabs(iLength);
//  
//  xSubStr.pAsStrVal()->vCopyString(pStart,llabs(iLength));
//  
//  nnpTask->m_xDataStack.vPush(&xSubStr);
//  
//  EXIT();
//}
void Task::vClip(NNP_Task nnpTask)
{
  ENTRY("vClip");
  
  i8 iLength;
  s8K sString;
  s8K sSubStr;
  
  // get the arguments
  if ( ! nnpTask->m_xDataStack.bPop(iLength,sString) )
    THROW_INVALID_DSTACK("clip");
  
  // validate length
  i8 iHasLength = String::iLength(sString.a());
  
  if ( iLength == 0 || llabs(iLength) >= iHasLength)
    THROW("Invalid length for: clip");
  
  char* pStart;
  
  if (iLength > 0)
    pStart = sString.a();
  else
    pStart = sString.a() + iHasLength - 1 - llabs(iLength);
  
  String::vCopyLen(sSubStr.a(),pStart,llabs(iLength));
  
  nnpTask->m_xDataStack.vPush(sSubStr);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* >DigitName -- convert to digit name                                        */
/*----------------------------------------------------------------------------*/
void Task::vToDigitName(NNP_Task nnpTask)
{
ENTRY("vToDigitName");
  
  i8 iDigit;
  
  // get the arguments
  if ( ! nnpTask->m_xDataStack.bPop(iDigit) )
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

  nnpTask->m_xDataStack.vPush(sName);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* prepend -- prepend a string before the other                               */
/*----------------------------------------------------------------------------*/
void Task::vPrepend(NNP_Task nnpTask)
{
  ENTRY("vPrepend");
  
  ValWsp xStrR;
  ValWsp xStrB;
  ValWsp xStrA;
  
  ValWsp xValA;
  ValWsp xValB;
  
  if ( ! nnpTask->m_xDataStack.bPop(xValB,xValA) )
  THROW_INVALID_DSTACK("prepend");
  
  // If received INTEGER convert to STRING
  if (xValA.eClass() == CN_INTVAL) {
    s8K sString;
    xValA.vToString(sString.a());
    xStrA = sString.a();
  }
  else {
   xStrA = xValA;
  }
  
  // If received INTEGER convert to STRING
  if (xValB.eClass() == CN_INTVAL) {
    s8K sString;
    xValB.vToString(sString.a());
    xStrB = sString.a();
  }
  else {
   xStrB = xValB;
  }
  
  xStrR = xStrB;
  
  String::vConcat(xStrR.sStr(),xStrA.sStr());
  xStrR.pAsStrVal()->vSize();
  
  nnpTask->m_xDataStack.vPush(xStrR);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* rndStr -- generate random string of given length                           */
/*----------------------------------------------------------------------------*/
void Task::vRndStr(NNP_Task nnpTask)
{
  ENTRY("vRndStr");
  
  i8 iLength;
  if ( ! nnpTask->m_xDataStack.bPop(iLength) )
    THROW_INVALID_DSTACK("rndStr");
  
  s8K sRnd;
  
  sRnd = Random::sRandomString(iLength).a();
  
  nnpTask->m_xDataStack.vPush(sRnd);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* sprint -- pop format string and use it for a sprintf                       */
/*----------------------------------------------------------------------------*/
void Task::vSprint(NNP_Task nnpTask)
{
  ENTRY("vSprint");
  
  s8K sFormat;
  if ( ! nnpTask->m_xDataStack.bPop(sFormat) )
    THROW_INVALID_DSTACK("sprint");
  
  s8K sString;
  
  nnpTask->vSprint(sFormat,sString);
  
  nnpTask->m_xDataStack.vPush(sString);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* s256.init -- initialise an s256                                            */
/*----------------------------------------------------------------------------*/
void Task::vS256Init(NNP_Task nnpTask)
{
  ENTRY("vS256Init");
  
  Object* pPtrStrVal;
  
  if ( ! nnpTask->m_xDataStack.bPop(pPtrStrVal) )
    THROW_INVALID_DSTACK("s256.init");
  
  StrVal* pStrVal = _pStrVal(pPtrStrVal);
  
  ASSERT(CN_STRVAL == pStrVal->eClass());
  
  new(pStrVal) StrVal(256);
  
  EXIT();
}

// table: //////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_Table(void)
{
  vAddPrimitive("table.append",       &vTableAppend,       false);
  vAddPrimitive("table.appendBuffer", &vTableAppendBuffer, false);
  vAddPrimitive("table.dic",          &vTableDic,          false);
  vAddPrimitive("table.drop",         &vTableDrop,         false);
  vAddPrimitive("table.init",         &vTableInit,         false);
  vAddPrimitive("table.insert",       &vTableInsert,       false);
  vAddPrimitive("table.name",         &vTableName,         false);
  vAddPrimitive("table.ref",          &vTableRef,          false);
  vAddPrimitive("table.space",        &vTableSpace,        false);
  vAddPrimitive("table.truncate",     &vTableTruncate,     false);
}

/*----------------------------------------------------------------------------*/
/* table.append -- append buffer into table (from seitf)                      */
/*----------------------------------------------------------------------------*/
void Task::vTableAppend(NNP_Task nnpTask)
{
  ENTRY("vTableAppend");
  i8 iPartition;
  s8K sString;
  
  if ( ! nnpTask->m_xDataStack.bPop(sString) )
    THROW_INVALID_DSTACK("table.append");
  
  Table* pTable = reinterpret_cast<Table*>(String::iToI8(sString.a()));
  ASSERT(CN_TABLE == pTable->eClass());
  
  if ( ! nnpTask->m_xDataStack.bPop(sString) )
    THROW_INVALID_DSTACK("table.append");
  
  RowBuffer* pRowBuffer = reinterpret_cast<RowBuffer*>(String::iToI8(sString.a()));
  ASSERT(CN_ROWBUFFER == pRowBuffer->eClass());
  
  if ( ! nnpTask->m_xDataStack.bPop(sString) )
    THROW_INVALID_DSTACK("table.append");
  
  iPartition = String::iToI8(sString.a());
  pTable->vAppendBuffer(iPartition,pRowBuffer);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* table.appendBuffer -- append row buffer into table                         */
/*----------------------------------------------------------------------------*/
void Task::vTableAppendBuffer(NNP_Task nnpTask)
{
  ENTRY("vTableAppendBuffer");

  Object* pPtrTable;
  if ( ! nnpTask->m_xDataStack.bPop(pPtrTable) )
    THROW_INVALID_DSTACK("table.appendBuffer");
  Table* pTable = _pTable(pPtrTable);
  ASSERT(CN_TABLE == pTable->eClass());
  
  i8 iPartition = 0;
  if ( ! nnpTask->m_xDataStack.bPop(iPartition) )
    THROW_INVALID_DSTACK("table.appendBuffer");
  
  Object* pPtrRowBuffer;
  if ( ! nnpTask->m_xDataStack.bPop(pPtrRowBuffer) )
    THROW_INVALID_DSTACK("table.appendBuffer");
  RowBuffer* pRowBuffer = _pRowBuffer(pPtrRowBuffer);
  ASSERT(CN_ROWBUFFER == pRowBuffer->eClass());
    
  pTable->vAppendBuffer(iPartition,pRowBuffer);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* table.dic -- get table dictionary                                          */
/*----------------------------------------------------------------------------*/
void Task::vTableDic(NNP_Task nnpTask)
{
ENTRY("vTableDic");
  
  Object* pPtrTable;
  if ( ! nnpTask->m_xDataStack.bPop(pPtrTable) )
    THROW_INVALID_DSTACK("table.dic");
  
  Table* pTable = _pTable(pPtrTable);
  ASSERT(CN_TABLE == pTable->eClass());
  
  Dic* pDic = pTable->pDic();
  
  nnpTask->m_xDataStack.vPush(pDic);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* table.drop -- drop of table on param stack                                 */
/*----------------------------------------------------------------------------*/
void Task::vTableDrop(NNP_Task nnpTask)
{
ENTRY("vTableDrop");
  
  Object* pPtrTable;
  if ( ! nnpTask->m_xDataStack.bPop(pPtrTable) )
    THROW_INVALID_DSTACK("table.drop");
  
  Table* pTable = _pTable(pPtrTable);
  ASSERT(CN_TABLE == pTable->eClass());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* table dump                                                                 */
/*----------------------------------------------------------------------------*/
void Task::vTableDump(NNP_Task nnpTask)
{
ENTRY("vTableDump");
  
  vTrace("TableDump\n");
  
  Object* pPtrTable;
  if ( ! nnpTask->m_xDataStack.bPop(pPtrTable) )
    THROW_INVALID_DSTACK("table.dump");
  
  Table* pTable = _pTable(pPtrTable);
  ASSERT(CN_TABLE == pTable->eClass());
  
  pTable->vDump();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* table.init -- init of table on param stack                                 */
/*----------------------------------------------------------------------------*/
void Task::vTableInit(NNP_Task nnpTask)
{
ENTRY("vTableInit");
  
  Object* pPtrTable;
  if ( ! nnpTask->m_xDataStack.bPop(pPtrTable) )
    THROW_INVALID_DSTACK("table.init");
  
  Table* pTable = _pTable(pPtrTable);
  ASSERT(CN_TABLE == pTable->eClass());
  
  new(pTable) Table;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* table.insert -- insert row into table on param stack                       */
/*----------------------------------------------------------------------------*/
void Task::vTableInsert(NNP_Task nnpTask)
{
ENTRY("vTableInsert");
  
  Object* pPtrTable;
  Object* pPtrTxRef;
  ValWsp xRow;
  
  if ( ! nnpTask->m_xDataStack.bPop(pPtrTable) )
    THROW_INVALID_DSTACK("table.insert");
  if ( ! nnpTask->m_xDataStack.bPop(pPtrTxRef) )
    THROW_INVALID_DSTACK("table.insert");
  if ( ! nnpTask->m_xDataStack.bPop(xRow) )
    THROW_INVALID_DSTACK("table.insert");

  i8 iPartition;
  if ( ! nnpTask->m_xDataStack.bPop(iPartition) )
    THROW_INVALID_DSTACK("table.insert");
  
  Table* pTable = _pTable(pPtrTable);
  ASSERT(CN_TABLE == pTable->eClass());

  TxRef* pTxRef = _pTxRef(pPtrTxRef);
  ASSERT(CN_TXREF == pTxRef->eClass());
  
  pTable->vInsertRow(iPartition,
                     pTxRef,
                     xRow.pAsStrVal()->sStr(),
                     xRow.pAsStrVal()->iChars());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* table.name -- get table name                                               */
/*----------------------------------------------------------------------------*/
void Task::vTableName(NNP_Task nnpTask)
{
  ENTRY("vTableName");
  
  Object* pPtrTable;
  if ( ! nnpTask->m_xDataStack.bPop(pPtrTable) )
    THROW_INVALID_DSTACK("table.init");
  
  Table* pTable = _pTable(pPtrTable);
  ASSERT(CN_TABLE == pTable->eClass());
  
  s32 sName;
  
  sName = pTable->pTableName();
  
  nnpTask->m_xDataStack.vPush(sName);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* table.ref -- has table got references?                                     */
/*----------------------------------------------------------------------------*/
void Task::vTableRef(NNP_Task nnpTask)
{
  ENTRY("vTableRef");
  
  Object* pPtrTable;
  
  if ( ! nnpTask->m_xDataStack.bPop(pPtrTable) )
    THROW_INVALID_DSTACK("table.ref");
  
  Table* pTable = _pTable(pPtrTable);
  
  bool bRef = pTable->bRefOut();
  
  nnpTask->m_xDataStack.vPush(bRef);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* table.truncate -- release all rows/buffers in table on param stack         */
/*----------------------------------------------------------------------------*/
void Task::vTableTruncate(NNP_Task nnpTask)
{
ENTRY("vTableTruncate");
  
  Object* pPtrTable;
  if ( ! nnpTask->m_xDataStack.bPop(pPtrTable) )
    THROW_INVALID_DSTACK("table.truncate");
  
  Table* pTable = _pTable(pPtrTable);
  ASSERT(CN_TABLE == pTable->eClass());
  
  pTable->vTruncate();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* table.space -- push the buffer space used bytable on param stack           */
/*----------------------------------------------------------------------------*/
void Task::vTableSpace(NNP_Task nnpTask)
{
ENTRY("vTableSpace");
  
  Object* pPtrTable;
  if ( ! nnpTask->m_xDataStack.bPop(pPtrTable) )
    THROW_INVALID_DSTACK("table.spave");
  
  Table* pTable = _pTable(pPtrTable);
  ASSERT(CN_TABLE == pTable->eClass());
  
  i8 iBuffers = pTable->iRowBuffers();
  i8 iSpace   = iBuffers * C_SIZE_POOL_BUFFER;
  s256 sMag;
  
  nnpTask->m_xDataStack.vPush(iSpace);
  
  EXIT();
}

// task: ///////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_Task(void)
{
  vAddPrimitive("task.ack",        &vTaskAck,          false);
  vAddPrimitive("task.acks@",      &vTaskAcksFetch,    false);
  vAddPrimitive("task.acks!",      &vTaskAcksStore,    false);
  vAddPrimitive("task.comment@",   &vTaskCommentFetch, false);
  vAddPrimitive("task.comment!",   &vTaskCommentStore, false);
  vAddPrimitive("task.dic",        &vTaskDic,          false);
  vAddPrimitive("task.dump",       &vTaskDump,         false);
  vAddPrimitive("task.init",       &vTaskInit,         false);
  vAddPrimitive("task.name@",      &vTaskNameFetch,    false);
  vAddPrimitive("task.name!",      &vTaskNameStore,    false);
  vAddPrimitive("task.new",        &vTaskNew,          false);
  vAddPrimitive("task.parent@",    &vTaskParentFetch,  false);
  vAddPrimitive("task.parent!",    &vTaskParentStore,  false);
  vAddPrimitive("task.queue@",     &vTaskQfetch,       false);
  vAddPrimitive("task.queue!",     &vTaskQstore,       false);
  vAddPrimitive("task.seed",       &vTaskSeed,         false);
  vAddPrimitive("task.seitf@",     &vTaskSeitfFetch,   false);
  vAddPrimitive("task.seitf!",     &vTaskSeitfStore,   false);
  vAddPrimitive("task.socket@",    &vTaskSocketFetch,  false);
  vAddPrimitive("task.socket!",    &vTaskSocketStore,  false);
  vAddPrimitive("task.state@",     &vTaskStateFetch,   false);
  vAddPrimitive("task.state!",     &vTaskStateStore,   false);
  vAddPrimitive("task.taskset@",   &vTaskTaskSetFetch, false);
  vAddPrimitive("task.taskset!",   &vTaskTaskSetStore, false);
  vAddPrimitive("task.this",       &vTaskThis,         false);
  vAddPrimitive("task.thread",     &vTaskThread,       false);
  vAddPrimitive("task.wait",       &vTaskWait,         false);
  vAddPrimitive("task.yield",      &vTaskYield,        false);
}

/*----------------------------------------------------------------------------*/
/* ack -- acknowledge that task of this tasks completion                      */
/*----------------------------------------------------------------------------*/
void Task::vTaskAck(NNP_Task nnpTask)
{
  ENTRY("vTaskAck");
  
  Object* pTaskObject;
  if ( ! nnpTask->m_xDataStack.bPop(pTaskObject) )
    THROW_INVALID_DSTACK("task.ack");
  Task* pThatTask = _pTask(pTaskObject);
  
  i8 iMore = --pThatTask->m_zAcks;
  
  if ( ! iMore ) {
    //    vTrace("Task::vTaskSignal: task %p %s resuming task %p %s\n",
    //           *nnpTask,nnpTask->m_sName.a(),
    //           pThatTask,pThatTask->m_sName.a());
    if (gxArguments.m_bTracepoint)
      vTrace("[N%lld:%8s\\%lld %-11s] ack resumes %s\\%lld\n",
             gxArguments.m_iNode,
             nnpTask->m_sName.a(),
             *nnpTask->m_zAcks,
             nnpTask->m_sComment.a(),
             
             pThatTask->m_sName.a(),
             *pThatTask->m_zAcks);
    
    while (TS_WAITING_ACK != pThatTask->m_eState)
      PosixThread::vSleepMicro(10);
    
    //Thread* pThread = Thread::pThread();
    //pThread->m_pTaskSet->vAppend(pThatTask);
    pThatTask->m_pTaskSet->vAppend(pThatTask);
    
  }
  else {
    //    vTrace("Task::vTaskSignal: task %p %s signalled task %p\\%lld %s\n",
    //           *nnpTask,nnpTask->m_sName.a(),
    //           pThatTask,iMore,pThatTask->m_sName.a());
    vTrace("[N%lld:%8s\\%lld %-11s] ack'd %s\\%lld\n",
           gxArguments.m_iNode,
           nnpTask->m_sName.a(),
           *nnpTask->m_zAcks,
           nnpTask->m_sComment.a(),
           
           pThatTask->m_sName.a(),
           iMore);
    
  }
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.acks@ -- push current value of given tasks m_zAcks                    */
/*----------------------------------------------------------------------------*/
void Task::vTaskAcksFetch(NNP_Task nnpTask)
{
  ENTRY("vTaskAcksFetch");
  
  Object* pPtrTask;
  if ( ! nnpTask->m_xDataStack.bPop(pPtrTask) )
    THROW_INVALID_DSTACK("task.acks@");
  Task* pGivenTask = _pTask(pPtrTask);
  
  nnpTask->m_xDataStack.vPush(*pGivenTask->m_zAcks);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.acks! -- store value into given tasks m_zAcks                         */
/*----------------------------------------------------------------------------*/
void Task::vTaskAcksStore(NNP_Task nnpTask)
{
  ENTRY("vTaskAcksStore");
  
  Object* pPtrTask;
  if ( ! nnpTask->m_xDataStack.bPop(pPtrTask) )
    THROW_INVALID_DSTACK("task.acks!");
  Task* pGivenTask = _pTask(pPtrTask);
  
  i8 iInt;
  if ( ! nnpTask->m_xDataStack.bPopParamInt(iInt) )
    THROW_INVALID_DSTACK("task.acks!");
  
  ASSERT(0 == *pGivenTask->m_zAcks);

  pGivenTask->m_zAcks = iInt;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.comment@ -- push comment of given task                                */
/*----------------------------------------------------------------------------*/
void Task::vTaskCommentFetch(NNP_Task nnpTask)
{
  ENTRY("vTaskCommentFetch");
  
  Object* pPtrTask;
  if ( ! nnpTask->m_xDataStack.bPop(pPtrTask) )
    THROW_INVALID_DSTACK("task.comment@");
  Task* pGivenTask = _pTask(pPtrTask);
  
  nnpTask->m_xDataStack.vPush(pGivenTask->m_sComment);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.comment! -- store comment into given task                             */
/*----------------------------------------------------------------------------*/
void Task::vTaskCommentStore(NNP_Task nnpTask)
{
  ENTRY("vTaskCommentStore");
  
  Object* pPtrTask;
  if ( ! nnpTask->m_xDataStack.bPop(pPtrTask) )
    THROW_INVALID_DSTACK("task.comment!");
  Task* pGivenTask = _pTask(pPtrTask);
  
  s32 sComment;
  if ( ! nnpTask->m_xDataStack.bPop(sComment) )
    THROW_INVALID_DSTACK("task.comment!");
  
  pGivenTask->m_sComment = sComment;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.dic -- push task dictionary onto param stack                          */
/*----------------------------------------------------------------------------*/
void Task::vTaskDic(NNP_Task nnpTask)
{
ENTRY("vTaskDic");
  
  Object* pPtrDic = &nnpTask->m_xTaskDic;
  
  nnpTask->m_xDataStack.vPush(pPtrDic);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.dump -- dump of task on param stack                                   */
/*----------------------------------------------------------------------------*/
void Task::vTaskDump(NNP_Task nnpTask)
{
ENTRY("vTaskDump");
  
  Object* pObject;
  if ( ! nnpTask->m_xDataStack.bPop(pObject) )
    THROW_INVALID_DSTACK("task.dump");
  Task* pThatTask = _pTask(pObject);
  
  pThatTask->vDump();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.init -- init of task on param stack                                   */
/*----------------------------------------------------------------------------*/
void Task::vTaskInit(NNP_Task nnpTask)
{
ENTRY("vTaskInit");
  
  Object* pPtrTask;
  if ( ! nnpTask->m_xDataStack.bPop(pPtrTask) )
    THROW_INVALID_DSTACK("task.init");
  
  Task* pThatTask = _pTask(pPtrTask);
  ASSERT(CN_TASK == pThatTask->eClass());
  
  new(pThatTask) Task;
  
  Word* pWordThread = Word::pToWord(pThatTask);
  
  s32 sName;
  sName = pWordThread->sName();
  
  pThatTask->vName(sName);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.name@ -- fetch task name                                              */
/*----------------------------------------------------------------------------*/
void Task::vTaskNameFetch(NNP_Task nnpTask)
{
ENTRY("vTaskNameFetch");
  
  Object* pTaskObject;
  if ( ! nnpTask->m_xDataStack.bPop(pTaskObject) )
    THROW_INVALID_DSTACK("task.name@");
  Task* pThatTask = _pTask(pTaskObject);
  
  nnpTask->m_xDataStack.vPush(pThatTask->m_sName);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.name! -- store task name                                              */
/*----------------------------------------------------------------------------*/
void Task::vTaskNameStore(NNP_Task nnpTask)
{
ENTRY("vTaskNameStore");
  
  Object* pTaskObject;
  if ( ! nnpTask->m_xDataStack.bPop(pTaskObject) )
    THROW_INVALID_DSTACK("task.name!");
  Task* pThatTask = _pTask(pTaskObject);
  
  s32 sName;
  if ( ! nnpTask->m_xDataStack.bPop(sName) )
    THROW_INVALID_DSTACK("task.name!");
  
  pThatTask->m_sName = sName;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.new -- create and seed a new task                                     */
/*----------------------------------------------------------------------------*/
void Task::vTaskNew(NNP_Task nnpTask)
{
ENTRY("vTaskNew");
  //                                   pParent   sSeed     nTaskArgs
  if ( ! nnpTask->m_xDataStack.bGuard(CN_PTRVAL,CN_STRVAL,CN_INTVAL) )
    THROW_INVALID_DSTACK("task.new");
  
  Object* pParent;
  if ( ! nnpTask->m_xDataStack.bPop(pParent) )
    THROW_INVALID_DSTACK("task.new");
  Task* pParentTask = _pTask(pParent);
  ASSERT( ! pParentTask || CN_TASK == pParentTask->eClass());
  
  s32 sSeed;
  if ( ! nnpTask->m_xDataStack.bPop(sSeed) )
    THROW_INVALID_DSTACK("task.new");
  char* aSeedWord = sSeed.a();
  
  i8 nTaskArgs;
  if ( ! nnpTask->m_xDataStack.bPop(nTaskArgs) )
    THROW_INVALID_DSTACK("task.new");
  //ASSERT(! nTaskArgs);

  if ( ! nnpTask->m_xDataStack.bGuard(nTaskArgs) )
    THROW_INVALID_DSTACK("task.new");
  
  // create new task (will be automatically appended to task list)
  NNP_Task nnpNewTask = new Task;
  
  nnpNewTask->m_pParentTask = pParentTask;
  nnpNewTask->vSeedWord(aSeedWord);
  
  // copy task arguments to new task (preserving stack order)
  ValWsp xValWsp;
  for (i8 iTaskArg = nTaskArgs - 1; iTaskArg >= 0; iTaskArg--) {
    nnpTask->m_xDataStack.vPeek(iTaskArg,&xValWsp);
    nnpNewTask->m_xDataStack.vPush(xValWsp);
  }
  
  // discard original task arguments
  for (i8 iTaskArg = nTaskArgs - 1; iTaskArg >= 0; iTaskArg--) {
    bool bPopped = nnpTask->m_xDataStack.bPop(xValWsp);
    ASSERT(bPopped);
  }
  
  Task* pNewTask = *nnpNewTask;
  
  Object* pTask = pNewTask;
  nnpTask->m_xDataStack.vPush(pTask);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.parent@ -- fetch parent of task on param stack                        */
/*----------------------------------------------------------------------------*/
void Task::vTaskParentFetch(NNP_Task nnpTask)
{
  ENTRY("vTaskParentFetch");
  
  nnpTask->m_xDataStack.vPush(nnpTask->m_pParentTask);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.parent! -- store parent task into this task                           */
/*----------------------------------------------------------------------------*/
void Task::vTaskParentStore(NNP_Task nnpTask)
{
  ENTRY("vTaskParentStore");
  
  Object* pParent;
  if ( ! nnpTask->m_xDataStack.bPop(pParent) )
    THROW_INVALID_DSTACK("task.parent!");
  Task* pParentTask = _pTask(pParent);
  ASSERT( ! pParentTask || CN_TASK == pParentTask->eClass());
  
  nnpTask->m_pParentTask = pParentTask;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.queue@ -- fetch queue of task on param stack                          */
/*----------------------------------------------------------------------------*/
void Task::vTaskQfetch(NNP_Task nnpTask)
{
ENTRY("vTaskQfetch");
  
  Object* pTaskObject;
  if ( ! nnpTask->m_xDataStack.bPop(pTaskObject) )
    THROW_INVALID_DSTACK("task.queue@");
  Task* pThatTask = _pTask(pTaskObject);
  
  Queue* pQueue = pThatTask->m_pQueue;
  
  Object* pQueueEmb = static_cast<Object*>(pQueue);
  
  nnpTask->m_xDataStack.vPush(pQueueEmb);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.queue! -- store queue on param stack into task on param stack         */
/*----------------------------------------------------------------------------*/
void Task::vTaskQstore(NNP_Task nnpTask)
{
ENTRY("vTaskQstore");
  
  Object* pTaskObject;
  if ( ! nnpTask->m_xDataStack.bPop(pTaskObject) )
    THROW_INVALID_DSTACK("task.queue!");
  Task* pThatTask = _pTask(pTaskObject);
  
  Object* pQueueEmb;
  
  if ( ! nnpTask->m_xDataStack.bPop(pQueueEmb) )
    THROW_INVALID_DSTACK("task.queue!");
  
  Queue* pQueue = static_cast<Queue*>(pQueueEmb);
  
  pThatTask->m_pQueue = pQueue;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.seed -- set seed word and args of task                                */
/*----------------------------------------------------------------------------*/
void Task::vTaskSeed(NNP_Task nnpTask)
{
ENTRY("vTaskSeed");
  
//  //                                   sSeed     nTaskArgs
//  if ( ! nnpTask->m_xDataStack.bGuard(CN_STRVAL,CN_INTVAL) )
//    THROW_INVALID_DSTACK("task.seed");
  
  Object* pTaskObject;
  if ( ! nnpTask->m_xDataStack.bPop(pTaskObject) )
    THROW_INVALID_DSTACK("task.seed");
  Task* pThatTask = _pTask(pTaskObject);
  
  s32 sSeed;
  if ( ! nnpTask->m_xDataStack.bPop(sSeed) )
    THROW_INVALID_DSTACK("task.seed");
  pThatTask->vSeedWord(sSeed.a());

  i8 nTaskArgs;
  if ( ! nnpTask->m_xDataStack.bPop(nTaskArgs) )
    THROW_INVALID_DSTACK("task.seed");
  //ASSERT(! nTaskArgs);
  
  if ( ! nnpTask->m_xDataStack.bGuard(nTaskArgs) )
    THROW_INVALID_DSTACK("task.seed");
  
  // copy task arguments to new task (preserving stack order)
  ValWsp xValWsp;
  for (i8 iTaskArg = nTaskArgs - 1; iTaskArg >= 0; iTaskArg--) {
    nnpTask->m_xDataStack.vPeek(iTaskArg,&xValWsp);
    pThatTask->m_xDataStack.vPush(xValWsp);
  }
  
  // discard original task arguments
  for (i8 iTaskArg = nTaskArgs - 1; iTaskArg >= 0; iTaskArg--) {
    bool bPopped = nnpTask->m_xDataStack.bPop(xValWsp);
    ASSERT(bPopped);
  }
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.seitf@ -- fetch SEitf of task on param stack                          */
/*----------------------------------------------------------------------------*/
void Task::vTaskSeitfFetch(NNP_Task nnpTask)
{
  ENTRY("vTaskSeitfFetch");
  
  Object* pObject;
  if ( ! nnpTask->m_xDataStack.bPop(pObject) )
    THROW_INVALID_DSTACK("task.seitf@");
  Task* pThatTask = _pTask(pObject);
  
  SEitf* pSEitf = pThatTask->m_pSEitf;
  
  Object* pPtrSEitf= pSEitf;
  
  nnpTask->m_xDataStack.vPush(pPtrSEitf);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.seitf! -- store SEitf on param stack into task on param stack         */
/*----------------------------------------------------------------------------*/
void Task::vTaskSeitfStore(NNP_Task nnpTask)
{
  ENTRY("vTaskSeitfStore");
  
  Object* pTaskObject;
  if ( ! nnpTask->m_xDataStack.bPop(pTaskObject) )
    THROW_INVALID_DSTACK("task.seitf!");
  Task* pThatTask = _pTask(pTaskObject);
  
  Object* pPtrSEitf;
  
  if ( ! nnpTask->m_xDataStack.bPop(pPtrSEitf) )
    THROW_INVALID_DSTACK("task.seitf!");
  
  SEitf* pSEitf = _pSEitf(pPtrSEitf);
  ASSERT(! pSEitf || CN_SEITF == pSEitf->eClass());
  
  pThatTask->m_pSEitf = pSEitf;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.socket@ -- fetch socket of task on param stack                        */
/*----------------------------------------------------------------------------*/
void Task::vTaskSocketFetch(NNP_Task nnpTask)
{
  ENTRY("vTaskSocketFetch");
  
  Object* pTaskObject;
  if ( ! nnpTask->m_xDataStack.bPop(pTaskObject) )
    THROW_INVALID_DSTACK("task.socket@");
  Task* pThatTask = _pTask(pTaskObject);
  
  Socket* pSocket = pThatTask->m_pSocket;
  
  Object* pSocketEmb = static_cast<Object*>(pSocket);
  
  nnpTask->m_xDataStack.vPush(pSocketEmb);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.socket! -- store socket on param stack into task                      */
/*----------------------------------------------------------------------------*/
void Task::vTaskSocketStore(NNP_Task nnpTask)
{
  ENTRY("vTaskSocketStore");
  
  Object* pTaskObject;
  if ( ! nnpTask->m_xDataStack.bPop(pTaskObject) )
    THROW_INVALID_DSTACK("task.socket!");
  Task* pThatTask = _pTask(pTaskObject);
  
  Object* ptrSocket;
  
  if ( ! nnpTask->m_xDataStack.bPop(ptrSocket) )
    THROW_INVALID_DSTACK("task.socket!");
  
  Socket* pSocket = static_cast<Socket*>(ptrSocket);
  
  pThatTask->m_pSocket = pSocket;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.state@ -- fetch state of task                                         */
/*----------------------------------------------------------------------------*/
void Task::vTaskStateFetch(NNP_Task nnpTask)
{
ENTRY("vTaskStateFetch");
  
  Object* pTaskObject;
  if ( ! nnpTask->m_xDataStack.bPop(pTaskObject) )
    THROW_INVALID_DSTACK("task.state@");
  Task* pThatTask = _pTask(pTaskObject);
  
  eTS eState = pThatTask->m_eState;
  
  nnpTask->m_xDataStack.vPush((i8)eState);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.state! -- store state of task                                         */
/*----------------------------------------------------------------------------*/
void Task::vTaskStateStore(NNP_Task nnpTask)
{
ENTRY("vTaskStateStore");
  
  Object* pTaskObject;
  if ( ! nnpTask->m_xDataStack.bPop(pTaskObject) )
    THROW_INVALID_DSTACK("task.state!");
  Task* pThatTask = _pTask(pTaskObject);
  
  i8 iState;
  
  if ( ! nnpTask->m_xDataStack.bPop(iState) )
    THROW_INVALID_DSTACK("task.state!");
  
  pThatTask->m_eState = (eTS)iState;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.taskset@ -- fetch taskset of task on param stack                      */
/*----------------------------------------------------------------------------*/
void Task::vTaskTaskSetFetch(NNP_Task nnpTask)
{
  ENTRY("vTaskTaskSetFetch");
  
  Object* pTaskObject;
  if ( ! nnpTask->m_xDataStack.bPop(pTaskObject) )
    THROW_INVALID_DSTACK("task.taskset@");
  Task* pThatTask = _pTask(pTaskObject);
  
  TaskSet* pTaskSet = pThatTask->m_pTaskSet;
  
  Object* pTaskSetEmb = static_cast<Object*>(pTaskSet);
  
  nnpTask->m_xDataStack.vPush(pTaskSetEmb);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.taskset! -- store taskset on param stack into task                    */
/*----------------------------------------------------------------------------*/
void Task::vTaskTaskSetStore(NNP_Task nnpTask)
{
  ENTRY("vTaskTaskSetStore");
  
  Object* pTaskObject;
  if ( ! nnpTask->m_xDataStack.bPop(pTaskObject) )
    THROW_INVALID_DSTACK("task.taskset!");
  Task* pThatTask = _pTask(pTaskObject);
  
  Object* ptrTaskSet;
  
  if ( ! nnpTask->m_xDataStack.bPop(ptrTaskSet) )
    THROW_INVALID_DSTACK("task.taskset!");
  
  TaskSet* pTaskSet = static_cast<TaskSet*>(ptrTaskSet);
  
  pThatTask->m_pTaskSet = pTaskSet;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.this -- push this task                                                */
/*----------------------------------------------------------------------------*/
void Task::vTaskThis(NNP_Task nnpTask)
{
ENTRY("vTaskThis");
  
  Object* pTask = *nnpTask;
  
  nnpTask->m_xDataStack.vPush(pTask);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.thread -- push given tasks thread                                     */
/*----------------------------------------------------------------------------*/
void Task::vTaskThread(NNP_Task nnpTask)
{
  ENTRY("vTaskThread");
  
  Object* pTaskObject;
  if ( ! nnpTask->m_xDataStack.bPop(pTaskObject) )
    THROW_INVALID_DSTACK("task.thread");
  Task* pThatTask = _pTask(pTaskObject);

  Thread* pThread = pThatTask->m_pThread;
  nnpTask->m_xDataStack.vPush(pThread);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.wait -- this task waits for signal                                    */
/*----------------------------------------------------------------------------*/
void Task::vTaskWait(NNP_Task nnpTask)
{
  ENTRY("vTaskWait");
  
  // ..tbd..
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.yield -- this task yields                                             */
/*----------------------------------------------------------------------------*/
void Task::vTaskYield(NNP_Task nnpTask)
{
  ENTRY("vTaskYield");
  
  nnpTask->vState(TS_YIELDED);
  
  EXIT();
}

// taskCursor: /////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_TaskCursor(void)
{
  vAddPrimitive("taskCursor.begin",  &vTaskCursorBegin,  false);
  vAddPrimitive("taskCursor.delete", &vTaskCursorDelete, false);
  vAddPrimitive("taskCursor.end",    &vTaskCursorEnd,    false);
  vAddPrimitive("taskCursor.first",  &vTaskCursorFirst,  false);
  vAddPrimitive("taskcursor.get",    &vTaskCursorGet,    false);
  vAddPrimitive("taskcursor.init",   &vTaskCursorInit,   false);
  vAddPrimitive("taskcursor.new",    &vTaskCursorNew,    false);
  vAddPrimitive("taskcursor.next",   &vTaskCursorNext,   false);
}

/*----------------------------------------------------------------------------*/
/* taskcursor.begin -- begin using task currsor                               */
/*----------------------------------------------------------------------------*/
void Task::vTaskCursorBegin(NNP_Task nnpTask)
{
ENTRY("vTaskCursorBegin");
  
  Object* pPtrTaskCursor;
  if ( ! nnpTask->m_xDataStack.bPop(pPtrTaskCursor) )
    THROW_INVALID_DSTACK("taskCursor.begin");
  
  TaskCursor* pTaskCursor = _pTaskCursor(pPtrTaskCursor);
  ASSERT(CN_TASKCURSOR == pTaskCursor->eClass());
  
  pTaskCursor->vBegin();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* taskCursor.delete -- delete an task cursor                                 */
/*----------------------------------------------------------------------------*/
void Task::vTaskCursorDelete(NNP_Task nnpTask)
{
ENTRY("vTaskCursorDelete");
  
  Object* pPtrTaskCursor;
  if ( ! nnpTask->m_xDataStack.bPop(pPtrTaskCursor) )
    THROW_INVALID_DSTACK("taskCursor.delete");
  
  TaskCursor* pTaskCursor = _pTaskCursor(pPtrTaskCursor);
  ASSERT(CN_TASKCURSOR == pTaskCursor->eClass());
  
  Object* pObject = reinterpret_cast<Object*>(pTaskCursor);
  delete pTaskCursor;
  
  Heap::vFree(pObject);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* taskCursor.end -- end using task cursor                                    */
/*----------------------------------------------------------------------------*/
void Task::vTaskCursorEnd(NNP_Task nnpTask)
{
ENTRY("vTaskCursorEnd");
  
  Object* pPtrTaskCursor;
  if ( ! nnpTask->m_xDataStack.bPop(pPtrTaskCursor) )
    THROW_INVALID_DSTACK("taskCursor.end");
  
  TaskCursor* pTaskCursor = _pTaskCursor(pPtrTaskCursor);
  ASSERT(CN_TASKCURSOR == pTaskCursor->eClass());
  
  pTaskCursor->vEnd();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* taskCursor.first -- set to first and get task                              */
/*----------------------------------------------------------------------------*/
void Task::vTaskCursorFirst(NNP_Task nnpTask)
{
ENTRY("vTaskCursorFirst");
  
  Object* pPtrTaskCursor;
  if ( ! nnpTask->m_xDataStack.bPop(pPtrTaskCursor) )
    THROW_INVALID_DSTACK("taskCursor.first");
  
  TaskCursor* pTaskCursor = _pTaskCursor(pPtrTaskCursor);
  ASSERT(CN_TASKCURSOR == pTaskCursor->eClass());
  
  pTaskCursor->vFirst();
  
  Object* pPtrTask = pTaskCursor->pObject();
  
  nnpTask->m_xDataStack.vPush(pPtrTask);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* taskCursor.get -- get current task of cursor                               */
/*----------------------------------------------------------------------------*/
void Task::vTaskCursorGet(NNP_Task nnpTask)
{
ENTRY("vTaskCursorGet");
  
  Object* pPtrTaskCursor;
  if ( ! nnpTask->m_xDataStack.bPop(pPtrTaskCursor) )
    THROW_INVALID_DSTACK("taskCursor.get");
  
  TaskCursor* pTaskCursor = _pTaskCursor(pPtrTaskCursor);
  ASSERT(CN_TASKCURSOR == pTaskCursor->eClass());
  
  Task* pThatTask = pTaskCursor->pObject();
  
  Object* pPtrTask = pTaskCursor->pObject();
  
  nnpTask->m_xDataStack.vPush(pPtrTask);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* taskCursor.init -- init an task cursor                                     */
/*----------------------------------------------------------------------------*/
void Task::vTaskCursorInit(NNP_Task nnpTask)
{
ENTRY("vTaskCursorInit");
  
  Object* pPtrTaskCursor;
  if ( ! nnpTask->m_xDataStack.bPop(pPtrTaskCursor) )
    THROW_INVALID_DSTACK("taskCursor.init");
  
//  s32 sName;
//  sName = "tasklist";
//  Word* pWord = nnpTask->pFindWord(sName);
//  ASSERT(pWord);
//  TaskList* pTaskList = reinterpret_cast<TaskList*>(pWord->pBodyArea());
  
  TaskCursor* pTaskCursor = _pTaskCursor(pPtrTaskCursor);
  ASSERT(CN_TASKCURSOR == pTaskCursor->eClass());
  
  new(pTaskCursor) TaskCursor(gpTaskList);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* taskCursor.new -- new a task cursor                                        */
/*----------------------------------------------------------------------------*/
void Task::vTaskCursorNew(NNP_Task nnpTask)
{
ENTRY("vTaskCursorNew");
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* taskCursor.next -- advance to next and get task                            */
/*----------------------------------------------------------------------------*/
void Task::vTaskCursorNext(NNP_Task nnpTask)
{
ENTRY("vTaskCursorNext");
  
  Object* pPtrTaskCursor;
  if ( ! nnpTask->m_xDataStack.bPop(pPtrTaskCursor) )
    THROW_INVALID_DSTACK("taskCursor.next");
  
  TaskCursor* pTaskCursor = _pTaskCursor(pPtrTaskCursor);
  ASSERT(CN_TASKCURSOR == pTaskCursor->eClass());
  
  pTaskCursor->vNext();
  
  Object* pPtrTask = pTaskCursor->pObject();
  
  nnpTask->m_xDataStack.vPush(pPtrTask);
  
  EXIT();
}

// tasklist: ///////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_TaskList(void)
{
  vAddPrimitive("taskList",        &vTaskList,         false);
  vAddPrimitive("taskList.append", &vTaskListAppend,   false);
  vAddPrimitive("taskList.dump",   &vTaskListDump,     false);
  vAddPrimitive("taskList.init",   &vTaskListInit,     false);
}

/*----------------------------------------------------------------------------*/
/* tasklist -- tasklist                                                       */
/*----------------------------------------------------------------------------*/
void Task::vTaskList(NNP_Task nnpTask)
{
ENTRY("vTaskList");
  
  nnpTask->m_xDataStack.vPush(gpTaskList);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* tasklist.append -- tasklist append                                         */
/*----------------------------------------------------------------------------*/
void Task::vTaskListAppend(NNP_Task nnpTask)
{
ENTRY("vTaskListAppend");
  
  Object* pPtrTaskList;
  if ( ! nnpTask->m_xDataStack.bPop(pPtrTaskList) )
    THROW_INVALID_DSTACK("tasklist.append");
  
  TaskList* pTaskList = _pTaskList(pPtrTaskList);
  ASSERT(CN_TASKLIST == pTaskList->eClass());
  
  Object* pTaskObject;
  if ( ! nnpTask->m_xDataStack.bPop(pTaskObject) )
    THROW_INVALID_DSTACK("task.dump");
  Task* pThatTask = _pTask(pTaskObject);
  
  pTaskList->vAppend(pThatTask);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* tasklist.dump -- tasklist dump                                             */
/*----------------------------------------------------------------------------*/
void Task::vTaskListDump(NNP_Task nnpTask)
{
ENTRY("vTaskListDump");
  
  Object* pPtrTaskList;
  if ( ! nnpTask->m_xDataStack.bPop(pPtrTaskList) )
    THROW_INVALID_DSTACK("tasklist.dump");
  
  TaskList* pTaskList = _pTaskList(pPtrTaskList);
  ASSERT(CN_TASKLIST == pTaskList->eClass());
 
  pTaskList->vDump();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* tasklist.init -- tasklist init                                             */
/*----------------------------------------------------------------------------*/
void Task::vTaskListInit(NNP_Task nnpTask)
{
ENTRY("vTaskListInit");
  
  EXIT();
}

// taskset: ////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_TaskSet(void)
{
  vAddPrimitive("taskSet.append",  &vTaskSetAppend,    false);
  vAddPrimitive("taskSet.dump",    &vTaskSetDump,      false);
  vAddPrimitive("taskSet.init",    &vTaskSetInit,      false);
}

/*----------------------------------------------------------------------------*/
/* taskset.append -- taskset append                                           */
/*----------------------------------------------------------------------------*/
void Task::vTaskSetAppend(NNP_Task nnpTask)
{
ENTRY("vTaskSetAppend");
  
  Object* pObject;
  
  if ( ! nnpTask->m_xDataStack.bPop(pObject) )
    THROW_INVALID_DSTACK("taskset.append");
  
  ASSERT(CN_TASKSET == pObject->eClass());
  
  TaskSet* pTaskSet = static_cast<TaskSet*>(pObject);
  
  Object* pTaskObject;
  if ( ! nnpTask->m_xDataStack.bPop(pTaskObject) )
    THROW_INVALID_DSTACK("taskset.append");
  Task* pThatTask = _pTask(pTaskObject);
  
  pTaskSet->vAppend(pThatTask);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* taskset.dump -- taskset dump                                               */
/*----------------------------------------------------------------------------*/
void Task::vTaskSetDump(NNP_Task nnpTask)
{
ENTRY("vTaskSetDump");
  
  Object* pObject;
  
  if ( ! nnpTask->m_xDataStack.bPop(pObject) )
    THROW_INVALID_DSTACK("taskset.dump");
  
  ASSERT(CN_TASKSET == pObject->eClass());
  
  TaskSet* pTaskSet = static_cast<TaskSet*>(pObject);
  
  pTaskSet->vDump();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* taskset.init -- taskset init                                               */
/*----------------------------------------------------------------------------*/
void Task::vTaskSetInit(NNP_Task nnpTask)
{
ENTRY("vTaskSetInit");
  
  Object* pObject;
  
  if ( ! nnpTask->m_xDataStack.bPop(pObject) )
    THROW_INVALID_DSTACK("taskset.init");
  
  ASSERT(CN_TASKSET == pObject->eClass());
  
  TaskSet* pTaskSet = static_cast<TaskSet*>(pObject);
  
  new(pTaskSet) TaskSet;
  
  EXIT();
}

// thread: /////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_Thread(void)
{
  vAddPrimitive("thread.append",      &vThreadAppend,      false);
  vAddPrimitive("thread.bind",        &vThreadBind,        false);
  vAddPrimitive("thread.condSignal",  &vThreadCondSignal,  false);
  vAddPrimitive("thread.condWait",    &vThreadCondWait,    false);
  vAddPrimitive("thread.cpu",         &vThreadCpu,         false);
  vAddPrimitive("thread.dump",        &vThreadDump,        false);
  vAddPrimitive("thread.init",        &vThreadInit,        false);
  vAddPrimitive("thread.id",          &vThreadId,          false);
  vAddPrimitive("thread.mutexLock",   &vThreadMutexLock,   false);
  vAddPrimitive("thread.mutexUnlock", &vThreadMutexUnlock, false);
  vAddPrimitive("thread.name@",       &vThreadNameFetch,   false);
  vAddPrimitive("thread.name!",       &vThreadNameStore,   false);
  vAddPrimitive("thread.taskset@",    &vThreadSfetch,      false);
  vAddPrimitive("thread.taskset!",    &vThreadSstore,      false);
  vAddPrimitive("thread.sleep_s",     &vThreadSleep_s,     false);
  vAddPrimitive("thread.sleep_ms",    &vThreadSleep_ms,    false);
  vAddPrimitive("thread.sleep_us",    &vThreadSleep_us,    false);
  vAddPrimitive("thread.sleep_ns",    &vThreadSleep_ns,    false);
  vAddPrimitive("thread.spawn",       &vThreadSpawn,       false);
}

/*----------------------------------------------------------------------------*/
/* thread.append -- append task to threads taskset                            */
/*----------------------------------------------------------------------------*/
void Task::vThreadAppend(NNP_Task nnpTask)
{
ENTRY("vThreadAppend");
  
  Object* pThreadEmb;
  
  if ( ! nnpTask->m_xDataStack.bPop(pThreadEmb) )
    THROW_INVALID_DSTACK("thread.append");
  
  ASSERT(CN_THREAD == pThreadEmb->eClass());
  
  Thread* pThread = static_cast<Thread*>(pThreadEmb);
  
  TaskSet* pTaskSet = pThread->m_pTaskSet;
  
  Object* pTaskObject;
  if ( ! nnpTask->m_xDataStack.bPop(pTaskObject) )
    THROW_INVALID_DSTACK("task.dump");
  Task* pThatTask = _pTask(pTaskObject);
  
  pTaskSet->vAppend(pThatTask);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* thread.bind -- bind this thread to a cpu                                   */
/*----------------------------------------------------------------------------*/
void Task::vThreadBind(NNP_Task nnpTask)
{
ENTRY("vThreadBind");
  
  i8 iCpu;
  Object* pObject;
  
  if ( ! nnpTask->m_xDataStack.bPop(iCpu) )
    THROW_INVALID_DSTACK("thread.bind");
  
  if ( ! nnpTask->m_xDataStack.bPop(pObject) )
    THROW_INVALID_DSTACK("thread.bind");
  
  ASSERT(CN_THREAD == pObject->eClass());
  
  Thread* pThread = static_cast<Thread*>(pObject);
  
  pThread->vBind(iCpu);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* thread.condSignal -- signal condition on threads mutex                     */
/*----------------------------------------------------------------------------*/
void Task::vThreadCondSignal(NNP_Task nnpTask)
{
 ENTRY("vThreadCondSignal");
  
  Object* pThreadObject;
  if ( ! nnpTask->m_xDataStack.bPop(pThreadObject) )
    THROW_INVALID_DSTACK("thread.condSignal");
  
  Thread* pThread = _pThread(pThreadObject);
  ASSERT(CN_THREAD == pThread->eClass());
  
  pThread->m_xPosixThread.vCondSignal();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* thread.condWait -- wait for cond on threads mutex                          */
/*----------------------------------------------------------------------------*/
void Task::vThreadCondWait(NNP_Task nnpTask)
{
 ENTRY("vThreadCondWait");
  
  Object* pThreadObject;
  if ( ! nnpTask->m_xDataStack.bPop(pThreadObject) )
    THROW_INVALID_DSTACK("thread.condWait");
  
  Thread* pThread = _pThread(pThreadObject);
  ASSERT(CN_THREAD == pThread->eClass());
  
  pThread->m_xPosixThread.vCondWait();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* thread.cpu -- get the cpu this thread is running on                        */
/*----------------------------------------------------------------------------*/
void Task::vThreadCpu(NNP_Task nnpTask)
{
ENTRY("vThreadCpu");
  
  Object* pObject;
  
  if ( ! nnpTask->m_xDataStack.bPop(pObject) )
    THROW_INVALID_DSTACK("thread.cpu");
  
  ASSERT(CN_THREAD == pObject->eClass());
  
  Thread* pThread = static_cast<Thread*>(pObject);
  
  nnpTask->m_xDataStack.vPush(pThread->iCpu());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* thread.dump -- thread dump                                                 */
/*----------------------------------------------------------------------------*/
void Task::vThreadDump(NNP_Task nnpTask)
{
ENTRY("vThreadDump");
  
  Object* pObject;
  
  if ( ! nnpTask->m_xDataStack.bPop(pObject) )
    THROW_INVALID_DSTACK("thread.dump");
  
  ASSERT(CN_THREAD == pObject->eClass());
  
  Thread* pThread = static_cast<Thread*>(pObject);
  
  pThread->vDump();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* thread.id -- get the id of thread                                          */
/*----------------------------------------------------------------------------*/
void Task::vThreadId(NNP_Task nnpTask)
{
ENTRY("vThreadId");
  
  Object* pObject;
  
  if ( ! nnpTask->m_xDataStack.bPop(pObject) )
    THROW_INVALID_DSTACK("thread.id");
  
  ASSERT(CN_THREAD == pObject->eClass());
  
  Thread* pThread = static_cast<Thread*>(pObject);
  
  nnpTask->m_xDataStack.vPush(pThread->m_xPosixThread.m_iOpSysThreadId);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* thread.init -- thread init                                                 */
/*----------------------------------------------------------------------------*/
void Task::vThreadInit(NNP_Task nnpTask)
{
ENTRY("vThreadInit");
  
  Object* pObject;
  
  if ( ! nnpTask->m_xDataStack.bPop(pObject) )
    THROW_INVALID_DSTACK("thread.init");
  
  ASSERT(CN_THREAD == pObject->eClass());
  
  Thread* pThread = static_cast<Thread*>(pObject);
  
  new(pThread) Thread;
  
  Word* pWordThread = Word::pToWord(pThread);
  
  s32 sName;
  sName = pWordThread->sName();
  
  pThread->vName(sName);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* thread.mutexLock -- acquire a threads mutex lock                           */
/*----------------------------------------------------------------------------*/
void Task::vThreadMutexLock(NNP_Task nnpTask)
{
ENTRY("vThreadMutexLock");
  
  Object* pThreadObject;
  if ( ! nnpTask->m_xDataStack.bPop(pThreadObject) )
    THROW_INVALID_DSTACK("thread.mutexLock");
  
  Thread* pThread = _pThread(pThreadObject);
  ASSERT(CN_THREAD == pThread->eClass());
  
  pThread->m_xPosixThread.vMutexLock();

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* thread.mutexUnlock -- release a threads mutex lock                         */
/*----------------------------------------------------------------------------*/
void Task::vThreadMutexUnlock(NNP_Task nnpTask)
{
ENTRY("vThreadMutexUnlock");

  Object* pThreadObject;
  if ( ! nnpTask->m_xDataStack.bPop(pThreadObject) )
    THROW_INVALID_DSTACK("thread.mutexUnlock");
  
  Thread* pThread = _pThread(pThreadObject);
  ASSERT(CN_THREAD == pThread->eClass());
  
  pThread->m_xPosixThread.vMutexUnlock();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* thread.name@ -- fetch thread name                                          */
/*----------------------------------------------------------------------------*/
void Task::vThreadNameFetch(NNP_Task nnpTask)
{
ENTRY("vThreadNameFetch");
  
  Object* pObject;
  
  if ( ! nnpTask->m_xDataStack.bPop(pObject) )
    THROW_INVALID_DSTACK("thread.name@");
  
  ASSERT(CN_THREAD == pObject->eClass());
  
  Thread* pThread = static_cast<Thread*>(pObject);
  
  s32 sName;
  sName = pThread->sName();
  
  nnpTask->m_xDataStack.vPush(sName);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* thread.name! -- store thread name                                          */
/*----------------------------------------------------------------------------*/
void Task::vThreadNameStore(NNP_Task nnpTask)
{
ENTRY("vThreadNameStore");
  
  Object* pObject;
  
  if ( ! nnpTask->m_xDataStack.bPop(pObject) )
    THROW_INVALID_DSTACK("thread.name!");
  
  ASSERT(CN_THREAD == pObject->eClass());
  
  Thread* pThread = static_cast<Thread*>(pObject);
  
  s32 sName;
  ValWsp xNameWsp;
  if ( ! nnpTask->m_xDataStack.bPop(xNameWsp) )
    THROW_INVALID_DSTACK("thread.name!");
  
  sName = xNameWsp.sStr();
  
  pThread->vName(sName);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* thread.taskset@ -- fetch taskset from the thread which is on param stack   */
/*----------------------------------------------------------------------------*/
void Task::vThreadSfetch(NNP_Task nnpTask)
{
ENTRY("vThreadSfetch");
 
  Object* pThreadEmb;
  
  if ( ! nnpTask->m_xDataStack.bPop(pThreadEmb) )
    THROW_INVALID_DSTACK("thread.taskset@");
  
  ASSERT(CN_THREAD == pThreadEmb->eClass());
  
  Thread* pThread = static_cast<Thread*>(pThreadEmb);
  
  TaskSet* pTaskSet = pThread->m_pTaskSet;

  Object* pTaskSetEmb = static_cast<Object*>(pTaskSet);;

  nnpTask->m_xDataStack.vPush(pTaskSetEmb);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* thread.taskset! -- store taskset into the thread which is on param stack   */
/*----------------------------------------------------------------------------*/
void Task::vThreadSstore(NNP_Task nnpTask)
{
ENTRY("vThreadSstore");
  
  Object* pThreadEmb;
  
  if ( ! nnpTask->m_xDataStack.bPop(pThreadEmb) )
    THROW_INVALID_DSTACK("thread.taskset!");
  
  ASSERT(CN_THREAD == pThreadEmb->eClass());
  
  Thread* pThread = static_cast<Thread*>(pThreadEmb);
  
  Object* pTaskSetEmb;
  
  if ( ! nnpTask->m_xDataStack.bPop(pTaskSetEmb) )
    THROW_INVALID_DSTACK("thread.taskset!");
  
  ASSERT(CN_TASKSET == pTaskSetEmb->eClass());
  
  TaskSet* pTaskSet = static_cast<TaskSet*>(pTaskSetEmb);
  
  pThread->m_pTaskSet = pTaskSet;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* thread.sleep_s -- sleep for n seconds                                      */
/*----------------------------------------------------------------------------*/
void Task::vThreadSleep_s(NNP_Task nnpTask)
{
ENTRY("vThreadSleep_s");
  
  i8 nSeconds;
  
  if ( ! nnpTask->m_xDataStack.bPop(nSeconds) )
    THROW_INVALID_DSTACK("sleep_s");
  
  Thread* pThread = Thread::pThread();
  
  pThread->m_xPosixThread.vSleepSec(nSeconds);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* thread.sleep_ms -- sleep for n milliseconds                                */
/*----------------------------------------------------------------------------*/
void Task::vThreadSleep_ms(NNP_Task nnpTask)
{
ENTRY("vThreadSleep_ms");
  
  i8 nMilliseconds;
  
  if ( ! nnpTask->m_xDataStack.bPop(nMilliseconds) )
    THROW_INVALID_DSTACK("sleep_ms");
  
  Thread* pThread = Thread::pThread();
  
  pThread->m_xPosixThread.vSleepMilli(nMilliseconds);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* thread.sleep_us -- sleep for n microseconds                                */
/*----------------------------------------------------------------------------*/
void Task::vThreadSleep_us(NNP_Task nnpTask)
{
ENTRY("vThreadSleep_us");
  
  i8 nMicroseconds;
  
  if ( ! nnpTask->m_xDataStack.bPop(nMicroseconds) )
    THROW_INVALID_DSTACK("sleep_us");
  
  Thread* pThread = Thread::pThread();
  
  pThread->m_xPosixThread.vSleepMicro(nMicroseconds);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* thread.sleep_ns -- sleep for n nanoseconds                                 */
/*----------------------------------------------------------------------------*/
void Task::vThreadSleep_ns(NNP_Task nnpTask)
{
ENTRY("vThreadSleep_ns");
  
  i8 nNanoseconds;
  
  if ( ! nnpTask->m_xDataStack.bPop(nNanoseconds) )
    THROW_INVALID_DSTACK("sleep_ns");
  
  Thread* pThread = Thread::pThread();
  
  pThread->m_xPosixThread.vSleepNano(nNanoseconds);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* thread.spawn -- spwan os thread associated with this thread                */
/*----------------------------------------------------------------------------*/
void Task::vThreadSpawn(NNP_Task nnpTask)
{
ENTRY("vThreadSpawn");
  
  Object* pThreadEmb;
  
  if ( ! nnpTask->m_xDataStack.bPop(pThreadEmb) )
    THROW_INVALID_DSTACK("thread.spawn");
  
  ASSERT(CN_THREAD == pThreadEmb->eClass());
  
  Thread* pThread = static_cast<Thread*>(pThreadEmb);
  
  pThread->vSpawn();
  
  EXIT();
}

// threadCursor: ///////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_ThreadCursor(void)
{
  vAddPrimitive("threadCursor.begin",  &vThreadCursorBegin,  false);
  vAddPrimitive("threadCursor.delete", &vThreadCursorDelete, false);
  vAddPrimitive("threadCursor.end",    &vThreadCursorEnd,    false);
  vAddPrimitive("threadCursor.first",  &vThreadCursorFirst,  false);
  vAddPrimitive("threadCursor.get",    &vThreadCursorGet,    false);
  vAddPrimitive("threadCursor.init",   &vThreadCursorInit,   false);
  vAddPrimitive("threadCursor.new",    &vThreadCursorNew,    false);
  vAddPrimitive("threadCursor.next",   &vThreadCursorNext,   false);
}

/*----------------------------------------------------------------------------*/
/* threadCursor.begin -- begin using thread cursor                            */
/*----------------------------------------------------------------------------*/
void Task::vThreadCursorBegin(NNP_Task nnpTask)
{
ENTRY("vThreadCursorBegin");
  
  Object* pPtrThreadCursor;
  if ( ! nnpTask->m_xDataStack.bPop(pPtrThreadCursor) )
    THROW_INVALID_DSTACK("threadCursor.begin");
  
  ThreadCursor* pThreadCursor = _pThreadCursor(pPtrThreadCursor);
  ASSERT(CN_THREADCURSOR == pThreadCursor->eClass());
  
  pThreadCursor->vBegin();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* threadCursor.delete -- delete a thread cursor                              */
/*----------------------------------------------------------------------------*/
void Task::vThreadCursorDelete(NNP_Task nnpTask)
{
ENTRY("vTheadCursorDelete");
  
  Object* pPtrThreadCursor;
  if ( ! nnpTask->m_xDataStack.bPop(pPtrThreadCursor) )
    THROW_INVALID_DSTACK("threadCursor.delete");
  
  ThreadCursor* pThreadCursor = _pThreadCursor(pPtrThreadCursor);
  ASSERT(CN_THREADCURSOR == pThreadCursor->eClass());
  
  delete pThreadCursor;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* threadCursor.end -- end using thread cursor                                */
/*----------------------------------------------------------------------------*/
void Task::vThreadCursorEnd(NNP_Task nnpTask)
{
ENTRY("vThreadCursorEnd");
  
  Object* pPtrThreadCursor;
  if ( ! nnpTask->m_xDataStack.bPop(pPtrThreadCursor) )
    THROW_INVALID_DSTACK("threadcursor.end");
  
  ThreadCursor* pThreadCursor = _pThreadCursor(pPtrThreadCursor);
  ASSERT(CN_THREADCURSOR == pThreadCursor->eClass());
  
  pThreadCursor->vEnd();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* threadCursor.first -- set to first and push thread                         */
/*----------------------------------------------------------------------------*/
void Task::vThreadCursorFirst(NNP_Task nnpTask)
{
ENTRY("vThreadCursorFirst");
  
  Object* pPtrThreadCursor;
  if ( ! nnpTask->m_xDataStack.bPop(pPtrThreadCursor) )
    THROW_INVALID_DSTACK("threadCursor.first");
  
  ThreadCursor* pThreadCursor = _pThreadCursor(pPtrThreadCursor);
  ASSERT(CN_THREADCURSOR == pThreadCursor->eClass());
  
  pThreadCursor->vFirst();
  
  Object* pThread = pThreadCursor->pObject();
  
  nnpTask->m_xDataStack.vPush(pThread);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* threadCursor.get -- push current thread                                    */
/*----------------------------------------------------------------------------*/
void Task::vThreadCursorGet(NNP_Task nnpTask)
{
ENTRY("vThreadCursorGet");
  
  Object* pPtrThreadCursor;
  if ( ! nnpTask->m_xDataStack.bPop(pPtrThreadCursor) )
    THROW_INVALID_DSTACK("threadCursor.get");
  
  ThreadCursor* pThreadCursor = _pThreadCursor(pPtrThreadCursor);
  ASSERT(CN_THREADCURSOR == pThreadCursor->eClass());
  
  Object* pObject = pThreadCursor->pObject();
  
  nnpTask->m_xDataStack.vPush(pObject);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* threadCursor.init -- init a thread cursor                                  */
/*----------------------------------------------------------------------------*/
void Task::vThreadCursorInit(NNP_Task nnpTask)
{
ENTRY("vThreadCursorInit");
  
  Object* pPtrThreadCursor;
  if ( ! nnpTask->m_xDataStack.bPop(pPtrThreadCursor) )
    THROW_INVALID_DSTACK("threadCursor.init");
  
  ThreadCursor* pThreadCursor = _pThreadCursor(pPtrThreadCursor);
  ASSERT(CN_THREADCURSOR == pThreadCursor->eClass());
  
  new(pThreadCursor) ThreadCursor(gpThreadList);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* threadCursor.new -- new a thread cursor                                    */
/*----------------------------------------------------------------------------*/
void Task::vThreadCursorNew(NNP_Task nnpTask)
{
ENTRY("vThreadCursorNew");
  
  ThreadCursor* pThreadCursor = new ThreadCursor(gpThreadList);
  ASSERT(CN_THREADCURSOR == pThreadCursor->eClass());
  
  nnpTask->m_xDataStack.vPush(pThreadCursor);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* threadCursor.next -- advance to next and push thread                       */
/*----------------------------------------------------------------------------*/
void Task::vThreadCursorNext(NNP_Task nnpTask)
{
ENTRY("vThreadCursorNext");
  
  Object* pPtrThreadCursor;
  if ( ! nnpTask->m_xDataStack.bPop(pPtrThreadCursor) )
    THROW_INVALID_DSTACK("threadCursor.next");
  
  ThreadCursor* pThreadCursor = _pThreadCursor(pPtrThreadCursor);
  ASSERT(CN_THREADCURSOR == pThreadCursor->eClass());
  
  pThreadCursor->vNext();
  
  Object* pThread = pThreadCursor->pObject();
  
  nnpTask->m_xDataStack.vPush(pThread);
  
  EXIT();
}

// threadlist: //////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_ThreadList(void)
{
  vAddPrimitive("threadList.dump", &vThreadListDump,   false);
  vAddPrimitive("threadList.init", &vThreadListInit,   false);
}

/*----------------------------------------------------------------------------*/
/* threadlist.dump -- dump the threadList on param stack                      */
/*----------------------------------------------------------------------------*/
void Task::vThreadListDump(NNP_Task nnpTask)
{
ENTRY("vThreadListDump");
  
  Object* pPtrThreadList;
  if ( ! nnpTask->m_xDataStack.bPop(pPtrThreadList) )
    THROW_INVALID_DSTACK("threadlist.dump");
  
  ThreadList* pThreadList = _pThreadList(pPtrThreadList);
  ASSERT(CN_THREADLIST == pThreadList->eClass());

  pThreadList->vDump("ThreadList:", pSingleTab());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* threadlist.init -- init of threadList on param stack                       */
/*----------------------------------------------------------------------------*/
void Task::vThreadListInit(NNP_Task nnpTask)
{
ENTRY("vThreadListInit");
  
  Object* pPtrThreadList;
  if ( ! nnpTask->m_xDataStack.bPop(pPtrThreadList) )
    THROW_INVALID_DSTACK("threadlist.init");
  
  ThreadList* pThreadList = _pThreadList(pPtrThreadList);
  ASSERT(CN_THREADLIST == pThreadList->eClass());
  
  new(pThreadList) ThreadList;
  
  EXIT();
}

// timer: //////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_Timer(void)
{
  vAddPrimitive("timer.delete",    &vTimerDelete,      false);
  vAddPrimitive("timer.init",      &vTimerInit,        false);
  vAddPrimitive("timer.microsecs", &vTimerMicrosecs,   false);
  vAddPrimitive("timer.millisecs", &vTimerMillisecs,   false);
  vAddPrimitive("timer.nanosecs",  &vTimerNanosecs,    false);
  vAddPrimitive("timer.new",       &vTimerNew,         false);
  vAddPrimitive("timer.reset",     &vTimerReset,       false);
  vAddPrimitive("timer.secs",      &vTimerSecs,        false);
  vAddPrimitive("timer.start",     &vTimerStart,       false);
  vAddPrimitive("timer.ticks",     &vTimerTicks,       false);
  vAddPrimitive("timer.update",    &vTimerUpdate,      false);
}

/*----------------------------------------------------------------------------*/
/* timer.delete -- delete timer                                               */
/*----------------------------------------------------------------------------*/
void Task::vTimerDelete(NNP_Task nnpTask)
{
ENTRY("vTimeDelete");
  
  Header* pPtrTimer;
  
  if ( ! nnpTask->m_xDataStack.bPop(pPtrTimer) )
    THROW_INVALID_DSTACK("timer.delete");
  
  Timer* pTimer = _pTimer(pPtrTimer);
  ASSERT(CN_TIMER == pTimer->eClass());
  
  Object* pTimerObject = reinterpret_cast<Object*>(pTimer);
  
  delete pTimer;
  
  Heap::vFree(pTimerObject);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* timer.init -- init timer                                                   */
/*----------------------------------------------------------------------------*/
void Task::vTimerInit(NNP_Task nnpTask)
{
ENTRY("vTimerInit");
  
  Header* pPtrTimer;
  if ( ! nnpTask->m_xDataStack.bPop(pPtrTimer) )
    THROW_INVALID_DSTACK("timer.init");
  
  Timer* pTimer = _pTimer(pPtrTimer);
  ASSERT(CN_TIMER == pTimer->eClass());
  
  new(pTimer) Timer;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* timer.microsecs -- get elapsed duration in microseconds                    */
/*----------------------------------------------------------------------------*/
void Task::vTimerMicrosecs(NNP_Task nnpTask)
{
ENTRY("vTimerMicrosecs");
  
  Header* pPtrTimer;
  if ( ! nnpTask->m_xDataStack.bPop(pPtrTimer) )
    THROW_INVALID_DSTACK("timer.microsecs");
  
  Timer* pTimer = _pTimer(pPtrTimer);
  ASSERT(CN_TIMER == pTimer->eClass());
  
  i8 iMicroseconds = pTimer->iMicroseconds();
  
  nnpTask->m_xDataStack.vPush(iMicroseconds);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* timer.millisecs -- get elapsed duration in milliseconds                    */
/*----------------------------------------------------------------------------*/
void Task::vTimerMillisecs(NNP_Task nnpTask)
{
ENTRY("vTimerMillisecs");
  
  Header* pPtrTimer;
  if ( ! nnpTask->m_xDataStack.bPop(pPtrTimer) )
    THROW_INVALID_DSTACK("timer.millisecs");
  
  Timer* pTimer = _pTimer(pPtrTimer);
  ASSERT(CN_TIMER == pTimer->eClass());
  
  i8 iMilliseconds = pTimer->iMilliseconds();
  
  nnpTask->m_xDataStack.vPush(iMilliseconds);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* timer.nanosecs -- get elapsed duration in nanoseconds                      */
/*----------------------------------------------------------------------------*/
void Task::vTimerNanosecs(NNP_Task nnpTask)
{
ENTRY("vTimerNanosecs");
  
  Header* pPtrTimer;
  if ( ! nnpTask->m_xDataStack.bPop(pPtrTimer) )
    THROW_INVALID_DSTACK("timer.nanosecs");
  
  Timer* pTimer = _pTimer(pPtrTimer);
  ASSERT(CN_TIMER == pTimer->eClass());
  
  i8 iNanoseconds = pTimer->iNanoseconds();
  
  nnpTask->m_xDataStack.vPush(iNanoseconds);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* timer.new -- new timer                                                     */
/*----------------------------------------------------------------------------*/
void Task::vTimerNew(NNP_Task nnpTask)
{
ENTRY("vTimerNew");
  
  void* pObject = Heap::pAllocate(CN_ROWSTRAND);

  Timer* pTimer = _pTimer(Heap::pAllocate(CN_TIMER));

  new(pTimer) Timer;
 
  ASSERT(CN_TIMER == pTimer->eClass());
  
  Header* pPtrTimer = pTimer;
  
  nnpTask->m_xDataStack.vPush(pPtrTimer);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* timer.reset -- reset timer                                                 */
/*----------------------------------------------------------------------------*/
void Task::vTimerReset(NNP_Task nnpTask)
{
ENTRY("vTimerReset");
  
  Header* pPtrTimer;
  if ( ! nnpTask->m_xDataStack.bPop(pPtrTimer) )
    THROW_INVALID_DSTACK("timer.reset");
  
  Timer* pTimer = _pTimer(pPtrTimer);
  ASSERT(CN_TIMER == pTimer->eClass());
  
  pTimer->vReset();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* timer.secs -- get elapsed duration in seconds                              */
/*----------------------------------------------------------------------------*/
void Task::vTimerSecs(NNP_Task nnpTask)
{
ENTRY("vTimerSecs");
  
  Header* pPtrTimer;
  if ( ! nnpTask->m_xDataStack.bPop(pPtrTimer) )
    THROW_INVALID_DSTACK("timer.secs");
  
  Timer* pTimer = _pTimer(pPtrTimer);
  ASSERT(CN_TIMER == pTimer->eClass());
  
  i8 iSeconds = pTimer->iSeconds();
  
  nnpTask->m_xDataStack.vPush(iSeconds);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* timer.start -- start timer                                                 */
/*----------------------------------------------------------------------------*/
void Task::vTimerStart(NNP_Task nnpTask)
{
ENTRY("vTimerStart");
  
  Header* pPtrTimer;
  if ( ! nnpTask->m_xDataStack.bPop(pPtrTimer) )
    THROW_INVALID_DSTACK("timer.Start");
  
  Timer* pTimer = _pTimer(pPtrTimer);
  ASSERT(CN_TIMER == pTimer->eClass());
  
  pTimer->vStart();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* timer.ticks -- get elapsed ticks                                           */
/*----------------------------------------------------------------------------*/
void Task::vTimerTicks(NNP_Task nnpTask)
{
ENTRY("vTimerTicks");
  
  Header* pPtrTimer;
  if ( ! nnpTask->m_xDataStack.bPop(pPtrTimer) )
    THROW_INVALID_DSTACK("timer.ticks");
  
  Timer* pTimer = _pTimer(pPtrTimer);
  ASSERT(CN_TIMER == pTimer->eClass());
  
  i8 iTicks = pTimer->iTicks();
  
  nnpTask->m_xDataStack.vPush(iTicks);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* timer.update -- update timer                                               */
/*----------------------------------------------------------------------------*/
void Task::vTimerUpdate(NNP_Task nnpTask)
{
ENTRY("vTimerUpdate");
  
  Header* pPtrTimer;
  if ( ! nnpTask->m_xDataStack.bPop(pPtrTimer) )
    THROW_INVALID_DSTACK("timer.update");
  
  Timer* pTimer = _pTimer(pPtrTimer);
  ASSERT(CN_TIMER == pTimer->eClass());
  
  pTimer->vUpdate();
  
  EXIT();
}

// to: /////////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_To(void)
{
  vAddPrimitive(">body",           &vToBody,           false);
  vAddPrimitive(">cmd",            &vToCmd,            false);
  vAddPrimitive(">digitName",      &vToDigitName,      false);
  vAddPrimitive(">int",            &vToInt,            false);
  vAddPrimitive(">seitf",          &vToSEitf,          false);
  vAddPrimitive(">str",            &vToStr,            false);
  vAddPrimitive(">task",           &vToTask,           false);
}

/*----------------------------------------------------------------------------*/
/* >body -- convert word pointer to body pointer                              */
/*----------------------------------------------------------------------------*/
void Task::vToBody(NNP_Task nnpTask)
{
  ENTRY("vToBody");
  
  Object* pWordObject;
  if ( ! nnpTask->m_xDataStack.bPop(pWordObject) )
    THROW_INVALID_DSTACK(">body");
  Word* pWord = _pWord(pWordObject);
  
  char* pBody = pcAdd(pWord,sizeof(Word));
  
  Object* pPtrBody = reinterpret_cast<Object*>(pBody);
  
  nnpTask->m_xDataStack.vPush(pPtrBody);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* >cmd -- convert stack values to cmd string                                 */
/*----------------------------------------------------------------------------*/
void Task::vToCmd(NNP_Task nnpTask)
{
  ENTRY("vToCmd");
  
  i8 nValues;
  if ( ! nnpTask->m_xDataStack.bPop(nValues) )
    THROW_INVALID_DSTACK(">cmd");
  
  s8K sCmd;
  
  ValWsp xStr;
  xStr = "<cmd>";
  
  for (i8 i = 0; i < nValues; i++) {
    ValWsp xValWsp;
    nnpTask->m_xDataStack.vPeek(i,&xValWsp);
    
    if (i)
      String::vConcat(sCmd.a()," ");
    
    xValWsp.vToCommand(sCmd.z());
  }
  
  xStr = sCmd.a();
  
  nnpTask->m_xDataStack.vPush(&xStr);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* >int -- convert value to int                                               */
/*----------------------------------------------------------------------------*/
void Task::vToInt(NNP_Task nnpTask)
{
  ENTRY("vToInt");
  
  ValWsp xStr;
  if ( ! nnpTask->m_xDataStack.bPop(xStr) )
    THROW_INVALID_DSTACK(">int");
  
  s256 sBuff;
  xStr.vToString(sBuff.a());
  
  i8 i = String::iToI8(sBuff.a());
  
  nnpTask->m_xDataStack.vPush(i);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* >seitf -- convert int to seitf ptr                                         */
/*----------------------------------------------------------------------------*/
void Task::vToSEitf(NNP_Task nnpTask)
{
  ENTRY("vToSEitf");
  
  i8 iVal;
  
  if ( ! nnpTask->m_xDataStack.bPop(iVal) )
    THROW_INVALID_DSTACK(">seitf");
  
  SEitf* pSEitf = _pSEitf(iVal);
  RASSERT(pSEitf->eClass() == CN_SEITF);
  
  Object* pPtrSEitf = pSEitf;
  
  nnpTask->m_xDataStack.vPush(pPtrSEitf);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* >str -- convert value to str                                               */
/*----------------------------------------------------------------------------*/
void Task::vToStr(NNP_Task nnpTask)
{
  ENTRY("vToStr");
  
  ValWsp xVal;
  if ( ! nnpTask->m_xDataStack.bPop(xVal) )
    THROW_INVALID_DSTACK(">str");
  
  s8K sString;
  xVal.vToString(sString.a());
  
  nnpTask->m_xDataStack.vPush(sString);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* >task -- convert int to task ptr                                           */
/*----------------------------------------------------------------------------*/
void Task::vToTask(NNP_Task nnpTask)
{
  ENTRY("vToTask");
  
  i8 iVal;
  if ( ! nnpTask->m_xDataStack.bPop(iVal) )
    THROW_INVALID_DSTACK(">task");
  
  Object* pPtrTask = _pTask(iVal);
  
  nnpTask->m_xDataStack.vPush(pPtrTask);
  
  EXIT();
}

// trace: //////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_Trace(void)
{
  vAddPrimitive("traceOff",        &vTraceOff,         false);
  vAddPrimitive("traceOn",         &vTraceOn,          false);
  vAddPrimitive("tracePoint",      &vTracePoint,       false);
  vAddPrimitive("trace!",          &vTraceSet,         false);
}

/*----------------------------------------------------------------------------*/
/* turn tracing off                                                           */
/*----------------------------------------------------------------------------*/
void Task::vTraceOff(NNP_Task nnpTask)
{
  ENTRY("vTraceOff");
  
  nnpTask->m_iTracing = 0;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* turn tracing on                                                            */
/*----------------------------------------------------------------------------*/
void Task::vTraceOn(NNP_Task nnpTask)
{
  ENTRY("vTraceOn");
  
  nnpTask->m_iTracing = C_TRACE_STEPS;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* trace point                                                                */
/*----------------------------------------------------------------------------*/
void Task::vTracePoint(NNP_Task nnpTask)
{
  ENTRY("vTracePoint");
  
  s256 sPointName;
  
  if ( ! nnpTask->m_xDataStack.bPop(sPointName) )
    THROW_INVALID_DSTACK("tracePoint");
  
  if (gxArguments.m_bTracepoint)
    nnpTask->vTracePoint(sPointName.a());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* set tracing                                                                */
/*----------------------------------------------------------------------------*/
void Task::vTraceSet(NNP_Task nnpTask)
{
  ENTRY("vTraceSet");
  
  i8 iTrace;
  
  if ( ! nnpTask->m_xDataStack.bPop(iTrace) )
    THROW_INVALID_DSTACK("trace!");
  
  nnpTask->m_iTracing = iTrace;
  
  EXIT();
}

// tx: /////////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_Tx(void)
{
  vAddPrimitive("tx.dump",         &vTxDump,           false);
}

/*----------------------------------------------------------------------------*/
/* tx.dump -- delete tx                                                       */
/*----------------------------------------------------------------------------*/
void Task::vTxDump(NNP_Task nnpTask)
{
ENTRY("vTxDump");
  
  Object* pPtrTx;
  
  if ( ! nnpTask->m_xDataStack.bPop(pPtrTx) )
    THROW_INVALID_DSTACK("tx.delete");
  
  Tx* pTx = _pTx(pPtrTx);
  ASSERT(CN_TX == pTx->eClass());

  pTx->vDump();
  
  EXIT();
}

// txCollection: ///////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_TxCollection(void)
{
  vAddPrimitive("txCollection.dump", &vTxCollectionDump, false);
  vAddPrimitive("txCollection.init", &vTxCollectionInit, false);
}

/*----------------------------------------------------------------------------*/
/* txCollection.dump -- dump of txCollection on param stack                  */
/*----------------------------------------------------------------------------*/
void Task::vTxCollectionDump(NNP_Task nnpTask)
{
ENTRY("vTxCollectionDump");
  
  Object* pPtrTxCollection;
  if ( ! nnpTask->m_xDataStack.bPop(pPtrTxCollection) )
    THROW_INVALID_DSTACK("txCollection.dump");
  
  TxCollection* pTxCollection = _pTxCollection(pPtrTxCollection);
  ASSERT(CN_TXCOLLECTION == pTxCollection->eClass());
  
  pTxCollection->vDump("gpTxCollection:");
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* txCollection.init -- init of txCollection on param stack                   */
/*----------------------------------------------------------------------------*/
void Task::vTxCollectionInit(NNP_Task nnpTask)
{
ENTRY("vTxCollectionInit");
  
  Object* pPtrTxCollection;
  if ( ! nnpTask->m_xDataStack.bPop(pPtrTxCollection) )
    THROW_INVALID_DSTACK("txCollection.init");
  
  TxCollection* pTxCollection = _pTxCollection(pPtrTxCollection);
  ASSERT(CN_TXCOLLECTION == pTxCollection->eClass());
  
  new(pTxCollection) TxCollection;
  
  EXIT();
}

// txCursor: ///////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_TxCursor(void)
{
  vAddPrimitive("txCursor.begin",  &vTxCursorBegin,  false);
  vAddPrimitive("txCursor.delete", &vTxCursorDelete, false);
  vAddPrimitive("txCursor.end",    &vTxCursorEnd,    false);
  vAddPrimitive("txCursor.first",  &vTxCursorFirst,  false);
  vAddPrimitive("txCursor.get",    &vTxCursorGet,    false);
  vAddPrimitive("txCursor.init",   &vTxCursorInit,   false);
  vAddPrimitive("txCursor.new",    &vTxCursorNew,    false);
  vAddPrimitive("txCursor.next",   &vTxCursorNext,   false);
}

/*----------------------------------------------------------------------------*/
/* txCursor.begin -- begin using tx cursor                                    */
/*----------------------------------------------------------------------------*/
void Task::vTxCursorBegin(NNP_Task nnpTask)
{
ENTRY("vTxCursorBegin");
  
  Object* pPtrTxCursor;
  if ( ! nnpTask->m_xDataStack.bPop(pPtrTxCursor) )
    THROW_INVALID_DSTACK("txCursor.begin");
  
  TxCursor* pTxCursor = _pTxCursor(pPtrTxCursor);
  ASSERT(CN_TXCURSOR == pTxCursor->eClass());
  
  pTxCursor->vBegin();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* txCursor.delete -- delete a tx cursor                                      */
/*----------------------------------------------------------------------------*/
void Task::vTxCursorDelete(NNP_Task nnpTask)
{
ENTRY("vTxCursorDelete");
  
  Object* pPtrTxCursor;
  if ( ! nnpTask->m_xDataStack.bPop(pPtrTxCursor) )
    THROW_INVALID_DSTACK("txCursor.delete");
  
  TxCursor* pTxCursor = _pTxCursor(pPtrTxCursor);
  ASSERT(CN_TXCURSOR == pTxCursor->eClass());
  
  delete pTxCursor;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* txCursor.end -- end using tx cursor                                        */
/*----------------------------------------------------------------------------*/
void Task::vTxCursorEnd(NNP_Task nnpTask)
{
ENTRY("vTxCursorEnd");
  
  Object* pPtrTxCursor;
  if ( ! nnpTask->m_xDataStack.bPop(pPtrTxCursor) )
    THROW_INVALID_DSTACK("txcursor.end");
  
  TxCursor* pTxCursor = _pTxCursor(pPtrTxCursor);
  ASSERT(CN_TXCURSOR == pTxCursor->eClass());
  
  pTxCursor->vEnd();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* txCursor.first -- set to first and push tx                                 */
/*----------------------------------------------------------------------------*/
void Task::vTxCursorFirst(NNP_Task nnpTask)
{
ENTRY("vTxCursorFirst");
  
  Object* pPtrTxCursor;
  if ( ! nnpTask->m_xDataStack.bPop(pPtrTxCursor) )
    THROW_INVALID_DSTACK("txCursor.first");
  
  TxCursor* pTxCursor = _pTxCursor(pPtrTxCursor);
  ASSERT(CN_TXCURSOR == pTxCursor->eClass());
  
  pTxCursor->vFirst();
  
  Object* pObject = pTxCursor->pObject();
  
  nnpTask->m_xDataStack.vPush(pObject);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* txCursor.get -- push current tx                                            */
/*----------------------------------------------------------------------------*/
void Task::vTxCursorGet(NNP_Task nnpTask)
{
ENTRY("vTxCursorGet");
  
  Object* pPtrTxCursor;
  if ( ! nnpTask->m_xDataStack.bPop(pPtrTxCursor) )
    THROW_INVALID_DSTACK("txCursor.get");
  
  TxCursor* pTxCursor = _pTxCursor(pPtrTxCursor);
  ASSERT(CN_TXCURSOR == pTxCursor->eClass());
  
  Object* pObject = pTxCursor->pObject();
  
  nnpTask->m_xDataStack.vPush(pObject);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* txCursor.init -- init a tx cursor                                          */
/*----------------------------------------------------------------------------*/
void Task::vTxCursorInit(NNP_Task nnpTask)
{
ENTRY("vTxCursorInit");
  
  Object* pPtrTxCursor;
  if ( ! nnpTask->m_xDataStack.bPop(pPtrTxCursor) )
    THROW_INVALID_DSTACK("txCursor.init");
  
  TxCursor* pTxCursor = _pTxCursor(pPtrTxCursor);
  ASSERT(CN_TXCURSOR == pTxCursor->eClass());
  
  new(pTxCursor) TxCursor(gpTxCollection);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* txCursor.new -- new a tx cursor                                            */
/*----------------------------------------------------------------------------*/
void Task::vTxCursorNew(NNP_Task nnpTask)
{
ENTRY("vTxCursorNew");
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* txCursor.next -- advance to next and push tx                               */
/*----------------------------------------------------------------------------*/
void Task::vTxCursorNext(NNP_Task nnpTask)
{
ENTRY("vTxCursorNext");
  
  Object* pPtrTxCursor;
  if ( ! nnpTask->m_xDataStack.bPop(pPtrTxCursor) )
    THROW_INVALID_DSTACK("txCursor.next");
  
  TxCursor* pTxCursor = _pTxCursor(pPtrTxCursor);
  ASSERT(CN_THREADCURSOR == pTxCursor->eClass());
  
  pTxCursor->vNext();
  
  Object* pObject = pTxCursor->pObject();
  
  nnpTask->m_xDataStack.vPush(pObject);
  
  EXIT();
}

// txMap: //////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_TxMap(void)
{
  vAddPrimitive("txMap.init",      &vTxMapInit,        false);
}

/*----------------------------------------------------------------------------*/
/* txMap.init -- init of txmap on param stack                                 */
/*----------------------------------------------------------------------------*/
void Task::vTxMapInit(NNP_Task nnpTask)
{
ENTRY("vTxMapInit");
  
  Object* pPtrTxMap;
  if ( ! nnpTask->m_xDataStack.bPop(pPtrTxMap) )
    THROW_INVALID_DSTACK("txMap.init");
  
  TxMap* pTxMap = _pTxMap(pPtrTxMap);
  ASSERT(CN_TXMAP == pTxMap->eClass());
  
  new(pTxMap) TxMap;
  
  EXIT();
}

// txRef: //////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_TxRef(void)
{
  vAddPrimitive("txRef.init",      &vTxRefInit,        false);
  vAddPrimitive("txRef.begin",     &vTxRefBegin,       false);
  vAddPrimitive("txRef.end",       &vTxRefEnd,         false);
  vAddPrimitive("txRef.commit",    &vTxRefCommit,      false);
  vAddPrimitive("txRef.rollback",  &vTxRefRollback,    false);
}

/*----------------------------------------------------------------------------*/
/* txRef.init -- init of txref on param stack                                 */
/*----------------------------------------------------------------------------*/
void Task::vTxRefInit(NNP_Task nnpTask)
{
ENTRY("vTxRefInit");
  
  Object* pPtrTxRef;
  if ( ! nnpTask->m_xDataStack.bPop(pPtrTxRef) )
    THROW_INVALID_DSTACK("txRef.init");
  
  TxRef* pTxRef = _pTxRef(pPtrTxRef);
  ASSERT(CN_TXREF == pTxRef->eClass());
  
  new(pTxRef) TxRef;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* txRef.begin -- begin a tx (via a txRef on stack)                           */
/*----------------------------------------------------------------------------*/
void Task::vTxRefBegin(NNP_Task nnpTask)
{
ENTRY("vTxRefBegin");
  
  Object* pPtrTxRef;
  
  if ( ! nnpTask->m_xDataStack.bPop(pPtrTxRef) )
    THROW_INVALID_DSTACK("tx.begin");
  
  TxRef* pTxRef = _pTxRef(pPtrTxRef);
  ASSERT(CN_TXREF == pTxRef->eClass());
  
  bool bBegun = pTxRef->bBeginTx();
  
  nnpTask->m_xDataStack.vPush(bBegun);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* txRef.end -- end a tx (via a txRef on stack)                               */
/*----------------------------------------------------------------------------*/
void Task::vTxRefEnd(NNP_Task nnpTask)
{
ENTRY("vTxRefEnd");
  
  Object* pPtrTxRef;
  
  if ( ! nnpTask->m_xDataStack.bPop(pPtrTxRef) )
    THROW_INVALID_DSTACK("txRef.end");
  
  TxRef* pTxRef = _pTxRef(pPtrTxRef);
  ASSERT(CN_TXREF == pTxRef->eClass());
  
  bool bEnded = pTxRef->bEndTx();
  
  nnpTask->m_xDataStack.vPush(bEnded);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* txRef.commit -- commit a tx (via a txRef on stack)                         */
/*----------------------------------------------------------------------------*/
void Task::vTxRefCommit(NNP_Task nnpTask)
{
ENTRY("vTxRefCommit");
  
  Object* pPtrTxRef;
  
  if ( ! nnpTask->m_xDataStack.bPop(pPtrTxRef) )
    THROW_INVALID_DSTACK("txRef.commit");
  
  TxRef* pTxRef = _pTxRef(pPtrTxRef);
  ASSERT(CN_TXREF == pTxRef->eClass());
  
  bool bCommitted = pTxRef->bCommitTx();
  
  nnpTask->m_xDataStack.vPush(bCommitted);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* txRef.rollback -- rollback a tx (via a txRef on stack)                     */
/*----------------------------------------------------------------------------*/
void Task::vTxRefRollback(NNP_Task nnpTask)
{
ENTRY("vTxRefRollback");
  
  Object* pPtrTxRef;
  
  if ( ! nnpTask->m_xDataStack.bPop(pPtrTxRef) )
    THROW_INVALID_DSTACK("txRef.rollback");
  
  TxRef* pTxRef = _pTxRef(pPtrTxRef);
  ASSERT(CN_TXREF == pTxRef->eClass());
  
  bool bRolledBack = pTxRef->bRollbackTx();
  
  nnpTask->m_xDataStack.vPush(bRolledBack);
  
  EXIT();
}

// words: //////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_Word(void)
{
  vAddPrimitive("allParticipants", &vAllParticipants,  true );
  vAddPrimitive("allot",           &vAllot,            false);
  vAddPrimitive("cell",            &vCell,             false);
  vAddPrimitive(":",               &vColon,            false);
  vAddPrimitive(",",               &vComma,            false);
  vAddPrimitive("compile,",        &vCompileComma,     false);
  vAddPrimitive("create",          &vCreate,           false);
  vAddPrimitive(">create",         &vCreateFrom,       false);
  vAddPrimitive("does>",           &vDoes,             true );
  vAddPrimitive("does_rt",         &vDoes_RT,          false);
  vAddPrimitive("find",            &vFind,             false);
  vAddPrimitive("findInCurrent",   &vFindInCurrent,    false);
  vAddPrimitive("forget",          &vForget,           false);
  vAddPrimitive("immediate",       &vImmediate,        true );
  vAddPrimitive("localCalls",      &vLocalCalls,       true );
  vAddPrimitive("locate",          &vLocate,           false);
  vAddPrimitive("oneParticipant",  &vOneParticipant,   true );
  vAddPrimitive("postpone",        &vPostpone,         true );
  vAddPrimitive("see",             &vSee,              false);
  vAddPrimitive(";",               &vSemicolon,        true );
  vAddPrimitive("'",               &vTick,             true );
  vAddPrimitive("word.dump",       &vWordDump,         false);
  vAddPrimitive("word.dic",        &vWordDic,          false);
  vAddPrimitive(">forget",         &vWordForget,       false);
  vAddPrimitive("word.immediate@", &vWordImmediateGet, false);
  vAddPrimitive("word.immediate!", &vWordImmediateSet, false);
  vAddPrimitive("word.isImmediate",&vWordIsImmediate,  false);
  vAddPrimitive("word.isPrimitive",&vWordIsPrimitive,  false);
  vAddPrimitive("word.kind@",      &vWordKindGet,      false);
  vAddPrimitive("word.kind!",      &vWordKindSet,      false);
  vAddPrimitive("word.name",       &vWordName,         false);
  vAddPrimitive("word.prev",       &vWordPrev,         false);
  vAddPrimitive("word.this",       &vWordThis,         false);
  vAddPrimitive("word.type@",      &vWordTypeGet,      false);
  vAddPrimitive("word.type!",      &vWordTypeSet,      false);
}

/*----------------------------------------------------------------------------*/
/* allParticipants -- create an allParticipant word                 immediate */
/*----------------------------------------------------------------------------*/
void Task::vAllParticipants(NNP_Task nnpTask)
{
  ENTRY("vAllParticipants");
  
  Word* pCurrentWord = nnpTask->pCurrentWord();
  
  //vTrace("Task::vImmediate: %s\n",pCurrentWord->sName().a());
  
  pCurrentWord->vDistributed(DT_ALLPARTICIPANTS);
  
  // get number of parameters
  nnpTask->vTokenNext();
  
  // convert string to integer
  char* aToken = nnpTask->m_sToken.a();
  i8 iParams = String::iToI8(aToken);
  
  // set the number of parameters in the current word
  pCurrentWord->vParams(iParams);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* allot -- reserve n bytes in current word                                   */
/*----------------------------------------------------------------------------*/
void Task::vAllot(NNP_Task nnpTask)
{
  ENTRY("vAllot");
  
  i8 iBytes;
  
  if ( !  nnpTask->m_xDataStack.bPop(iBytes) )
    THROW_INVALID_DSTACK("allot");
  
  Word* pCurrentWord = nnpTask->pCurrentWord();
  
  char* pAddress = pCurrentWord->pAllot(iBytes);
  ASSERT(pAddress);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* cell -- push size of cell                                                  */
/*----------------------------------------------------------------------------*/
void Task::vCell(NNP_Task nnpTask)
{
  ENTRY("vCell");
  
  i8 nBytes = sizeof(void*);
  
  nnpTask->m_xDataStack.vPush(nBytes);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* : -- colon                                                                 */
/*----------------------------------------------------------------------------*/
void Task::vColon(NNP_Task nnpTask)
{
  ENTRY("vColon");
  
  nnpTask->m_bCompiling = true;
  
  nnpTask->vTokenNext();
  
  Word* pNewWord = new(nnpTask->m_xDicStack.pTop()) Word(nnpTask->m_sToken.a());
  
  nnpTask->vCurrentWord(pNewWord);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* comma -- encode push value                                                 */
/*----------------------------------------------------------------------------*/
void Task::vComma(NNP_Task nnpTask)
{
  ENTRY("vComma");
  
  Word* pCurrentWord = nnpTask->pCurrentWord();
  
  ValWsp xValWsp;
  if ( ! nnpTask->m_xDataStack.bPop(xValWsp) )
    THROW_INVALID_DSTACK("comma");
  
  pCurrentWord->vEncode("pushValue",&xValWsp);
  pCurrentWord->vEncode("exit");
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* compile, -- encode call                                                    */
/*----------------------------------------------------------------------------*/
void Task::vCompileComma(NNP_Task nnpTask)
{
  ENTRY("vCompileComma");
  
  Object* pWordObject;
  if ( ! nnpTask->m_xDataStack.bPop(pWordObject) )
    THROW_INVALID_DSTACK("compile,");
  Word* pCalledWord = _pWord(pWordObject);
  
  Word* pCurrentWord = nnpTask->pCurrentWord();
  
  TaskStep* ppCalledEntryStep = pCalledWord->ppEntryStep();
  
  pCurrentWord->vEncode(ppCalledEntryStep);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* create -- create word in dictionary                                        */
/*----------------------------------------------------------------------------*/
void Task::vCreate(NNP_Task nnpTask)
{
ENTRY("vCreate");
  
  nnpTask->vTokenNext();
  
  Dic* pDic;
  if (nnpTask->m_pSEitf)
    pDic = nnpTask->m_pSEitf->pDicStack()->pTop();
  else
    pDic = nnpTask->m_xDicStack.pTop();
  
  Word* pNewWord = new(pDic) Word(nnpTask->m_sToken.a());
  
  nnpTask->vCurrentWord(pNewWord);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* >create -- create word in dictionary using string on stack                 */
/*----------------------------------------------------------------------------*/
void Task::vCreateFrom(NNP_Task nnpTask)
{
ENTRY("vCreateFrom");

  s32 sName;
  if ( ! nnpTask->m_xDataStack.bPop(sName) )
    THROW_INVALID_DSTACK(">create");
  
  Dic* pDic;
  if (nnpTask->m_pSEitf)
    pDic = nnpTask->m_pSEitf->pDicStack()->pTop();
  else
    pDic = nnpTask->m_xDicStack.pTop();
  
  Word* pNewWord = new(pDic) Word(sName.a());
  
  nnpTask->vCurrentWord(pNewWord);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* does> -- routine does                                                      */
/*----------------------------------------------------------------------------*/
void Task::vDoes(NNP_Task nnpTask)
{
ENTRY("vDoes");
  
  Word* pCurrentWord = nnpTask->pCurrentWord();
  
  pCurrentWord->vEncode("does_rt");
  pCurrentWord->vEncode("exit");
  
  //pCurrentWord->vEncode("pushBody");
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* does_rt -- runtime for does>                                               */
/*----------------------------------------------------------------------------*/
void Task::vDoes_RT(NNP_Task nnpTask)
{
  ENTRY("vDoes_RT");
  
  Word* pCurrentWord = nnpTask->pCurrentWord();
  
  pCurrentWord->vEntryStep(&Task::vEnterObject);
  pCurrentWord->vEntryCell(nnpTask->IP.m_pCell+1);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* find -- find word                                                          */
/*----------------------------------------------------------------------------*/
void Task::vFind(NNP_Task nnpTask)
{
ENTRY("vFind");
  
  s256 sName;
  if ( ! nnpTask->m_xDataStack.bPop(sName) )
    THROW_INVALID_DSTACK("find");
 
  if (nnpTask->m_iTracing) {
    i8 i = 0; i++;
  }
  
  Word* pWord = nnpTask->pFindWord(sName);
  
  nnpTask->m_xDataStack.vPush(pWord);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* findInCurrent -- find word in current dictionary                           */
/*----------------------------------------------------------------------------*/
void Task::vFindInCurrent(NNP_Task nnpTask)
{
  ENTRY("vFind");
  
  s256 sName;
  if ( ! nnpTask->m_xDataStack.bPop(sName) )
    THROW_INVALID_DSTACK("findInCurrent");
  
  if (nnpTask->m_iTracing) {
    i8 i = 0; i++;
  }
  
  Word* pWord = nnpTask->pFindWordInCurrent(sName);
  
  nnpTask->m_xDataStack.vPush(pWord);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* forget -- forget word                                                      */
/*----------------------------------------------------------------------------*/
void Task::vForget(NNP_Task nnpTask)
{
ENTRY("vForget");
  
  nnpTask->vTokenNext();
  
  Word* pWord = nnpTask->pFindWord(nnpTask->m_sToken);
  
  if ( ! pWord )
    THROW("No such word: %s",nnpTask->m_sToken.a());
  
  bool bResult = nnpTask->bForgetWord(nnpTask->m_sToken);
  ASSERT(bResult);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* immediate -- make the most recent definition an immediate word             */
/*----------------------------------------------------------------------------*/
void Task::vImmediate(NNP_Task nnpTask)
{
  ENTRY("vImmediate");
  
  Word* pCurrentWord = nnpTask->pCurrentWord();
  
  //vTrace("Task::vImmediate: %s\n",pCurrentWord->sName().a());
  
  pCurrentWord->vImmediate(true);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* localCalls -- set current word as a locals calls only word       immediate */
/*----------------------------------------------------------------------------*/
void Task::vLocalCalls(NNP_Task nnpTask)
{
  ENTRY("vLocalCalls");
  
  Word* pCurrentWord = nnpTask->pCurrentWord();
  
  pCurrentWord->vDistributed(DT_LOCALCALLS);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* locate -- locate word based on ip                                          */
/*----------------------------------------------------------------------------*/
void Task::vLocate(NNP_Task nnpTask)
{
  ENTRY("vLocate");
  
  i8 iAsInt;
  if ( ! nnpTask->m_xDataStack.bPopParamInt(iAsInt) )
    THROW_INVALID_DSTACK("locate");
  
  void* pCellVoid = (void*)iAsInt;
  Cell* pCell = _pCell(pCellVoid);
  
  Word* pLocatedWord = nnpTask->pLocateWord(pCell);
  
  if (pLocatedWord) {
    s32 sName;
    sName = pLocatedWord->sName();
    nnpTask->m_xDataStack.vPush(sName);
  }
  else {
    i8 iZero = 0;
    nnpTask->m_xDataStack.vPush(iZero);
  }
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* oneParticipant -- create a oneParticipant word                   immediate */
/*----------------------------------------------------------------------------*/
void Task::vOneParticipant(NNP_Task nnpTask)
{
  ENTRY("vAllParticipants");
  
  Word* pCurrentWord = nnpTask->pCurrentWord();
  
  //vTrace("Task::vImmediate: %s\n",pCurrentWord->sName().a());
  
  pCurrentWord->vDistributed(DT_ONEPARTICIPANT);
  
  // get number of parameters
  nnpTask->vTokenNext();
  
  // convert token to integer
  i8 iParams = String::iToI8(nnpTask->m_sToken.a());
  
  // set the number of parameters in the current word
  pCurrentWord->vParams(iParams);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* postpone -- append call to next word from input to current word  immediate */
/*----------------------------------------------------------------------------*/
void Task::vPostpone(NNP_Task nnpTask)
{
  ENTRY("vPostpone");
  
  Word* pCurrentWord = nnpTask->pCurrentWord();
  
  // get name of word to be called
  nnpTask->vTokenNext();
  
  // find the word in dic stack
  Word* pCalledWord = nnpTask->pFindWord(nnpTask->m_sToken);
  if ( ! pCalledWord )
    THROW("No word: %s ",nnpTask->m_sToken.a());
  
  if (pCalledWord->bImmediate()) {
    
    // put call to immediate word into this current definition
    
    // get indirect pointer to entry step of called word
    TaskStep* ppCalledEntryStep = pCalledWord->ppEntryStep();
    
    // copy indirect pointer into the current word
    pCurrentWord->vEncode(ppCalledEntryStep);
  }
  else {
  
    // put call to normal word into some future definition
    
    // copy indirect pointer into the current word
    pCurrentWord->vEncode("pushValue",pCalledWord);
    if (WT_PRIMITIVE == pCalledWord->eWordType())
      pCurrentWord->vEncode("encodeCallPrimitive");
    else
      pCurrentWord->vEncode("encodeCallRoutine");
  }
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* see -- dump routine                                                        */
/*----------------------------------------------------------------------------*/
void Task::vSee(NNP_Task nnpTask)
{
  ENTRY("vSee");
  
  s8K sWorkspace;
  
  nnpTask->vTokenNext();
  
  Word* pWord = nnpTask->pFindWord(nnpTask->m_sToken);
  if ( ! pWord ) {
    vTrace("See: %s unknown\n",nnpTask->m_sToken.a());
    return;
  }
  
  vTrace("See: %s\n",pWord->sName().a());
  
  Cell* pDoesCell = pWord->pEntryCell();
  
  Word* pDoesWord = (Word*)( ((u8)pDoesCell & (u8)~0xFFFF)
                            + sizeof(HeapBuffer));
  
  while (pDoesCell < pDoesWord->pLastCell()) {
    
    Word* pCallsWord = nnpTask->m_xDicStack.pFindStep(*pDoesCell->m_ppEntryStep);
    
    // jumps
    if ( *pDoesCell->m_ppEntryStep == Task::vFlowJump ||
        *pDoesCell->m_ppEntryStep == Task::vFlowJumpFalse ||
        *pDoesCell->m_ppEntryStep == Task::vFlowJumpTrue ) {
      
      vTrace("%p: %s",pDoesCell,pCallsWord->sName().a());
      
      pDoesCell++;
      
      vTrace(" %p\n",pDoesCell->m_pJumpTo);
      
      pDoesCell++;
      
      continue;
    }
    
    // push
    if ( *pDoesCell->m_ppEntryStep == Task::vPushValue ) {
      
      vTrace("%p: %s",pDoesCell,pCallsWord->sName().a());
      
      pDoesCell++;
      
      ValWsp* pValue = reinterpret_cast<ValWsp*>(pDoesCell);
      
      if (CN_STRVAL == pValue->eClass()) {
        vTrace(" \"%s\"\n",pValue->sStr());
      }
      else {
        pValue->vToString(sWorkspace.a());
        vTrace(" %s\n",sWorkspace.a());
      }
      
      i8 iBytes = pValue->iSize();
      pDoesCell = reinterpret_cast<Cell*>(pcAdd(pDoesCell,iBytes));
      
      continue;
    }
    
    // enterRoutine
    if ( *pDoesCell->m_ppEntryStep == Task::vEnterRoutine ) {
      
      Word* pCallsWord = (Word*)( ((u8)pDoesCell->m_ppEntryStep & (u8)~0xFFFF)
                                 + sizeof(HeapBuffer));
      
      vTrace("%p: %s\n",pDoesCell,pCallsWord->sName().a());
      pDoesCell++;
      continue;
    }
    
    // other
    vTrace("%p: %s\n",pDoesCell,pCallsWord->sName().a());
    
    pDoesCell++;
  }
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* ; -- end compile routine                                                   */
/*----------------------------------------------------------------------------*/
void Task::vSemicolon(NNP_Task nnpTask)
{
  ENTRY("vSemicolon");
  
  Word* pCurrentWord = nnpTask->pCurrentWord();
  
  pCurrentWord->vEncode("exit");
  
  nnpTask->m_bCompiling = false;
  
  if (nnpTask->m_nLocals) {
    
    // reset the number of local variables
    nnpTask->m_nLocals = 0;
    
    // pop top dictionary
    nnpTask->vDicDrop();
  }
  
  
  if (DT_ALLPARTICIPANTS == pCurrentWord->eDistributed() ||
      DT_ONEPARTICIPANT  == pCurrentWord->eDistributed()) {
    
    s32 sTildeName;
    sTildeName = "~";
    String::vConcat(sTildeName.a(),pCurrentWord->sName().a());
    
    // create new word for tilde of current word
    Dic* pDic = nnpTask->pDicPeek();
    Word* pWordTilde = new(pDic) Word(sTildeName.a());
    // ** do not set as current word **
    
    // encode the word
    ValWsp xValWsp;
    
    xValWsp = sTildeName.a();
    pWordTilde->vEncode("pushValue",&xValWsp);
    pWordTilde->vEncode("tracePoint");
    
    xValWsp = pCurrentWord->iParams();
    pWordTilde->vEncode("pushValue",&xValWsp);
    pWordTilde->vEncode("put$$");
    
    xValWsp = pCurrentWord->sName().a();
    pWordTilde->vEncode("pushValue",&xValWsp);
    
    xValWsp = "theIndirectCall";
    pWordTilde->vEncode("pushValue",&xValWsp);
    pWordTilde->vEncode("tracePoint");
    
    pWordTilde->vEncode("$$>valset");
    
    if (DT_ALLPARTICIPANTS == pCurrentWord->eDistributed())
      pWordTilde->vEncode(">>allParticipants>");
    else
      pWordTilde->vEncode(">>oneParticipant>");
    
    xValWsp = sTildeName.a();
    pWordTilde->vEncode("pushValue",&xValWsp);
    pWordTilde->vEncode("tracePoint");
    
    pWordTilde->vEncode("checkResult");
    
    pWordTilde->vEncode("exit");
  }
  
  nnpTask->vCurrentWord(nullptr);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* tick -- find next word                                                     */
/*----------------------------------------------------------------------------*/
void Task::vTick(NNP_Task nnpTask)
{
  ENTRY("vTick");
  
  nnpTask->vTokenNext();
  
  //  if ( ! bExtracted ) {
  //    String::vSprint(nnpTask->m_sException,"No name: ' ");
  //    nnpTask->m_sFile = __FILE__;
  //    throw (i8)__LINE__;
  //  }
  
  Word* pWord = nnpTask->pFindWord(nnpTask->m_sToken);
  
  if ( ! pWord )
    THROW("No word: %s ",nnpTask->m_sToken.a());
  
  if (nnpTask->m_bCompiling) {
    Word* pCurrentWord = nnpTask->pCurrentWord();
    pCurrentWord->vEncode("pushValue",pWord);
  }
  else {
    nnpTask->m_xDataStack.vPush(pWord);
  }
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* word -- root kind                                                          */
/*----------------------------------------------------------------------------*/
void Task::vWord(NNP_Task nnpTask)
{
  ENTRY("vWord");
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* >forget -- forget word on stack                                            */
/*----------------------------------------------------------------------------*/
void Task::vWordForget(NNP_Task nnpTask)
{
ENTRY("vWordForget");
  
  Object* pWordObject;
  if ( ! nnpTask->m_xDataStack.bPop(pWordObject) )
    THROW_INVALID_DSTACK("vWordForget");
  Word* pWord = _pWord(pWordObject);
  
  bool bResult = nnpTask->bForgetWord(pWord->sName());
  ASSERT(bResult);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* word.dump -- dump word                                                     */
/*----------------------------------------------------------------------------*/
void Task::vWordDump(NNP_Task nnpTask)
{
ENTRY("vWordDump");
  
  Object* pWordObject;
  if ( ! nnpTask->m_xDataStack.bPop(pWordObject) )
    THROW_INVALID_DSTACK("vWordDump");
  Word* pWord = _pWord(pWordObject);
  
  pWord->vDump();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* word.dic -- push word's dictionary                                         */
/*----------------------------------------------------------------------------*/
void Task::vWordDic(NNP_Task nnpTask)
{
ENTRY("vWordDic");
  
  Object* pWordObject;
  if ( ! nnpTask->m_xDataStack.bPop(pWordObject) )
    THROW_INVALID_DSTACK("vWordDic");
  Word* pWord = _pWord(pWordObject);
  
  Dic* pDic = pWord->pDic();
  
  nnpTask->m_xDataStack.vPush(pDic->sName().a());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* word.immediate@ -- get immediate flag of word                              */
/*----------------------------------------------------------------------------*/
void Task::vWordImmediateGet(NNP_Task nnpTask)
{
  ENTRY("vWordImmediateGet");
  
  Object* pWordObject;
  if ( ! nnpTask->m_xDataStack.bPop(pWordObject) )
    THROW_INVALID_DSTACK("word.immediate@");
  Word* pWord = _pWord(pWordObject);
  
  i8 iImmediate = pWord->bImmediate();
  
  nnpTask->m_xDataStack.vPush(iImmediate);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* word.immediate! -- set immediate flag of word                              */
/*----------------------------------------------------------------------------*/
void Task::vWordImmediateSet(NNP_Task nnpTask)
{
  ENTRY("vWordImmediateSet");
  
  Object* pWordObject;
  if ( ! nnpTask->m_xDataStack.bPop(pWordObject) )
    THROW_INVALID_DSTACK("word.immediate!");
  Word* pWord = _pWord(pWordObject);
  
  i8 iImmediate;
  if ( ! nnpTask->m_xDataStack.bPop(iImmediate) )
    THROW_INVALID_DSTACK("word.immediate!");
  Word* pWordKind = _pWord(pWordObject);
  
  pWord->vImmediate(iImmediate);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* isImmediate -- is word immediate?                                          */
/*----------------------------------------------------------------------------*/
void Task::vWordIsImmediate(NNP_Task nnpTask)
{
  ENTRY("vWordIsImmediate");
  
  // we should just modify the pointers rather than actually pop
  
  Object* pWordObject;
  if ( ! nnpTask->m_xDataStack.bPop(pWordObject) )
    THROW_INVALID_DSTACK("isImmediate");
  Word* pWord = _pWord(pWordObject);
  
  i8 iIsImmediate = pWord->bImmediate() ? 1 : 0;
  
  nnpTask->m_xDataStack.vPush(iIsImmediate);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* word.isPrimitive -- is word a primitive?                                   */
/*----------------------------------------------------------------------------*/
void Task::vWordIsPrimitive(NNP_Task nnpTask)
{
  ENTRY("vWordIsPrimitive");
  
  Object* pWordObject;
  if ( ! nnpTask->m_xDataStack.bPop(pWordObject) )
    THROW_INVALID_DSTACK("word.isPrimitive");
  Word* pWord = _pWord(pWordObject);
  
  i8 iIsPrimitive = WT_PRIMITIVE == pWord->eWordType() ? 1 : 0;
  
  nnpTask->m_xDataStack.vPush(iIsPrimitive);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* word.kind@ -- word kind get                                                */
/*----------------------------------------------------------------------------*/
void Task::vWordKindGet(NNP_Task nnpTask)
{
ENTRY("vWordKindGet");
  
  Object* pWordObject;
  if ( ! nnpTask->m_xDataStack.bPop(pWordObject) )
    THROW_INVALID_DSTACK("vWordKindGet");
  Word* pWord = _pWord(pWordObject);
  
  Word* pWordKind = pWord->pKind();
  
  nnpTask->m_xDataStack.vPush(pWordKind);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* word.kind! -- word kind set                                                */
/*----------------------------------------------------------------------------*/
void Task::vWordKindSet(NNP_Task nnpTask)
{
ENTRY("vWordKindSet");
  
  Object* pWordObject;
  if ( ! nnpTask->m_xDataStack.bPop(pWordObject) )
    THROW_INVALID_DSTACK("vWordKindSet");
  Word* pWord = _pWord(pWordObject);
  
  if ( ! nnpTask->m_xDataStack.bPop(pWordObject) )
    THROW_INVALID_DSTACK("vWordKindSet");
  Word* pWordKind = _pWord(pWordObject);
  
  pWord->vKind(pWordKind);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* word.name -- push name of word                                             */
/*----------------------------------------------------------------------------*/
void Task::vWordName(NNP_Task nnpTask)
{
ENTRY("vWordName");
  
  Object* pWordObject;
  if ( ! nnpTask->m_xDataStack.bPop(pWordObject) )
    THROW_INVALID_DSTACK("word.name");
  Word* pWord = _pWord(pWordObject);
  
  s32 sWordName(pWord->sName().a());
  
  nnpTask->m_xDataStack.vPush(sWordName);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* word.prev -- push prev word                                                */
/*----------------------------------------------------------------------------*/
void Task::vWordPrev(NNP_Task nnpTask)
{
  ENTRY("vWordPrev");
  
  Object* pWordObject;
  if ( ! nnpTask->m_xDataStack.bPop(pWordObject) )
    THROW_INVALID_DSTACK("wordPrev");
  Word* pWord = _pWord(pWordObject);
  
  Word* pPrevWord = pWord->pPrevWord();
  
  nnpTask->m_xDataStack.vPush(pPrevWord);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* word.this -- push current word                                             */
/*----------------------------------------------------------------------------*/
void Task::vWordThis(NNP_Task nnpTask)
{
ENTRY("vWordThis");
  
  Word* pWord = nnpTask->pCurrentWord();
  
  nnpTask->m_xDataStack.vPush(pWord);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* word.type@ -- word type get                                                */
/*----------------------------------------------------------------------------*/
void Task::vWordTypeGet(NNP_Task nnpTask)
{
  ENTRY("vWordTypeGet");
  
  Object* pWordObject;
  if ( ! nnpTask->m_xDataStack.bPop(pWordObject) )
    THROW_INVALID_DSTACK("vWordTypeGet");
  Word* pWord = _pWord(pWordObject);
  
  eWT eWordType = pWord->eWordType();
  
  nnpTask->m_xDataStack.vPush(eWordType);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* word.type! -- word type set                                                */
/*----------------------------------------------------------------------------*/
void Task::vWordTypeSet(NNP_Task nnpTask)
{
  ENTRY("vWordTypeSet");
  
  Object* pWordObject;
  if ( ! nnpTask->m_xDataStack.bPop(pWordObject) )
    THROW_INVALID_DSTACK("vWordTypeSet");
  Word* pWord = _pWord(pWordObject);
  
  i8 iWordType;
  
  if ( ! nnpTask->m_xDataStack.bPop(iWordType) )
    THROW_INVALID_DSTACK("vWordTypeSet");
  Word* pWordKind = _pWord(pWordObject);
  
  pWord->vWordType((eWT)iWordType);
  
  EXIT();
}

// zero: ///////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_Zero(void)
{
  vAddPrimitive("0<",              &vZeroLess,         false);
  vAddPrimitive("0<=",             &vZeroLessEqual,    false);
  vAddPrimitive("0==",             &vZeroEqual,        false);
  vAddPrimitive("0!=",             &vZeroNotEqual,     false);
  vAddPrimitive("0>=",             &vZeroGreaterEqual, false);
  vAddPrimitive("0>",              &vZeroGreater,      false);
}

/*----------------------------------------------------------------------------*/
/* 0< -- test if top value is less than zero                                  */
/*----------------------------------------------------------------------------*/
void Task::vZeroLess(NNP_Task nnpTask)
{
  ENTRY("vZeroLess");
  
  i8 iZero;
  if ( ! nnpTask->m_xDataStack.bPopParamInt(iZero) )
    THROW_INVALID_DSTACK("0<");
  
  if (iZero < 0)
    nnpTask->m_xDataStack.vPush((i8)1);
  else
    nnpTask->m_xDataStack.vPush((i8)0);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* 0<= -- test if top value is less than or equal to zero                     */
/*----------------------------------------------------------------------------*/
void Task::vZeroLessEqual(NNP_Task nnpTask)
{
  ENTRY("vZeroLessEqual");
  
  i8 iZero;
  if ( ! nnpTask->m_xDataStack.bPopParamInt(iZero) )
    THROW_INVALID_DSTACK("0<=");
  
  if (iZero <= 0)
    nnpTask->m_xDataStack.vPush((i8)1);
  else
    nnpTask->m_xDataStack.vPush((i8)0);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* 0== -- test if top value is equal to zero / nullptr                        */
/*----------------------------------------------------------------------------*/
void Task::vZeroEqual(NNP_Task nnpTask)
{
  ENTRY("vZeroEqual");
  
  i8 iZero;
  if ( ! nnpTask->m_xDataStack.bPopParamInt(iZero) )
    THROW_INVALID_DSTACK("0==");
  
  if (iZero == 0)
    nnpTask->m_xDataStack.vPush((i8)1);
  else
    nnpTask->m_xDataStack.vPush((i8)0);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* 0!= -- test if top value is not equal to zero                              */
/*----------------------------------------------------------------------------*/
void Task::vZeroNotEqual(NNP_Task nnpTask)
{
  ENTRY("vZeroNotEqual");
  
  i8 iZero;
  if ( ! nnpTask->m_xDataStack.bPopParamInt(iZero) )
    THROW_INVALID_DSTACK("0!=");
  
  if (iZero != 0)
    nnpTask->m_xDataStack.vPush((i8)1);
  else
    nnpTask->m_xDataStack.vPush((i8)0);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* zeroGreaterEqual -- test if top value is greater than or equal to zero     */
/*----------------------------------------------------------------------------*/
void Task::vZeroGreaterEqual(NNP_Task nnpTask)
{
  ENTRY("vZeroGreaterEqual");
  
  i8 iZero;
  if ( ! nnpTask->m_xDataStack.bPopParamInt(iZero) )
    THROW_INVALID_DSTACK("0>=");
  
  if (iZero >= 0)
    nnpTask->m_xDataStack.vPush((i8)1);
  else
    nnpTask->m_xDataStack.vPush((i8)0);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* 0> -- test if top value is greater than zero                              */
/*----------------------------------------------------------------------------*/
void Task::vZeroGreater(NNP_Task nnpTask)
{
  ENTRY("vZeroGreater");
  
  i8 iZero;
  if ( ! nnpTask->m_xDataStack.bPopParamInt(iZero) )
    THROW_INVALID_DSTACK("0>");
  
  if (iZero > 0)
    nnpTask->m_xDataStack.vPush((i8)1);
  else
    nnpTask->m_xDataStack.vPush((i8)0);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
