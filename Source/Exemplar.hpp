/*============================================================================*/
/* Exemplar.hpp -- Best Practive Example Class                                */
/*============================================================================*/
#pragma once

namespace Oblique
{
  class Exemplar : public Object
  {
  private:
 
    // must be last
    Footer m_xFooter;

  public:
    Exemplar(void);
   ~Exemplar(void) noexcept(false);
  
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
