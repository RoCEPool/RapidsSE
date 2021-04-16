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

  s32 sTableName;
  eCN uCn;
  eTT eKind;
  // get number of parts
  i8 nParts = m_pDataStack->nValues();

  // Loop to get parts ?
  //m_pDataStack->vPopStr(pBuffer,__func__);

  /*
   memcpy((char *) &uCn, pBuffer, sizeof(eCN));

  if (uCn == CN_TABLE) {
    // Main part of CN_TABLE
    MemLib::vCopy(&sTableName, pBuffer + sizeof(eCN), sizeof(sTableName), CN_STZVAL);
    MemLib::vCopy(&eKind, pBuffer + sizeof(eCN)+sizeof(sTableName), sizeof(eKind), CN_STZVAL);

  }
  Table* pTable1 = new Table(sTableName,eKind);
  pTable1->iBuildArrayObjParts(pChar);

  // call reassemble parts code
  pTable1->iReassembleObj();
  // put results ??
  m_pDataStack->vPushPtr(pTable1);

   */

  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
