/*============================================================================*/
/* Method.hpp - Kbase Methods                                                  */
/*============================================================================*/
#pragma once

namespace Facts
{
  class Method : public Object
  {
  private:
  
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);

    Method(void);
   ~Method(void) noexcept(false);

  private:
    Footer* aFooter(void);
  };

  Method* _pMethod(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
