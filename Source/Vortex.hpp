/*============================================================================*/
/* Vortex.hpp -- Vortex                                                       */
/*============================================================================*/
#pragma once

namespace Utilities
{
  class Vortex : public Object
  {
  public:
    File*  m_pVortexFile;
    s256   m_sSourcePath;
    Input* m_pInput;
    Token  m_xToken_ClassNm;
    Token  m_xToken_Other;

    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void  operator delete(void*);
 
    Vortex(void);
   ~Vortex(void) noexcept(false);

  public:
    void vClose(void);
    bool bOpen(Stz* szVortexPath);
    void vParseBase(void);
    void vParseClass(void);
    void vParseClassFunction(void);
    bool bParseMember(void);
    void vParseMemberProper(void);
    void vParseMembers(void);
    void vProcessSource(Stz* szSourcePath);

  private:
    Footer* aFooter(void);
  };

  Vortex* _pVortex(void* aAddr);
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
