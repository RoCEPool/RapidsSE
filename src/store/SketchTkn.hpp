/*============================================================================*/
/* SketchTkn.hpp -- Sketch Token                                              */
/*============================================================================*/
#pragma once

#include "BASE.h"
#include "eST.h"

namespace STORE
{
  class SketchTkn : public Object
  {
  private:
    eST     m_eSketch;
    s256    m_sChrSeq;

  protected:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);

    SketchTkn(Stz* szArg);
    ~SketchTkn(void) noexcept(false);

    static eST eEvalSketch(Stz* szArg);

    Stz* szChrSeq(void);
    eST  eSketch(void);

  protected:
    Footer* aFooter(void);
  };

  SketchTkn* _pSketchTkn(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
