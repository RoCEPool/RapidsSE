/*============================================================================*/
/* Xsentinals.hpp --                                                       */
/*============================================================================*/
#pragma once

#include "Xsentinals.h"

namespace STORE
{
  class Xsentinals : public Object
                    , public ListExo2_t<Xsentinals>
  {
  public:
    CardIter xBI_x;

    _Xsentinals G;

    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);

    Xsentinals(void);
    Xsentinals(eCN eClass);
   ~Xsentinals(void) noexcept(false);


    void vRestore(void);
    void vSave(void);

    i8   iSentinalsDepth(void);

  private:
    void vCopyFrom(_Xsentinals* aXsentinals);
    _Xsentinals* aXsentinels(void);

    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
