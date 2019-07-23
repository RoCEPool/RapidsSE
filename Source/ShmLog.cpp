/*============================================================================*/
/* ShmLog.cpp -- Log in shared memory                                         */
/*============================================================================*/

#include "Logging.h"
#include "StorageEngine.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* ShmLog::operator new(size_t, ShmLog* pShmLog)
{
  return pShmLog;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void ShmLog::operator delete(void* aAddr)
{
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
ShmLog::ShmLog(eCN eClassNo)
: Object(eClassNo)
{
  _precondition_(_MAGIC_,eClassNo);
  
  _postcondition_(_MAGIC_,eClassNo);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
ShmLog::~ShmLog(void) noexcept(false)
{
  _precondition_(_MAGIC_);
}

/*----------------------------------------------------------------------------*/
/* -- Private Static ---------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* create a new shm region and map into vm segment                            */
/*----------------------------------------------------------------------------*/
void* ShmLog::aCreateNew(s32 sRegionNm, i8 nRegionSz)
{
  int iRegionFd;
  SysCall oSysCall;
  bool bResult;
  
  // unlink the name of any previous segment
  IGNORE_RETURN( Shm::bUnlink(sRegionNm,oSysCall) );
  
  // open a new shm region which will contain the activity map
  bResult = Shm::bOpenCreate(sRegionNm,iRegionFd,oSysCall);
  if (! bResult)
    vFatal("ShmLog::aCreateNew: failed to create region %s\n"_Stz,
           sRegionNm.aChar());
  
  // extend the shm region
  bResult = Shm::bExtend(iRegionFd,nRegionSz,oSysCall);
  if (! bResult)
    vFatal("ShmLog::aCreateNew: failed to extend region %s to %lldB\n"_Stz,
           sRegionNm.aChar(),nRegionSz);
  
  // map the shm region into a vm segment within this process address space
  void* aSegment = Shm::aMap(iRegionFd,
                             nullptr,
                             nRegionSz,
                             C_NO_HUGE_PAGES,
                             oSysCall);
  if (! aSegment)
    vFatal("ShmLog::aCreateNew: failed to map region %s\n"_Stz,
           sRegionNm.aChar());
  
  return aSegment;
}

/*----------------------------------------------------------------------------*/
/* attach to existing shm region and map into vm segment                      */
/*----------------------------------------------------------------------------*/
void* ShmLog::aOpenExisting(s32 sRegionNm, i8 nRegionSz)
{
  int iFD;
  SysCall oSysCall;
  
  // open existing shm region
  bool bResult = Shm::bOpenExisting(sRegionNm,iFD,oSysCall);
  if (! bResult)
    vFatal("ShmLog::aOpenExisting: failed to open region: %s\n"_Stz,
           sRegionNm.aChar());
  
  // map the shm region into a vm segment within this process address space
  void* aSegment = Shm::aMap(iFD,
                             nullptr,
                             nRegionSz,
                             C_NO_HUGE_PAGES,
                             oSysCall);
  if (! aSegment)
    vFatal("ShmLog::aOpenExisting: failed to map region %s\n"_Stz,
           sRegionNm.aChar());
  
  return aSegment;
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
