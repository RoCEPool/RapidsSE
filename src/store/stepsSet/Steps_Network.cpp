/*============================================================================*/
/* Steps_Network.cpp - network step group                                     */
/*============================================================================*/

#include "BASE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "Steps.hpp"

#include "Grid.hpp"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Network(void)
{
  vAddStep("network::reset"_Stz,   &Step::vNetworkReset);
  vAddStep("network::show"_Stz,    &Step::vNetworkShow);
}

/*----------------------------------------------------------------------------*/
/* network::reset -- reset network stats                                      */
/*----------------------------------------------------------------------------*/
void Step::vNetworkReset(void)
{
  ENTRY();
  
  AM::pGrid()->vResetNetwork();

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* network::show -- show network in detail                                    */
/*----------------------------------------------------------------------------*/
void Step::vNetworkShow(void)
{
  ENTRY();
  
  AM::pGrid()->vShowNetwork();

  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
