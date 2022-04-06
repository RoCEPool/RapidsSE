/*============================================================================*/
/* Steps_PtnDef.cpp -- PtnDef step group                                      */
/*============================================================================*/

#include "BASE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "Steps.hpp"

#include "eYX.h"

#include "SkipRoots.hpp"
#include "EntBuffer.hpp"
#include "IndexTree.hpp"
#include "CrvBuffer.hpp"
#include "TablePtn.hpp"
#include "Table.hpp"
#include "Op.hpp"
#include "Job.hpp"
#include "Node_I.hpp"
#include "LogicUnit.hpp"
#include "StageSpec.hpp"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_PtnDef(void)
{
  vAddStep("PtnDef[]"_Stz,                 &Step::vPtnDefGet);
  vAddStep("PtnDef[]\\clear"_Stz,          &Step::vPtnDefGetAndClear);
  vAddStep("PtnDef[].columnCount@"_Stz,    &Step::vPtnDefColumnCountFetch);
  vAddStep("PtnDef[].column@"_Stz,         &Step::vPtnDefColumnFetch);
  vAddStep("PtnDef[].get"_Stz,             &Step::vPtnDefGet);
  vAddStep("PtnDef[].isNull"_Stz,          &Step::vPtnDefIsNull);
  vAddStep("PtnDef[].print"_Stz,           &Step::vPtnDefPrint);
  vAddStep("PtnDef[].set"_Stz,             &Step::vPtnDefSet);
}

/*----------------------------------------------------------------------------*/
/* PtnDef[].column@ -- get column from ptnDef                                 */
/*----------------------------------------------------------------------------*/
void Step::vPtnDefColumnFetch(void)
{
  ENTRY();
  
  PtnDef* pPtnDef = _pPtnDef(*m_pLogicUnit->m_xpPtnDef);
                                      
  i8 iColumn = m_pDataStack->iPopInt(__func__);
  
  m_pDataStack->vPushPtr(pPtnDef->pGetColumn(iColumn));
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* PtnDef[].columnCount@ -- get column count from ptnDef                      */
/*----------------------------------------------------------------------------*/
void Step::vPtnDefColumnCountFetch(void)
{
  ENTRY();
  
  PtnDef* pPtnDef = _pPtnDef(*m_pLogicUnit->m_xpPtnDef);
  
  m_pDataStack->vPushInt(pPtnDef->nColumns());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* PtnDef[].get -- get ptnDef                                                 */
/*----------------------------------------------------------------------------*/
void Step::vPtnDefGet(void)
{
  ENTRY();
  
  m_pDataStack->vPushPtr(*m_pLogicUnit->m_xpPtnDef);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* PtnDef[].getAndClear -- get ptnDef and set to nullptr                      */
/*----------------------------------------------------------------------------*/
void Step::vPtnDefGetAndClear(void)
{
  ENTRY();

  m_pDataStack->vPushPtr(*m_pLogicUnit->m_xpPtnDef);

  m_pLogicUnit->m_xpPtnDef = nullptr;

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* PtnDef[].isNull -- is ptnDef null ?                                        */
/*----------------------------------------------------------------------------*/
void Step::vPtnDefIsNull(void)
{
  ENTRY();
  
  m_pDataStack->vPushInt(m_pLogicUnit->m_xpPtnDef.bIsNull() ? 1 : 0);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* PtnDef[].print -- print ptnDef                                             */
/*----------------------------------------------------------------------------*/
void Step::vPtnDefPrint(void)
{
  ENTRY();
    
  PtnDef* pPtnDef = _pPtnDef(*m_pLogicUnit->m_xpPtnDef);
  
  pPtnDef->vPrint();

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* PtnDef[].set -- set ptnDef                                                 */
/*----------------------------------------------------------------------------*/
void Step::vPtnDefSet(void)
{
  ENTRY();
  
  m_pLogicUnit->m_xpPtnDef = _pPtnDef(m_pDataStack->pPopPtr(__func__));
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
