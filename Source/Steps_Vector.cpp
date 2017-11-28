/*============================================================================*/
/* Steps_Vector.cpp - vector step group                                       */
/*============================================================================*/

#include "StorageEngine.h"
//#include "Tasks.h"
#include "SePrompt.hpp"

extern AttachMap*      gpAttachMap;
extern NP_Task         gnpBootTask;

// words: //////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_Vector(void)
{
  vAddPrimitive("vector.diff"_Stz,     &vVectorDifference,  false );
}

/*----------------------------------------------------------------------------*/
/* vector.difference -- get the difference of two vectors                     */
/*----------------------------------------------------------------------------*/
void Task::vVectorDifference(MP_Task mpTask)
{
  ENTRY("vVectorDifference"_Stz);
  
  ValSet xValSet1;
  ValSet xValSet2;
  
  if ( ! mpTask->m_pDataStack->bPop(xValSet2) )
    THROW_INVALID_DSTACK("vector.difference"_Stz);
  
  if ( ! mpTask->m_pDataStack->bPop(xValSet1) )
    THROW_INVALID_DSTACK("vector.difference"_Stz);
  
  Value* pValue1;
  Value* pValue2;
  
  Stz* pCursor1 = nullptr;
  Stz* pCursor2 = nullptr;
  i8 nVals = 0;
  while (xValSet2.bExtractValue(pCursor2,pValue2)) {
    
    bool bResult = xValSet1.bExtractValue(pCursor1,pValue1);
    ASSERT(bResult);
    i8 iDiff = _pIntVal(pValue1)->iInt() - _pIntVal(pValue2)->iInt();
    mpTask->m_pDataStack->vPushInt(iDiff);
    nVals++;
  }
  
  mpTask->m_pDataStack->vPushInt(nVals);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
