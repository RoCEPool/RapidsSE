/*============================================================================*/
/* ColInj.hpp -- Columns Required Metadata                                */
/*============================================================================*/
#pragma once

#include "BASE.h"

namespace STORE
{
  class ColInj : public Object
  {
    static const i8 C_COLINJ = 128;

  public:

    i8 m_nCrwCols;
    i8 m_nCols;

    struct {
      i8   m_i_CrwCol;     // column of compact row (base 0)
      i8   m_i_StructCell; // cell of struct (base 0)
      s32  m_sCn_memberNm; // member name

      bool m_bPriorCrwCol; // is same m_i_CrwCol as prior

      s32  m_sNote1;       // note
      s32  m_sNote2;       // note
    } m_axCol[C_COLINJ];

  protected:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);

    ColInj(i8 nCrwCols);

   ~ColInj(void) noexcept(false);

    void vAdd(i8 i_CrwCol, i8 i_StructCell, Stz* szCn_memberNm,
              Stz* szNote1= nullptr, Stz* szNote2= nullptr);

    void vDump(i8 i_Tab=0);

    void vRepeatsPrior(void);

    void vValidate(void);

  protected:
    Footer* aFooter(void);
  };

  ColInj* _pColInj(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/