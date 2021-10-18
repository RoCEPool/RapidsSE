/*============================================================================*/
/* Lit.hpp -- Recognised Literal                                              */
/*============================================================================*/
#pragma once

namespace BASE
{
  class Lit : public Object
  {
  private:

    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void* pLit);
  
    Lit(void);
   ~Lit(void) noexcept(false);
  
    Footer* aFooter(void);
  };
  
  Lit* _pLit(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
