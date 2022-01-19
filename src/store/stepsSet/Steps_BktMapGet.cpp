/*============================================================================*/
/* Steps_BktMapGet.cpp -- BktMapGet  step group                               */
/*============================================================================*/


#include "BASE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "Steps.hpp"
#include "BktMapGet.hpp"

#include "LogicUnit.hpp"


/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_BktMapGet(void)
{
  vAddStep("BktMap[]"_Stz,                   &Step::vBktMapGet);
  vAddStep("BktMap[]\\clear"_Stz,            &Step::vBktMapGetAndClear);
  vAddStep("BktMap[].delete"_Stz,            &Step::vBktMapDelete);
  vAddStep("BktMap[].getTableName"_Stz,      &Step::vBktMapGetGetTableName);
  vAddStep("BktMap[].isNotNull"_Stz,         &Step::vBktMapIsNotNull);
  vAddStep("BktMap[].isNull"_Stz,            &Step::vBktMapIsNull);
  vAddStep("BktMap[].set"_Stz,               &Step::vBktMapSet);
  //vAddStep("BktMap[].po"_Stz,           &Step::vBktMapPipeOut);
  vAddStep("BktMapGet::getBktMap"_Stz,       &Step::vBktMapGetGetBktMapdata);
}

/*----------------------------------------------------------------------------*/
/* BktMap[].delete -- planOperand delete                                        */
/*----------------------------------------------------------------------------*/
void Step::vBktMapDelete(void)
{
  ENTRY();

  AM::vDeleteOperand(*m_p_TaskLogicUnit->m_xpBktMapGet);

  m_p_TaskLogicUnit->m_xpBktMapGet = nullptr;

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* BktMap[] -- fetch planOperand                                              */
/*----------------------------------------------------------------------------*/
void Step::vBktMapGet(void)
{
  ENTRY();

  vTrace("C++ gets called: vBktMapGet\n"_Stz);

  m_pDataStack->vPushPtr(*m_p_TaskLogicUnit->m_xpBktMapGet);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* BktMap[].getAndClear -- fetch planOperand and set to nullptr                 */
/*----------------------------------------------------------------------------*/
void Step::vBktMapGetAndClear(void)
{
  ENTRY();

  m_pDataStack->vPushPtr(*m_p_TaskLogicUnit->m_xpBktMapGet);

  m_p_TaskLogicUnit->m_xpBktMapGet = nullptr;

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* BktMap[].isNotNull -- is planOperand not null ?                              */
/*----------------------------------------------------------------------------*/
void Step::vBktMapIsNotNull(void)
{
  ENTRY();

  m_pDataStack->vPushInt(m_p_TaskLogicUnit->m_xpBktMapGet.bIsNull() ? 0 : 1);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* BktMap[].isNull -- is planOperand null ?                                     */
/*----------------------------------------------------------------------------*/
void Step::vBktMapIsNull(void)
{
  ENTRY();

  m_pDataStack->vPushInt(m_p_TaskLogicUnit->m_xpBktMapGet.bIsNull() ? 1 : 0);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* BktMap[].set -- set planOperand                                              */
/*----------------------------------------------------------------------------*/
void Step::vBktMapSet(void)
{
  ENTRY();

  vTrace("C++ gets called: vBktMapSet\n"_Stz);
  BktMapGet* pBktMapGet = _pBktMapGet(m_pDataStack->pPopPtr(__func__));

  //ASSERT(! pBktMapGet || m_p_TaskLogicUnit->m_xpBktMapGet.bIsNull());

  m_p_TaskLogicUnit->m_xpBktMapGet = pBktMapGet;

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* BktMapGet::getBktMap -- start get parts of BktMap-object                   */
/*----------------------------------------------------------------------------*/
void Step::vBktMapGetGetBktMapdata(void)
{
  ENTRY();

  // ... code which initiates the BktMapdata transfer ...
  //vTrace("C++ gets called: vBktMapGetGetBktMapdata\n"_Stz);
  s32 sTableNm;
  m_pDataStack->vPopStr(sTableNm, __func__);

  BktMapGet *pBktMapGet = new BktMapGet(sTableNm);

  vTrace("C++ gets called: vBktMapGetGetBktMapdata %s %p\n"_Stz, sTableNm.azChar(), pBktMapGet);

  m_pDataStack->vPushPtr(pBktMapGet);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* BktMap[].getTableName -- get tableName from BktMapGet object                   */
/*----------------------------------------------------------------------------*/
void Step::vBktMapGetGetTableName(void)
{
  ENTRY();
  //vTrace("C++ gets called: vBktMapGetGetTableName\n"_Stz);

  BktMapGet* pBktMapGet = *m_p_TaskLogicUnit->m_xpBktMapGet;

  if (pBktMapGet) {
    s32 sTableNm = pBktMapGet->sTableName();
    vTrace("C++ gets called: vBktMapGetGetTableName  :%s\n"_Stz, sTableNm.azChar());
    m_pDataStack->vPushStz(sTableNm);
  } else {
    m_pDataStack->vPushStz("boltbl3"_Stz);
  }

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* BktMap[].po -- append marker onto next operator in pipeline      */
/*----------------------------------------------------------------------------*/
void Step::vBktMapPipeOut(void)
{
  ENTRY();

  vTrace("C++ gets called: BktMap[].po\n"_Stz);
  ASSERT(*m_p_TaskLogicUnit->m_xpBktMapGet);

  Op* pOp = *m_pOp;

  AM::OP::vPipeToNextOp(pOp,*m_p_TaskLogicUnit->m_xpBktMapGet,this);
  m_p_TaskLogicUnit->m_xpBktMapGet = nullptr;

  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/