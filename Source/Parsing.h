/*============================================================================*/
/* Parsing.h -- Parsing namespace                                             */
/*============================================================================*/
#pragma once

#include "Structures.h"

//-----------------------------------------------------------------------------

namespace Parsing
{
  void vCheckAssumptions(void);
  
  static const i8 C_TOKENS = 256;
  
  typedef Array<CN_TOKENARY,Str*,C_TOKENS> TokenAry;
}

using namespace Parsing;

//-----------------------------------------------------------------------------

#include "Parser.hpp"
#include "Token.hpp"
#include "TokenVec.hpp"
#include "Tokeniser.hpp"

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
