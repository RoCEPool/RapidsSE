/*============================================================================*/
/* XC.hpp -- transform crosscheck                                             */
/*============================================================================*/
#pragma once

#include "XF_TRANSFORM.hpp"


namespace STORE
{
  class XC : public XF_TRANSFORM
  {
  private:

    CardIter* m_pCI;

  protected:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    XC(CardIter* pCI);
   ~XC(void) noexcept(false);

    static void vXC(CardIter* pCI);

  private:

    void vCheck(void);

  protected:
    Footer* aFooter(void);
  };

  XC* _pXC(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
