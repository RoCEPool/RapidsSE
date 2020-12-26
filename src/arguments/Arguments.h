/*============================================================================*/
/* Arguments.h -- Arguments namespace                                         */
/*============================================================================*/
#pragma once

#include "Resources.h"

//------------------------------------------------------------------------------

namespace Arguments
{
  static const i8 C_DEF_LVL_LEVELS = 3;
  static const i8 C_DEF_LVL_RATIO  = 3; // this means 1/2^3 i.e. reducing by 8

  void vCheckAssumptions(void);
}

//------------------------------------------------------------------------------

#include "Args.hpp"
#include "Limits.hpp"
#include "Licence.hpp"


//------------------------------------------------------------------------------

#include "Format.hpp"

#include "DataChunk.hpp"
#include "RecordChunk.hpp"
#include "FileChunk.hpp"

#include "IngestChunk.hpp"
#include "IoChunk.hpp"
#include "IoCopy.hpp"
#include "LoadChunk.hpp"



#include "Operand.hpp"
#include "LinkedBuffer.hpp"
#include "Definition.hpp"
#include "OperandQueue.hpp"



/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
