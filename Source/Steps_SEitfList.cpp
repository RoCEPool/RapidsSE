/*============================================================================*/
/* Steps_SEitfList.cpp - SEitf list step group                                */
/*============================================================================*/

#include "StorageEngine.h"
//#include "Tasks.h"
#include "SePrompt.hpp"

extern AttachMap*      gpAttachMap;
extern BufferPool*     gpBufferPool;
extern BPA*            gpBPA;
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

// seitflist: //////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_SEitfList(void)
{
  vAddPrimitive("seitfList.dump"_Stz,  &vSEitfListDump,    false);
  vAddPrimitive("seitfList.init"_Stz,  &vSEitfListInit,    false);
}

/*----------------------------------------------------------------------------*/
/* seitflist.dump -- dump of seitfList on data stack                          */
/*----------------------------------------------------------------------------*/
void Task::vSEitfListDump(MP_Task mpTask)
{
ENTRY("vSEitfListDump"_Stz);
  
  Object* pPtrSEitfList;
  if ( ! mpTask->m_pDataStack->bPop(pPtrSEitfList) )
    THROW_INVALID_DSTACK("seitflist.dump"_Stz);
  
  SEitfList* pSEitfList = _pSEitfList(pPtrSEitfList);
  ASSERT(CN_SEITFLIST == pSEitfList->eClass());
  
  pSEitfList->vContentDump("SEitfList:"_Stz, aSingleTab());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* seitflist.init -- init of seitfList on data stack                          */
/*----------------------------------------------------------------------------*/
void Task::vSEitfListInit(MP_Task mpTask)
{
ENTRY("vSEitfListInit"_Stz);
  
  Object* pPtrSEitfList;
  if ( ! mpTask->m_pDataStack->bPop(pPtrSEitfList) )
    THROW_INVALID_DSTACK("seitflist.init"_Stz);
  
  SEitfList* pSEitfList = _pSEitfList(pPtrSEitfList);
  ASSERT(CN_SEITFLIST == pSEitfList->eClass());
  
  new(pSEitfList) SEitfList;
  
  EXIT();
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
