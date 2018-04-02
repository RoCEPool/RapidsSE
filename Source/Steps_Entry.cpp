/*============================================================================*/
/* Steps_Entry.cpp - entry step group                                         */
/*============================================================================*/

#include "StorageEngine.h"
//#include "Tasks.h"
#include "SePrompt.hpp"

extern BufferPool*     gpBufferPool;
extern AttachMap*      gpAttachMap;

// index: //////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_Entry(void)
{
  vAddPrimitive("entry.matches"_Stz,      &vEntryMatches,     false);
  vAddPrimitive("entry.toDataRow"_Stz,    &vEntryToDataRow,   false);
  vAddPrimitive("entry.same"_Stz,         &vEntrySame,        false);
}

/*----------------------------------------------------------------------------*/
/* entry.matches -- return true if entry matches key                          */
/*----------------------------------------------------------------------------*/
void Task::vEntryMatches(MP_Task mpTask)
{
  ENTRY("vEntryMatches"_Stz);
  
  Object* pPtrEntry = nullptr;
  Object* pPtrIndex = nullptr;
  ValWrk xValWrk;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrEntry) )
    THROW_INVALID_DSTACK("entry.matches"_Stz);
  
  Entry* pEntry = _pEntry(pPtrEntry);
  ASSERT(CN_ENTRY == pEntry->eClass());
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrIndex) )
    THROW_INVALID_DSTACK("entry.matches"_Stz);
  
  Index* pIndex = _pIndex(pPtrIndex);
  ASSERT(CN_INDEX == pIndex->eClass());
  
  if ( ! mpTask->m_pDataStack->bPop(xValWrk) )
    THROW_INVALID_DSTACK("entry.matches"_Stz);
  
  KeyWrk xKeyWrk;
  Formatters* pFormatters = &mpTask->m_pSEitf->m_xFormatters;
  xKeyWrk.vAddDatum(xValWrk, pIndex->pIdxColVec()->pGet(0),pFormatters);

  //pEntry->vShow(0, pIndex,true);
  bool bIsEqual = false;
  bool bSame = pEntry->aKey()->bJuxtaCompare(JC_SAME, xKeyWrk.aKey(),
                                             pIndex->pIdxColVec(), 1, bIsEqual);

  mpTask->m_pDataStack->vPushInt(bSame ? 1 : 0);
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* entry.toDataRow -- return row from entry                                   */
/*----------------------------------------------------------------------------*/
void Task::vEntryToDataRow(MP_Task mpTask)
{
  ENTRY("vEntryRow"_Stz);
  
  Object* pPtrEntry = nullptr;
  Object* pPtrIndex = nullptr;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrEntry) )
    THROW_INVALID_DSTACK("entry.toDataRow"_Stz);
  
  Entry* pEntry = _pEntry(pPtrEntry);
  ASSERT(CN_ENTRY == pEntry->eClass());
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrIndex) )
    THROW_INVALID_DSTACK("entry.toDataRow"_Stz);
  
  Index* pIndex = _pIndex(pPtrIndex);
  ASSERT(CN_INDEX == pIndex->eClass());
  
  Entry* pRowEntry = pEntry->pDataEntry();
  
  // return the row entry
  mpTask->m_pDataStack->vPushObject(pRowEntry);
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* entry.same -- return true if entry is same as given entry                  */
/*----------------------------------------------------------------------------*/
void Task::vEntrySame(MP_Task mpTask)
{
  ENTRY("vEntryMatches"_Stz);
  
  Object* pPtrEntry = nullptr;
  Object* pPtrOtherEntry = nullptr;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrEntry) )
    THROW_INVALID_DSTACK("entry.same"_Stz);
  
  Entry* pEntry = _pEntry(pPtrEntry);
  ASSERT(CN_ENTRY == pEntry->eClass());
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrOtherEntry) )
    THROW_INVALID_DSTACK("entry.same"_Stz);
  
  Entry* pOtherEntry = _pEntry(pPtrOtherEntry);
  ASSERT(CN_ENTRY == pOtherEntry->eClass());
  
  mpTask->m_pDataStack->vPushInt(pEntry == pOtherEntry ? 1 : 0);
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
