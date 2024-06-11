/*============================================================================*/
/* Steps_TableDefErrWarn.cpp - TableDefErrWarn table step group                     */
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
void Step::vAddSteps_TableDefErr(void)
{
  vAddStep("TableDefErr[]"_Stz,         &Step::vTableDefErrGet);
  vAddStep("TableDefErr[].column@"_Stz, &Step::vTableDefErrColumnFetch);
  vAddStep("TableDefErr[].dump"_Stz,    &Step::vTableDefErrDump);
  vAddStep("TableDefErr[].set"_Stz,     &Step::vTableDefErrSet);
}

/*----------------------------------------------------------------------------*/
/* TableDefErr[].column -- fetch the column from the TableDef table          */
/*----------------------------------------------------------------------------*/
void Step::vTableDefErrColumnFetch(void)
{
  ENTRY();
  
  Table* pTable = _pTable(*m_pLogicUnit->m_xpTableDefErr);
  
  i8 iColumnNo = m_oDataStack.iPopInt(__func__);
  
  StreamDef* pStreamDef = pTable->pStreamDef();
  
  m_oDataStack.vPushPtr(pStreamDef->pGetColumn(iColumnNo));
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* TableDefErr[].dump                                                        */
/*----------------------------------------------------------------------------*/
void Step::vTableDefErrDump(void)
{
  ENTRY();
  
  vTrace("TableDump\n"_Stz);
  
  Table* pTable = _pTable(*m_pLogicUnit->m_xpTableDefErr);
  
  for (i8 i_TablePtn = 0; i_TablePtn < AM::PTNMAP::nPtnsPerNode(); i_TablePtn++) {
    TablePtn* pTablePtn = pTable->pTablePtn(i_TablePtn);
    vTrace("Partition %lld\n"_Stz, i_TablePtn);
    pTablePtn->vDump();
  }
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* TableDefErr[] -- get table                                                */
/*----------------------------------------------------------------------------*/
void Step::vTableDefErrGet(void)
{
  ENTRY();
  
  m_oDataStack.vPushPtr(*m_pLogicUnit->m_xpTableDefErr);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* TableDefErr[].set -- set table                                            */
/*----------------------------------------------------------------------------*/
void Step::vTableDefErrSet(void)
{
  ENTRY();
  
  m_pLogicUnit->m_xpTableDefErr = _pTable(m_oDataStack.pPopPtr(__func__));
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
