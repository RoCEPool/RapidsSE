/*============================================================================*/
/* ObjectTracker.cpp -- count of allocs and frees by class                    */
/*============================================================================*/

#include "BASE.h"
#include "ObjectTracker.hpp"

extern SizeHeap  gxaSizeHeap[];
/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(BASE,ObjectTracker,CN_OBJTRACKER)
__aNewFunction(ObjectTracker,CN_OBJTRACKER)
__vDeleteFunction(ObjectTracker,CN_OBJTRACKER)
__aFooterFunction(ObjectTracker)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
ObjectTracker::ObjectTracker(void)
: Object(CN_OBJTRACKER)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OBJTRACKER,_FOOTER_);

  m_sLabel = ""_Stz;

  for(u8 i = 0; i < CN_ENUMS; i++)
  {
    m_xClassCount[i].m_uAllocateCount = 0;
    m_xClassCount[i].m_uFreeCount = 0;
  }

  vTrackAllocClass(CN_OBJTRACKER);

  _checkInitialised_(CN_OBJTRACKER);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_OBJTRACKER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
ObjectTracker::~ObjectTracker(void) noexcept(false)
{
_precondition_(_ADDRESS_,_MAGIC_,CN_OBJTRACKER,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Reset                                                                      */
/*----------------------------------------------------------------------------*/
void ObjectTracker::vAddLabel(Stz *szLabel)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  m_sLabel.vReset();
  m_sLabel.vCopy(szLabel);
}

/*----------------------------------------------------------------------------*/
/* Dump contents of this tracker                                              */
/*----------------------------------------------------------------------------*/
void ObjectTracker::vDump(Stz* szName)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  vTrace("-------------------------------------------\n"_Stz);

  if( ! m_sLabel.bIsEmpty()) {
    vTrace("Label: %s\n"_Stz, m_sLabel.azChar());
  }

  vTrace("Job: %s\n"_Stz, szName->azChar());
//  vTrace("            Class type     Alloc   Free   Class size\n\n"_Stz);
  vTrace("%23s  %7s  %7s  %10s %20s %10s\n\n"_Stz,"Class type", "Alloc", "Free", "Class size", "Heap Type", "Usage");
  for(u8 i = 0; i < CN_ENUMS; i++)
  {
    if(m_xClassCount[i].m_uAllocateCount)
      vTrace("%23s  %7llu  %7llu  %10lld %20s %10lld\n"_Stz,
             aCN[i],
             m_xClassCount[i].m_uAllocateCount,
             m_xClassCount[i].m_uFreeCount,
             gxaSizeHeap[i].m_nClassSize,
             aHT[gxaSizeHeap[i].m_eWhichHeap],
             ((m_xClassCount[i].m_uAllocateCount -
                m_xClassCount[i].m_uFreeCount) *
                gxaSizeHeap[i].m_nClassSize)
            );
  }
  vTrace("-------------------------------------------\n"_Stz);
}

/*----------------------------------------------------------------------------*/
/* Reset                                                                      */
/*----------------------------------------------------------------------------*/
void ObjectTracker::vReset(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  m_sLabel.vReset();

  for(u8 i = 0; i < CN_ENUMS; i++)
  {
    m_xClassCount[i].m_uAllocateCount = 0;
    m_xClassCount[i].m_uFreeCount = 0;
  }
}

/*----------------------------------------------------------------------------*/
/* Increment allocate count for class type                                    */
/*----------------------------------------------------------------------------*/
void ObjectTracker::vTrackAllocClass(eCN eClassNo)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  m_xClassCount[eClassNo].m_uAllocateCount++;
}

/*----------------------------------------------------------------------------*/
/* Increment free count for class type                                        */
/*----------------------------------------------------------------------------*/
void ObjectTracker::vTrackFreeClass(eCN eClassNo)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  m_xClassCount[eClassNo].m_uFreeCount++;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/