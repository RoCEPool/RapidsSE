/*============================================================================*/
/* Steps_MetaTable.cpp -- metatable step group                                */
/*============================================================================*/

#include "BASE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "StepTiming.hpp"

#include "RowBuffer.hpp"

#include "Node_I.hpp"
#include "LogicUnit.hpp"
#include "Response.hpp"
#include "Result.hpp"
#include "Job.hpp"

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
  
  Pot* pPot = *m_pJob->pLogicUnit_job()->m_xpPot;
  
  pPot->vRowPix(m_pJob->pLogicUnit_job()->m_xpStreamDef->aRowPix());
  
  bool bNewBuffer;
  RowBuffer* pRowBuffer = nullptr;
  
  
  Dic* pDic = m_oDicStack.pTop();
  Word* pWord = nullptr;
  while ( (pWord = pDic->pEachWord(pWord)) ) {
    Row* aRow = pPot->aNewDstRow(0,&pRowBuffer,&bNewBuffer);
    pRowBuffer->vDepositColVal(0,pWord->szWordNm());
  }

  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
