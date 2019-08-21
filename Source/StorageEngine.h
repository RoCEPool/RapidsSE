/*============================================================================*/
/* StorageEngine.h -- StorageEngine namespace                                 */
/*============================================================================*/
#pragma once

#include "Prompt.h"

//------------------------------------------------------------------------------

namespace StorageEngine
{
  void vCheckAllAssumptions(void);
  void vCheckAssumptions(void);
  void vSetSignalHandlers(void);
}

//------------------------------------------------------------------------------

#include "Arguments.hpp"
#include "AttachMap.hpp"
#include "Constants.hpp"
#include "Globals.hpp"
#include "Portrait.hpp"

#include "Engine.hpp"

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
