/*============================================================================*/
/* Steps_AttachMap.cpp - attach map step group                                */
/*============================================================================*/

#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_AttachMap(void)
{
  vAddStep("attachmap::attachmap@"_Stz,   &Step::vAttachMapFetch);
  vAddStep("attachmap::attachmap!"_Stz,   &Step::vAttachMapStore);
  vAddStep("attachmap::tenants@"_Stz,     &Step::vAttachMapTenantsFetch);
}

/*----------------------------------------------------------------------------*/
/* attachmap@ -- fetch value from attachmap                                   */
/*----------------------------------------------------------------------------*/
void Step::vAttachMapFetch(MP_Task mpTask)
{
  ENTRY("vAttachMapFetch"_Stz);
  
  i8  iAttachMapEnum;
  eAM eAttachMap;
  
  if ( ! mpTask->m_pDataStack->bPop(iAttachMapEnum) )
    THROW_INVALID_DSTACK("attachmap::attachmap@"_Stz);
  
  eAttachMap = (eAM)iAttachMapEnum;
  
  switch (eAttachMap) {
    case AM_INDEXENTRIES:
      mpTask->m_pDataStack->vPushI8(gpAttachMap->m_nIndexEntries);
      break;
    case AM_INDEXLEVELS:
      mpTask->m_pDataStack->vPushI8(gpAttachMap->m_nIndexLevels);
      break;
    default:
      THROW_INVALID_DSTACK("attachmap::attachmap@"_Stz);
  }
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* attachmap! -- store value in attachmap                                     */
/*----------------------------------------------------------------------------*/
void Step::vAttachMapStore(MP_Task mpTask)
{
  ENTRY("vAttachMapStore"_Stz);
  
  i8  iAttachMapEnum;
  eAM eAttachMap;
  
  s256 sValue;
  
  if ( ! mpTask->m_pDataStack->bPop(iAttachMapEnum) )
    THROW_INVALID_DSTACK("attachmap::attachmap!"_Stz);
  
  eAttachMap = (eAM)iAttachMapEnum;
  
  switch (eAttachMap) {
    case AM_INDEXENTRIES:
      if ( ! mpTask->m_pDataStack->bPop(sValue) )
        THROW_INVALID_DSTACK("attachmap::attachmap!"_Stz);
      gpAttachMap->m_nIndexEntries = StrLib::iToI8(sValue.aChar());
      break;
      
    case AM_INDEXLEVELS:
      i8 iIndexLevels;
      if ( ! mpTask->m_pDataStack->bPop(sValue) )
        THROW_INVALID_DSTACK("attachmap::attachmap!"_Stz);
      iIndexLevels = StrLib::iToI8(sValue.aChar());
      if (iIndexLevels >= C_FLAT_LEVELS_MAX)
        THROW("IndexLevel should be < %lldattachmap::attachmap!"_Stz, C_FLAT_LEVELS_MAX);
      gpAttachMap->m_nIndexLevels = iIndexLevels;
      gpAttachMap->m_nIndexOffsets = gpAttachMap->m_nIndexLevels;
      break;
    
    default:
      THROW_INVALID_DSTACK("attachmap::attachmap!"_Stz);
  }
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* attachmap::tenants@ -- fetch word tenants from attachmap                   */
/*----------------------------------------------------------------------------*/
void Step::vAttachMapTenantsFetch(MP_Task mpTask)
{
  ENTRY("vAttachMapTenantsFetch"_Stz);
  
  mpTask->m_pDataStack->vPushPointer(gpAttachMap->m_pTenants);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
