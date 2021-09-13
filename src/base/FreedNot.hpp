/*============================================================================*/
/* FreedNot.hpp -- free chunk in heap (but no in free list)                   */
/*============================================================================*/
#pragma once

namespace BASE
{
  class FreedNot : public Header
  {
  private:
    eCN        m_eWasClass; // what was the class
  
  public:
    void* operator new(size_t, FreedNot* aFreedNot);
    void  operator delete(void*);

    FreedNot(void);
   ~FreedNot(void) noexcept(false);
  
    void vInit(eCN eWasClass);
  
    eCN eWasClass(void);
    void vWasClass(eCN eWasClass);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
