/*============================================================================*/
/* ForthClump.hpp -- Forth Clump                                              */
/*============================================================================*/
#pragma once

namespace Forth
{
  class ForthClump : public Object
  {
  
  private:

    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void  operator delete(void*);

    ForthClump(void);
   ~ForthClump(void) noexcept(false);

  private:
    Footer* aFooter(void);
  };
  
  ForthClump* _pForthClump(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
