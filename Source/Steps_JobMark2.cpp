/*============================================================================*/
/* Steps_JobMark2.cpp - job mark II step group                                */
/*============================================================================*/

#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_JobMark2(void)
{
  vAddStep("jobMark2.tableName@"_Stz,   &Step::vJobMark2TableNameFetch);
  vAddStep("jobMark2.table!"_Stz,       &Step::vJobMark2TableStore);
}


/*----------------------------------------------------------------------------*/
/* jobMark2.tableName@ -- fetch table name                                    */
/*----------------------------------------------------------------------------*/
void Step::vJobMark2TableNameFetch(MP_Task mpTask)
{
  ENTRY("vJobMark2TableNameFetch"_Stz);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* jobMark2.table! -- store table pointer                                     */
/*----------------------------------------------------------------------------*/
void Step::vJobMark2TableStore(MP_Task mpTask)
{
  ENTRY("table"_Stz);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
