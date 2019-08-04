/*============================================================================*/
/* Digest.hpp -- Operator Set Digest                                          */
/*============================================================================*/
#pragma once

namespace PushDown
{
  class Digest : public Object
  {
  public:
    bool m_bPulsed;
  
  private:
  
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    Digest(void);
   ~Digest(void) noexcept(false);
  
    Footer* aFooter(void);
  };
  
  Digest* _pDigest(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
