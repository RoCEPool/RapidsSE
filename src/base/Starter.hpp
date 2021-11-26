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

    i8     m_nStarters;
    Atomic m_nxReady;
  
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Starter(void);
   ~Starter(void) noexcept(false);

    void vDropReadLock(void);
    void vDropWriteLock(void);
    i8   nGetReady(void);
    i8   nGetStarters(void);
    void vIncReady(void);
    void vIncStarters(void);
    void vGrabReadLock(void);
    void vGrabWriteLock(void);

    Footer* aFooter(void);
  };

  Starter* _pStarter(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
