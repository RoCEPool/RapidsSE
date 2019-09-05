/*============================================================================*/
/* Dynamo.hpp -- Dynamo (dynamic plan compiler)                               */
/*============================================================================*/
#pragma once

namespace Dynamics
{
  class Dynamo : public Object
  {
  private:

    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Dynamo(void);
   ~Dynamo(void) noexcept(false);
  
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
