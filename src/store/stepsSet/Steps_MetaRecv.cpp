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
  // get number of parts
  i8 nObjParts = m_pDataStack->nValues();

  Table* pTable = nullptr;
  // Loop to get parts ?
  for (i8 i_Obj = 0; i_Obj < nObjParts; i_Obj++) {
    Object* pObject = m_pDataStack->pPopPtr(__func__);
    if (pObject->eGetClassNo() == CN_TABLE) {
      pTable = (Table*)pObject;
    } else {
      // get all the parts
      if (pTable)
        pTable->iBuildArrayObjParts(pObject);
    }
  }

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
