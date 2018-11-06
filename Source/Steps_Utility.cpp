/*============================================================================*/
/* Steps_Utility.cpp - utility step group                                     */
/*============================================================================*/

#include "StorageEngine.h"


extern AttachMap*      gpAttachMap;

// words: //////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Utility(void)
{
  vAddStep("utility::prefetch"_Stz,   &Task::vUtilityPrefetch);
  vAddStep("utility::schemaTest"_Stz, &Task::vUtilitySchemaTest);
}

/*----------------------------------------------------------------------------*/
/* utility::prefetch -- test prefetch                                         */
/*----------------------------------------------------------------------------*/
void Task::vUtilityPrefetch(MP_Task mpTask)
{
  ENTRY("vUtilityPrefetch"_Stz);
  
  Prefetch::vTest();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* utility::schemaTest -- run schema test                                     */
/*----------------------------------------------------------------------------*/
void Task::vUtilitySchemaTest(MP_Task mpTask)
{
  ENTRY("vUtilitySchemaTest"_Stz);
  
  SchemaTest::vRun();
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
