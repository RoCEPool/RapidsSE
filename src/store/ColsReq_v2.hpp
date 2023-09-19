/*============================================================================*/
/* ColsReq_v2.hpp -- Columns Required Metadata                                   */
/*============================================================================*/
#pragma once

#include "BASE.h"

namespace STORE
{
  class ColsReq_v2 : public Object
  {
    static const i8 C_COLSREQ = 128;

  public:

    i8 m_nCols;

    struct {
      i8   m_i_CrwCol;     // column of compact row (base 0)
      i8   m_i_StructCell; // cell of struct (base 0)
    } m_axCols[C_COLSREQ];

  protected:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);

    ColsReq_v2(void);

   ~ColsReq_v2(void) noexcept(false);

    void vAddMapping(i8 i_CrwCol, i8 i_StructCell);

    void vDump(i8 i_Tab=0);

    void vValidate(void);

  protected:
    Footer* aFooter(void);
  };

  ColsReq_v2* _pColsReq_v2(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/