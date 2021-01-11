/*============================================================================*/
/* Steps_MetaTable.cpp -- metatable step group                                */
/*============================================================================*/

#include "BASE.h"
#include "STORE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "StepTiming.hpp"


/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_MetaTable(void)
{
  vAddStep("MetaTable::listTables"_Stz,&Step::vMetaTableListTables);
}

/*----------------------------------------------------------------------------*/
/* memory.bytedump -- byte-oriented dump                                      */
/*----------------------------------------------------------------------------*/
void Step::vMetaTableListTables(void)
{
  ENTRY();
  
  vTrace("vMetaTableListTable\n"_Stz);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
