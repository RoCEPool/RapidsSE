/*============================================================================*/
/* Steps_bktMapSend.cpp -- bktMapSend step group                                  */
/*============================================================================*/

#include "BASE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "Steps.hpp"

#include "SkipRoots.hpp"
#include "EntBuffer.hpp"
#include "IndexTree.hpp"
#include "TablePtn.hpp"
#include "Table.hpp"


/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_BktMapSend(void)
{
  vAddStep("bktMapSend::getParts"_Stz,&Step::vBktMapSendGetParts);
  vAddStep("bktMapSend::getPartsByName"_Stz,&Step::vBktMapSendGetPartsByName);
}

/*----------------------------------------------------------------------------*/
/* bktMapSend::getParts -- get parts of meta-object                             */
/*----------------------------------------------------------------------------*/
void Step::vBktMapSendGetParts(void)
{
  ENTRY();

  Table* pTable = _pTable(m_pDataStack->pPopPtr(__func__));

  vTrace("C++ gets called: vbktMapSendGetParts for pTable:%p pTable->szTableNm()->azChar(): %s\n"_Stz,
         pTable,
         pTable->szTableNm()->azChar());

  // this parcel contains updated bucket map
  // create an original parcel
  Parcel* pParcel = new Parcel(true);
  pTable->vGetBktMap(pParcel);

  m_pDataStack->vPushPtr(pParcel);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* bktMapSend::getPartsByName -- get parts of meta-object                             */
/*----------------------------------------------------------------------------*/
void Step::vBktMapSendGetPartsByName(void)
{
  ENTRY();

  s32 sTableNm;
  m_pDataStack->vPopStr(sTableNm, __func__);

  vTrace("C++ gets called: vbktMapSendGetPartsByName for  szTableNm: %s\n"_Stz,
         sTableNm.azChar());

  Table* pTable = AM::pTableFind(sTableNm);
  Parcel* pParcel = nullptr;
  if (pTable) {
    vTrace("C++ gets called: vbktMapSendGetPartsByName FOUND table for  szTableNm: %s\n"_Stz,
           sTableNm.azChar());
    // this parcel contains updated bucket map
    // create an original parcel
    pParcel = new Parcel(true);
    pTable->vGetBktMap(pParcel);
  }

  m_pDataStack->vPushPtr(pParcel);

  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
