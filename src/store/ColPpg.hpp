/*============================================================================*/
/* ColPpg.hpp -- Columns Propagated Metadata                                  */
/*============================================================================*/
#pragma once

#include "BASE.h"

namespace STORE
{
  class ColPpg : public Object
  {
    static const i8 C_COLPPG = 128;

  public:

    s32  m_sRefer;

    i8 m_nArriving;
    i8 m_nPropagated;

    struct {
      i8   m_i_CellIn;     // in cell of struct (base 0)
      i8   m_i_StructCell; // out cell of struct (base 0)

      s256 m_sCn_memberNm; // member name

      s32  m_sNote1;       // note
      s32  m_sNote2;       // note
    } m_axCol[C_COLPPG];

  protected:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);

    ColPpg(i8 nCrwCols);

   ~ColPpg(void) noexcept(false);

    void vAdd(i8 i_CellIn, i8 i_CellOut, Stz* szCn_memberNm,
              Stz* szNote1= nullptr, Stz* szNote2= nullptr);

    void vDump(i8 i_Tab=0);

    void vValidate(void);

  protected:
    Footer* aFooter(void);
  };

  ColPpg* _pColPpg(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/