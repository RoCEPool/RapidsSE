/*============================================================================*/
/* Forth.h - includes for Forth namespace                                     */
/*============================================================================*/
#pragma once

#include "Commands.h"

//------------------------------------------------------------------------------

namespace SenVM
{
  void vCheckAssumptions(void);
}

//------------------------------------------------------------------------------

#include "SenVar.hpp"
#include "StkVal.hpp"

#include "FipVal.hpp"
#include "FipVar.hpp"
#include "IntVal.hpp"
#include "IntVar.hpp"
#include "PtrVal.hpp"
#include "PtrVar.hpp"
#include "StrVal.hpp"
#include "StrVar.hpp"

#include "Cell.hpp"
#include "Dic.hpp"
#include "DicPath.hpp"
#include "DicStack.hpp"
#include "GenVar.hpp"
#include "GenVar2.hpp"
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
