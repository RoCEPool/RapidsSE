/*============================================================================*/
/* StkVal.hpp -- Base Class for a Self-Describing Values on Stack             */
/*============================================================================*/
#pragma once

namespace SenVM
{
  class StkVal
  {
  protected:
    Header m_xHeader;
  
  public:
    void* operator new(size_t);
    StkVal(eCN eClass);
  
    eCN  eClass(void);
    i8   nStackBytes(void);
    void vToString(Stz* aString);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
