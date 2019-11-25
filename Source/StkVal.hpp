/*============================================================================*/
/* StkVal.hpp -- Base Class for a Self-Describing Values on Stack             */
/*============================================================================*/
#pragma once

namespace SenVM
{
  class StkVal : public _Header
  {
  protected:
   
  public:
    void* operator new(size_t);
  
    eCN  eClass(void);
    i8   nStackBytes(void);
    void vToString(Stz* aString);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
