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
  public:
    eST     m_eSketch;
    s256    m_sSymbol;

  protected:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);

    SketchTkn(void);
    ~SketchTkn(void) noexcept(false);
    
  protected:
    Footer* aFooter(void);
  };

  SketchTkn* _pSketchTkn(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
