/*============================================================================*/
/* Arg.hpp - Set of Candidate Paths                                           */
/*============================================================================*/
#pragma once

namespace Plans
{  
  enum eAK // arg kind
  {
  /* 0 */ AK_NONE=0,
  /* 1 */ AK_COLUMNNM,
  /* 2 */ AK_LITERAL,
  /* 4 */ AK_ENUMS
  };
  
  static Stz* aAK[]
  {
  /* 0 */ "none"_Stz,
  /* 1 */ "columnnm"_Stz,
  /* 2 */ "literal"_Stz,
  };

  class Arg : public Object
  {
  private:
    eAK   m_eArgKind; // arg kind
    s256  m_sArgVal;  // the arg proper
  
    Footer m_xFooter;
    
   public:
    
    void* operator new(size_t);
    void  operator delete(void*);
    
    Arg(void);
    Arg(eAK eArgKind, Stz* aArgVal);
    ~Arg(void) noexcept(false);
  
    eAK eArgKind(void);
    void vArgKind(eAK eArgKind);
  
    void vArgVal(s256& sArgVal);
    s256& sArgVal(void);
  
    bool bIsColumnNm(s256& sName);
  
  
    Footer* aFooter(void);
  };
  
  Arg* _pArg(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
