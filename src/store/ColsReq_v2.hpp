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
      i8   m_i_EmittedCol;  // emitted column $n

      bool m_bPayload;      // payload or key?
      i8   m_i_Cell;        // cell (base 0) of payload/key

      //s32  m_sColNm;
    } m_axCols[C_COLSREQ];

  protected:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);

    ColsReq_v2(void);

   ~ColsReq_v2(void) noexcept(false);

    void vAddCol(i8 i_EmittedCol, bool bPayLoad, i8 i_Cell);

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