/*============================================================================*/
/* OptimiserCommands.cpp -- the optimiser commands                            */
/*============================================================================*/

#include "OptimiserCommands.hpp"

/*----------------------------------------------------------------------------*/
/* optimiser a                                                                */
/*----------------------------------------------------------------------------*/
bool STORE::bOptimser_a(Optimiser* pOptimiser, TokenVec* pTokenVec)
{
  vTrace("bOptimser_a\n"_Stz);
  
  s4K sCmd;
  pTokenVec->vToTknString(sCmd);
  
  vTrace("Cmd : %s\n"_Stz,sCmd.szStz());

  return true;
}

OptimiserFun __Optimiser_a
(
  "Optimiser_a"_Stz,  // name of the prompt function
  "a"_Stz,            // command first word
  nullptr,            // command second word
  bOptimser_a,        // function to call
  0,0,                // spare
  "description of a"_Stz
);


/*----------------------------------------------------------------------------*/
/* optimiser b                                                                */
/*----------------------------------------------------------------------------*/
bool STORE::bOptimser_b(Optimiser* pOptimiser, TokenVec* pTokenVec)
{
  vTrace("bOptimser_b\n"_Stz);
  
  s4K sCmd;
  pTokenVec->vToTknString(sCmd);
  
  vTrace("Cmd : %s\n"_Stz,sCmd.szStz());
  
  return true;
}

OptimiserFun __Optimiser_b
(
  "Optimiser_b"_Stz,  // name of the prompt function
  "b"_Stz,            // command first word
  nullptr,            // command second word
  bOptimser_b,        // function to call
  0,0,                // spare
  "description of b"_Stz
);

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
