/*============================================================================*/
/* String.hpp -- String (maps over String_t<>)                                */
/*============================================================================*/
#pragma once

namespace Focals
{
  class String
  {
  public:
    Header m_xHeader;
    i8     m_nSize;       // size of str
    char   m_caStr[1];    // str area

  public:
    void* operator new(size_t);
  
    String(void);
   ~String(void) noexcept(false);

    void  vCopy(Stz* szArg);
  
    i8    nSize(void);
    char* azStr(void);
    Stz*  szStz(void);
  
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
