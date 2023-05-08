/*============================================================================*/
/* ArgDef.hpp -- Args Metadata                                               */
/*============================================================================*/
#pragma once

#include "Meta.hpp"

namespace STORE
{
  class ArgDef : public Meta
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


  protected:
    Footer* aFooter(void);
  };

  ArgDef* _pArgDef(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
