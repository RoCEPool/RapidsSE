/*============================================================================*/
/* Ppy.hpp -- property                                                        */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "ePY.h"

namespace STORE
{
  class Ppy : public Object
  {
  private:

    ePY     m_ePpy;

    eTN     m_eToken;

    i8      m_iValue;
    Object* m_pValue;
    s256    m_sValue;

    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);

    Ppy(void);
   ~Ppy(void) noexcept(false);

  private:

    Footer* aFooter(void);
  };

  Ppy* _pPpy(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
