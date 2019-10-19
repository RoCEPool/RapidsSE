/*============================================================================*/
/* Steps_JobCat.cpp - job_cat step group                                      */
/*============================================================================*/

#include "Steps.h"

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
/* job_cat.begin -- begin cat                                                 */
/*----------------------------------------------------------------------------*/
void Step::vJob_CatBegin(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vJob_CatBegin"_Stz);
  
  Object* pPtrJobCat;
  if ( ! pForth->m_pDataStack->bPop(pPtrJobCat) )
    THROW_DSTACK();
  Job_Cat* pJob_Cat = _pJob_Cat(pPtrJobCat);
  
  bool bBegun = pJob_Cat->bBegin();
  
  pForth->m_pDataStack->vPushBool(bBegun);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job_cat.chunk -- cat data from pipe                                        */
/*----------------------------------------------------------------------------*/
void Step::vJob_CatChunk(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vJob_CatChunk"_Stz);
  
  Object* pPtrJobCat;
  if ( ! pForth->m_pDataStack->bPop(pPtrJobCat) )
    THROW_DSTACK();
  Job_Cat* pJob_Cat = _pJob_Cat(pPtrJobCat);
  
  bool bCallAgain = pJob_Cat->bChunk();
  
  pForth->m_pDataStack->vPushBool(bCallAgain);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job_cat.end -- end cat                                                     */
/*----------------------------------------------------------------------------*/
void Step::vJob_CatEnd(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vJob_CatEnd"_Stz);
  
  Object* pPtrJob_Cat;
  if ( ! pForth->m_pDataStack->bPop(pPtrJob_Cat) )
    THROW_DSTACK();
  Job_Cat* pJob_Cat = _pJob_Cat(pPtrJob_Cat);
  
  pJob_Cat->vEnd();

  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
