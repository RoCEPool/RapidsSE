/*============================================================================*/
/* Steps_JobCat.cpp - job_cat step group                                      */
/*============================================================================*/

#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_JobCat(void)
{
  vAddStep("job_cat.begin"_Stz,     &Step::vJob_CatBegin);
  vAddStep("job_cat.chunk"_Stz,     &Step::vJob_CatChunk);
  vAddStep("job_cat.end"_Stz,       &Step::vJob_CatEnd);
}

/*----------------------------------------------------------------------------*/
/* job_cat.begin -- cat data from pipe                                        */
/*----------------------------------------------------------------------------*/
void Step::vJob_CatBegin(MP_Task mpTask)
{
  ENTRY("vJob_CatBegin"_Stz);
  
  Object* pPtrJobCat;
  if ( ! mpTask->m_pDataStack->bPop(pPtrJobCat) )
    THROW_INVALID_DSTACK("job_cat.begin"_Stz);
  Job_Cat* pJob_Cat = _pJob_Cat(pPtrJobCat);
  
  bool bBegun = pJob_Cat->bBegin();
  
  mpTask->m_pDataStack->vPushInt((i8)bBegun);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job_cat.chunk -- cat data from pipe                                        */
/*----------------------------------------------------------------------------*/
void Step::vJob_CatChunk(MP_Task mpTask)
{
  ENTRY("vJob_CatChunk"_Stz);
  
  Object* pPtrJobCat;
  if ( ! mpTask->m_pDataStack->bPop(pPtrJobCat) )
    THROW_INVALID_DSTACK("job_cat.chunk"_Stz);
  Job_Cat* pJob_Cat = _pJob_Cat(pPtrJobCat);
  
  bool bCallAgain = pJob_Cat->bChunk();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job_cat.end -- cat data from pipe                                        */
/*----------------------------------------------------------------------------*/
void Step::vJob_CatEnd(MP_Task mpTask)
{
  ENTRY("vJob_CatEnd"_Stz);
  
  Object* pPtrJobCat;
  if ( ! mpTask->m_pDataStack->bPop(pPtrJobCat) )
    THROW_INVALID_DSTACK("job_cat.end"_Stz);
  Job_Cat* pJob_Cat = _pJob_Cat(pPtrJobCat);
  
  pJob_Cat->vEnd();
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
