/*============================================================================*/
/* ColMap.hpp -- Column Mapping                                               */
/*============================================================================*/
#pragma once

#include "BASE.h"

namespace STORE
{
  class ColMap : public Object
  {
    static const i8 C_COLMAP = 128;

  public:

    i8 m_nCols;

    struct {
      bool m_bPayload;      //    payload or key?
      i8   m_i_Cell;        //    cell (base 0) of payload/key
                            // ->
      i8   m_i_EmittedCol;  //    emitted column $n
    } m_axCol[C_COLMAP];

  protected:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);

    ColMap(void);

   ~ColMap(void) noexcept(false);

    void vAdd3(bool bPayLoad, i8 i_Cell, i8 i_EmittedCol);

    void vDump(i8 i_Tab=0);

    void vValidate(void);

  protected:
    Footer* aFooter(void);
  };

  ColMap* _pColMap(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/