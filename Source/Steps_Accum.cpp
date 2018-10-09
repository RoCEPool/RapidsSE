/*============================================================================*/
/* Steps_Accum.cpp -- Accumulator step group                                  */
/*============================================================================*/

#include "StorageEngine.h"

extern AttachMap*      gpAttachMap;

// accum: //////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Accum(void)
{
  vAddStep("accum::new"_Stz,   &Task::vAccumNew);

  vAddStep("accum.accumBuffer@"_Stz, &Task::vAccumAccumBuffer);  
  vAddStep("accum.count@"_Stz, &Task::vAccumCountFetch);
  vAddStep("accum.count!"_Stz, &Task::vAccumCountStore);
  vAddStep("accum.delete"_Stz, &Task::vAccumDelete);
  vAddStep("accum.group@"_Stz, &Task::vAccumGroupFetch);
  vAddStep("accum.group!"_Stz, &Task::vAccumGroupStore);
  vAddStep("accum.max@"_Stz,   &Task::vAccumMaxFetch);
  vAddStep("accum.max!"_Stz,   &Task::vAccumMaxStore);
  vAddStep("accum.min@"_Stz,   &Task::vAccumMinFetch);
  vAddStep("accum.min!"_Stz,   &Task::vAccumMinStore);
  vAddStep("accum.sum@"_Stz,   &Task::vAccumSumFetch);
  vAddStep("accum.sum!"_Stz,   &Task::vAccumSumStore);
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
