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
void MatrixPrint::vHeader(Stz* szHeading, ...)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_MATRIXPRINT, _FOOTER_);

  va_list args;
  va_start(args, szHeading);

  i8 i_Col = 0;
  while (szHeading) {
    vPutHeading(szHeading, i_Col);
    szHeading = va_arg(args, Stz*);
    i_Col++;
  }

  va_end(args);
}

/*----------------------------------------------------------------------------*/
/* add row                                                                    */
/*----------------------------------------------------------------------------*/
void MatrixPrint::vRow(Stz* szValue, ...)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_MATRIXPRINT, _FOOTER_);

  va_list args;
  va_start(args,szValue);

  i8 i_Col = 0;
  while (szValue) {
    vPutValue(szValue,m_nRows,i_Col);
    szValue = va_arg(args,Stz*);
    i_Col++;
  }

  va_end(args);

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

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // heading

  for (i8 i_Col = 0; i_Col < nCols; i_Col++) {
    if (i_Col)
      sOutput.vConcat(" "_Stz);
    vConcatHeading(sOutput, i_Col);
  }
  sOutput.vConcat("\n"_Stz);

  for (i8 i_Col = 0; i_Col < nCols; i_Col++) {
    if (i_Col)
      sOutput.vConcat(" "_Stz);
    vConcatDashes(sOutput, i_Col);
  }
  sOutput.vConcat("\n"_Stz);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // rows

  for (i8 i_Row = 0; i_Row < m_nRows; i_Row++) {
    for (i8 i_Col = 0; i_Col < nCols; i_Col++) {
      if (i_Col)
        sOutput.vConcat(" "_Stz);
      vConcatValue(sOutput, i_Row, i_Col);
    }
    sOutput.vConcat("\n"_Stz);
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // trace
  vTrace("%s"_Stz,sOutput.szStz());

  vDebugPoint();
}

/*----------------------------------------------------------------------------*/
/* reset                                                                      */
/*----------------------------------------------------------------------------*/
void MatrixPrint::vReset(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_MATRIXPRINT, _FOOTER_);

  vResetHeader();
  vResetRows();
}

/*----------------------------------------------------------------------------*/
/*-- private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* determine number of columns                                                */
/*----------------------------------------------------------------------------*/
i8 MatrixPrint::nColumns(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_MATRIXPRINT, _FOOTER_);

  return 1;
}

/*----------------------------------------------------------------------------*/
/* concat heading                                                             */
/*----------------------------------------------------------------------------*/
void MatrixPrint::vConcatDashes(String* pOutput, i8 i_Col)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_MATRIXPRINT, _FOOTER_);

  for (i8 nDashes =  m_iaWidth[i_Col]; nDashes; nDashes--)
    pOutput->vConcat("-"_Stz);
}

/*----------------------------------------------------------------------------*/
/* concat heading                                                             */
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
/* concat column value                                                        */
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
/* reset                                                                      */
/*----------------------------------------------------------------------------*/
void MatrixPrint::vResetHeader(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_MATRIXPRINT, _FOOTER_);

  for (i8 i_Col = 0; i_Col < C_MATRIXPRINT_COL; i_Col++) {
    m_iaWidth[i_Col]  = 0;
    m_saHeader[i_Col] = ""_Stz;
  }

}

/*----------------------------------------------------------------------------*/
/* reset                                                                      */
/*----------------------------------------------------------------------------*/
void MatrixPrint::vResetRows(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_MATRIXPRINT, _FOOTER_);

  m_nRows = 0;
  for (i8 i_Row = 0; i_Row < C_MATRIXPRINT_ROW; i_Row++) {
    for (i8 i_Col = 0; i_Col < C_MATRIXPRINT_COL; i_Col++) {
      m_xaRow[i_Row].m_asCol[i_Col] = ""_Stz;
    }
  }
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
