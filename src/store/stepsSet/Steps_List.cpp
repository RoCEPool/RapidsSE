/*============================================================================*/
/* Steps_List.cpp -- list step group                                          */
/*============================================================================*/

#include "BASE.h"
#include "STORE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "StepTiming.hpp"

#include "Node_I.hpp"
#include "LogicUnit.hpp"
#include "Response.hpp"
#include "Result.hpp"
#include "Job.hpp"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_List(void)
{
  vAddStep("List[]"_Stz,              &Step::vListGet);
  vAddStep("List[].addRowBuffer"_Stz, &Step::vListAddRowBuffer);
  vAddStep("List[].delete"_Stz,       &Step::vListDelete);
  vAddStep("List[].get"_Stz,          &Step::vListGet);
  vAddStep("List[].new"_Stz,          &Step::vListNew);
  vAddStep("List[].print"_Stz,        &Step::vListPrint);
}

/*----------------------------------------------------------------------------*/
/* List[].addRowBuffer-- add row buffer to list                               */
/*----------------------------------------------------------------------------*/
void Step::vListAddRowBuffer(void)
{
  ENTRY();
  
  vTrace("vListAddRowBuffer\n"_Stz);
  
  RowBuffer* pRowBuffer = _pRowBuffer( m_pDataStack->pPopPtr(__func__) );

  RowSingleList* pRowSingleList = * m_pJob->pLogicUnit_job()->m_xpList;

  for (Row* aRow = pRowBuffer->aFirstRow();
       aRow;
       aRow = pRowBuffer->aNextRow(aRow)) {

    RowSingle* pRowSingle =
      new RowSingle( * m_pJob->pLogicUnit_job()->m_xpStreamDef );

    pRowSingle->vCopyRow(aRow);
    pRowSingle->vPrintRow();

    pRowSingleList->vAppend(pRowSingle);
  }

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* List[].delete -- delete list                                               */
/*----------------------------------------------------------------------------*/
void Step::vListDelete(void)
{
  ENTRY();
  
  vTrace("vListDelete\n"_Stz);
  
  delete * m_pJob->pLogicUnit_job()->m_xpList;

  m_pJob->pLogicUnit_job()->m_xpList = nullptr;

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* List[].get-- get list                                                      */
/*----------------------------------------------------------------------------*/
void Step::vListGet(void)
{
  ENTRY();
  
  vTrace("vListGet\n"_Stz);
  
  m_pDataStack->vPushPtr(* m_pJob->pLogicUnit_job()->m_xpList);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* List[].new -- new list                                                     */
/*----------------------------------------------------------------------------*/
void Step::vListNew(void)
{
  ENTRY();
  
  vTrace("vListNew\n"_Stz);
  
  m_pJob->pLogicUnit_job()->m_xpList = new RowSingleList;

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* List[].print -- print list                                                 */
/*----------------------------------------------------------------------------*/
void Step::vListPrint(void)
{
  ENTRY();
  
  vTrace("vListPrint\n"_Stz);
  
  RowSingleList* pRowSingleList = * m_pJob->pLogicUnit_job()->m_xpList;


  ListExoIter_t<RowSingle>* pIter = pRowSingleList->pNewIter();
  
  while (pIter->bHasNext()) {
    RowSingle* pRowSingle = pIter->pNext();
    pRowSingle->vPrintRow();
  }
  
  delete pIter;
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
