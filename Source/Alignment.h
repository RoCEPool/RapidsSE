/*============================================================================*/
/* Alignment.h -- Alignment namespace                                         */
/*============================================================================*/
#pragma once

#include "Locking.h"

//------------------------------------------------------------------------------

namespace Alignment
{
  void vCheckAssumptions(void);
}

//------------------------------------------------------------------------------

#include "SingleWord.hpp"
#include "AlignedObject.hpp"

#include "Line.hpp"
#include "DoubleWord.hpp"
#include "QuadWord.hpp"
#include "CacheLine.hpp"
#include "SpaceUnit.hpp"

#include "WspConverge_t.hpp"
#include "Wsp_t.hpp"

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
