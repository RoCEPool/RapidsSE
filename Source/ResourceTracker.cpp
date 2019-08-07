/*============================================================================*/
/* ResourceTracker.cpp - Resource Tracker                                     */
/*============================================================================*/

#include "Resources.h"

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
ResourceTracker::ResourceTracker(void)
: Object(CN_RESOURCETRACKER)
{
  _precondition_(_MAGIC_,CN_RESOURCETRACKER,_FOOTER_);
 
  m_ixDirOpen.vCheckInit();
  m_ixFileOpen.vCheckInit();
  m_ixMallocAmt.vCheckInit();
  m_ixMutexOpen.vCheckInit();
  m_ixShmOpen.vCheckInit();
  m_ixSocketOpen.vCheckInit();
  m_ixThreadOpen.vCheckInit();

  _checkInitialised_(CN_RESOURCETRACKER);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
ResourceTracker::~ResourceTracker(void) noexcept(false)
{
  _precondition_(_MAGIC_,CN_RESOURCETRACKER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* add                                                                        */
/*----------------------------------------------------------------------------*/
void ResourceTracker::vAddMallocAmt(i8 nBytes)
{
  i8 iMallocAmt = m_ixMallocAmt.iAdd(nBytes);
  RASSERT(iMallocAmt >= 0);
}

/*----------------------------------------------------------------------------*/
/* sub                                                                        */
/*----------------------------------------------------------------------------*/
void ResourceTracker::vSubMallocAmt(i8 nBytes)
{
  i8 iMallocAmt = m_ixMallocAmt.iAdd( - nBytes);
  RASSERT(iMallocAmt >= 0);
}

/*----------------------------------------------------------------------------*/
/* decrement                                                                  */
/*----------------------------------------------------------------------------*/
void ResourceTracker::vDecDirOpen(void)
{
  i8 iDirOpen = --m_ixDirOpen;
  RASSERT(iDirOpen >= 0);
}

/*----------------------------------------------------------------------------*/
/* decrement                                                                  */
/*----------------------------------------------------------------------------*/
void ResourceTracker::vDecFileOpen(void)
{
  i8 iFileOpen = --m_ixFileOpen;
  RASSERT(iFileOpen >= 0);
}

/*----------------------------------------------------------------------------*/
/* decrement                                                                  */
/*----------------------------------------------------------------------------*/
void ResourceTracker::vDecMutexOpen(void)
{
  i8 iMutexOpen = --m_ixMutexOpen;
  RASSERT(iMutexOpen >= 0);
}

/*----------------------------------------------------------------------------*/
/* decrement                                                                  */
/*----------------------------------------------------------------------------*/
void ResourceTracker::vDecShmOpen(void)
{
  i8 iShmOpen = --m_ixShmOpen;
  RASSERT(iShmOpen >= 0);
}

/*----------------------------------------------------------------------------*/
/* decrement                                                                  */
/*----------------------------------------------------------------------------*/
void ResourceTracker::vDecSocketOpen(void)
{
  i8 iSocketOpen = --m_ixSocketOpen;
  RASSERT(iSocketOpen >= 0);
}

/*----------------------------------------------------------------------------*/
/* decrement                                                                  */
/*----------------------------------------------------------------------------*/
void ResourceTracker::vDecThreadOpen(void)
{
  i8 iThreadOpen = --m_ixThreadOpen;
  RASSERT(iThreadOpen >= 0);
}

/*----------------------------------------------------------------------------*/
/* increment                                                                  */
/*----------------------------------------------------------------------------*/
void ResourceTracker::vIncDirOpen(void)
{
  i8 iDirOpen = ++m_ixDirOpen;
  RASSERT(iDirOpen > 0);
}

/*----------------------------------------------------------------------------*/
/* increment                                                                  */
/*----------------------------------------------------------------------------*/
void ResourceTracker::vIncFileOpen(void)
{
  i8 iFileOpen = ++m_ixFileOpen;
  RASSERT(iFileOpen > 0);
}

/*----------------------------------------------------------------------------*/
/* increment                                                                  */
/*----------------------------------------------------------------------------*/
void ResourceTracker::vIncMutexOpen(void)
{
  ++m_ixMutexOpen;
}

/*----------------------------------------------------------------------------*/
/* increment                                                                  */
/*----------------------------------------------------------------------------*/
void ResourceTracker::vIncShmOpen(void)
{
  i8 iShmOpen = ++m_ixShmOpen;
  RASSERT(iShmOpen > 0);
}

/*----------------------------------------------------------------------------*/
/* increment                                                                  */
/*----------------------------------------------------------------------------*/
void ResourceTracker::vIncSocketOpen(void)
{
  i8 iSocketOpen = ++m_ixSocketOpen;
  RASSERT(iSocketOpen > 0);
}

/*----------------------------------------------------------------------------*/
/* increment                                                                  */
/*----------------------------------------------------------------------------*/
void ResourceTracker::vIncThreadOpen(void)
{
  i8 iThreadOpen = ++m_ixThreadOpen;
  RASSERT(iThreadOpen > 0);
}

/*----------------------------------------------------------------------------*/
/* get                                                                        */
/*----------------------------------------------------------------------------*/
i8 ResourceTracker::nDirOpen(void)
{
  i8 iDirOpen = *m_ixDirOpen;
  
  return iDirOpen;
}

/*----------------------------------------------------------------------------*/
/* get                                                                        */
/*----------------------------------------------------------------------------*/
i8 ResourceTracker::nFileOpen(void)
{
  i8 iFileOpen = *m_ixFileOpen;
  
  return iFileOpen;
}

/*----------------------------------------------------------------------------*/
/* get                                                                        */
/*----------------------------------------------------------------------------*/
i8 ResourceTracker::nMallocAmt(void)
{
  i8 iMallocAmt = *m_ixMallocAmt;
  
  return iMallocAmt;
}

/*----------------------------------------------------------------------------*/
/* get                                                                        */
/*----------------------------------------------------------------------------*/
i8 ResourceTracker::nMutexOpen(void)
{
  i8 iMutexOpen = *m_ixMutexOpen;
  
  return iMutexOpen;
}

/*----------------------------------------------------------------------------*/
/* get                                                                        */
/*----------------------------------------------------------------------------*/
i8 ResourceTracker::nShmOpen(void)
{
  i8 iShmOpen = *m_ixShmOpen;
  
  return iShmOpen;
}

/*----------------------------------------------------------------------------*/
/* get                                                                        */
/*----------------------------------------------------------------------------*/
i8 ResourceTracker::nSocketOpen(void)
{
  i8 iSocketOpen = *m_ixSocketOpen;
  
  return iSocketOpen;
}

/*----------------------------------------------------------------------------*/
/* get                                                                        */
/*----------------------------------------------------------------------------*/
i8 ResourceTracker::nThreadOpen(void)
{
  i8 iThreadOpen = *m_ixThreadOpen;
  
  return iThreadOpen;
}

/*----------------------------------------------------------------------------*/
/* monitor portrait                                                           */
/*----------------------------------------------------------------------------*/
void ResourceTracker::vMonitorPortrait(s48K& sPortrait)
{
  s48K s;
  
  s = ""_Stz;

  sPortrait.vSprintCat("Resources: "_Stz);
  sPortrait.vSprintCat("dir %lld file %lld malloc %lldB mutex %lld "
                       "shm %lld socket %lld thread %lld\n"_Stz,
                       nDirOpen(),
                       nFileOpen(),
                       nMallocAmt(),
                       nMutexOpen(),
                       nShmOpen(),
                       nSocketOpen(),
                       nThreadOpen());
}

/*----------------------------------------------------------------------------*/
/* report                                                                     */
/*----------------------------------------------------------------------------*/
void ResourceTracker::vReport(void)
{
  vTrace("Resources:\n"_Stz);

  vTrace("  dir %lld\n"_Stz,     nDirOpen());
  vTrace("  file %lld\n"_Stz,    nFileOpen());
  vTrace("  malloc %lldB\n"_Stz, nMallocAmt());
  vTrace("  mutex %lld\n"_Stz,   nMutexOpen());
  vTrace("  shm %lld\n"_Stz,     nShmOpen());
  vTrace("  socket %lld\n"_Stz,  nSocketOpen());
  vTrace("  thread %lld\n"_Stz,  nThreadOpen());
}


/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* ResourceTracker::aFooter(void)
{
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
