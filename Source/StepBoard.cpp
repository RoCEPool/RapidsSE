/*============================================================================*/
/* StepBoard.cpp -- Step Board                                                */
/*============================================================================*/

#include "Tasks.h"
#include "Threads.h"

void* C_ADDR_STEPBOARD = (void*)0x102400000;

extern pthread_key_t giTLS_ThreadKey;

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* StepBoard::operator new(size_t iSize)
{
  RASSERT(iSize);
  
  int iShmFD;
  SysCall xSysCall;
  void* pRegion;
  bool bResult;
  
  s256 sRegionName("/RapidsSE_StepBoard"_Str);
  
  // open a shared memory region
  bResult = Shm::bAttach(sRegionName,iShmFD,xSysCall);
  if (bResult) {
    // map the opened shared memory region to memory
    pRegion = Shm::pMap(iShmFD,C_ADDR_STEPBOARD,sizeof(StepBoard),C_SHARED_REGION,(! C_HUGE_PAGES),xSysCall);
    if ( ! pRegion )  {
      vFatal("StepBoard::new: failed to map!\n"_Str);
      Shm::bClose(iShmFD,xSysCall);
      exit(EXIT_FAILURE);
    }
    
    StepBoard* pStepBoard =
      reinterpret_cast<StepBoard*>(pRegion);
    
    pStepBoard->vContentDump();
    
    return pStepBoard;
  }
  
  // open a new shm region
  bResult = Shm::bCreate(sRegionName,
                         sizeof(StepBoard),
                         iShmFD,
                         true,
                         xSysCall);
  if ( ! bResult ) {
    vFatal("StepBoard::new: failed to create region\n"_Str);
    exit(EXIT_FAILURE);
  }
  
  // allocate memory for the shm region
  pRegion = Shm::pMap(iShmFD,C_ADDR_STEPBOARD,sizeof(StepBoard),C_SHARED_REGION,(! C_HUGE_PAGES),xSysCall);
  if ( ! pRegion )  {
    vFatal("StepBoard::new failed to map!\n"_Str);
    Shm::bClose(iShmFD,xSysCall);
    exit(EXIT_FAILURE);
  }
  
  StepBoard* pStepBoard =
    reinterpret_cast<StepBoard*>(pRegion);
  
  return pStepBoard;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void StepBoard::operator delete(void* pStepBoardAddress)
{
  StepBoard* pStepBoard =
    reinterpret_cast<StepBoard*>(pStepBoardAddress);
  
  free(pStepBoard);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
StepBoard::StepBoard(i8 iThreads)
{
  vReset();
  
  m_iThreads = iThreads;
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
StepBoard::~StepBoard(void)
{
  
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dump the step board                                                        */
/*----------------------------------------------------------------------------*/
void StepBoard::vContentDump(void)
{
  vInfo("Thread Step Board:\n"_Str);
  for (i8 iThread = 0; iThread < m_iThreads; iThread++)
    vInfo("Thread %2lld: step %16s point %lld\n"_Str,
           iThread,
           m_sStepName[iThread].s(),
           m_iStepPoint[iThread]
           );
}

/*----------------------------------------------------------------------------*/
/* set step name of thread                                                    */
/*----------------------------------------------------------------------------*/
void StepBoard::vName(i8 iThread, Str* aName)
{
  if (iThread < 0 || iThread >= C_STEPBOARD_THREADS) {
    vFatal("iThread error: %lld\n"_Str,iThread);
    exit(EXIT_FAILURE);
  }

  m_sStepName[iThread] = aName;
}

/*----------------------------------------------------------------------------*/
/* set step point of thread                                                   */
/*----------------------------------------------------------------------------*/
void StepBoard::vPoint(i8 iThread, i8 iPoint)
{
  if (iThread < 0 || iThread >= C_STEPBOARD_THREADS) {
    vFatal("iThread error: %lld\n"_Str,iThread);
    exit(EXIT_FAILURE);
  }
  
  m_iStepPoint[iThread] = iPoint;
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
void StepBoard::vReset(void)
{
  m_iThreads = 0;
  
  for (i8 iThread = 0; iThread < C_STEPBOARD_THREADS; iThread++) {
    m_sStepName[iThread]= "<none>"_Str;
    m_iStepPoint[iThread] = -1;
  }
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
