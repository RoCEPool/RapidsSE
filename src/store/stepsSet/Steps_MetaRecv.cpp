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

  // get arguments
  // get number of parts
  i8 nObjParts = m_pDataStack->nValues();

  // Loop to get parts ?
  for (i8 i_Obj = 0; i_Obj < nObjParts; i_Obj++) {
    //Object* pObject = m_pDataStack->pPopPtr(...)

    // if (Object class is TABLE, create new table
    //Table* pTable1 = new
    // if object is not TABLE, build table parts array
    //pTable1->iBuildArrayObjParts(pObject);

  }

  // call reassemble parts code
  //pTable1->iReassembleObj();
  // put results ??
  //m_pDataStack->vPushPtr(pTable1);

  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
