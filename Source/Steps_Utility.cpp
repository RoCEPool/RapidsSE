/*============================================================================*/
/* Steps_Utility.cpp - utility step group                                     */
/*============================================================================*/

#include "StorageEngine.h"
//#include "Tasks.h"
#include "SePrompt.hpp"

extern AttachMap*      gpAttachMap;

extern NP_Task         gnpBootTask;
extern Lock            gxDdlLock;
extern Lock            gxDicLock;

// words: //////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_Utility(void)
{
  vAddPrimitive("utility::prefetch"_Stz, &vUtilityPrefetch,  false );
}

/*----------------------------------------------------------------------------*/
/* utility::prefetch" -- test prefetch                                        */
/*----------------------------------------------------------------------------*/
void Task::vUtilityPrefetch(MP_Task mpTask)
{
  ENTRY("vUtilityPrefetch"_Stz);
  
  Prefetch::vTest();
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
