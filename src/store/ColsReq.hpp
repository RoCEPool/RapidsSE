/*============================================================================*/
/* ColsReq.hpp -- Columns Required Metadata                                   */
/*============================================================================*/
#pragma once

#include "BASE.h"

namespace STORE
{
  class ColsReq : public Object
  {
    static const i8 C_COLSREQ = 128;

  public:

    i8 m_nCols;

    struct {
      i8   m_i_Col_base1;
      s32  m_sColNm;
      bool m_bPayload;
    } m_axCols[C_COLSREQ];

  protected:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);

    ColsReq(void);

   ~ColsReq(void) noexcept(false);

    void vAddCol(i8 i_Col_base1, Stz* szColNm, bool bPayLoad=false);

    void vDump(i8 i_Tab=0);

    void vValidate(void);

  protected:
    Footer* aFooter(void);
  };

  ColsReq* _pColsReq(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/