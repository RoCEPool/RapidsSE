/*============================================================================*/
/* DBobjects.h - includes for DBobjects namespace                             */
/*============================================================================*/

#pragma once

#include "Dictionaries.h"

#include "Row.hpp"
#include "RowCursor.hpp"
#include "Index.hpp"
#include "Partition.hpp"
#include "Partitions.hpp"
#include "Ring.hpp"
#include "Table.hpp"

#include "User.hpp"

//-----------------------------------------------------------------------------
namespace DBobjects
{
  void vCheckAssumptions(void);
}

using namespace DBobjects;

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
