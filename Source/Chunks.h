/*============================================================================*/
/* Loader.h -- includes for Loader namespace                                  */
/*============================================================================*/
#pragma once

#include "DBobjects.h"

//------------------------------------------------------------------------------

namespace Chunks
{
  void vCheckAssumptions(void);
}

using namespace Chunks;

//------------------------------------------------------------------------------

#include "DataChunk.hpp"
#include "FileChunk.hpp"
#include "RecordExtractor.hpp"

#include "IoChunk.hpp"
#include "IoCopy.hpp"
#include "LoadChunk.hpp"
#include "LoadMap.hpp"
#include "RecordIngestor.hpp"

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
