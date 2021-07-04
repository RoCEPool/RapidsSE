/*============================================================================*/
/* SP_Op.hpp - Op sprint helper                                               */
/*============================================================================*/
#pragma once

namespace STORE
{
  class SP_Op : public SPmedium
  {
  public:
  
    // must be last
    Footer m_xFooter;
    
  public:
    SP_Op(void);
    SP_Op(Op* pOp, bool bDescend=false);
   ~SP_Op(void) noexcept(false);

    void vOp(Op* pOp, bool bDescend=false);
  
    Stz* sz(void);
    Stz* sz(Op* pOp);
  
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
