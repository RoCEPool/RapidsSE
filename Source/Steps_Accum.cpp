/*============================================================================*/
/* Steps_Accum.cpp -- Accumulator step group                                  */
/*============================================================================*/

#include "StorageEngine.h"

extern StepArray*      gpStepArray;
extern ThreadList*     gpThreadList;
extern TaskList*       gpTaskList;

extern NP_Task         gnpBootTask;
extern Lock            gxDdlLock;
extern Lock            gxDicLock;
extern SEitfList*      gpSEitfList;

// accum: //////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_Accum(void)
{
  vAddPrimitive("accum.delete"_Stz, &vAccumDelete, false);
  vAddPrimitive("accum::new"_Stz,   &vAccumNew,    false);

  vAddPrimitive("accum.accumBuffer@"_Stz, &vAccumAccumBuffer, false);
  
  vAddPrimitive("accum.count@"_Stz, &vAccumCountFetch, false);
  vAddPrimitive("accum.count!"_Stz, &vAccumCountStore, false);
  vAddPrimitive("accum.group@"_Stz, &vAccumGroupFetch, false);
  vAddPrimitive("accum.group!"_Stz, &vAccumGroupStore, false);
  vAddPrimitive("accum.max@"_Stz,   &vAccumMaxFetch,   false);
  vAddPrimitive("accum.max!"_Stz,   &vAccumMaxStore,   false);
  vAddPrimitive("accum.min@"_Stz,   &vAccumMinFetch,   false);
  vAddPrimitive("accum.min!"_Stz,   &vAccumMinStore,   false);
  vAddPrimitive("accum.sum@"_Stz,   &vAccumSumFetch,   false);
  vAddPrimitive("accum.sum!"_Stz,   &vAccumSumStore,   false);
}

/*----------------------------------------------------------------------------*/
/* accum.delete -- delete accumulator                                         */
/*----------------------------------------------------------------------------*/
void Task::vAccumDelete(MP_Task mpTask)
{
  ENTRY("vAccumDelete"_Stz);
  
  Object* pPtrAccum;
  if ( ! mpTask->m_pDataStack->bPop(pPtrAccum) )
    THROW_INVALID_DSTACK("accum.delete"_Stz);
  Accum* pAccum = _pAccum(pPtrAccum);
  
  delete pAccum;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* accum::new -- accumulator new                                              */
/*----------------------------------------------------------------------------*/
void Task::vAccumNew(MP_Task mpTask)
{
  ENTRY("vAccumNew"_Stz);
  
  Accum* pAccum = new Accum;
  
  mpTask->m_pDataStack->vPushObject(pAccum);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* accum.accumBuffer-- accumulat a buffer of rows                             */
/*----------------------------------------------------------------------------*/
void Task::vAccumAccumBuffer(MP_Task mpTask)
{
  ENTRY("vAccumAccumBuffer"_Stz);
  
  Object* pPtrAccum;
  if ( ! mpTask->m_pDataStack->bPop(pPtrAccum) )
    THROW_INVALID_DSTACK("accum.accumBuffer"_Stz);
  Accum* pAccum = _pAccum(pPtrAccum);
  
  // tbd...
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* accum.count@ -- fetch count                                                */
/*----------------------------------------------------------------------------*/
void Task::vAccumCountFetch(MP_Task mpTask)
{
  ENTRY("vAccumCountFetch"_Stz);
  
  Object* pPtrAccum;
  if ( ! mpTask->m_pDataStack->bPop(pPtrAccum) )
    THROW_INVALID_DSTACK("accum.count@"_Stz);
  Accum* pAccum = _pAccum(pPtrAccum);
  
  mpTask->m_pDataStack->vPushInt(pAccum->m_iCount);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* accum.count! -- store count                                                */
/*----------------------------------------------------------------------------*/
void Task::vAccumCountStore(MP_Task mpTask)
{
  ENTRY("vAccumCountStore"_Stz);
  
  Object* pPtrAccum;
  if ( ! mpTask->m_pDataStack->bPop(pPtrAccum) )
    THROW_INVALID_DSTACK("accum.count!"_Stz);
  Accum* pAccum = _pAccum(pPtrAccum);

  i8 iCount;
  if ( ! mpTask->m_pDataStack->bPop(iCount) )
    THROW_INVALID_DSTACK("accum.count!"_Stz);

  pAccum->m_iCount = iCount;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* accum.group@ -- fetch group                                                */
/*----------------------------------------------------------------------------*/
void Task::vAccumGroupFetch(MP_Task mpTask)
{
  ENTRY("vAccumGroupFetch"_Stz);
  
  Object* pPtrAccum;
  if ( ! mpTask->m_pDataStack->bPop(pPtrAccum) )
    THROW_INVALID_DSTACK("accum.group@"_Stz);
  Accum* pAccum = _pAccum(pPtrAccum);
  
  mpTask->m_pDataStack->vPushInt(pAccum->m_iGroup);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* accum.group! -- store group                                                */
/*----------------------------------------------------------------------------*/
void Task::vAccumGroupStore(MP_Task mpTask)
{
  ENTRY("vAccumGroupStore"_Stz);
  
  Object* pPtrAccum;
  if ( ! mpTask->m_pDataStack->bPop(pPtrAccum) )
    THROW_INVALID_DSTACK("accum.group!"_Stz);
  Accum* pAccum = _pAccum(pPtrAccum);
  
  i8 iGroup;
  if ( ! mpTask->m_pDataStack->bPop(iGroup) )
    THROW_INVALID_DSTACK("accum.group!"_Stz);
  
  pAccum->m_iGroup = iGroup;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* accum.max@ -- fetch max                                                    */
/*----------------------------------------------------------------------------*/
void Task::vAccumMaxFetch(MP_Task mpTask)
{
  ENTRY("vAccumMaxFetch"_Stz);
  
  Object* pPtrAccum;
  if ( ! mpTask->m_pDataStack->bPop(pPtrAccum) )
    THROW_INVALID_DSTACK("accum.max@"_Stz);
  Accum* pAccum = _pAccum(pPtrAccum);
  
  mpTask->m_pDataStack->vPushInt(pAccum->m_iMax);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* accum.max! -- store max                                                    */
/*----------------------------------------------------------------------------*/
void Task::vAccumMaxStore(MP_Task mpTask)
{
  ENTRY("vAccumMaxStore"_Stz);
  
  Object* pPtrAccum;
  if ( ! mpTask->m_pDataStack->bPop(pPtrAccum) )
    THROW_INVALID_DSTACK("accum.max!"_Stz);
  Accum* pAccum = _pAccum(pPtrAccum);
  
  i8 iMax;
  if ( ! mpTask->m_pDataStack->bPop(iMax) )
    THROW_INVALID_DSTACK("accum.max!"_Stz);
  
  pAccum->m_iMax = iMax;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* accum.min@ -- fetch min                                                    */
/*----------------------------------------------------------------------------*/
void Task::vAccumMinFetch(MP_Task mpTask)
{
  ENTRY("vAccumMinFetch"_Stz);
  
  Object* pPtrAccum;
  if ( ! mpTask->m_pDataStack->bPop(pPtrAccum) )
    THROW_INVALID_DSTACK("accum.min@"_Stz);
  Accum* pAccum = _pAccum(pPtrAccum);
  
  mpTask->m_pDataStack->vPushInt(pAccum->m_iMin);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* accum.min! -- store min                                                    */
/*----------------------------------------------------------------------------*/
void Task::vAccumMinStore(MP_Task mpTask)
{
  ENTRY("vAccumMinStore"_Stz);
  
  Object* pPtrAccum;
  if ( ! mpTask->m_pDataStack->bPop(pPtrAccum) )
    THROW_INVALID_DSTACK("accum.min!"_Stz);
  Accum* pAccum = _pAccum(pPtrAccum);
  
  i8 iMin;
  if ( ! mpTask->m_pDataStack->bPop(iMin) )
    THROW_INVALID_DSTACK("accum.min!"_Stz);
  
  pAccum->m_iMin = iMin;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* accum.sum@ -- fetch sum                                                    */
/*----------------------------------------------------------------------------*/
void Task::vAccumSumFetch(MP_Task mpTask)
{
  ENTRY("vAccumSumFetch"_Stz);
  
  Object* pPtrAccum;
  if ( ! mpTask->m_pDataStack->bPop(pPtrAccum) )
    THROW_INVALID_DSTACK("accum.sum@"_Stz);
  Accum* pAccum = _pAccum(pPtrAccum);
  
  mpTask->m_pDataStack->vPushInt(pAccum->m_iSum);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* accum.sum! -- store sum                                                    */
/*----------------------------------------------------------------------------*/
void Task::vAccumSumStore(MP_Task mpTask)
{
  ENTRY("vAccumSumStore"_Stz);
  
  Object* pPtrAccum;
  if ( ! mpTask->m_pDataStack->bPop(pPtrAccum) )
    THROW_INVALID_DSTACK("accum.sum!"_Stz);
  Accum* pAccum = _pAccum(pPtrAccum);
  
  i8 iSum;
  if ( ! mpTask->m_pDataStack->bPop(iSum) )
    THROW_INVALID_DSTACK("accum.sum!"_Stz);
  
  pAccum->m_iSum = iSum;
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
