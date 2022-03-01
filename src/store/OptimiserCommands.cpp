/*============================================================================*/
/* OptimiserCommands.cpp -- the optimiser commands                            */
/*============================================================================*/

#include "OptimiserCommands.hpp"

/*----------------------------------------------------------------------------*/
/* optimiser a                                                                */
/*----------------------------------------------------------------------------*/
bool STORE::bOptimser_a(Optimiser* pOptimiser)
{
  vTrace("bOptimser_a\n"_Stz);
  
  return true;
}

OptimiserFun __Optimiser_a
(
  "Optimiser_a"_Stz,  // name of the prompt function
  "a"_Stz,            // command first word
  nullptr,            // command second word
  bOptimser_a,        // function to call
  "description of a"_Stz
);


/*----------------------------------------------------------------------------*/
/* optimiser b                                                                */
/*----------------------------------------------------------------------------*/
bool STORE::bOptimser_b(Optimiser* pOptimiser)
{
  vTrace("bOptimser_b\n"_Stz);
  
  return true;
}

OptimiserFun __Optimiser_b
(
  "Optimiser_b"_Stz,  // name of the prompt function
  "b"_Stz,            // command first word
  nullptr,            // command second word
  bOptimser_b,        // function to call
  "description of b"_Stz
);

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
