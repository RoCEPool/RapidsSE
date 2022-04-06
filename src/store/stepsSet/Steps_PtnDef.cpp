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
  vAddStep("ShardDef[]"_Stz,                 &Step::vShardDefGet);
  vAddStep("ShardDef[]\\clear"_Stz,          &Step::vShardDefGetAndClear);
  vAddStep("ShardDef[].columnCount@"_Stz,    &Step::vShardDefColumnCountFetch);
  vAddStep("ShardDef[].column@"_Stz,         &Step::vShardDefColumnFetch);
  vAddStep("ShardDef[].get"_Stz,             &Step::vShardDefGet);
  vAddStep("ShardDef[].isNull"_Stz,          &Step::vShardDefIsNull);
  vAddStep("ShardDef[].print"_Stz,           &Step::vShardDefPrint);
  vAddStep("ShardDef[].set"_Stz,             &Step::vShardDefSet);
}

/*----------------------------------------------------------------------------*/
/* ShardDef[].column@ -- get column from shardDef                             */
/*----------------------------------------------------------------------------*/
void Step::vShardDefColumnFetch(void)
{
  ENTRY();
  
  PtnDef* pShardDef = _pPtnDef(*m_pLogicUnit->m_xpPtnDef);
                                      
  i8 iColumn = m_pDataStack->iPopInt(__func__);
  
  m_pDataStack->vPushPtr(pShardDef->pGetColumn(iColumn));
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* ShardDef[].columnCount@ -- get column count from shardDef                  */
/*----------------------------------------------------------------------------*/
void Step::vShardDefColumnCountFetch(void)
{
  ENTRY();
  
  PtnDef* pShardDef = _pPtnDef(*m_pLogicUnit->m_xpPtnDef);
  
  m_pDataStack->vPushInt(pShardDef->nColumns());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* ShardDef[].get -- get shardDef                                             */
/*----------------------------------------------------------------------------*/
void Step::vShardDefGet(void)
{
  ENTRY();
  
  m_pDataStack->vPushPtr(*m_pLogicUnit->m_xpPtnDef);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* ShardDef[].getAndClear -- get shardDef and set to nullptr                  */
/*----------------------------------------------------------------------------*/
void Step::vShardDefGetAndClear(void)
{
  ENTRY();

  m_pDataStack->vPushPtr(*m_pLogicUnit->m_xpPtnDef);

  m_pLogicUnit->m_xpPtnDef = nullptr;

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* ShardDef[].isNull -- is shardDef null ?                                    */
/*----------------------------------------------------------------------------*/
void Step::vShardDefIsNull(void)
{
  ENTRY();
  
  m_pDataStack->vPushInt(m_pLogicUnit->m_xpPtnDef.bIsNull() ? 1 : 0);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* ShardDef[].print -- print shardDef                                         */
/*----------------------------------------------------------------------------*/
void Step::vShardDefPrint(void)
{
  ENTRY();
    
  PtnDef* pShardDef = _pPtnDef(*m_pLogicUnit->m_xpPtnDef);
  
  pShardDef->vPrint();

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* ShardDef[].set -- set shardDef                                             */
/*----------------------------------------------------------------------------*/
void Step::vShardDefSet(void)
{
  ENTRY();
  
  m_pLogicUnit->m_xpPtnDef = _pPtnDef(m_pDataStack->pPopPtr(__func__));
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
