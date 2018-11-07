/*============================================================================*/
/* ResourceTracker.hpp - Resource Tracker                                     */
/*============================================================================*/
#pragma once

namespace System
{
  class ResourceTracker : public Object
  {
  public:
  
    Atomic m_ziDirOpen;
    Atomic m_ziFileOpen;
    Atomic m_ziShmOpen;
    Atomic m_ziSocketOpen;
    Atomic m_ziThreadOpen;

    // must be last
    Footer m_xFooter;
    
  public:
    ResourceTracker(void);
    ~ResourceTracker(void);
  
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

    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
