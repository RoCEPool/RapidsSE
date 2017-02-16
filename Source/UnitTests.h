/*============================================================================*/
/* UnitTests.h - unit tests                                                   */
/*============================================================================*/
#pragma once

#include "StorageEngine.h"
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

//-----------------------------------------------------------------------------
namespace UnitTests
{
  //extern RunTests* gpRunTests;
  void vCheckAssumptions(void);
}

using namespace UnitTests;

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
