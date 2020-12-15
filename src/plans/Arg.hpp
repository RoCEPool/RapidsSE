/*============================================================================*/
/* Arg.hpp - Set of Candidate Paths                                           */
/*============================================================================*/
#pragma once

#include "eAK.h"

namespace Plans
{  
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
