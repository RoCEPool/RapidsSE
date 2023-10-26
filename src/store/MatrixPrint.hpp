/*============================================================================*/
/* MatrixPrint.hpp -- Deck Short Form                                           */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "Board.hpp"

namespace STORE
{
  class MatrixPrint : public Object
  {
    static const i8 C_MATRIXPRINT_ROW = 128;
    static const i8 C_MATRIXPRINT_COL = 16;

  public:
    i8   m_iaWidth[C_MATRIXPRINT_COL];
    s256 m_saHeader[C_MATRIXPRINT_COL];

    i8 m_nRows;
    struct {
      s256 m_asCol[C_MATRIXPRINT_COL];
    } m_xaRow[C_MATRIXPRINT_ROW];

  protected:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);

    MatrixPrint(void);
   ~MatrixPrint(void) noexcept(false);

    void vHeader(Stz* szHeading, ...);
    void vRow(Stz* szValue, ...);

    void vDump(i8 i_Tab=0);

    void vReset(void);

  private:
    i8   nColumns(void);
    void vConcatDashes(String* pOutput, i8 i_Col);
    void vConcatHeading(String* pOutput, i8 i_Col);
    void vConcatValue(String* pOutput, i8 i_Row, i8 i_Col);

    void vFormat(String* pFormat, i8 i_Col);

    void vPutHeading(Stz* szHeading, i8 i_Col);
    void vPutValue(Stz* szValue, i8 i_Row, i8 i_Col);

    void vResetHeader(void);
    void vResetRows(void);

    void vWidth(Stz* szArg, i8 i_Col);

  protected:
    Footer* aFooter(void);
  };

  MatrixPrint* _pMatrixPrint(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/