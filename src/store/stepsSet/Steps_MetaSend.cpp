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
  
  // ... code which gets all the parts of a meta-object and pipes them out ...
  // get arguments
  //m_pDataStack->vPopStr(sTableName,__func__);
  // pTable = ...;

  // Don't know what to get from the in-pipe yet, but for now... create a dummy table
  Table* pTable = new Table("BolTbl"_Stz,TT_NORMAL);

  // call get parts code
  pTable->iPrepareObjParts();
  std::vector<Object*>aParts = pTable->aObjParts();

  // put results


  // foreach part, push part
  for (auto it = aParts.begin();  it != aParts.end(); ++it) {
    Object* pObject = *it;
    m_pDataStack->vPushPtr((Object*)pObject);
  }

  // push number of object parts
  m_pDataStack->vPushInt(pTable->aObjParts().size());
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
