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
    bool          (*m_fpOpEntryPt)(void);
    Op*           m_pOpOut;
  
  public:
    Op(eCN eClass);
   ~Op(void) noexcept(false);
  
    void vOpLoop(void);
  };

  Op* _pOp(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
