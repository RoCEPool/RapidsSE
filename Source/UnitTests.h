/*============================================================================*/
/* UnitTests.h -- UnitTests namespace                                         */
/*============================================================================*/
#pragma once

#include "StorageEngine.h"

//-----------------------------------------------------------------------------

namespace UnitTests
{
  void vCheckAssumptions(void);
}

using namespace UnitTests;

//-----------------------------------------------------------------------------

#include "SePrompt.hpp"
#include "RunTests.hpp"
#include "Tests.hpp"
#include "EndTests.hpp"
#include "StartTests.hpp"
#include "TenantTests.hpp"
#include "CatalogTests.hpp"
#include "SchemaTests.hpp"
#include "TableTests.hpp"
#include "LoadTests.hpp"
#include "InsertTests.hpp"

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
