/*============================================================================*/
/* StorageEngine.h -- StorageEngine namespace                                 */
/*============================================================================*/
#pragma once

#include "Steps.h"

//------------------------------------------------------------------------------

namespace StorageEngine
{
  void vCheckAllAssumptions(void);
  void vCheckAssumptions(void);
  void vSetSignalHandlers(void);
}

//------------------------------------------------------------------------------

#include "FocalObjects.hpp"
#include "AttachMap.hpp"
#include "Constants.hpp"
#include "EnvCheck.hpp"
#include "Globals.hpp"
#include "Portrait.hpp"

#include "Engine.hpp"

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
