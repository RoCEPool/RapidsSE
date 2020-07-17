/*============================================================================*/
/* Now.hpp - Now sprint helper                                                */
/*============================================================================*/
#pragma once

namespace Gadgets
{
  class Now : public Object
  {
  //private:
  public:
    char m_caResult[32];
    i8   m_iSinceEpoch;
    Tsp  m_wTsp;

    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void operator delete(void*);

    Now(void);
    ~Now(void) noexcept(false);
  
    void vSetNow(void);
    void vSetNowUTC(void);
  
    i8   iSinceEpoch(void);

    Stz* sz(void);
    Stz* sz(Dtm* pTsp);

    i8   wTsp(void);
  
    Footer* aFooter(void);
  };

  Now* _pNow(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
