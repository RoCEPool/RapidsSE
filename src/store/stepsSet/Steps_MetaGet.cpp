/*============================================================================*/
/* Steps_MetaGet.cpp -- metaGet step group                                    */
/*============================================================================*/


#include "BASE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "Steps.hpp"
#include "MetaGet.hpp"

#include "LogicUnit.hpp"


/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_MetaGet(void)
{
  vAddStep("Meta[]"_Stz,                   &Step::vMetaGet);
  vAddStep("Meta[]\\clear"_Stz,            &Step::vMetaGetAndClear);
  vAddStep("Meta[].delete"_Stz,            &Step::vMetaDelete);
  vAddStep("Meta[].getTableName"_Stz,      &Step::vMetaGetGetTableName);
  vAddStep("Meta[].isNotNull"_Stz,         &Step::vMetaIsNotNull);
  vAddStep("Meta[].isNull"_Stz,            &Step::vMetaIsNull);
  vAddStep("Meta[].set"_Stz,               &Step::vMetaSet);

  vAddStep("MetaGet::getMetaData"_Stz,     &Step::vMetaGetGetMetadata);
}

/*----------------------------------------------------------------------------*/
/* Meta[].delete -- planOperand delete                                        */
/*----------------------------------------------------------------------------*/
void Step::vMetaDelete(void)
{
  ENTRY();

  AM::vDeleteOperand(*m_p_TaskLogicUnit->m_xpMeta);

  m_p_TaskLogicUnit->m_xpMeta = nullptr;

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Meta[] -- fetch planOperand                                            */
/*----------------------------------------------------------------------------*/
void Step::vMetaGet(void)
{
  ENTRY();
  //vTrace("C++ gets called: vMetaGet\n"_Stz);
  m_pDataStack->vPushPtr(*m_p_TaskLogicUnit->m_xpMeta);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Meta[].getAndClear -- fetch planOperand and set to nullptr                 */
/*----------------------------------------------------------------------------*/
void Step::vMetaGetAndClear(void)
{
  ENTRY();

  m_pDataStack->vPushPtr(*m_p_TaskLogicUnit->m_xpMeta);

  m_p_TaskLogicUnit->m_xpMeta = nullptr;

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Meta[].isNotNull -- is planOperand not null ?                              */
/*----------------------------------------------------------------------------*/
void Step::vMetaIsNotNull(void)
{
  ENTRY();

  m_pDataStack->vPushInt(m_p_TaskLogicUnit->m_xpMeta.bIsNull() ? 0 : 1);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Meta[].isNull -- is planOperand null ?                                     */
/*----------------------------------------------------------------------------*/
void Step::vMetaIsNull(void)
{
  ENTRY();
  //vTrace("C++ gets called: vMetaIsNull\n"_Stz);
  m_pDataStack->vPushInt(m_p_TaskLogicUnit->m_xpMeta.bIsNull() ? 1 : 0);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Meta[].set -- set planOperand                                              */
/*----------------------------------------------------------------------------*/
void Step::vMetaSet(void)
{
  ENTRY();

  //vTrace("C++ gets called: vMetaSet\n"_Stz);
  MetaGet* pMeta = _pMetaGet(m_pDataStack->pPopPtr(__func__));

  ASSERT(! pMeta || m_p_TaskLogicUnit->m_xpMeta.bIsNull());

  m_p_TaskLogicUnit->m_xpMeta = pMeta;

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* MetaGet::getMetaData -- start get parts of meta-object                     */
/*----------------------------------------------------------------------------*/
void Step::vMetaGetGetMetadata(void)
{
  ENTRY();
  
  // ... code which initiates the metadata transfer ...
  //vTrace("C++ gets called: vMetaGetGetMetadata\n"_Stz);
  s32 sTableNm;
  m_pDataStack->vPopStr(sTableNm, __func__);

  MetaGet *pMetaGet = new MetaGet(sTableNm);


  m_pDataStack->vPushPtr(pMetaGet);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Meta[].getTableName -- get tableName from MetaGet object                   */
/*----------------------------------------------------------------------------*/
void Step::vMetaGetGetTableName(void)
{
  ENTRY();
  //vTrace("C++ gets called: vMetaGetGetTableName\n"_Stz);

  MetaGet* pMetaGet = *m_p_TaskLogicUnit->m_xpMeta;

  s32 sTableNm = pMetaGet->sTableName();

  m_pDataStack->vPushStz(sTableNm);

  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
