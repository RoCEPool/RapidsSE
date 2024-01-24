/*============================================================================*/
/* Steps_MetaTable.cpp -- metatable step group                                */
/*============================================================================*/

#include "BASE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "Steps.hpp"
#include "Node_I.hpp"
#include "LogicUnit.hpp"
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
  
  //vTrace("vMetaTableListTable\n"_Stz);
  
  Pot* pPot = _pPot(m_pLogicUnit->m_xRluCommon.m_pPot);

  RowPix* pRowPix = new RowPix(m_pLogicUnit->m_xpStreamDefLft->pRowPix(), 0);
  pPot->vRowPix(pRowPix);
  
  bool bNewBuffer;
  Pds* pPdsRowBuffer = nullptr;
  
  
  Dic* pDic = m_oDicStack.pTop();
  Word* pWord = nullptr;
  while ( (pWord = pDic->pEachWord(pWord)) ) {
    Row* aRow = pPot->aNewDstRow(0,&pPdsRowBuffer,&bNewBuffer);
    UNUSED((aRow));
    bool bResult = pPdsRowBuffer->bDepositColVal(pWord->szWordNm());
    ASSERT(bResult);
    UNUSED(bResult);
  }

  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
