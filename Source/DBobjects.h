/*============================================================================*/
/* DBobjects.h - includes for DBobjects namespace                             */
/*============================================================================*/

#pragma once

#include "Dictionaries.h"

#include "Row.hpp"
#include "RowBuffer.hpp"
#include "RowStrand.hpp"

#include "Index.hpp"

#include "Partition.hpp"
#include "Partitions.hpp"

#include "Ring.hpp"

#include "Table.hpp"

#include "RowCursor.hpp"

#include "LoadBuffer.hpp"
#include "Loader.hpp"
#include "LoaderSub.hpp"

#include "User.hpp"

#include "RowWsp.hpp"

//------------------------------------------------------------------------------
namespace DBobjects
{
  void vCheckAssumptions(void);
}

using namespace DBobjects;

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
