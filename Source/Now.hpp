/*============================================================================*/
/* Now.hpp - current date & time                                              */
/*============================================================================*/
#pragma once

namespace System
{
  class Now : public Object
  {
  private:
    Time m_oTime;
    s32  m_sFormatted;

    // must be last
    Footer m_xFooter;
    
  public:
    Now();
    ~Now() noexcept(false);
  
    Stz* sFormatted(void);  
  
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
