/*============================================================================*/
/* Steps_StreamDef.cpp -- StreamDef step group                                */
/*============================================================================*/

#include "BASE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "Steps.hpp"

#include "ePX.h"

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
void Step::vAddSteps_StreamDefRht(void)
{
  vAddStep("StreamDefRht[]"_Stz,                 &Step::vStreamDefRhtGet);
  vAddStep("StreamDefRht[]\\clear"_Stz,          &Step::vStreamDefRhtGetAndClear);
  vAddStep("StreamDefRht[].delete"_Stz,          &Step::vStreamDefRhtDelete);
  vAddStep("StreamDefRht[].get"_Stz,             &Step::vStreamDefRhtGet);
  vAddStep("StreamDefRht[].isNull"_Stz,          &Step::vStreamDefRhtIsNull);
  vAddStep("StreamDefRht[].print"_Stz,           &Step::vStreamDefRhtPrint);
  vAddStep("StreamDefRht[].set"_Stz,             &Step::vStreamDefRhtSet);
}

/*----------------------------------------------------------------------------*/
/* StreamDefRht[].delete -- delete streamDef                                  */
/*----------------------------------------------------------------------------*/
void Step::vStreamDefRhtDelete(void)
{
  ENTRY();

  AM::vDeleteOperand(*m_pJob->pLogicUnit_job()->m_xpStreamDefRht);
  
  m_pJob->pLogicUnit_job()->m_xpStreamDefRht = nullptr;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* StreamDefRht[].get -- get streamDef                                        */
/*----------------------------------------------------------------------------*/
void Step::vStreamDefRhtGet(void)
{
  ENTRY();
  
  m_pDataStack->vPushPtr(*m_pJob->pLogicUnit_job()->m_xpStreamDefRht);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* StreamDefRht[].getAndClear -- get streamDef and set to nullptr             */
/*----------------------------------------------------------------------------*/
void Step::vStreamDefRhtGetAndClear(void)
{
  ENTRY();

  m_pDataStack->vPushPtr(*m_pJob->pLogicUnit_job()->m_xpStreamDefRht);

  m_pJob->pLogicUnit_job()->m_xpStreamDefRht = nullptr;

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* StreamDefRht[].isNull -- is streamDef null ?                               */
/*----------------------------------------------------------------------------*/
void Step::vStreamDefRhtIsNull(void)
{
  ENTRY();
  
  m_pDataStack->vPushInt(m_pJob->pLogicUnit_job()->m_xpStreamDefRht.bIsNull() ? 1 : 0);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* StreamDefRht[].print -- print streamDef                                    */
/*----------------------------------------------------------------------------*/
void Step::vStreamDefRhtPrint(void)
{
  ENTRY();
    
  StreamDef* pStreamDef = _pStreamDef(*m_pJob->pLogicUnit_job()->m_xpStreamDefRht);
  
  pStreamDef->vPrint();

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* StreamDefRht[].set -- set streamDef                                        */
/*----------------------------------------------------------------------------*/
void Step::vStreamDefRhtSet(void)
{
  ENTRY();
  
  m_pJob->pLogicUnit_job()->m_xpStreamDefRht = _pStreamDef(m_pDataStack->pPopPtr(__func__));
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
