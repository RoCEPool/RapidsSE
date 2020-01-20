/*============================================================================*/
/* bn.hpp - buffer number                                                     */
/*============================================================================*/
#pragma once

namespace Pool
{
  class bn
  {
  private:
    i8 m_iBufferNo;

  public:
    bn(void);
    bn(i8 iBufferNo);
    ~bn(void) noexcept(false);
    
    bool operator ==(bn n);
    bool operator !=(bn n);
    bool operator >=(bn n);
    bool operator <=(bn n);
    i8 operator *(void);
    bn operator ++(int);
    bn operator +(int i);
    bn operator -(int i);
  };
  
  bn operator "" _bn(unsigned long long i); // user-defined literal suffix
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
