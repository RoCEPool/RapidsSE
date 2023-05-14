/*============================================================================*/
/* ArgDef.hpp -- Args Metadata                                               */
/*============================================================================*/
#pragma once

#include "BASE.h"

namespace STORE
{
  class ArgDef : public Object
  {
  public:
    i8  m_i_Arg;
    s32 m_sArgNm;
    s2K m_sArgVal;

  protected:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);

    ArgDef(void);
    ~ArgDef(void) noexcept(false);

    i8 i_Arg(void);
    void vArg(i8 i_Arg);

    Stz* szArgNm(void);
    void vArgNm(Stz* szArgNm);

    Stz* szArgVal(void);
    void vArgVal(Stz* szArgVal);

    void vDump(void);

  protected:
    Footer* aFooter(void);
  };

  ArgDef* _pArgDef(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
