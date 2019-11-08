/*============================================================================*/
/* Steps_TableDef.cpp - table definition step group                           */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_TableDef(void)
{
  vAddStep("tableDef::parse"_Stz, &Step::vTableDefParse);
}

/*----------------------------------------------------------------------------*/
/* tableDef::parse -- parse Create Table statement and produce a TableDef     */
/*----------------------------------------------------------------------------*/
void Step::vTableDefParse(void)
{
  ENTRY("vTableDefParse"_Stz);
  
  s48K sStmt;
  if (! m_pDataStack->bPopS48K(sStmt))
    THROW_DSTACK();

  vTrace("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n"_Stz);
  vTrace("Stmt: %s\n"_Stz,sStmt.szStz());
  vTrace("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n"_Stz);

  TableDef* pTableDef = new TableDef;
  
  delete pTableDef;
  pTableDef = nullptr;
  
  m_pDataStack->vPushPointer(pTableDef);

  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
