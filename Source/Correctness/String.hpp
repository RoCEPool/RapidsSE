/*============================================================================*/
/* String.hpp -- String (maps over String_t<>)                                */
/*============================================================================*/
#pragma once

namespace Correctness
{
  class String
  {
  public:
    Header m_xHeader;
    i8     m_nSize;       // size of char area
  
    char   m_caStr[1];    // char area

    // m_xFooter which is :
    //   this + sizeof(m_xHeader) + sizeof(m_nSize) + m_nSize
  
  public:
    void* operator new(size_t);
  
    String(void);
   ~String(void) noexcept(false);

    void  vCopy(Stz* szArg);
  
    void  vCatSprint(Stz* szFormat, ...);
    void  vConcat(Stz* szArg);
    i8    nLength(void);
    i8    nSize(void);
    i8    nStackUsage(void);
    void  vSprint(Stz* szFormat, ...);
    char* azStr(void);
    Stz*  szStz(void);
  
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
