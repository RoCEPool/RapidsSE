/*============================================================================*/
/* Steps_Optimiser.cpp -- Optimiser step group                                */
/*============================================================================*/

#include "BASE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "Steps.hpp"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Optimiser(void)
{
  vAddStep("Optimiser::delete"_Stz ,   &Step::vOptimiserDelete);
  vAddStep("Optimiser::new"_Stz,       &Step::vOptimiserNew);
  
  vAddStep("Optimiser::optimise"_Stz,  &Step::vOptimiserOptimise);
}


/*----------------------------------------------------------------------------*/
/* Optimiser::delete -- delete optimiser                                      */
/*----------------------------------------------------------------------------*/
void Step::vOptimiserDelete(void)
{
  ENTRY();
    

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Optimiser::new -- new optimiser                                            */
/*----------------------------------------------------------------------------*/
void Step::vOptimiserNew(void)
{
  ENTRY();
    

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Optimiser::optimise -- optimise a pipeline statement                       */
/*----------------------------------------------------------------------------*/
void Step::vOptimiserOptimise(void)
{
  ENTRY();
    

  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
