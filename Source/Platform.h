/*============================================================================*/
/* Platform.h - includes for Platform namespace                               */
/*============================================================================*/
#pragma once

#include "Base.h"

#include "SystemIncludes.h"
#include "System.hpp"
#include "PosixThread.hpp"

#if !( (defined(__APPLE__) || defined(__LINUX__)) )
  #include "Asm.h"
#endif

#include "Part.hpp"
#include "Static.hpp"
#include "Synch.hpp"
#include "BinaryNumbers.h"
#include "Addressing.hpp"
#include "ClassNumbers.hpp"
#include "Memory.hpp"
#include "String.hpp"

#include "ByteLiterals.hpp"
#include "Tabs.hpp"

//-----------------------------------------------------------------------------
namespace Platform
{
  void vCheckAssumptions(void);
}

using namespace Platform;

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
