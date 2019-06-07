/*============================================================================*/
/* Chunks.h -- includes for Chunks namespace                                  */
/*============================================================================*/
#pragma once

#include "Structures.h"

//------------------------------------------------------------------------------

namespace Chunks
{
  void vCheckAssumptions(void);
}

using namespace Chunks;

//------------------------------------------------------------------------------

#include "DataChunk.hpp"
#include "RecordExtractor.hpp"
#include "FileChunk.hpp"

#include "IngestChunk.hpp"
#include "IoChunk.hpp"
#include "IoCopy.hpp"
#include "LoadChunk.hpp"
#include "LoadMap.hpp"

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
