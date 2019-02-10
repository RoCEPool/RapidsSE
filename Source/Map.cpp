/*============================================================================*/
/* Map.cpp -- Map in shared memory                                            */
/*============================================================================*/

#include "Maps.h"
#include "StorageEngine.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* Map::operator new(size_t, Map* pMap)
{
  return pMap;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void Map::operator delete(void* aAddr)
{
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Map::Map(eCN eClassNo)
: Object(eClassNo)
{
  _precondition_(_MAGIC_,eClassNo);
  
  _postcondition_(_MAGIC_,eClassNo);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Map::~Map(void) noexcept(false)
{
  _precondition_(_MAGIC_);
}

/*----------------------------------------------------------------------------*/
/* -- Private Static ---------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* create a new shm region and map into vm segment                            */
/*----------------------------------------------------------------------------*/
void* Map::aMapCreate(s32 sRegionNm, i8 nRegionSz)
{
  int iRegionFd;
  SysCall oSysCall;
  bool bResult;
  
  // unlink the name of any previous segment
  IGNORE_RETURN( Shm::bUnlink(sRegionNm,oSysCall) );
  
  // open a new shm region which will contain the activity map
  bResult = Shm::bOpenCreate(sRegionNm,iRegionFd,oSysCall);
  if (! bResult)
    vFatal("Map::aMapCreate: failed to create region %s\n"_Stz,
           sRegionNm.aChar());
  
  // extend the shm region
  bResult = Shm::bExtend(iRegionFd,nRegionSz,oSysCall);
  if (! bResult)
    vFatal("Map::aMapCreate: failed to extend region %s to %lldB\n"_Stz,
           sRegionNm.aChar(),nRegionSz);
  
  // map the shm region into a vm segment within this process address space
  void* aSegment = Shm::aMap(iRegionFd,
                             nullptr,
                             nRegionSz,
                             C_NO_HUGE_PAGES,
                             oSysCall);
  if (! aSegment)
    vFatal("Map::aMapCreate: failed to map region %s\n"_Stz,
           sRegionNm.aChar());
  
  return aSegment;
}

/*----------------------------------------------------------------------------*/
/* attach to existing shm region and map into vm segment                      */
/*----------------------------------------------------------------------------*/
void* Map::aMapExisting(s32 sRegionNm, i8 nRegionSz)
{
  int iFD;
  SysCall oSysCall;
  
  // open existing shm region
  bool bResult = Shm::bOpenExisting(sRegionNm,iFD,oSysCall);
  if (! bResult)
    vFatal("Map::aMapExisting: failed to open region: %s\n"_Stz,
           sRegionNm.aChar());
  
  // map the shm region into a vm segment within this process address space
  void* aSegment = Shm::aMap(iFD,
                             nullptr,
                             nRegionSz,
                             C_NO_HUGE_PAGES,
                             oSysCall);
  if (! aSegment)
    vFatal("Map::aMapExisting: failed to map region %s\n"_Stz,
           sRegionNm.aChar());
  
  return aSegment;
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
