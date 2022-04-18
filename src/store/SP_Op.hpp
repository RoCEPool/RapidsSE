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

    Stz* sz(Op* pOp);
  
    Stz* szAck(Op* pOp);
    Stz* szExHandler(Op* pOp);
    Stz* szLeading(Op* pOp);
    Stz* szSpot(Op* pOp);
    Stz* szStream(Op* pOp, bool bDescend=false);
  
  private:
    void vOp(Op* pOp, bool bDescend=false);

    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
