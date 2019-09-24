/*============================================================================*/
/* Operand.hpp -- Operand                                                     */
/*============================================================================*/
#pragma once

namespace Streams
{
  class Operand : public AlignedObject,
                  public ListNode_t<Operand>
  {
  private:
    Word m__Padding[5];   // padding to cache line

  public:
    Operand(eCN eClass);
    ~Operand(void) noexcept(false);
  };
  
  Operand* _pOperand(void* aAddr);
  bool _bIsValidOperand(eCN eClass);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
