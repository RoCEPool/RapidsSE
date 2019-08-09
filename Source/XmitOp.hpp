/*============================================================================*/
/* XmitOp.hpp -- Transmit                                                     */
/*============================================================================*/
#pragma once

namespace Operators
{
  class XmitOp : public Op
  {
  private:
  
    // must be last
    Footer m_xFooter;

  public:
    XmitOp(Stz* szOpLabel, Digest* pDigest);
   ~XmitOp(void) noexcept(false);
  
  private:
    Footer* aFooter(void);
  };

  XmitOp* _pXmitOp(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
