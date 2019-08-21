/*============================================================================*/
/* ShmLog.cpp -- Log in shared memory                                         */
/*============================================================================*/

#include "Logging.h"


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
: Focal(eClassNo,0)
{
  _precondition_(_MAGIC_,eClassNo);
  
  m_okLock.vInitLock(LK_ACTION_LOG);
  m_i_Entry = 0;
  m_nEntries = C_LOG_ENTRIES;
  

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
void* ShmLog::aOpenCreate(s32 sRegionNm, i8 nRegionSz)
{
  int iRegionFd;
  SysCall oSysCall;
  bool bResult;
  
  // unlink the name of any previous segment
  IGNORE_RETURN( Shm::bUnlink(sRegionNm,oSysCall) );
  
  // open a new shm region which will contain the activity map
  bResult = Shm::bOpenCreate(iRegionFd,sRegionNm,oSysCall);
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
  bool bResult = Shm::bOpenExisting(iFD,sRegionNm,oSysCall);
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
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* drop lock on ShmLog                                                        */
/*----------------------------------------------------------------------------*/
void ShmLog::vDropLock(void)
{
  _precondition_(_MAGIC_);
  
  m_okLock.vDropWrite();
}

/*----------------------------------------------------------------------------*/
/* grab lock on ShmLog                                                        */
/*----------------------------------------------------------------------------*/
void ShmLog::vGrabLock(void)
{
  _precondition_(_MAGIC_);
  
  m_okLock.vGrabWrite();
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
