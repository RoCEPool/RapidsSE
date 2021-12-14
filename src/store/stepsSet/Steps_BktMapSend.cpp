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
  pTable->vPrepareBktMap();
  Parcel* pParcel = pTable->pParcel();

  m_pDataStack->vPushPtr(pParcel);

  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
