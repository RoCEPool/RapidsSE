/*============================================================================*/
/* Mark.hpp -- Mark                                                           */
/*============================================================================*/
#pragma once

#include "BASE.h"

namespace STORE
{
  class Mark : public Object
  {
  private:
    s256    m_sChrSeq;

  protected:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);

    Mark(void);
    Mark(char cChr);
    Mark(Stz* szChrSeq);

    ~Mark(void);

    char cChr(void);
    void vChr(char cChr);
    Stz* szChrSeq(void);
    void vChrSeq(Stz* szChrSeq);
    void vDump(void);

  protected:
    Footer* aFooter(void);
  };

  Mark* _pMark(void* aAddr);

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
