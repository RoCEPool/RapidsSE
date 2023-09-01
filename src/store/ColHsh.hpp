/*============================================================================*/
/* ColHsh.hpp -- Column Mapping                                               */
/*============================================================================*/
#pragma once

#include "BASE.h"
#include "Card_Hashing.hpp"

namespace STORE
{
  class ColHsh : public Object
  {
    static const i8 C_COLMAP = 128;

  public:

    i8 m_nCols;

    struct {
      i8   m_i_Col;
      eCT  m_eColumnType;

      s32  m_sHashOf;

    } m_axCols[C_COLMAP];

  protected:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);

    ColHsh(void);

   ~ColHsh(void) noexcept(false);

    void vAdd(i8 i_Col, eCT eColumnType, Stz* szHashOf);

    void vDump(i8 i_Tab=0);

    void vUpdate(Card_Hashing* p_hashing);

    void vValidate(void);

  protected:
    Footer* aFooter(void);
  };

  ColHsh* _pColHsh(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/