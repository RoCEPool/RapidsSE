/*============================================================================*/
/* OptimiserCommands.cpp -- the optimiser commands                            */
/*============================================================================*/

#include "OptimiserCommands.hpp"

#include "OptimiserCatalog.hpp"
#include "Dynamo.hpp"

extern OptimiserCatalog gxOptimiserCatalog;

/*----------------------------------------------------------------------------*/
/* help                                                                       */
/*----------------------------------------------------------------------------*/
bool STORE::bOpt_help(Optimiser* pOptimiser, TokenVec* pTknVec, eOA& reOptAct)
{
  gxOptimiserCatalog.vHelp();
  
  reOptAct = OA_INPUT;
  
  return true;
}

OptimiserFun __Opt_help_0
(
  "help"_Stz,         // name of the prompt function
  "?"_Stz,            // command first word
  nullptr,            // command second word
  bOpt_help,          // function to call
  0,0,                // spare
  "help"_Stz,
 "usage???"_Stz
);

OptimiserFun __Opt_help_1
(
  "help"_Stz,         // name of the prompt function
  "help"_Stz,         // command first word
  nullptr,            // command second word
  bOpt_help,          // function to call
  0,0,                // spare
  "help"_Stz,
  "usage???"_Stz
);

/*----------------------------------------------------------------------------*/
/* drafter: weave                                                             */
/*----------------------------------------------------------------------------*/
bool STORE::bOpt_weave(Optimiser* pOptimiser, TokenVec* pTknVec, eOA& reOptAct)
{
  vTrace("bOpt_weave\n"_Stz);
  
  Drafter* pDrafter = pOptimiser->pDrafter();
  bool bResult = pDrafter->bWeave(pTknVec,reOptAct);
  return bResult;
}

OptimiserFun __Opt_weave
(
  "weave"_Stz,        // name of the prompt function
  "weave"_Stz,        // command first word
  nullptr,            // command second word
  bOpt_weave,           // function to call
  0,0,                // spare
  "weave statement from cards"_Stz,
  "usage???"_Stz
);

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
