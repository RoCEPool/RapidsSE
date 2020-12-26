/*============================================================================*/
/* AlignedObject.hpp -- Base Class for Aligned Objects                        */
/*============================================================================*/
#pragma once

namespace BASE
{
  class AlignedObject : public Object
  {
  private:
  
  public:
    AlignedObject(eCN eClass, i8 nChildSize);
  
    static bool bOnBoundary(void* aAddress, i8 nByteBoundary);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
