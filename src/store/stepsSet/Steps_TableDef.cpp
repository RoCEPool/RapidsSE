/*============================================================================*/
/* Steps_TableDef.cpp - TableDef table step group                             */
/*============================================================================*/

#include "BASE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "Steps.hpp"

#include "SkipRoots.hpp"
#include "EntBuffer.hpp"
#include "IndexTree.hpp"
#include "CrvBuffer.hpp"
#include "TablePtn.hpp"
#include "Table.hpp"
#include "Job.hpp"
#include "Node_I.hpp"
#include "LogicUnit.hpp"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_TableDef(void)
{
  vAddStep("TableDef[]"_Stz,         &Step::vTableDefGet);
  vAddStep("TableDef[].column@"_Stz, &Step::vTableDefColumnFetch);
  vAddStep("TableDef[].dump"_Stz,    &Step::vTableDefDump);
  vAddStep("TableDef[].set"_Stz,     &Step::vTableDefSet);
	vAddStep("TableDef[].tablePtnGet"_Stz, &Step::vTableDefTablePtnGet);
}

/*----------------------------------------------------------------------------*/
/* TableDef[].column -- fetch the column from the TableDef table              */
/*----------------------------------------------------------------------------*/
void Step::vTableDefColumnFetch(void)
{
  ENTRY();
  
  Table* pTable = _pTable(*m_pLogicUnit->m_xpTableDef);
  
  i8 iColumnNo = m_pDataStack->iPopInt(__func__);
  
  StreamDef* pStreamDef = pTable->pStreamDef();
  
  m_pDataStack->vPushPtr(pStreamDef->pGetColumn(iColumnNo));
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* TableDef[].dump                                                            */
/*----------------------------------------------------------------------------*/
void Step::vTableDefDump(void)
{
  ENTRY();
  
  vTrace("TableDump\n"_Stz);
  
  Table* pTable = _pTable(*m_pLogicUnit->m_xpTableDef);
  
  for (i8 i_TablePtn = 0; i_TablePtn < AM::PTNMAP::nPtnsPerNode(); i_TablePtn++) {
    TablePtn* pTablePtn = pTable->pTablePtn(i_TablePtn);
    vTrace("Partition %lld\n"_Stz, i_TablePtn);
    pTablePtn->vDump();
  }
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* TableDef[] -- get table                                                    */
/*----------------------------------------------------------------------------*/
void Step::vTableDefGet(void)
{
  ENTRY();
  
  m_pDataStack->vPushPtr(*m_pLogicUnit->m_xpTableDef);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* TableDef[].set -- set table                                                */
/*----------------------------------------------------------------------------*/
void Step::vTableDefSet(void)
{
  ENTRY();
  
  m_pLogicUnit->m_xpTableDef = _pTable(m_pDataStack->pPopPtr(__func__));
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* TableDef[].tablePtnGet -- fetch given table partition[tLglPtn]             */
/*----------------------------------------------------------------------------*/
void Step::vTableDefTablePtnGet(void)
{
	ENTRY();

	Table* pTable = _pTable(*m_pLogicUnit->m_xpTableDef);

	LglPtn tLglPtn;
	tLglPtn = m_pDataStack->iPopInt(__func__);
	i8 i_TablePtn = pTable->iPtnNoToTablePtn(tLglPtn);

	TablePtn* pTablePtn = pTable->pTablePtn(i_TablePtn);

	m_pDataStack->vPushPtr(pTablePtn);

	EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
