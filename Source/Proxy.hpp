/*============================================================================*/
/* Proxy.hpp - Proxy                                                          */
/*============================================================================*/
#pragma once

namespace GridModel
{
  class Proxy : public Object
  {
  private:
    
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    Proxy(void);
    ~Proxy(void);
    
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
