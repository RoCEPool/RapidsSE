/*============================================================================*/
/* ColPpg.cpp -- Columns Propagated Metadata                                  */
/*============================================================================*/

#include "ColPpg.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE, ColPpg, CN_COLPPG)
__aNewFunction(ColPpg, CN_COLPPG)
__vDeleteFunction(ColPpg, CN_COLPPG)
__aFooterFunction(ColPpg)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
ColPpg::ColPpg(i8 nCrwCols, const char* azFile, i8 iLine)
    : AbstractDef(CN_COLPPG, azFile, iLine)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_COLPPG, _FOOTER_);

  m_nArriving = nCrwCols;

  for (i8 i_Col = 0; i_Col < C_COLPPG; i_Col++) {
    m_axCol[i_Col].m_i_CellIn  = C_UNKNOWN;
    m_axCol[i_Col].m_i_StructCell = C_UNKNOWN;
  }
  m_nPropagated = 0;

  _postcondition_(_ADDRESS_, _MAGIC_, CN_COLPPG, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
ColPpg::~ColPpg(void) noexcept(false)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_COLPPG, _FOOTER_);

}


/*----------------------------------------------------------------------------*/
/*-- static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/*-- public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* add propagated column                                                      */
/*----------------------------------------------------------------------------*/
void ColPpg::vAdd(i8 i_CellIn, i8 i_CellOut, Stz* szCn_memberNm,
                  Stz* szNote1, Stz* szNote2)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_COLPPG, _FOOTER_);

  m_axCol[m_nPropagated].m_i_CellIn  = i_CellIn;
  m_axCol[m_nPropagated].m_i_StructCell = i_CellOut;

  if (szCn_memberNm)
    m_axCol[m_nPropagated].m_sCn_memberNm = szCn_memberNm;

  if (szNote1)
    m_axCol[m_nPropagated].m_sNote1 = szNote1;
  if (szNote2)
    m_axCol[m_nPropagated].m_sNote2 = szNote2;

  m_nPropagated++;
}

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void ColPpg::vDump(i8 i_Tab)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_COLPPG, _FOOTER_);
  UNUSED(i_Tab);

  s256 sNotes;

  vTrace("               %s:%lld\n"_Stz,m_sFile.szStz(),m_iLine);
  vTrace("%s%snArriving %lld\n"_Stz,
         Stz::szTab(i_Tab),
         Stz::szSpaces(7),
         m_nArriving);

  for (i8 i_Col = 0; i_Col < m_nPropagated; i_Col++) {
    if (*m_axCol[i_Col].m_sNote1.azChar() || *m_axCol[i_Col].m_sNote2.azChar()) {
      sNotes.vSprint("(%s %s)"_Stz,
                     m_axCol[i_Col].m_sNote1.azChar(),
                     m_axCol[i_Col].m_sNote2.azChar());
    }
    else {
      sNotes = ""_Stz;
    }

    vTrace("%s%sm_axCol[%lld] : m_i_CellIn [%lld] -> m_i_StructCell [%lld] %s %s\n"_Stz,
           Stz::szTab(i_Tab),
           Stz::szSpaces(7),
           i_Col,

           m_axCol[i_Col].m_i_CellIn,
           m_axCol[i_Col].m_i_StructCell,
           m_axCol[i_Col].m_sCn_memberNm.szStz(),
           sNotes.azChar());
  }
}

/*----------------------------------------------------------------------------*/
/* validate                                                                   */
/*----------------------------------------------------------------------------*/
void ColPpg::vValidate(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_COLPPG, _FOOTER_);

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
