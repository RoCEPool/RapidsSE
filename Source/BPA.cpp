/*============================================================================*/
/* BPA.cpp - buffer pool analyzer                                             */
/*============================================================================*/

#include "Utilities.h"

extern SizeHeap gxaSizeHeap[];

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(Utilities,BPA,CN_BPA)
__aNewFunction(BPA,CN_BPA)
__vDeleteFunction(BPA,CN_BPA)
__aFooterFunction(BPA)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
BPA::BPA(void)
: Object(CN_BPA)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BPA,_FOOTER_);
  
  for (i8 i_Object = 0 ; i_Object < CN_ENUMS; i_Object++) {
    m_naObjectsAtTake[i_Object] = 0;
    m_naObjectsAtDump[i_Object] = 0;
  }
  
  _checkInitialised_(CN_BPA);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_BPA,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
BPA::~BPA(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BPA,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* produce report on the object delta                                         */
/*----------------------------------------------------------------------------*/
void BPA::vDeltaReport(s48K& rsPortrait)
{
  BPA::vObjectSnapshot(m_naObjectsAtDump);
  
  for (i8 i_Object = 0 ; i_Object < CN_ENUMS; i_Object++) {
    
    eCN eClass = (eCN)i_Object;
        
    if (m_naObjectsAtDump[eClass] == m_naObjectsAtTake[eClass])
      continue;

    rsPortrait.vSprintCat(
      "%s "
      "%-6lld eCN %3lld %s:%12s \n"_Stz,
      Stz::szTab(1),
                          
      m_naObjectsAtDump[eClass] - m_naObjectsAtTake[eClass],
      eClass,
      aHT_short[ gxaSizeHeap[eClass].m_eWhichHeap ],
      aCN[eClass]
                          
      );
  }

}

/*----------------------------------------------------------------------------*/
/* dump all objects in given heap                                             */
/*----------------------------------------------------------------------------*/
void BPA::vEpochDump(s48K& rsPortrait)
{
  //vTrace("\n"_Stz);
  //vTrace("%sWords created since last epoch:\n"_Stz,Stz::szTab(1));
  
  PoolHelper::vWordList(AM::iEpoch());
  
  BPA::vObjectSnapshot(m_naObjectsAtDump);
  
  rsPortrait.vSprintCat("Objects created since last epoch:\n"_Stz);

  for (i8 i_Object = 0 ; i_Object < CN_ENUMS; i_Object++) {
    
    eCN eClass = (eCN)i_Object;
    
    if (m_naObjectsAtDump[eClass] == m_naObjectsAtTake[eClass])
      continue;
    
    rsPortrait.vSprintCat(
      "%s  %12s (eCN %3lld) %-6lld \n"_Stz,
      Stz::szTab(1),
      aCN[eClass],
      eClass,
      m_naObjectsAtDump[eClass] - m_naObjectsAtTake[eClass]
      );
  }

}

/*----------------------------------------------------------------------------*/
/* dump all objects in given heap                                             */
/*----------------------------------------------------------------------------*/
void BPA::vEpochTake(void)
{
  // increment the epoch
  AM::iNextEpoch();
  BPA::vObjectSnapshot(m_naObjectsAtTake);
}

/*----------------------------------------------------------------------------*/
/* produce report on the object currently in the buffer pool                  */
/*----------------------------------------------------------------------------*/
void BPA::vObjectReport(s48K& rsPortrait)
{
  BPA::vObjectSnapshot(m_naObjectsAtDump);
  
  for (i8 i_Object = 0 ; i_Object < CN_ENUMS; i_Object++) {
    
    eCN eClass = (eCN)i_Object;
        
    if (! m_naObjectsAtDump[eClass])
      continue;

    rsPortrait.vSprintCat(
      "%s "
      "%-6lld eCN %3lld:%12s \n"_Stz,
      Stz::szTab(1),
                          
      m_naObjectsAtDump[eClass],
      eClass,
      aCN[eClass]
                          
      );
  }

}

/*----------------------------------------------------------------------------*/
/* calc delta of objects                                                      */
/*----------------------------------------------------------------------------*/
i8 BPA::nObjectDelta(void)
{
  BPA::vObjectSnapshot(m_naObjectsAtDump);
  
  i8 nDelta = 0;
  
  for (i8 i_Object = 0 ; i_Object < CN_ENUMS; i_Object++) {
    
    eCN eClass = (eCN)i_Object;
    if (m_naObjectsAtDump[eClass] == m_naObjectsAtTake[eClass])
      continue;
    
    nDelta += (m_naObjectsAtDump[eClass] - m_naObjectsAtTake[eClass]);
  }
  
  return nDelta;
}

/*----------------------------------------------------------------------------*/
/* summary report on objects in the pool                                      */
/*----------------------------------------------------------------------------*/
void BPA::vObjectSnapshot(i8* pCountObjects)
{
  i8 iCountObjects[CN_ENUMS];
  
  for (i8 i_Index = 0; i_Index < CN_ENUMS; i_Index++)
    pCountObjects[i_Index] = 0;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  for (i8 i_Buffer = gpBufferPool->iBufferFirst();
       i_Buffer <= gpBufferPool->iBufferLast();
       i_Buffer++) {
    
    PoolBuffer* pBuffer = gpBufferPool->pToPoolBuffer(i_Buffer);
    
    eCN eClass = pBuffer->eClass();
    
    pCountObjects[eClass]++;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  AM::pHeapMem()->Transient.m_pTiny->vAnalyse(iCountObjects);
  
  for (i8 i_Index = 0; i_Index < CN_ENUMS; i_Index++) {
    eCN eClass = (eCN)i_Index;
    
    if (iCountObjects[eClass] == 0)
      continue;
    
    pCountObjects[eClass] += iCountObjects[eClass];
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  AM::pHeapMem()->Transient.m_pSmall->vAnalyse(iCountObjects);
  
  for (i8 i_Index = 0; i_Index < CN_ENUMS; i_Index++) {
    eCN eClass = (eCN)i_Index;
    
    if (iCountObjects[eClass] == 0)
      continue;
    
    pCountObjects[eClass] += iCountObjects[eClass];
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  AM::pHeapMem()->Transient.m_pMedium->vAnalyse(iCountObjects);
  
  for (i8 i_Index = 0; i_Index < CN_ENUMS; i_Index++) {
    eCN eClass = (eCN)i_Index;
    
    if (iCountObjects[eClass] == 0)
      continue;

    pCountObjects[eClass] += iCountObjects[eClass];
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  AM::pHeapMem()->Transient.m_pLarge->vAnalyse(iCountObjects);
  
  for (i8 i_Index = 0; i_Index < CN_ENUMS; i_Index++) {
    eCN eClass = (eCN)i_Index;
    
    if (iCountObjects[eClass] == 0)
      continue;

    pCountObjects[eClass] += iCountObjects[eClass];
  }
}

/*----------------------------------------------------------------------------*/
/* reset take counts                                                          */
/*----------------------------------------------------------------------------*/
void BPA::vReset(void)
{
  vEpochTake();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
