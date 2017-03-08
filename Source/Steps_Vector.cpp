/*============================================================================*/
/* Steps_Vector.cpp - vector step group                                       */
/*============================================================================*/

#include "StorageEngine.h"
//#include "Tasks.h"
#include "SePrompt.hpp"

extern Arguments       gxArguments;

extern AttachMap*      gpAttachMap;
extern BufferPool*     gpBufferPool;
extern Heap*           gpMediumHeap;
extern BPA*            gpBPA;
extern Dic*            gpMainDic;
extern Word*           gpWord;

extern Grid*           gpGrid;
extern StepArray*      gpStepArray;
extern ThreadList*     gpThreadList;
extern TaskList*       gpTaskList;
extern TxCollection*   gpTxCollection;

extern NP_Task         gnpRootTask;
extern StepBoard*      gpStepBoard;
extern Lock            gxDdlLock;
extern Lock            gxDicLock;
extern SEitfList*      gpSEitfList;

// words: //////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_Vector(void)
{
  vAddPrimitive("vector.diff"_Str,     &vVectorDifference,  false );
}

/*----------------------------------------------------------------------------*/
/* vector.difference -- get the difference of two vectors                     */
/*----------------------------------------------------------------------------*/
void Task::vVectorDifference(NNP_Task nnpTask)
{
  ENTRY("vVectorDifference"_Str);
  
  ValSet xValSet1;
  ValSet xValSet2;
  
  if ( ! nnpTask->m_pDataStack->bPop(xValSet2) )
    THROW_INVALID_DSTACK("vector.difference"_Str);
  
  if ( ! nnpTask->m_pDataStack->bPop(xValSet1) )
    THROW_INVALID_DSTACK("vector.difference"_Str);
  
  ValWsp* pValWsp1;
  ValWsp* pValWsp2;
  
  char* pCursor1 = nullptr;
  char* pCursor2 = nullptr;
  i8 nVals = 0;
  while (xValSet2.bExtract(pCursor2,pValWsp2)) {
    
    bool bResult = xValSet1.bExtract(pCursor1,pValWsp1);
    ASSERT(bResult);
    i8 iDiff = _pIntVal(pValWsp1)->iInt() - _pIntVal(pValWsp2)->iInt();
    nnpTask->m_pDataStack->vPush(iDiff);
    nVals++;
  }

  nnpTask->m_pDataStack->vPush(nVals);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
