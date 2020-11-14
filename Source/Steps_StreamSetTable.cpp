/*============================================================================*/
/* Steps_StreamSetTable.cpp - StreamSet table step group                      */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_StreamSetTable(void)
{
  vAddStep("StreamSetTable[]"_Stz,         &Step::vStreamSetTableGet);
  vAddStep("StreamSetTable[].column@"_Stz, &Step::vStreamSetTableColumnFetch);
  vAddStep("StreamSetTable[].dump"_Stz,    &Step::vStreamSetTableDump);
  vAddStep("StreamSetTable[].set"_Stz,     &Step::vStreamSetTableSet);
}

/*----------------------------------------------------------------------------*/
/* StreamSetTable[].column -- fetch the column from the StreamSet table       */
/*----------------------------------------------------------------------------*/
void Step::vStreamSetTableColumnFetch(void)
{
  ENTRY();
  
  Table* pTable = _pTable(*m_pLogicUnit->m_xpStreamSetTable);
  
  i8 iColumnNo = m_pDataStack->iPopInt(__func__);
  
  StreamDef* pStreamDef = pTable->pStreamDef();
  
  m_pDataStack->vPushPtr(pStreamDef->pGetColumn(iColumnNo));
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* StreamSetTable[].dump                                                      */
/*----------------------------------------------------------------------------*/
void Step::vStreamSetTableDump(void)
{
  ENTRY();
  
  vTrace("TableDump\n"_Stz);
  
  Table* pTable = _pTable(*m_pLogicUnit->m_xpStreamSetTable);
  
  for (i8 i_TablePtn = 0; i_TablePtn < AM::nPtnsPerTable(); i_TablePtn++) {
    TablePtn* pTablePtn = pTable->pTablePtn(i_TablePtn);
    vTrace("Partition %lld\n"_Stz, i_TablePtn);
    pTablePtn->vDump();
  }
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* StreamSetTable[] -- get table                                              */
/*----------------------------------------------------------------------------*/
void Step::vStreamSetTableGet(void)
{
  ENTRY();
  
  m_pDataStack->vPushPtr(*m_pLogicUnit->m_xpStreamSetTable);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* StreamSetTable[].set -- set table                                          */
/*----------------------------------------------------------------------------*/
void Step::vStreamSetTableSet(void)
{
  ENTRY();
  
  m_pLogicUnit->m_xpStreamSetTable = _pTable(m_pDataStack->pPopPtr(__func__));
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
