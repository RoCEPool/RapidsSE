/*============================================================================*/
/* Steps_MetaSend.cpp -- metaSend step group                                  */
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
void Step::vAddSteps_MetaSend(void)
{
  vAddStep("MetaSend::getParts"_Stz,&Step::vMetaSendGetParts);
}

/*----------------------------------------------------------------------------*/
/* MetaSend::getParts -- get parts of meta-object                             */
/*----------------------------------------------------------------------------*/
void Step::vMetaSendGetParts(void)
{
  ENTRY();

  Table* pTable = _pTable(m_pDataStack->pPopPtr(__func__));

  vTrace("C++ gets called: vMetaSendGetParts for pTable:%p pTable->szTableNm()->azChar(): %s\n"_Stz,
         pTable,
         pTable->szTableNm()->azChar());

  pTable->iPrepareParcel();
  Parcel* pParcel = pTable->pParcel();

  //if (pParcel && pParcel->bOriginalParcel()) {
  //  vTrace("C++ prepared pParcel:%p for sending, it is originally created by Table\n"_Stz, pParcel);
  //}

  m_pDataStack->vPushPtr(pParcel);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
