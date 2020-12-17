/*============================================================================*/
/* SP_ReqTuple.hpp - ReqTuple sprint helper                                   */
/*============================================================================*/
#pragma once

namespace Resources
{
  class SP_ReqTuple : public SP
  {
  public:

    // must be last
    Footer m_xFooter;
    
  public:
    SP_ReqTuple(void);
   ~SP_ReqTuple(void) noexcept(false);
  
    Stz* sz(ReqTuple* aReqTuple);

    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
