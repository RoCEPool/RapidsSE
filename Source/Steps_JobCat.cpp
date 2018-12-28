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
  vAddStep("job_cat.chunk"_Stz,     &Step::vJob_CatChunk);
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

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
