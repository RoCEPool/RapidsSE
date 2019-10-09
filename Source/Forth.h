/*============================================================================*/
/* Forth.h - includes for Forth namespace                                     */
/*============================================================================*/
#pragma once

#include "Commands.h"

//------------------------------------------------------------------------------

namespace Forth
{
  void vCheckAssumptions(void);
}

//------------------------------------------------------------------------------

#include "ForthValue.hpp"

#include "FipVal.hpp"
#include "IntVal.hpp"
#include "PtrVal.hpp"
#include "StrVal.hpp"

#include "Cell.hpp"
#include "Dic.hpp"
#include "DicPath.hpp"
#include "DicStack.hpp"
#include "GenVar.hpp"
#include "Local.hpp"
#include "LocalSet.hpp"
#include "Routine.hpp"
#include "Word.hpp"

#include "Stack.hpp"

#include "Compiler.hpp"

#include "ForthClump.hpp"

// -- -- -- -- -- -- -- --

#include "SP_Word.hpp"

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
