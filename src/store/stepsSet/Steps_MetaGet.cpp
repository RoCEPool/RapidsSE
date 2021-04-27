/*============================================================================*/
/* Steps_MetaGet.cpp -- metaGet step group                                    */
/*============================================================================*/


#include "BASE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "Steps.hpp"
#include <MetaGet.hpp>


/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_MetaGet(void)
{
  vAddStep("MetaGet::getMetaData"_Stz,&Step::vMetaGetGetMetadata);
}

/*----------------------------------------------------------------------------*/
/* MetaGet::getMetaData -- start get parts of meta-object                     */
/*----------------------------------------------------------------------------*/
void Step::vMetaGetGetMetadata(void)
{
  ENTRY();
  
  // ... code which initiates the metadata transfer ...
  s32 sTableNm;
  m_pDataStack->vPopStr(sTableNm, __func__);

  MetaGet *pMetaGet = new MetaGet(sTableNm);


  m_pDataStack->vPushPtr(pMetaGet);

  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
