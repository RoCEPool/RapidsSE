/*============================================================================*/
/* Steps_AttachMap.cpp - attach map step group                                */
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
extern Lock            gxDdlLock;
extern Lock            gxDicLock;
extern SEitfList*      gpSEitfList;

extern bool         gbIndexSpace;
extern i8           gnIndexEntries;
extern i8           gnIndexLevels;
extern i8           gnIndexOffsets;

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
void Task::vAttachMapFetch(MP_Task mpTask)
{
  ENTRY("vAttachMapFetch"_Str);
  
  i8  iAttachMapEnum;
  eAM eAttachMap;
  
  if ( ! mpTask->m_pDataStack->bPop(iAttachMapEnum) )
    THROW_INVALID_DSTACK("attachmap::attachmap@"_Str);
  
  eAttachMap = (eAM)iAttachMapEnum;
  
  switch (eAttachMap) {
    case AM_INDEXENTRIES:
      mpTask->m_pDataStack->vPushInt(gnIndexEntries);
      break;
    case AM_INDEXLEVELS:
      mpTask->m_pDataStack->vPushInt(gnIndexLevels);
      break;
    case AM_INDEXSPACE:
      mpTask->m_pDataStack->vPushInt(gbIndexSpace ? 1 : 0);
      break;
    default:
      THROW_INVALID_DSTACK("attachmap::attachmap@"_Str);
  }
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* attachmap! -- store value in attachmap                                     */
/*----------------------------------------------------------------------------*/
void Task::vAttachMapStore(MP_Task mpTask)
{
  ENTRY("vAttachMapStore"_Str);
  
  i8  iAttachMapEnum;
  eAM eAttachMap;
  
  s256 sValue;
  
  if ( ! mpTask->m_pDataStack->bPop(iAttachMapEnum) )
    THROW_INVALID_DSTACK("attachmap::attachmap!"_Str);
  
  eAttachMap = (eAM)iAttachMapEnum;
  
  switch (eAttachMap) {
    case AM_INDEXENTRIES:
      if ( ! mpTask->m_pDataStack->bPop(sValue) )
        THROW_INVALID_DSTACK("attachmap::attachmap!"_Str);
      gnIndexEntries = StrLib::iToI8(sValue.s());
      break;
      
    case AM_INDEXLEVELS:
      i8 iIndexLevels;
      if ( ! mpTask->m_pDataStack->bPop(sValue) )
        THROW_INVALID_DSTACK("attachmap::attachmap!"_Str);
      iIndexLevels = StrLib::iToI8(sValue.s());
      if (iIndexLevels >= C_FLAT_LEVELS_MAX)
        THROW("IndexLevel should be < %lldattachmap::attachmap!"_Str, C_FLAT_LEVELS_MAX);
      gnIndexLevels = iIndexLevels;
      gnIndexOffsets = gnIndexLevels - 1;
      break;
  
    case AM_INDEXSPACE:
      i8 iIndexSpace;
      if ( ! mpTask->m_pDataStack->bPop(sValue) )
        THROW_INVALID_DSTACK("attachmap::attachmap!"_Str);
      iIndexSpace = StrLib::iToI8(sValue.s());
      gbIndexSpace = (iIndexSpace == 1 ? true : false);
      break;
    
        default:
      THROW_INVALID_DSTACK("attachmap::attachmap!"_Str);
  }
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
