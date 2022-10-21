/*============================================================================*/
/* OptimiserCommands.cpp -- the optimiser commands                            */
/*============================================================================*/

#include "OptimiserCommands.hpp"

#include "OptimiserCatalog.hpp"
#include "Dynamo.hpp"

extern OptimiserCatalog gxOptimiserCatalog;

/*----------------------------------------------------------------------------*/
/* disable                                                                    */
/*----------------------------------------------------------------------------*/
bool STORE::bOpt_disable(Optimiser* pOptimiser, TokenVec* pTknVec, eOA& reOptAct)
{
  vTrace("bOpt_weave\n"_Stz);

  UNUSED(pTknVec);

  reOptAct = OA_DISABLE_CUEING;

  return true;
}

OptimiserFun __Opt_disable_0
(
  "disable"_Stz,      // name of the opt function
  "disable"_Stz,      // command first word
  nullptr,            // command second word
  bOpt_disable,       // function to call
  0,0,                // spare
  "disable further cueing"_Stz,
  "usage???"_Stz
);

OptimiserFun __Opt_disable_1
(
  "disable"_Stz,      // name of the opt function
  "d"_Stz,            // command first word
  nullptr,            // command second word
  bOpt_disable,       // function to call
  0,0,                // spare
  "disable further cueing"_Stz,
  "usage???"_Stz
);

/*----------------------------------------------------------------------------*/
/* help                                                                       */
/*----------------------------------------------------------------------------*/
bool STORE::bOpt_help(Optimiser* pOptimiser, TokenVec* pTknVec, eOA& reOptAct)
{
  gxOptimiserCatalog.vHelp();
  
  reOptAct = OA_INPUT_AGAIN;
  
  return true;
}

OptimiserFun __Opt_help_0
(
  "help"_Stz,         // name of the opt function
  "?"_Stz,            // command first word
  nullptr,            // command second word
  bOpt_help,          // function to call
  0,0,                // spare
  "help"_Stz,
 "usage???"_Stz
);

OptimiserFun __Opt_help_1
(
  "help"_Stz,         // name of the opt function
  "help"_Stz,         // command first word
  nullptr,            // command second word
  bOpt_help,          // function to call
  0,0,                // spare
  "help"_Stz,
  "usage???"_Stz
);

/*----------------------------------------------------------------------------*/
/* weave                                                                      */
/*----------------------------------------------------------------------------*/
bool STORE::bOpt_weave(Optimiser* pOptimiser, TokenVec* pTknVec, eOA& reOptAct)
{
  vTrace("bOpt_weave\n"_Stz);

  UNUSED(pTknVec);

  vTrace("%s\n"_Stz,pOptimiser->szWeave());

  //reOptAct = OA_INPUT_AGAIN;

  return true;
}

OptimiserFun __Opt_weave_0
(
  "weave"_Stz,        // name of the opt function
  "weave"_Stz,        // command first word
  nullptr,            // command second word
  bOpt_weave,         // function to call
  0,0,                // spare
  "weave statement from cards"_Stz,
  "usage???"_Stz
);

OptimiserFun __Opt_weave_1
(
  "weave"_Stz,        // name of the opt function
  "w"_Stz,            // command first word
  nullptr,            // command second word
  bOpt_weave,         // function to call
  0,0,                // spare
  "weave statement from cards"_Stz,
  "usage???"_Stz
);


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
