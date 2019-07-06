/*============================================================================*/
/* XmitOp.hpp -- Transmit                                                     */
/*============================================================================*/
#pragma once

namespace PushDown
{
  class XmitOp : public Op
  {
  private:
  
    // must be last
    Footer m_xFooter;

  public:
    XmitOp(void);
   ~XmitOp(void) noexcept(false);
  
  private:
    Footer* aFooter(void);
  };

  XmitOp* _pXmitOp(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
