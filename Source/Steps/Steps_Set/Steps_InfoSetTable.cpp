/*============================================================================*/
/* Steps_InfoSetTable.cpp - infoSet table step group                          */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_InfoSetTable(void)
{
  vAddStep("infoSetTable[]"_Stz,         &Step::vInfoSetTableGet);
  vAddStep("infoSetTable[].column@"_Stz, &Step::vInfoSetTableColumnFetch);
  vAddStep("infoSetTable[].dump"_Stz,    &Step::vInfoSetTableDump);
  vAddStep("infoSetTable[].set"_Stz,     &Step::vInfoSetTableSet);
}

/*----------------------------------------------------------------------------*/
/* infoSetTable[].column -- fetch the column from the infoSet table           */
/*----------------------------------------------------------------------------*/
void Step::vInfoSetTableColumnFetch(void)
{
  ENTRY();
  
  Table* pTable = _pTable(*m_pLogicUnit->m_xpInfoSetTable);
  
  i8 iColumnNo = m_pDataStack->iPopInt(__func__);
  
  StreamDef* pStreamDef = pTable->pStreamDef();
  
  m_pDataStack->vPushPtr(pStreamDef->pGetColumn(iColumnNo));
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* infoSetTable[].dump                                                        */
/*----------------------------------------------------------------------------*/
void Step::vInfoSetTableDump(void)
{
  ENTRY();
  
  vTrace("TableDump\n"_Stz);
  
  Table* pTable = _pTable(*m_pLogicUnit->m_xpInfoSetTable);
  
  for (i8 i_TablePtn = 0; i_TablePtn < AM::nPtnsPerTable(); i_TablePtn++) {
    TablePtn* pTablePtn = pTable->pTablePtn(i_TablePtn);
    vTrace("Partition %lld\n"_Stz, i_TablePtn);
    pTablePtn->vDump();
  }
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* infoSetTable[] -- get table                                                */
/*----------------------------------------------------------------------------*/
void Step::vInfoSetTableGet(void)
{
  ENTRY();
  
  m_pDataStack->vPushPtr(*m_pLogicUnit->m_xpInfoSetTable);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* infoSetTable[].set -- set table                                            */
/*----------------------------------------------------------------------------*/
void Step::vInfoSetTableSet(void)
{
  ENTRY();
  
  m_pLogicUnit->m_xpInfoSetTable = _pTable(m_pDataStack->pPopPtr(__func__));
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
