/*============================================================================*/
/* IntVar.hpp - integer sen variable                                          */
/*============================================================================*/
#pragma once

namespace SenVM
{
  class IntVar
  {
  private:
    _Header m_xHeader;
    i8      m_iInt;

  public:
    void* operator new(size_t);
  
    void operator =(i8 iVal);
  
    i8   iInt(void);
    i8   nSize(void);
    void vToString(Stz* szStz);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
