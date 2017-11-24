/*============================================================================*/
/* Steps_Row.cpp - row step group                                             */
/*============================================================================*/

#include "StorageEngine.h"
//#include "Tasks.h"
#include "SePrompt.hpp"

extern StepArray*      gpStepArray;
extern NP_Task         gnpBootTask;
extern Lock            gxDdlLock;
extern Lock            gxDicLock;
extern SEitfList*      gpSEitfList;

// index: //////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_Row(void)
{
  vAddPrimitive("row.string"_Stz,  &vRowString, false);
}

void Task::vRowString(MP_Task mpTask)
{
  ENTRY("vRowString"_Stz);
  
  Object* pPtrRow;
  if ( ! mpTask->m_pDataStack->bPop(pPtrRow) )
    THROW_INVALID_DSTACK("row.string"_Stz);
  
  Entry* pRow = _pEntry(pPtrRow);
  ASSERT(CN_ENTRY == pRow->eClass());
  
  StrVal* pStrVal = reinterpret_cast<StrVal*>(pRow->aRow());
  s256 sRow = pStrVal->aStr();
  
  mpTask->m_pDataStack->vPushS256(sRow);
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
