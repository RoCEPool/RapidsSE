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

#include "eTT.h"
#include "SkipRoots.hpp"
#include "EntBuffer.hpp"
#include "IndexTree.hpp"
#include "TablePtn.hpp"
#include "Table.hpp"
#include "Parcel.hpp"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_MetaGet(void)
{
  vAddStep("MetaGet::new"_Stz,          &Step::vMetaGetNew);
  
  vAddStep("MetaGet[]"_Stz,             &Step::vMetaGetGet);
  vAddStep("MetaGet[]\\clear"_Stz,      &Step::vMetaGetGetAndClear);
  vAddStep("MetaGet[].delete"_Stz,      &Step::vMetaGetDelete);
  vAddStep("MetaGet[].getParts"_Stz,    &Step::vMetaGetGetParts);
  vAddStep("MetaGet[].isNotNull"_Stz,   &Step::vMetaGetIsNotNull);
  vAddStep("MetaGet[].isNull"_Stz,      &Step::vMetaGetIsNull);
  vAddStep("MetaGet[].putParts"_Stz,    &Step::vMetaGetPutParts);
  vAddStep("MetaGet[].set"_Stz,         &Step::vMetaGetSet);
  vAddStep("MetaGet[].tableName@"_Stz,  &Step::vMetaGetTableNameFetch);
}

/*----------------------------------------------------------------------------*/
/* MetaGet[].delete -- delete metaGet register                                */
/*----------------------------------------------------------------------------*/
void Step::vMetaGetDelete(void)
{
  ENTRY();

  AM::vDeleteOperand(*m_pLogicUnit->m_xpMetaGet);

  m_pLogicUnit->m_xpMetaGet = nullptr;

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* MetaGet[] -- fetch metaGet onto datastack                                  */
/*----------------------------------------------------------------------------*/
void Step::vMetaGetGet(void)
{
  ENTRY();

  m_pDataStack->vPushPtr(*m_pLogicUnit->m_xpMetaGet);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* MetaGet[].getAndClear -- fetch metaGet register and set to nullptr         */
/*----------------------------------------------------------------------------*/
void Step::vMetaGetGetAndClear(void)
{
  ENTRY();

  m_pDataStack->vPushPtr(*m_pLogicUnit->m_xpMetaGet);

  m_pLogicUnit->m_xpMetaGet = nullptr;

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* MetaGet[].isNotNull -- is metaGet register not null ?                      */
/*----------------------------------------------------------------------------*/
void Step::vMetaGetIsNotNull(void)
{
  ENTRY();

  m_pDataStack->vPushInt(m_pLogicUnit->m_xpMetaGet.bIsNull() ? 0 : 1);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* MetaGet[].isNull -- is metaGet register null ?                             */
/*----------------------------------------------------------------------------*/
void Step::vMetaGetIsNull(void)
{
  ENTRY();

  m_pDataStack->vPushInt(m_pLogicUnit->m_xpMetaGet.bIsNull() ? 1 : 0);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* MetaGet::new -- new metaGet with table name                                */
/*----------------------------------------------------------------------------*/
void Step::vMetaGetNew(void)
{
  ENTRY();
  
  s32 sTableNm;
  m_pDataStack->vPopStr(sTableNm, __func__);

  MetaGet *pMetaGet = new MetaGet(sTableNm);

  m_pDataStack->vPushPtr(pMetaGet);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* MetaGet[].getParts -- get parts of table named in metaGet register         */
/*----------------------------------------------------------------------------*/
void Step::vMetaGetGetParts(void)
{
  ENTRY();

  Table* pTable = _pTable(m_pDataStack->pPopPtr(__func__));

  //vTrace("vMetaSendGetParts pTable: %p %s\n"_Stz,
  //       pTable,
  //       pTable->szTableNm()->azChar());

  // create an original parcel
  Parcel* pParcel = new Parcel(true);
  
  pTable->vGetMetaParts(pParcel);

  m_pDataStack->vPushPtr(pParcel);
  
  EXIT();
}


/*----------------------------------------------------------------------------*/
/* MetaSend::putParts -- put parts of meta-object                             */
/*----------------------------------------------------------------------------*/
void Step::vMetaGetPutParts(void)
{
  ENTRY();
  
  // ... code which pipesi in all the parts of a meta-object and
  //     gets the meta-object to reassemble itself ...


  // get arguments
  Table* pTable = nullptr;
  Parcel* pParcel = (Parcel*)m_pDataStack->pPopPtr(__func__);

  i8 nObjParts = pParcel->nItems();

  vTrace("C++ gets called: vMetaRecvPutParts. Received pParcel:%p with %lld parts\n"_Stz, pParcel, nObjParts);

  for (i8 i_Item = 0; i_Item < nObjParts; i_Item++) {
    Object *pObject = pParcel->pGetItem(i_Item);
    if (pObject && (pObject->eGetClassNo() == CN_TABLE)) {
      pTable = (Table *) pObject;
      //vTrace("Table:%p reassemble from parcel, tablename:%s\n"_Stz,pTable, pTable->szTableNm()->azChar());
      // call reassemble parts code
      pTable->iReassembleFromParcel(pParcel);
      break;
    }
  }

  // Here we have a table came from another node
  if (pTable) pTable->vDumpLocation();

  m_pDataStack->vPushPtr(pTable);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* MetaGet[].set -- set metaGet register                                      */
/*----------------------------------------------------------------------------*/
void Step::vMetaGetSet(void)
{
  ENTRY();

  MetaGet* pMeta = _pMetaGet(m_pDataStack->pPopPtr(__func__));

  ASSERT(! pMeta || m_pLogicUnit->m_xpMetaGet.bIsNull());

  m_pLogicUnit->m_xpMetaGet = pMeta;

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* MetaGet[].tableName@ -- fetch tableName from metaGet register              */
/*----------------------------------------------------------------------------*/
void Step::vMetaGetTableNameFetch(void)
{
  ENTRY();
  //vTrace("C++ gets called: vMetaGetGetTableName\n"_Stz);

  MetaGet* pMetaGet = *m_pLogicUnit->m_xpMetaGet;

  s32 sTableNm = pMetaGet->sTableName();

  m_pDataStack->vPushStz(sTableNm);

  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
