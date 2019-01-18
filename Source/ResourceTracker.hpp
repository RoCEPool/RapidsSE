/*============================================================================*/
/* ResourceTracker.hpp - Resource Tracker                                     */
/*============================================================================*/
#pragma once

namespace Measurement
{
  class ResourceTracker : public Object
  {
  public:
  
    Atomic m_ziDirOpen;
    Atomic m_ziFileOpen;
    Atomic m_ziMallocAmt;
    Atomic m_ziShmOpen;
    Atomic m_ziSocketOpen;
    Atomic m_ziThreadOpen;

    // must be last
    Footer m_xFooter;
    
  public:
    ResourceTracker(void);
    ~ResourceTracker(void) noexcept(false);
  
    void vAddMallocAmt(i8 nBytes);
    void vSubMallocAmt(i8 nBytes);

    void vDecDirOpen(void);
    void vDecFileOpen(void);
    void vDecShmOpen(void);
    void vDecSocketOpen(void);
    void vDecThreadOpen(void);

    void vIncDirOpen(void);
    void vIncFileOpen(void);
    void vIncShmOpen(void);
    void vIncSocketOpen(void);
    void vIncThreadOpen(void);

    i8 nDirOpen(void);
    i8 nFileOpen(void);
    i8 nMallocAmt(void);
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
