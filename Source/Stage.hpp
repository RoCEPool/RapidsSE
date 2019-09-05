/*============================================================================*/
/* Stage.hpp -- Pipe Stage                                                    */
/*============================================================================*/
#pragma once

namespace Operators
{
  class Stage : public Object
  {
  private:
 
    // must be last
    Footer m_xFooter;

  public:
    Stage(void);
   ~Stage(void) noexcept(false);
  
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
