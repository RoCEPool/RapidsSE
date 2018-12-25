/*============================================================================*/
/* Loader.h -- includes for Loader namespace                                  */
/*============================================================================*/
#pragma once

#include "DBobjects.h"

//------------------------------------------------------------------------------

namespace Loader
{
  void vCheckAssumptions(void);
}

using namespace Loader;

//------------------------------------------------------------------------------

#include "DataChunk.hpp"
#include "FileChunk.hpp"
#include "IoChunk.hpp"
#include "IoCopy.hpp"
#include "IngestChunk.hpp"
#include "LoadChunk.hpp"
#include "LoadMap.hpp"

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
