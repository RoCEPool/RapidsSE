/*============================================================================*/
/* Steps_BktMapRecv.cpp -- BktMapRecv step group                                  */
/*============================================================================*/


#include "BASE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "Steps.hpp"

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
void Step::vAddSteps_BktMapRecv(void)
{
  vAddStep("BktMapRecv::putParts"_Stz,&Step::vBktMapRecvPutParts);
}

/*----------------------------------------------------------------------------*/
/* BktMapRecv::putParts -- put parts of meta-object                             */
/*----------------------------------------------------------------------------*/
void Step::vBktMapRecvPutParts(void)
{
  ENTRY();

  // ... code which pipesi in all the parts of a meta-object and
  //     gets the meta-object to reassemble itself ...


  // get arguments
  Table* pTable = nullptr;
  Table* pUpdateTable = nullptr;
  Parcel* pParcel = (Parcel*)m_pDataStack->pPopPtr(__func__);

  i8 nObjParts = pParcel->nItems();

  vTrace("C++ gets called: vBktMapRecvPutParts. Received pParcel:%p with %lld parts\n"_Stz, pParcel, nObjParts);

  for (i8 i_Item = 0; i_Item < nObjParts; i_Item++) {
    Object *pObject = pParcel->pGetItem(i_Item);
    if (pObject && (pObject->eGetClassNo() == CN_TABLE)) {
      pTable = (Table *) pObject;
      //vTrace("Table:%p reassemble from parcel, tablename:%s\n"_Stz,pTable, pTable->szTableNm()->azChar());
      // call reassemble parts code
      pUpdateTable = AM::pTableFind(pTable->szTableNm());
      if (pUpdateTable) {
        vTrace("C++ gets called: vBktMapRecvPutParts. Found table to update\n"_Stz);
        if (pUpdateTable->bUpdateBktMap(pParcel))
          vTrace("C++ gets called: vBktMapRecvPutParts. updated bktmap OK\n"_Stz);
      }
      break;
    }
  }

  // Here we have a table came from another node
  //if (pTable) pTable->vDumpLocation();

  //m_pDataStack->vPushPtr(pUpdateTable);

  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/