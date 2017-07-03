/*============================================================================*/
/* Steps_Accum.cpp -- Accumulator step group                                  */
/*============================================================================*/

#include "StorageEngine.h"

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

// accum: //////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_Accum(void)
{
  vAddPrimitive("accum.delete"_Str, &vAccumDelete, false);
  vAddPrimitive("accum::new"_Str,   &vAccumNew,    false);

  vAddPrimitive("accum.accumBuffer@"_Str, &vAccumAccumBuffer, false);
  
  vAddPrimitive("accum.count@"_Str, &vAccumCountFetch, false);
  vAddPrimitive("accum.count!"_Str, &vAccumCountStore, false);
  vAddPrimitive("accum.group@"_Str, &vAccumGroupFetch, false);
  vAddPrimitive("accum.group!"_Str, &vAccumGroupStore, false);
  vAddPrimitive("accum.max@"_Str,   &vAccumMaxFetch,   false);
  vAddPrimitive("accum.max!"_Str,   &vAccumMaxStore,   false);
  vAddPrimitive("accum.min@"_Str,   &vAccumMinFetch,   false);
  vAddPrimitive("accum.min!"_Str,   &vAccumMinStore,   false);
  vAddPrimitive("accum.sum@"_Str,   &vAccumSumFetch,   false);
  vAddPrimitive("accum.sum!"_Str,   &vAccumSumStore,   false);
}

/*----------------------------------------------------------------------------*/
/* accum.delete -- delete accumulator                                         */
/*----------------------------------------------------------------------------*/
void Task::vAccumDelete(MP_Task mpTask)
{
  ENTRY("vAccumDelete"_Str);
  
  Object* pPtrAccum;
  if ( ! mpTask->m_pDataStack->bPop(pPtrAccum) )
    THROW_INVALID_DSTACK("accum.delete"_Str);
  Accum* pAccum = _pAccum(pPtrAccum);
  
  delete pAccum;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* accum::new -- accumulator new                                              */
/*----------------------------------------------------------------------------*/
void Task::vAccumNew(MP_Task mpTask)
{
  ENTRY("vAccumNew"_Str);
  
  Accum* pAccum = new Accum;
  
  mpTask->m_pDataStack->vPushObject(pAccum);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* accum.accumBuffer-- accumulat a buffer of rows                             */
/*----------------------------------------------------------------------------*/
void Task::vAccumAccumBuffer(MP_Task mpTask)
{
  ENTRY("vAccumAccumBuffer"_Str);
  
  Object* pPtrAccum;
  if ( ! mpTask->m_pDataStack->bPop(pPtrAccum) )
    THROW_INVALID_DSTACK("accum.accumBuffer"_Str);
  Accum* pAccum = _pAccum(pPtrAccum);
  
  // tbd...
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* accum.count@ -- fetch count                                                */
/*----------------------------------------------------------------------------*/
void Task::vAccumCountFetch(MP_Task mpTask)
{
  ENTRY("vAccumCountFetch"_Str);
  
  Object* pPtrAccum;
  if ( ! mpTask->m_pDataStack->bPop(pPtrAccum) )
    THROW_INVALID_DSTACK("accum.count@"_Str);
  Accum* pAccum = _pAccum(pPtrAccum);
  
  mpTask->m_pDataStack->vPushInt(pAccum->m_iCount);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* accum.count! -- store count                                                */
/*----------------------------------------------------------------------------*/
void Task::vAccumCountStore(MP_Task mpTask)
{
  ENTRY("vAccumCountStore"_Str);
  
  Object* pPtrAccum;
  if ( ! mpTask->m_pDataStack->bPop(pPtrAccum) )
    THROW_INVALID_DSTACK("accum.count!"_Str);
  Accum* pAccum = _pAccum(pPtrAccum);

  i8 iCount;
  if ( ! mpTask->m_pDataStack->bPop(iCount) )
    THROW_INVALID_DSTACK("accum.count!"_Str);

  pAccum->m_iCount = iCount;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* accum.group@ -- fetch group                                                */
/*----------------------------------------------------------------------------*/
void Task::vAccumGroupFetch(MP_Task mpTask)
{
  ENTRY("vAccumGroupFetch"_Str);
  
  Object* pPtrAccum;
  if ( ! mpTask->m_pDataStack->bPop(pPtrAccum) )
    THROW_INVALID_DSTACK("accum.group@"_Str);
  Accum* pAccum = _pAccum(pPtrAccum);
  
  mpTask->m_pDataStack->vPushInt(pAccum->m_iGroup);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* accum.group! -- store group                                                */
/*----------------------------------------------------------------------------*/
void Task::vAccumGroupStore(MP_Task mpTask)
{
  ENTRY("vAccumGroupStore"_Str);
  
  Object* pPtrAccum;
  if ( ! mpTask->m_pDataStack->bPop(pPtrAccum) )
    THROW_INVALID_DSTACK("accum.group!"_Str);
  Accum* pAccum = _pAccum(pPtrAccum);
  
  i8 iGroup;
  if ( ! mpTask->m_pDataStack->bPop(iGroup) )
    THROW_INVALID_DSTACK("accum.group!"_Str);
  
  pAccum->m_iGroup = iGroup;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* accum.max@ -- fetch max                                                    */
/*----------------------------------------------------------------------------*/
void Task::vAccumMaxFetch(MP_Task mpTask)
{
  ENTRY("vAccumMaxFetch"_Str);
  
  Object* pPtrAccum;
  if ( ! mpTask->m_pDataStack->bPop(pPtrAccum) )
    THROW_INVALID_DSTACK("accum.max@"_Str);
  Accum* pAccum = _pAccum(pPtrAccum);
  
  mpTask->m_pDataStack->vPushInt(pAccum->m_iMax);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* accum.max! -- store max                                                    */
/*----------------------------------------------------------------------------*/
void Task::vAccumMaxStore(MP_Task mpTask)
{
  ENTRY("vAccumMaxStore"_Str);
  
  Object* pPtrAccum;
  if ( ! mpTask->m_pDataStack->bPop(pPtrAccum) )
    THROW_INVALID_DSTACK("accum.max!"_Str);
  Accum* pAccum = _pAccum(pPtrAccum);
  
  i8 iMax;
  if ( ! mpTask->m_pDataStack->bPop(iMax) )
    THROW_INVALID_DSTACK("accum.max!"_Str);
  
  pAccum->m_iMax = iMax;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* accum.min@ -- fetch min                                                    */
/*----------------------------------------------------------------------------*/
void Task::vAccumMinFetch(MP_Task mpTask)
{
  ENTRY("vAccumMinFetch"_Str);
  
  Object* pPtrAccum;
  if ( ! mpTask->m_pDataStack->bPop(pPtrAccum) )
    THROW_INVALID_DSTACK("accum.min@"_Str);
  Accum* pAccum = _pAccum(pPtrAccum);
  
  mpTask->m_pDataStack->vPushInt(pAccum->m_iMin);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* accum.min! -- store min                                                    */
/*----------------------------------------------------------------------------*/
void Task::vAccumMinStore(MP_Task mpTask)
{
  ENTRY("vAccumMinStore"_Str);
  
  Object* pPtrAccum;
  if ( ! mpTask->m_pDataStack->bPop(pPtrAccum) )
    THROW_INVALID_DSTACK("accum.min!"_Str);
  Accum* pAccum = _pAccum(pPtrAccum);
  
  i8 iMin;
  if ( ! mpTask->m_pDataStack->bPop(iMin) )
    THROW_INVALID_DSTACK("accum.min!"_Str);
  
  pAccum->m_iMin = iMin;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* accum.sum@ -- fetch sum                                                    */
/*----------------------------------------------------------------------------*/
void Task::vAccumSumFetch(MP_Task mpTask)
{
  ENTRY("vAccumSumFetch"_Str);
  
  Object* pPtrAccum;
  if ( ! mpTask->m_pDataStack->bPop(pPtrAccum) )
    THROW_INVALID_DSTACK("accum.sum@"_Str);
  Accum* pAccum = _pAccum(pPtrAccum);
  
  mpTask->m_pDataStack->vPushInt(pAccum->m_iSum);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* accum.sum! -- store sum                                                    */
/*----------------------------------------------------------------------------*/
void Task::vAccumSumStore(MP_Task mpTask)
{
  ENTRY("vAccumSumStore"_Str);
  
  Object* pPtrAccum;
  if ( ! mpTask->m_pDataStack->bPop(pPtrAccum) )
    THROW_INVALID_DSTACK("accum.sum!"_Str);
  Accum* pAccum = _pAccum(pPtrAccum);
  
  i8 iSum;
  if ( ! mpTask->m_pDataStack->bPop(iSum) )
    THROW_INVALID_DSTACK("accum.sum!"_Str);
  
  pAccum->m_iSum = iSum;
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
