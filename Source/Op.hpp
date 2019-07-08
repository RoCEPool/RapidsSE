/*============================================================================*/
/* Op.hpp -- Operator                                                         */
/*============================================================================*/
#pragma once

namespace PushDown
{
  class Op : public Object
  {
  private:
    OperandQueue* m_pOperandQueue;
    //! fp* m_fpOpEntryPt; .. some op function entry pt TBD
    Op* m_pOpOut;
  
  public:
    Op(eCN eClass);
   ~Op(void) noexcept(false);
  };

  Op* _pOp(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
