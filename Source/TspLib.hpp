/*============================================================================*/
/* TspLib.hpp -- TimeStamp library                                            */
/*============================================================================*/
#pragma once

namespace System
{
  class TspLib : public Object
  {
  public:
    
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    
    TspLib(void);
  ~TspLib(void) noexcept(false);
    
    static Dtm wGetLocalNow(void);
    static Dtm wGetUTCNow(void);
  
    static Dtm wToLocal(Since* pSince);
    static Dtm wToUTC(Since* pSince);

    Footer* aFooter(void);
  };

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
