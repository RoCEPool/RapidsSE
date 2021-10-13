/*============================================================================*/
/* Steps_IndexDefRlu.cpp - index definition step group in Rlu                 */
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

#include "Node_I.hpp"
#include "Op.hpp"
#include "Job.hpp"
#include "LogicUnit.hpp"
#include "StageSpec.hpp"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_IndexDefRlu(void)
{
  vAddStep("indexDefRlu[]"_Stz,          &Step::vIndexDefRluGet);
  vAddStep("indexDefRlu[].addColumn"_Stz,&Step::vIndexDefRluAddColumn);
  vAddStep("indexDefRlu[].delete"_Stz,   &Step::vIndexDefRluDelete);
  vAddStep("indexDefRlu[].isNull"_Stz,   &Step::vIndexDefRluIsNull);
  vAddStep("indexDefRlu[].name!"_Stz,    &Step::vIndexDefRluNameStore);
  vAddStep("indexDefRlu[].new"_Stz,      &Step::vIndexDefRluNew);
  vAddStep("indexDefRlu[].set"_Stz,      &Step::vIndexDefRluSet);
}

/*----------------------------------------------------------------------------*/
/* indexDefRlu[].addColumn -- add a columnto the indexDef                        */
/*----------------------------------------------------------------------------*/
void Step::vIndexDefRluAddColumn(void)
{
  ENTRY();
  
  s32 sColumnNm;
  m_pDataStack->vPopStr(sColumnNm,__func__);
  
  eCT eColType = (eCT)m_pDataStack->iPopInt(__func__);
  
  IndexDef* pIndexDef = _pIndexDef(m_p_TaskLogicUnit->m_xRluCommon.m_pIndexDef);
  
  pIndexDef->vAddColumn(sColumnNm,eColType);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* indexDefRlu[].delete -- delete index definition                           */
/*----------------------------------------------------------------------------*/
void Step::vIndexDefRluDelete(void)
{
  ENTRY();

  IndexDef* pIndexDef = _pIndexDef(m_p_TaskLogicUnit->m_xRluCommon.m_pIndexDef);
  
  m_p_TaskLogicUnit->m_xpIndexDef = nullptr;
  
  delete pIndexDef;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* indexDefRlu[].get -- get indexDef                                         */
/*----------------------------------------------------------------------------*/
void Step::vIndexDefRluGet(void)
{
  ENTRY();

  IndexDef* pIndexDef = _pIndexDef(m_p_TaskLogicUnit->m_xRluCommon.m_pIndexDef);
  m_pDataStack->vPushPtr(pIndexDef);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* indexDefRlu[].isNull -- is indexDef null ?                                */
/*----------------------------------------------------------------------------*/
void Step::vIndexDefRluIsNull(void)
{
  ENTRY();
  
  m_pDataStack->vPushInt(m_p_TaskLogicUnit->m_xRluCommon.m_pIndexDef ? 0 : 1);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* indexDefRlu[].name! -- set name                                           */
/*----------------------------------------------------------------------------*/
void Step::vIndexDefRluNameStore(void)
{
  ENTRY();
  
  s32 sName;
  m_pDataStack->vPopStr(sName,__func__);

  IndexDef* pIndexDef = _pIndexDef(m_p_TaskLogicUnit->m_xRluCommon.m_pIndexDef);
  pIndexDef->vIndexNm(sName);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* indexDefRlu[].new -- new a stream definition out                              */
/*----------------------------------------------------------------------------*/
void Step::vIndexDefRluNew(void)
{
  ENTRY();

  m_p_TaskLogicUnit->m_xRluCommon.m_pIndexDef = new IndexDef;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* indexDefRlu[].set -- set indexDef                                         */
/*----------------------------------------------------------------------------*/
void Step::vIndexDefRluSet(void)
{
  ENTRY();

  m_p_TaskLogicUnit->m_xRluCommon.m_pIndexDef = m_pDataStack->pPopPtr(__func__);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
