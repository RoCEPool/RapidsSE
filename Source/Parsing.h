/*============================================================================*/
/* Parsing.h -- Parsing namespace                                             */
/*============================================================================*/
#pragma once

#include "Measurement.h"

//-----------------------------------------------------------------------------

namespace Parsing
{
  void vCheckAssumptions(void);
  
  static const i8 C_TOKENS = 1024;
  
  typedef ArrayNL_t<CN_TOKENARY,Stz*,C_TOKENS> TokenAry;
}

using namespace Parsing;

//-----------------------------------------------------------------------------

#include "Parser.hpp"
#include "Token.hpp"
#include "TokenVec.hpp"
#include "Tokeniser.hpp"
#include "CommandProcessor.hpp"

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
