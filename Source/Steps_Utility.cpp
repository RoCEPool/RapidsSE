/*============================================================================*/
/* Steps_Utility.cpp - utility step group                                     */
/*============================================================================*/

#include "StorageEngine.h"
//#include "Tasks.h"
#include "SePrompt.hpp"

extern Dic*            gpMainDic;
extern Word*           gpWord;

extern Grid*           gpGrid;
extern StepArray*      gpStepArray;
extern ThreadList*     gpThreadList;
extern TaskList*       gpTaskList;

extern NP_Task         gnpBootTask;
extern Lock            gxDdlLock;
extern Lock            gxDicLock;
extern SEitfList*      gpSEitfList;

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
