/*============================================================================*/
/* Steps_GroupBy.cpp -- Group By step group                                   */
/*============================================================================*/

#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_GroupBy(void)
{
  vAddStep("groupBy::new"_Stz,         &Step::vGroupByNew);
  
  vAddStep("groupBy.applyBuffer"_Stz,  &Step::vGroupByApplyBuffer);
  vAddStep("groupBy.applyEntry"_Stz,   &Step::vGroupByApplyEntry);
  vAddStep("groupBy.applyPartial"_Stz, &Step::vGroupByApplyPartial);
  vAddStep("groupBy.delete"_Stz,       &Step::vGroupByDelete);
  vAddStep("groupBy.end"_Stz,          &Step::vGroupByEnd);
  vAddStep("groupBy.start"_Stz,        &Step::vGroupByStart);
}

/*----------------------------------------------------------------------------*/
/* groupBy.applyBuffer -- apply buffer of lineitems to accum                  */
/*----------------------------------------------------------------------------*/
void Step::vGroupByApplyBuffer(MP_Task mpTask)
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
void Step::vGroupByApplyEntry(MP_Task mpTask)
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
void Step::vGroupByApplyPartial(MP_Task mpTask)
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
void Step::vGroupByDelete(MP_Task mpTask)
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
void Step::vGroupByEnd(MP_Task mpTask)
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
void Step::vGroupByNew(MP_Task mpTask)
{
  ENTRY("vGroupByNew"_Stz);
  
  GroupBy* pGroupBy = new GroupBy;
  
  mpTask->m_pDataStack->vPushPointer(pGroupBy);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* groupBy.start -- start group by processing                                 */
/*----------------------------------------------------------------------------*/
void Step::vGroupByStart(MP_Task mpTask)
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
