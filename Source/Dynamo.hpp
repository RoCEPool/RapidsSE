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
    Dynamo(void);
   ~Dynamo(void) noexcept(false);
  
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
