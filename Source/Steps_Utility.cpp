/*============================================================================*/
/* Steps_Utility.cpp - utility step group                                     */
/*============================================================================*/

#include "Steps.h"

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
void Step::vUtilityPrefetch(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vUtilityPrefetch"_Stz);
  
  Prefetch::vTest();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* utility::schemaTest -- run schema test                                     */
/*----------------------------------------------------------------------------*/
void Step::vUtilitySchemaTest(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vUtilitySchemaTest"_Stz);
  
  s32 sStream;
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(sStream) )
    vThrow(__func__,__FILE__,__LINE__,FK_DSTACK,__STEP__);

  SchemaTest::vRun(sStream);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
