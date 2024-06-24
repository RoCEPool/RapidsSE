/*============================================================================*/
/* Steps_MdTable.cpp - getter functions for metadata tables                   */
/*============================================================================*/

#include "BASE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "Steps.hpp"
#include "AttachMap.hpp"
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

#include "MetaDataTables.hpp"

extern CmdLineArgs  gxCmdLineArgs;
extern AttachMap*   gpAttachMap;

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_MdTable(void)
{
  vAddStep("MdTable[].catalog"_Stz,        &Step::vMdTableCatalog);
  vAddStep("MdTable[].cluster"_Stz,         &Step::vMdTableCluster);
  vAddStep("MdTable[].columns"_Stz,         &Step::vMdTableColumns);
  vAddStep("MdTable[].indexColumns"_Stz,    &Step::vMdTableIndexColumns);
  vAddStep("MdTable[].indexes"_Stz,         &Step::vMdTableIndexes);
  vAddStep("MdTable[].nodes"_Stz,           &Step::vMdTableNodes);
  vAddStep("MdTable[].queries"_Stz,         &Step::vMdTableQueries);
  vAddStep("MdTable[].schema"_Stz,          &Step::vMdTableSchema);
  vAddStep("MdTable[].sessions"_Stz,        &Step::vMdTableSessions);
  vAddStep("MdTable[].tables"_Stz,          &Step::vMdTableTables);
}

/*----------------------------------------------------------------------------*/
/* MdTable[].catalog --                                                       */
/*----------------------------------------------------------------------------*/
void Step::vMdTableCatalog(void)
{
  ENTRY();
  m_pDataStack->vPushPtr(AM::pMetaDataTableCatalog());
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* MdTable[].cluster --                                                       */
/*----------------------------------------------------------------------------*/
void Step::vMdTableCluster(void)
{
  ENTRY();
  m_pDataStack->vPushPtr(AM::pMetaDataTableCluster());
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* MdTable[].columns --                                                       */
/*----------------------------------------------------------------------------*/
void Step::vMdTableColumns(void)
{
  ENTRY();
  m_pDataStack->vPushPtr(AM::pMetaDataTableColumns());
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* MdTable[].indexColumns --                                                       */
/*----------------------------------------------------------------------------*/
void Step::vMdTableIndexColumns(void)
{
  ENTRY();
  m_pDataStack->vPushPtr(AM::pMetaDataTableIndexColumns());
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* MdTable[]. --                                                       */
/*----------------------------------------------------------------------------*/
void Step::vMdTableIndexes(void)
{
  ENTRY();
  m_pDataStack->vPushPtr(AM::pMetaDataTableIndexes());
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* MdTable[].nodes --                                                       */
/*----------------------------------------------------------------------------*/
void Step::vMdTableNodes(void)
{
  ENTRY();
  m_pDataStack->vPushPtr(AM::pMetaDataTableNodes());
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* MdTable[].queries --                                                       */
/*----------------------------------------------------------------------------*/
void Step::vMdTableQueries(void)
{
  ENTRY();
  m_pDataStack->vPushPtr(AM::pMetaDataTableQueries());
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* MdTable[].schema --                                                       */
/*----------------------------------------------------------------------------*/
void Step::vMdTableSchema(void)
{
  ENTRY();
  m_pDataStack->vPushPtr(AM::pMetaDataTableSchema());
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* MdTable[].sessions --                                                      */
/*----------------------------------------------------------------------------*/
void Step::vMdTableSessions(void)
{
  ENTRY();
  m_pDataStack->vPushPtr(AM::pMetaDataTableSessions());
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* MdTable[].tables --                                                       */
/*----------------------------------------------------------------------------*/
void Step::vMdTableTables(void)
{
  ENTRY();
  m_pDataStack->vPushPtr(AM::pMetaDataTableOfTables());
  EXIT();
}

