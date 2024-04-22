/*============================================================================*/
/* ArgDef.hpp -- Args Metadata                                               */
/*============================================================================*/
#pragma once

#include "BASE.h"
#include "AbstractDef.hpp"
#include "SketchTkn.hpp"
#include "ListExo2_t.hpp"

namespace STORE
{
  class ArgDef : public AbstractDef
               , public ListExo2_t<SketchTkn>
  {
  public:
    i8   m_i_Arg;
    s32  m_sArgNm;
    s2K  m_sArgVal;

  protected:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);

    ArgDef(i8 i_Arg, Stz* szArgNm, Stz* szArgVal, const char* azFile, i8 iLine);
    ~ArgDef(void) noexcept(false);

    i8 i_Arg(void);
    void vArg(i8 i_Arg);

    Stz* szArgNm(void);
    void vArgNm(Stz* szArgNm);

    Stz* szArgVal(void);
    void vArgVal(Stz* szArgVal);

    void vDump(i8 i_Tab=3);

    void vStore(i8 i_Arg, Stz* szArgNm, Stz* szArgVal);

    void vValidate(void);

  protected:
    Footer* aFooter(void);
  };

  ArgDef* _pArgDef(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
