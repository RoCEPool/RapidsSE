/*============================================================================*/
/* Definition.hpp -- Definition                                               */
/*============================================================================*/
#pragma once

namespace Streams
{
  class Definition : public Operand
  {
  protected:
  
  public:
    void operator delete(void*);
  
    Definition(eCN eClass);
   ~Definition(void) noexcept(false);
  };

  Definition* _pDefinition(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
