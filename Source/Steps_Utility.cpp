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
  vAddStep("utility::prefetch"_Stz,   &Step::vUtilityPrefetch);
  vAddStep("utility::schemaTest"_Stz, &Step::vUtilitySchemaTest);
}

/*----------------------------------------------------------------------------*/
/* utility::prefetch -- test prefetch                                         */
/*----------------------------------------------------------------------------*/
void Step::vUtilityPrefetch(MP_Task mpTask)
{
  ENTRY("vUtilityPrefetch"_Stz);
  
  Prefetch::vTest();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* utility::schemaTest -- run schema test                                     */
/*----------------------------------------------------------------------------*/
void Step::vUtilitySchemaTest(MP_Task mpTask)
{
  ENTRY("vUtilitySchemaTest"_Stz);
  
  s32 sStream;
  if ( ! mpTask->m_pDataStack->bPop(sStream) )
    THROW_INVALID_DSTACK("task.seed"_Stz);

  SchemaTest::vRun(sStream);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
