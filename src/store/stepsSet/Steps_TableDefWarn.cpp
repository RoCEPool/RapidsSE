/*============================================================================*/
/* Steps_TableDefWarnWarn.cpp - TableDefWarnWarn table step group                     */
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
void Step::vAddSteps_TableDefWarn(void)
{
  vAddStep("TableDefWarn[]"_Stz,         &Step::vTableDefWarnGet);
  vAddStep("TableDefWarn[].column@"_Stz, &Step::vTableDefWarnColumnFetch);
  vAddStep("TableDefWarn[].dump"_Stz,    &Step::vTableDefWarnDump);
  vAddStep("TableDefWarn[].set"_Stz,     &Step::vTableDefWarnSet);
}

/*----------------------------------------------------------------------------*/
/* TableDefWarn[].column -- fetch the column from the StreamSet table          */
/*----------------------------------------------------------------------------*/
void Step::vTableDefWarnColumnFetch(void)
{
  ENTRY();
  
  Table* pTable = _pTable(*m_pLogicUnit->m_xpTableDefWarn);
  
  i8 iColumnNo = m_pDataStack->iPopInt(__func__);
  
  StreamDef* pStreamDef = pTable->pStreamDef();
  
  m_pDataStack->vPushPtr(pStreamDef->pGetColumn(iColumnNo));
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* TableDefWarn[].dump                                                        */
/*----------------------------------------------------------------------------*/
void Step::vTableDefWarnDump(void)
{
  ENTRY();
  
  vTrace("TableDump\n"_Stz);
  
  Table* pTable = _pTable(*m_pLogicUnit->m_xpTableDefWarn);
  
  for (i8 i_TablePtn = 0; i_TablePtn < AM::PTNMAP::nPtnsPerNode(); i_TablePtn++) {
    TablePtn* pTablePtn = pTable->pTablePtn(i_TablePtn);
    vTrace("Partition %lld\n"_Stz, i_TablePtn);
    pTablePtn->vDump();
  }
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* TableDefWarn[] -- get table                                                */
/*----------------------------------------------------------------------------*/
void Step::vTableDefWarnGet(void)
{
  ENTRY();
  
  m_pDataStack->vPushPtr(*m_pLogicUnit->m_xpTableDefWarn);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* TableDefWarn[].set -- set table                                            */
/*----------------------------------------------------------------------------*/
void Step::vTableDefWarnSet(void)
{
  ENTRY();
  
  m_pLogicUnit->m_xpTableDefWarn = _pTable(m_pDataStack->pPopPtr(__func__));
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
