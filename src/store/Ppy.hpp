/*============================================================================*/
/* Ppy.hpp -- Abstract Property                                               */
/*============================================================================*/
#pragma once

#include "BASE.h"

namespace STORE
{
  class Ppy : public Object
  {
  public:
    s256  m_sPpy;

  protected:
    // must be last
    Footer m_xFooter;

  public:
    Ppy(void);
    Ppy(i8 iPpy);
    Ppy(Ppy* pPpy);
    Ppy(Ppy& xPpy);
    Ppy(Stz* szPpy);
    ~Ppy(void) noexcept(false);

    void operator =(i8   iPpy);
    //void operator =(Ppy& rxPpy);
    void operator =(Ppy  xPpy);
    void operator =(Stz* szPpy);

    i8   iPpy(void);
    Stz* szPpy(void);

    void vSet(i8   iPpy);
    void vSet(Stz* szPpy);
    void vSet(Ppy* pPpy);
    void vSet(Ppy& rxPpy);
    void vSet(Ppy  xPpy);

    void vSetSprint(Stz* szFormat, ...);

  protected:
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
