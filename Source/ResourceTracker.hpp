/*============================================================================*/
/* ResourceTracker.hpp - Resource Tracker                                     */
/*============================================================================*/
#pragma once

namespace Measurement
{
  class ResourceTracker : public Object
  {
  public:
  
    Atomic m_ixDirOpen;
    Atomic m_ixFileOpen;
    Atomic m_ixMallocAmt;
    Atomic m_ixMutexOpen;
    Atomic m_ixShmOpen;
    Atomic m_ixSocketOpen;
    Atomic m_ixThreadOpen;

    // must be last
    Footer m_xFooter;
    
  public:
    ResourceTracker(void);
    ~ResourceTracker(void) noexcept(false);
  
    void vAddMallocAmt(i8 nBytes);
    void vSubMallocAmt(i8 nBytes);

    void vDecDirOpen(void);
    void vDecFileOpen(void);
    void vDecMutexOpen(void);
    void vDecShmOpen(void);
    void vDecSocketOpen(void);
    void vDecThreadOpen(void);

    void vIncDirOpen(void);
    void vIncFileOpen(void);
    void vIncMutexOpen(void);
    void vIncShmOpen(void);
    void vIncSocketOpen(void);
    void vIncThreadOpen(void);

    i8 nDirOpen(void);
    i8 nFileOpen(void);
    i8 nMallocAmt(void);
    i8 nMutexOpen(void);
    i8 nShmOpen(void);
    i8 nSocketOpen(void);
    i8 nThreadOpen(void);
  
    void vReport(void);
    void vMonitorPortrait(s48K& sPortrait);

    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
