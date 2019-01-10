/*============================================================================*/
/* Steps_Accum.cpp -- Accumulator step group                                  */
/*============================================================================*/

#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Accum(void)
{
  vAddStep("accum::new"_Stz,   &Step::vAccumNew);

  vAddStep("accum.accumBuffer@"_Stz, &Step::vAccumAccumBuffer);  
  vAddStep("accum.count@"_Stz, &Step::vAccumCountFetch);
  vAddStep("accum.count!"_Stz, &Step::vAccumCountStore);
  vAddStep("accum.delete"_Stz, &Step::vAccumDelete);
  vAddStep("accum.group@"_Stz, &Step::vAccumGroupFetch);
  vAddStep("accum.group!"_Stz, &Step::vAccumGroupStore);
  vAddStep("accum.max@"_Stz,   &Step::vAccumMaxFetch);
  vAddStep("accum.max!"_Stz,   &Step::vAccumMaxStore);
  vAddStep("accum.min@"_Stz,   &Step::vAccumMinFetch);
  vAddStep("accum.min!"_Stz,   &Step::vAccumMinStore);
  vAddStep("accum.sum@"_Stz,   &Step::vAccumSumFetch);
  vAddStep("accum.sum!"_Stz,   &Step::vAccumSumStore);
}

/*----------------------------------------------------------------------------*/
/* accum.delete -- delete accumulator                                         */
/*----------------------------------------------------------------------------*/
void Step::vAccumDelete(MP_Task mpTask)
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
void Step::vAccumNew(MP_Task mpTask)
{
  ENTRY("vAccumNew"_Stz);
  
  Accum* pAccum = new Accum;
  
  mpTask->m_pDataStack->vPushPointer(pAccum);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* accum.accumBuffer-- accumulat a buffer of rows                             */
/*----------------------------------------------------------------------------*/
void Step::vAccumAccumBuffer(MP_Task mpTask)
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
void Step::vAccumCountFetch(MP_Task mpTask)
{
  ENTRY("vAccumCountFetch"_Stz);
  
  Object* pPtrAccum;
  if ( ! mpTask->m_pDataStack->bPop(pPtrAccum) )
    THROW_INVALID_DSTACK("accum.count@"_Stz);
  Accum* pAccum = _pAccum(pPtrAccum);
  
  mpTask->m_pDataStack->vPushI8(pAccum->m_iCount);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* accum.count! -- store count                                                */
/*----------------------------------------------------------------------------*/
void Step::vAccumCountStore(MP_Task mpTask)
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
void Step::vAccumGroupFetch(MP_Task mpTask)
{
  ENTRY("vAccumGroupFetch"_Stz);
  
  Object* pPtrAccum;
  if ( ! mpTask->m_pDataStack->bPop(pPtrAccum) )
    THROW_INVALID_DSTACK("accum.group@"_Stz);
  Accum* pAccum = _pAccum(pPtrAccum);
  
  mpTask->m_pDataStack->vPushI8(pAccum->m_iGroup);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* accum.group! -- store group                                                */
/*----------------------------------------------------------------------------*/
void Step::vAccumGroupStore(MP_Task mpTask)
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
void Step::vAccumMaxFetch(MP_Task mpTask)
{
  ENTRY("vAccumMaxFetch"_Stz);
  
  Object* pPtrAccum;
  if ( ! mpTask->m_pDataStack->bPop(pPtrAccum) )
    THROW_INVALID_DSTACK("accum.max@"_Stz);
  Accum* pAccum = _pAccum(pPtrAccum);
  
  mpTask->m_pDataStack->vPushI8(pAccum->m_iMax);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* accum.max! -- store max                                                    */
/*----------------------------------------------------------------------------*/
void Step::vAccumMaxStore(MP_Task mpTask)
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
void Step::vAccumMinFetch(MP_Task mpTask)
{
  ENTRY("vAccumMinFetch"_Stz);
  
  Object* pPtrAccum;
  if ( ! mpTask->m_pDataStack->bPop(pPtrAccum) )
    THROW_INVALID_DSTACK("accum.min@"_Stz);
  Accum* pAccum = _pAccum(pPtrAccum);
  
  mpTask->m_pDataStack->vPushI8(pAccum->m_iMin);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* accum.min! -- store min                                                    */
/*----------------------------------------------------------------------------*/
void Step::vAccumMinStore(MP_Task mpTask)
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
void Step::vAccumSumFetch(MP_Task mpTask)
{
  ENTRY("vAccumSumFetch"_Stz);
  
  Object* pPtrAccum;
  if ( ! mpTask->m_pDataStack->bPop(pPtrAccum) )
    THROW_INVALID_DSTACK("accum.sum@"_Stz);
  Accum* pAccum = _pAccum(pPtrAccum);
  
  mpTask->m_pDataStack->vPushI8(pAccum->m_iSum);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* accum.sum! -- store sum                                                    */
/*----------------------------------------------------------------------------*/
void Step::vAccumSumStore(MP_Task mpTask)
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
