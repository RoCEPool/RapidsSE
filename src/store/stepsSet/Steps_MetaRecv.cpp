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

  ///// FOR NOW THIS CODE IS SPECIFIC to TABLE
  // get arguments
  Table* pTable = nullptr;
  Parcel* pParcel = (Parcel*)m_pDataStack->pPopPtr(__func__);

  i8 nObjParts = pParcel->nObjects();

  if (nObjParts > 0) {
    Object *pObject = pParcel->pGetObject(0);
    if (pObject->eGetClassNo() == CN_TABLE) {
      pTable = (Table *) pObject;
      pTable->iBuildParcel(pParcel);
    }
  }

  /*
  // get number of parts
  for (i8 i_Obj = 0; i_Obj < nObjParts; i_Obj++) {
    Object *pObject = pParcel->pGetObject(i_Obj);
    if (pObject->eGetClassNo() == CN_TABLE) {
      pTable = (Table *) pObject;
    }
    // get all the parts, includes itself

    if (pTable) {
      pTable->iBuildParcelObjs(pObject);
    }
  }
  */

  // call reassemble parts code
  if (pTable) {
    pTable->iReassembleObj();
    // put results ??
    m_pDataStack->vPushPtr(pTable);
  }


  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
