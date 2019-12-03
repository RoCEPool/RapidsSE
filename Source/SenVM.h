/*============================================================================*/
/* Forth.h - includes for Forth namespace                                     */
/*============================================================================*/
#pragma once

#include "TokenStream.h"

//------------------------------------------------------------------------------

namespace SenVM
{
  void vCheckAssumptions(void);
}

//------------------------------------------------------------------------------

#include "AnyVal.hpp"

#include "FipVal.hpp"
#include "IntVal.hpp"
#include "PtrVal.hpp"
#include "StzVal.hpp"

#include "AllVal.hpp"

// -- -- -- -- -- -- -- --

#include "Cell.hpp"
#include "Dic.hpp"
#include "DicPath.hpp"
#include "DicStack.hpp"
#include "GenVar.hpp"
#include "Local.hpp"
#include "LocalChain.hpp"
#include "Routine.hpp"
#include "Stack.hpp"

#include "Compiler.hpp"
#include "Forth.hpp"
#include "Word.hpp"

// -- -- -- -- -- -- -- --

#include "SP_Word.hpp"

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
