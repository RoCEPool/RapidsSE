/*============================================================================*/
/* OperandBuffer.hpp -- Operand Buffer                                        */
/*============================================================================*/
#pragma once

namespace MetaData
{
  class OperandBuffer : public LinkedBuffer,
                        public ListNode_t<OperandBuffer>
  {
  private:
    Word m__Padding[6];   // padding to cache line

  public:
    OperandBuffer(eCN eClass);
    ~OperandBuffer(void) noexcept(false);
  };
  
  OperandBuffer* _pOperandBuffer(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
