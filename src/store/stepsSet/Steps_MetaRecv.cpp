/*============================================================================*/
/* Steps_MetaRecv.cpp -- metaRecv step group                                  */
/*============================================================================*/


#include "BASE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "Steps.hpp"

#include <eTT.h>
#include "SkipRoots.hpp"
#include "EntBuffer.hpp"
#include "IndexTree.hpp"
#include "TablePtn.hpp"
#include "Table.hpp"
#include "Parcel.hpp"


/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_MetaRecv(void)
{
  vAddStep("MetaRecv::putParts"_Stz,&Step::vMetaRecvPutParts);
}

/*----------------------------------------------------------------------------*/
/* MetaSend::putParts -- put parts of meta-object                             */
/*----------------------------------------------------------------------------*/
void Step::vMetaRecvPutParts(void)
{
  ENTRY();
  
  // ... code which pipesi in all the parts of a meta-object and
  //     gets the meta-object to reassemble itself ...


  // get arguments
  Table* pTable = nullptr;
  Parcel* pParcel = (Parcel*)m_pDataStack->pPopPtr(__func__);

  i8 nObjParts = pParcel->nObjects();

  vTrace("C++ gets called: vMetaRecvPutParts. Received pParcel:%p with %lld parts\n"_Stz, pParcel, nObjParts);

  for (i8 i_Obj = 0; i_Obj < nObjParts; i_Obj++) {
    Object *pObject = pParcel->pGetObject(i_Obj);
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

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
