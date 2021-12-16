/*============================================================================*/
/* Starter.hpp -- Starter                                                     */
/*============================================================================*/
#pragma once

namespace BASE
{
  class Starter : public Object
  {
  public:

    Lock   m_okLock;

    i8     m_nStarted;
    Atomic m_nxReady;
  
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Starter(void);
   ~Starter(void) noexcept(false);

    void vDecReady(void);
    void vDropReadLock(void);
    void vDropWriteLock(void);
    i8   nGetReady(void);
    i8   nGetStarted(void);
    void vGrabReadLock(void);
    void vGrabWriteLock(void);
    void vIncReady(void);
    void vIncStarted(void);
    void vResetStarter(void);
    void vStatus(void);

    Footer* aFooter(void);
  };

  Starter* _pStarter(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
