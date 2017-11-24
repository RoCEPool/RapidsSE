/*============================================================================*/
/* Steps_GroupBy.cpp -- Group By step group                                   */
/*============================================================================*/

#include "StorageEngine.h"

extern StepArray*      gpStepArray;
extern NP_Task         gnpBootTask;
extern Lock            gxDdlLock;
extern Lock            gxDicLock;
extern SEitfList*      gpSEitfList;

// accum: //////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_GroupBy(void)
{
  vAddPrimitive("groupBy.applyBuffer"_Stz,  &vGroupByApplyBuffer,  false);
  vAddPrimitive("groupBy.applyEntry"_Stz,   &vGroupByApplyEntry,   false);
  vAddPrimitive("groupBy.applyPartial"_Stz, &vGroupByApplyPartial, false);
  vAddPrimitive("groupBy.delete"_Stz,       &vGroupByDelete,       false);
  vAddPrimitive("groupBy.end"_Stz,          &vGroupByEnd,          false);
  vAddPrimitive("groupBy::new"_Stz,         &vGroupByNew,          false);
  vAddPrimitive("groupBy.start"_Stz,        &vGroupByStart,        false);
}

/*----------------------------------------------------------------------------*/
/* groupBy.applyBuffer -- apply buffer of lineitems to accum                  */
/*----------------------------------------------------------------------------*/
void Task::vGroupByApplyBuffer(MP_Task mpTask)
{
  ENTRY("vGroupByApplyBuffer"_Stz);
  
  Object* pPtrGroupBy;
  if ( ! mpTask->m_pDataStack->bPop(pPtrGroupBy) )
    THROW_INVALID_DSTACK("groupBy.applyBuffer"_Stz);
  GroupBy* pGroupBy = _pGroupBy(pPtrGroupBy);
  
  Object* pPtrEntryBuffer;
  if ( ! mpTask->m_pDataStack->bPop(pPtrEntryBuffer) )
    THROW_INVALID_DSTACK("groupBy.applyBuffer"_Stz);
  EntryBuffer* pEntryBuffer = _pEntryBuffer(pPtrEntryBuffer);
  
  pGroupBy->vApplyBuffer(pEntryBuffer);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* groupBy.applyEntry -- apply entry of lineitems to accum                    */
/*----------------------------------------------------------------------------*/
void Task::vGroupByApplyEntry(MP_Task mpTask)
{
  ENTRY("vGroupByApplyBuffer"_Stz);
  
  Object* pPtrGroupBy;
  if ( ! mpTask->m_pDataStack->bPop(pPtrGroupBy) )
    THROW_INVALID_DSTACK("groupBy.applyEntry"_Stz);
  GroupBy* pGroupBy = _pGroupBy(pPtrGroupBy);
  
  Object* pPtrEntry;
  if ( ! mpTask->m_pDataStack->bPop(pPtrEntry) )
    THROW_INVALID_DSTACK("groupBy.applyEntry"_Stz);
  Entry* pEntry = _pEntry(pPtrEntry);
  
  pGroupBy->vApplyEntry(pEntry);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* groupBy.applyPartial -- apply groupBy partial result of lineitems to accum  */
/*----------------------------------------------------------------------------*/
void Task::vGroupByApplyPartial(MP_Task mpTask)
{
  ENTRY("vGroupByApplyBuffer"_Stz);
  
  Object* pPtrGroupBy;
  if ( ! mpTask->m_pDataStack->bPop(pPtrGroupBy) )
    THROW_INVALID_DSTACK("groupBy.applyPartial"_Stz);
  GroupBy* pGroupBy = _pGroupBy(pPtrGroupBy);
  
  Object* pPtrPartialGroupBy;
  if ( ! mpTask->m_pDataStack->bPop(pPtrPartialGroupBy) )
    THROW_INVALID_DSTACK("groupBy.applyPartial"_Stz);
  GroupBy* pPartialGroupBy = _pGroupBy(pPtrPartialGroupBy);
  
  pGroupBy->vApplyPartial(pPartialGroupBy);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* groupBy.delete -- delete group by                                          */
/*----------------------------------------------------------------------------*/
void Task::vGroupByDelete(MP_Task mpTask)
{
  ENTRY("vGroupByDelete"_Stz);
  
  Object* pPtrGroupBy;
  if ( ! mpTask->m_pDataStack->bPop(pPtrGroupBy) )
    THROW_INVALID_DSTACK("groupBy.delete"_Stz);
  GroupBy* pGroupBy = _pGroupBy(pPtrGroupBy);
  
  delete pGroupBy;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* groupBy.end -- end group by processing                                     */
/*----------------------------------------------------------------------------*/
void Task::vGroupByEnd(MP_Task mpTask)
{
  ENTRY("vGroupByEnd"_Stz);
  
  Object* pPtrGroupBy;
  if ( ! mpTask->m_pDataStack->bPop(pPtrGroupBy) )
    THROW_INVALID_DSTACK("groupBy.end"_Stz);
  GroupBy* pGroupBy = _pGroupBy(pPtrGroupBy);

  pGroupBy->vEnd();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* groupBy::new -- group by new                                               */
/*----------------------------------------------------------------------------*/
void Task::vGroupByNew(MP_Task mpTask)
{
  ENTRY("vGroupByNew"_Stz);
  
  GroupBy* pGroupBy = new GroupBy;
  
  mpTask->m_pDataStack->vPushObject(pGroupBy);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* groupBy.start -- start group by processing                                 */
/*----------------------------------------------------------------------------*/
void Task::vGroupByStart(MP_Task mpTask)
{
  ENTRY("vGroupByStart"_Stz);
  
  Object* pPtrGroupBy;
  if ( ! mpTask->m_pDataStack->bPop(pPtrGroupBy) )
    THROW_INVALID_DSTACK("groupBy.start"_Stz);
  GroupBy* pGroupBy = _pGroupBy(pPtrGroupBy);
  
  pGroupBy->vStart();
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
