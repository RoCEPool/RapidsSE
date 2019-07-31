/*============================================================================*/
/* StorageEngine.h -- StorageEngine namespace                                 */
/*============================================================================*/

#pragma once

#include "Prompt.h"

//-----------------------------------------------------------------------------

namespace StorageEngine
{
  void vCheckAllAssumptions(void);
  void vCheckAssumptions(void);
  void vSetSignalHandlers(void);
}

using namespace StorageEngine;

//-----------------------------------------------------------------------------

#include "Arguments.hpp"
#include "AttachMap.hpp"
#include "Constants.hpp"
#include "SizesDumps.hpp"
#include "Finalizers.hpp"
#include "Globals.hpp"

#include "Engine.hpp"

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
