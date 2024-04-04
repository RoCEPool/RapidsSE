/*============================================================================*/
/* Steps_Table.cpp - table step group                                         */
/*============================================================================*/

#include "BASE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "Steps.hpp"

#include "LglPtnMap.hpp"

#include "SkipRoots.hpp"
#include "EntBuffer.hpp"
#include "IndexTree.hpp"
#include "TablePtn.hpp"
#include "Table.hpp"
#include "Node_I.hpp"
#include "OpSpec.hpp"
#include "Op.hpp"
#include "Job.hpp"
#include "LogicUnit.hpp"

#include <new>

#include "Stream.hpp"
#include "Grid.hpp"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_MetaDataTable(void)
{
  vAddStep("MetaDataTable[].print"_Stz,    &Step::vMetaDataTablePrint);
}

/*----------------------------------------------------------------------------*/
/* MetaDataTable[].print -- delete an instance of table                       */
/*----------------------------------------------------------------------------*/
void Step::vMetaDataTablePrint(void)
{
  ENTRY();

  vTrace("GOT METADATASTEP\n"_Stz);

  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
