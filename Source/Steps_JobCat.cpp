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
  vAddStep("job_cat.cat"_Stz,     &Step::vJob_CatCat);
}

/*----------------------------------------------------------------------------*/
/* job_cat.cat -- cat data from pipe                                          */
/*----------------------------------------------------------------------------*/
void Step::vJob_CatCat(MP_Task mpTask)
{
  ENTRY("vJob_CatCat"_Stz);
  
  Object* pPtrJobCat;
  if ( ! mpTask->m_pDataStack->bPop(pPtrJobCat) )
    THROW_INVALID_DSTACK("job_cat.cat"_Stz);
  Job_Cat* pJob_Cat = _pJob_Cat(pPtrJobCat);
  
  pJob_Cat->vCat();
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
