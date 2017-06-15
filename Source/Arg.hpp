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
  
  static Str* aAM[]
  {
  /* 0 */ "none"_Str,
  /* 1 */ "columnnm"_Str,
  /* 2 */ "literal"_Str,
  };

  class Arg : public Object
  {
  private:
    eAK   m_eArg;    // arg kind
    s256  m_sStrVal; // the arg proper
  
    Footer m_xFooter;
    
   public:
    
    void* operator new(size_t);
    void  operator delete(void*);
    
    Arg(void);
    ~Arg(void);
  
    Footer* pFooter(void);
  };
  
  Arg* _pArg(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
