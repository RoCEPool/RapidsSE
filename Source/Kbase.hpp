/*============================================================================*/
/* Kbase.hpp - Kbase                                                          */
/*============================================================================*/
#pragma once

namespace Facts
{
  class Kbase : public Object
  {
  private:

    Fact* m_pRoot; //
  
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);

    Kbase(void);
   ~Kbase(void) noexcept(false);

  private:
    Footer* aFooter(void);
  };

  Kbase* _pKbase(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
