/*============================================================================*/
/* StrVar.hpp - String Sen Variable                                           */
/*============================================================================*/
#pragma once

namespace SenVM
{
  class StrVar : public SenVar
  {
  private:
    i8      m_nBytesFollowing;
    char    m_caStr[C_48K];

  public:
    void* operator new(size_t);
  
    void operator =(Stz* szStz);
  
    Stz* szStz(void);
    i8   nSize(void);
    void vToString(Stz* szStz);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
