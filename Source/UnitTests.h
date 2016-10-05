/*============================================================================*/
/* UnitTests.h - unit tests                                                   */
/*============================================================================*/
#pragma once

#include "StorageEngine.h"
#include "SePrompt.hpp"
#include "RunTests.hpp"
#include "Tests.hpp"
#include "TenantTests.hpp"
#include "CatalogTests.hpp"
#include "SchemaTests.hpp"
#include "TableTests.hpp"

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
