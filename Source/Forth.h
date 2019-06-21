/*============================================================================*/
/* Forth.h - includes for Forth namespace                                     */
/*============================================================================*/
#pragma once

#include "Commands.h"

//-----------------------------------------------------------------------------

namespace Tasks
{
  class Task;
  typedef MP<Task,CN_TASK> MP_Task;
}

namespace Forth
{
  typedef void (*StepFp)(Tasks::MP_Task);
  
  void vCheckAssumptions(void);
}

using namespace Forth;

typedef String_t<C_MAX_VCH> sVCH;

//-----------------------------------------------------------------------------

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
#include "ForthWord.hpp"

#include "Stack.hpp"

#include "Compiler.hpp"

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
