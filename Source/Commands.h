/*============================================================================*/
/* Commands.h -- Commands namespace                                           */
/*============================================================================*/
#pragma once

#include "Transactions.h"

//-----------------------------------------------------------------------------

namespace Commands
{
  void vCheckAssumptions(void);
  
  static const i8 C_TOKENS = 1024;
  
  typedef ArrayNL_t<CN_TOKENARY,Stz*,C_TOKENS> TokenAry;
}

using namespace Commands;

//-----------------------------------------------------------------------------

#include "Parser.hpp"
#include "Token.hpp"
#include "TokenVec.hpp"
#include "Tokeniser.hpp"
#include "Command.hpp"

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
