/*============================================================================*/
/* Steps_Parcel.cpp -- discern steps                                          */
/*============================================================================*/

#include "BASE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "Steps.hpp"

#include "Discern.hpp"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Discern(void)
{
  vAddStep("discern::rdCsvChunk"_Stz,    &Step::vDiscernRdCsvChunk);
}

/*----------------------------------------------------------------------------*/
/* discern::rdCsvChunk -- discern action for op rdCsvChunk                    */
/*----------------------------------------------------------------------------*/
void Step::vDiscernRdCsvChunk(void)
{
  ENTRY();

  Pipeline* pPipeline = MY::pPipeline();
  
  i8 nSumQueued = Discern::nRdCsvChunk(pPipeline);
  
  //vTrace("nSumQueued %lld\n"_Stz,nSumQueued);
  
  if (nSumQueued < 100)
    m_pDataStack->vPushInt(true);
  else
    m_pDataStack->vPushInt(false);

  EXIT();
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
