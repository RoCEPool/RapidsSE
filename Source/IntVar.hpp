/*============================================================================*/
/* IntVar.hpp - integer sen variable                                          */
/*============================================================================*/
#pragma once

namespace SenVM
{
  class IntVar : public SenVar
  {
  private:
    i8      m_iInt;

  public:
    void* operator new(size_t);
  
    void operator =(i8 iVal);
    i8   iInt(void);
  
    i8   nStackBytes(void);
    void vToString(Stz* szStz);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
