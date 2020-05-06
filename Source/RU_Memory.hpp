/*============================================================================*/
/* RU_Memory.hpp - Memory Resource Ussage                                     */
/*============================================================================*/
#pragma once

namespace Gadgets
{
  class RU_Memory : public RU
  {
  public:

    // must be last
    Footer m_xFooter;
    
  public:
    RU_Memory(void);
   ~RU_Memory(void) noexcept(false);
  
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
