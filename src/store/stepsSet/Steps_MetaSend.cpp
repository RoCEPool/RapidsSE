/*============================================================================*/
/* Steps_MetaSend.cpp -- metaSend step group                                  */
/*============================================================================*/

#include "BASE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "Steps.hpp"


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
  // pTable = ...;
  
  // call get parts code
  // pTable->vPrepareObjParts();
  // aParts = pTable->aObjParts();
  
  // put results
  // foreach part, push part
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
