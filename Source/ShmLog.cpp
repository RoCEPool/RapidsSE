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
: Focal(eClassNo,0,C_UNKNOWN)
, m_okLock(LK_LOG,this)
{
  _precondition_(_MAGIC_,eClassNo);
  
  m_i_Entry = 0;
  m_nEntries = C_MAX_LOG_ENTRIES;
  

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
  IGNORE_RETURN( ShmLib::bUnlink(sRegionNm,oSysCall) );
  
  // open a new shm region which will contain the activity map
  bResult = ShmLib::bOpenCreate(iRegionFd,sRegionNm,oSysCall);
  if (! bResult)
    vThrow(FK_FATAL,VL_FATAL,__func__,__FILE__,__LINE__,
           "ShmLog::aCreateNew: failed to create region %s\n"_Stz,
           sRegionNm.azChar());
  
  // extend the shm region
  bResult = ShmLib::bExtend(iRegionFd,nRegionSz,oSysCall);
  if (! bResult)
    vThrow(FK_FATAL,VL_FATAL,__func__,__FILE__,__LINE__,
           "ShmLog::aCreateNew: failed to extend region %s to %lldB\n"_Stz,
           sRegionNm.azChar(),nRegionSz);
  
  // map the shm region into a vm segment within this process address space
  void* aSegment = ShmLib::aMap(iRegionFd,
                             nullptr,
                             nRegionSz,
                             C_NO_HUGE_PAGES,
                             oSysCall);
  if (! aSegment)
    vThrow(FK_FATAL,VL_FATAL,__func__,__FILE__,__LINE__,
           "ShmLog::aCreateNew: failed to map region %s\n"_Stz,
           sRegionNm.azChar());
  
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
  bool bResult = ShmLib::bOpenExisting(iFD,sRegionNm,oSysCall);
  if (! bResult)
    vThrow(FK_FATAL,VL_FATAL,__func__,__FILE__,__LINE__,
           "ShmLog::aOpenExisting: failed to open region: %s\n"_Stz,
           sRegionNm.azChar());
  
  // map the shm region into a vm segment within this process address space
  void* aSegment = ShmLib::aMap(iFD,
                             nullptr,
                             nRegionSz,
                             C_NO_HUGE_PAGES,
                             oSysCall);
  if (! aSegment)
    vThrow(FK_FATAL,VL_FATAL,__func__,__FILE__,__LINE__,
           "ShmLog::aOpenExisting: failed to map region %s\n"_Stz,
           sRegionNm.azChar());
  
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

\
/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
