/*============================================================================*/
/* Steps_Table.cpp - table step group                                         */
/*============================================================================*/

#include "StorageEngine.h"
//#include "Tasks.h"
#include "SePrompt.hpp"

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

extern bool         gbIndexSpace;
extern i8           gnIndexEntries;

// table: //////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_AttachMap(void)
{
  vAddPrimitive("attachmap::attachmap@"_Str,   &vAttachMapFetch,  false);
  vAddPrimitive("attachmap::attachmap!"_Str,   &vAttachMapStore,  false);
}

/*----------------------------------------------------------------------------*/
/* attachmap@ -- fetch value from attachmap                                   */
/*----------------------------------------------------------------------------*/
void Task::vAttachMapFetch(NNP_Task nnpTask)
{
  ENTRY("vAttachMapFetch"_Str);
  
  i8  iAttachMapEnum;
  eAM eAttachMap;
  
  if ( ! nnpTask->m_pDataStack->bPop(iAttachMapEnum) )
    THROW_INVALID_DSTACK("attachmap::attachmap@"_Str);
  
  eAttachMap = (eAM)iAttachMapEnum;
  
  switch (eAttachMap) {
    case AM_INDEXSPACE:
      nnpTask->m_pDataStack->vPushInt(gbIndexSpace ? 1 : 0);
      break;
      
    case AM_INDEXENTRIES:
      nnpTask->m_pDataStack->vPushInt(gnIndexEntries);
      break;
      
    default:
      THROW_INVALID_DSTACK("attachmap::attachmap@"_Str);
  }
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* attachmap! -- store value in attachmap                                     */
/*----------------------------------------------------------------------------*/
void Task::vAttachMapStore(NNP_Task nnpTask)
{
  ENTRY("vAttachMapStore"_Str);
  
  i8  iAttachMapEnum;
  eAM eAttachMap;
  
  s256 sValue;
  
  if ( ! nnpTask->m_pDataStack->bPop(iAttachMapEnum) )
    THROW_INVALID_DSTACK("attachmap::attachmap!"_Str);
  
  eAttachMap = (eAM)iAttachMapEnum;
  
  switch (eAttachMap) {
    case AM_INDEXSPACE:
      
      i8 iIndexSpace;
      if ( ! nnpTask->m_pDataStack->bPop(sValue) )
        THROW_INVALID_DSTACK("attachmap::attachmap!"_Str);
      iIndexSpace = StrLib::iToI8(sValue.s());
      gbIndexSpace = (iIndexSpace == 1 ? true : false);
      break;
      
    case AM_INDEXENTRIES:
      i8 iIndexEntries;
      if ( ! nnpTask->m_pDataStack->bPop(sValue) )
        THROW_INVALID_DSTACK("attachmap::attachmap!"_Str);
      gnIndexEntries = StrLib::iToI8(sValue.s());
      break;
      
    default:
      THROW_INVALID_DSTACK("attachmap::attachmap!"_Str);
  }
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
