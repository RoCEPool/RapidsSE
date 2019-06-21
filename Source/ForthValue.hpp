/*============================================================================*/
/* ForthValue.hpp -- Base Class for a Self-Describing ForthValue              */
/*============================================================================*/
#pragma once

namespace Forth {
  class Cell;
  class ForthWord;
}

namespace Values
{
  class ForthValue
  {
  protected:
    Header m_xHeader;
  
  public:
    void* operator new(size_t);
    ForthValue(eCN eClass);
  
    eCN  eClass(void);
    i8   nSize(void);
    void vToString(Stz* aString);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
