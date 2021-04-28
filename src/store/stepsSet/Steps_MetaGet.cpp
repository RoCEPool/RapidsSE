/*============================================================================*/
/* Steps_MetaGet.cpp -- metaGet step group                                    */
/*============================================================================*/


#include "BASE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "Steps.hpp"
#include "MetaGet.hpp"


/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_MetaGet(void)
{
  vAddStep("MetaGet::getMetaData"_Stz,&Step::vMetaGetGetMetadata);
  vAddStep("MetaGet.getTableName"_Stz,&Step::vMetaGetGetTableName);
}

/*----------------------------------------------------------------------------*/
/* MetaGet::getMetaData -- start get parts of meta-object                     */
/*----------------------------------------------------------------------------*/
void Step::vMetaGetGetMetadata(void)
{
  ENTRY();
  
  // ... code which initiates the metadata transfer ...
  vTrace("C++ gets called: vMetaGetGetMetadata\n"_Stz);
  s32 sTableNm;
  m_pDataStack->vPopStr(sTableNm, __func__);

  MetaGet *pMetaGet = new MetaGet(sTableNm);


  m_pDataStack->vPushPtr(pMetaGet);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* MetaGet.vMetaGetGetTableName -- get tableName from MetaGet object          */
/*----------------------------------------------------------------------------*/
void Step::vMetaGetGetTableName(void)
{
  ENTRY();
  vTrace("C++ gets called: vMetaGetGetTableName\n"_Stz);

  MetaGet* pMetaGet = _pMetaGet(m_pDataStack->pPopPtr(__func__));

  s32 sTableNm = pMetaGet->sTableName();

  m_pDataStack->vPushStz(sTableNm);

  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
