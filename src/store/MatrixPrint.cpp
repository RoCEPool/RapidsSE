/*============================================================================*/
/* MatrixPrint.cpp -- Matrix Print                                            */
/*============================================================================*/

#include "MatrixPrint.hpp"

#include "CardIter.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE, MatrixPrint, CN_MATRIXPRINT)
__aNewFunction(MatrixPrint, CN_MATRIXPRINT)
__vDeleteFunction(MatrixPrint, CN_MATRIXPRINT)
__aFooterFunction(MatrixPrint)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
MatrixPrint::MatrixPrint(void)
    : Object(CN_MATRIXPRINT)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_MATRIXPRINT, _FOOTER_);

  vReset();

  _checkInitialised_(CN_MATRIXPRINT);

  _postcondition_(_ADDRESS_, _MAGIC_, CN_MATRIXPRINT, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
MatrixPrint::~MatrixPrint(void) noexcept(false)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_MATRIXPRINT, _FOOTER_);

}


/*----------------------------------------------------------------------------*/
/*-- static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*-- public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* add header                                                                 */
/*----------------------------------------------------------------------------*/
void MatrixPrint::vHeading(Stz* szHeader0, ...)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_MATRIXPRINT, _FOOTER_);

  UNUSED(szHeader0);
}

/*----------------------------------------------------------------------------*/
/* add row                                                                    */
/*----------------------------------------------------------------------------*/
void MatrixPrint::vRow(Stz* szCol0, ...)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_MATRIXPRINT, _FOOTER_);

  UNUSED(szCol0);

  m_nRows++;
}

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void MatrixPrint::vDump(i8 i_Tab)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_MATRIXPRINT, _FOOTER_);
  UNUSED(i_Tab);

  s512K sOutput;

  i8 nCols = nColumns();

  for (i8 i_Col = 0; i_Col < nCols; i_Col++) {
    vConcatHeading(sOutput, i_Col);
  }

  for (i8 i_Row = 0; i_Row < m_nRows; i_Row++) {
    for (i8 i_Col = 0; i_Col < nCols; i_Col++) {
      vConcatValue(sOutput, i_Row, i_Col);
    }

  }

  vTrace("%s"_Stz,sOutput.szStz());

  vDebugPoint();
}

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void MatrixPrint::vReset(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_MATRIXPRINT, _FOOTER_);

  for (i8 i_Col = 0; i_Col < C_MATRIXPRINT_COL; i_Col++) {
    m_iaWidth[i_Col]  = 0;
    m_saHeader[i_Col] = ""_Stz;
  }

  m_nRows = 0;
  for (i8 i_Row = 0; i_Row < C_MATRIXPRINT_ROW; i_Row++) {
    for (i8 i_Col = 0; i_Col < C_MATRIXPRINT_COL; i_Col++) {
      m_xaRow[i_Row].m_asCol[i_Col] = ""_Stz;
    }
  }
}

/*----------------------------------------------------------------------------*/
/*-- private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* append heading                                                             */
/*----------------------------------------------------------------------------*/
void MatrixPrint::vConcatHeading(String* pOutput, i8 i_Col)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_MATRIXPRINT, _FOOTER_);

  Stz* szHeading = m_saHeader[i_Col];

  s32 sFormat;
  vFormat(sFormat,i_Col);

  pOutput->vCatSprint(sFormat.szStz(),szHeading);
}

/*----------------------------------------------------------------------------*/
/* append column value                                                        */
/*----------------------------------------------------------------------------*/
void MatrixPrint::vConcatValue(String* pOutput, i8 i_Row, i8 i_Col)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_MATRIXPRINT, _FOOTER_);

  Stz* szValue = m_xaRow[i_Row].m_asCol[i_Col];

  s32 sFormat;
  vFormat(sFormat,i_Col);

  pOutput->vCatSprint(sFormat.szStz(), szValue);
}

/*----------------------------------------------------------------------------*/
/* determine number of columns                                                */
/*----------------------------------------------------------------------------*/
i8 MatrixPrint::nColumns(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_MATRIXPRINT, _FOOTER_);

  return 1;
}

/*----------------------------------------------------------------------------*/
/* craft format for given column                                              */
/*----------------------------------------------------------------------------*/
void MatrixPrint::vFormat(String* pFormat, i8 i_Col)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_MATRIXPRINT, _FOOTER_);

  pFormat->vSprint("%%%llds"_Stz,m_iaWidth[i_Col]);
}

/*----------------------------------------------------------------------------*/
/* put heading                                                                */
/*----------------------------------------------------------------------------*/
void MatrixPrint::vPutHeading(Stz* szHeading, i8 i_Col)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_MATRIXPRINT, _FOOTER_);

  m_saHeader[i_Col] = szHeading;

  vWidth(szHeading, i_Col);
}

/*----------------------------------------------------------------------------*/
/* put value into column cell                                                 */
/*----------------------------------------------------------------------------*/
void MatrixPrint::vPutValue(Stz* szValue, i8 i_Row, i8 i_Col)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_MATRIXPRINT, _FOOTER_);

  m_xaRow[i_Row].m_asCol[i_Col] = szValue;

  vWidth(szValue, i_Col);
}

/*----------------------------------------------------------------------------*/
/* update column width for given argument                                     */
/*----------------------------------------------------------------------------*/
void MatrixPrint::vWidth(Stz* szArg, i8 i_Col)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_MATRIXPRINT, _FOOTER_);

  i8 nLen = szArg->nLength();
  if (nLen > m_iaWidth[i_Col])
    m_iaWidth[i_Col] = nLen;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
