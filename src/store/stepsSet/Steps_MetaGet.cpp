/*============================================================================*/
/* Steps_MetaGet.cpp -- metaGet step group                                    */
/*============================================================================*/

#include "BASE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "Steps.hpp"


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

  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
