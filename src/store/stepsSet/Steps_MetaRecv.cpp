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
  vTrace("C++ gets called: vMetaRecvPutParts\n"_Stz);

  // get arguments
  Table* pTable = nullptr;
  Parcel* pParcel = (Parcel*)m_pDataStack->pPopPtr(__func__);

  i8 nObjParts = pParcel->nObjects();

  for (i8 i_Obj = 0; i_Obj < nObjParts; i_Obj++) {
    Object *pObject = pParcel->pGetObject(i_Obj);
    if (pObject && (pObject->eGetClassNo() == CN_TABLE)) {
      pTable = (Table *) pObject;
      vTrace("Table reassemble from parcel as %s\n"_Stz,pTable->szTableNm()->azChar());
      pTable->iBuildParcel(pParcel);
      break;
    }
  }

  // call reassemble parts code

  pTable->iReassembleObj();

  // put results as a table
  m_pDataStack->vPushPtr(pTable);
  vTrace("Table is in the stack\n"_Stz);
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
