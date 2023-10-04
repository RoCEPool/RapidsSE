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
ColPpg::ColPpg(i8 nCrwCols)
    : Object(CN_COLPPG)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_COLPPG, _FOOTER_);

  m_nCrwCols = nCrwCols;

  for (i8 i_Col = 0; i_Col < C_COLPPG; i_Col++) {
    m_axCol[i_Col].m_i_CellIn  = C_UNKNOWN;
    m_axCol[i_Col].m_i_CellOut = C_UNKNOWN;
  }
  m_nCols = 0;

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

  m_axCol[m_nCols].m_i_CellIn  = i_CellIn;
  m_axCol[m_nCols].m_i_CellOut = i_CellOut;

  if (szCn_memberNm)
    m_axCol[m_nCols].m_sCn_memberNm = szCn_memberNm;

  if (szNote1)
    m_axCol[m_nCols].m_sNote1 = szNote1;
  if (szNote2)
    m_axCol[m_nCols].m_sNote2 = szNote2;

  m_nCols++;
}

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void ColPpg::vDump(i8 i_Tab)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_COLPPG, _FOOTER_);
  UNUSED(i_Tab);

  s256 sNotes;

  vTrace("%s%snCrwCols %lld\n"_Stz,
         Stz::szTab(i_Tab),
         Stz::szSpaces(7),
         m_nCrwCols);

  for (i8 i_Col = 0; i_Col < m_nCols; i_Col++) {
    if (*m_axCol[i_Col].m_sNote1.azChar() || *m_axCol[i_Col].m_sNote2.azChar()) {
      sNotes.vSprint("(%s %s)"_Stz,
                     m_axCol[i_Col].m_sNote1.azChar(),
                     m_axCol[i_Col].m_sNote2.azChar());
    }
    else {
      sNotes = ""_Stz;
    }

    vTrace("%s%sm_axCols[%lld] : m_i_CellIn [%lld] -> m_i_CellOut [%lld] %s %s\n"_Stz,
           Stz::szTab(i_Tab),
           Stz::szSpaces(7),
           i_Col,

           m_axCol[i_Col].m_i_CellIn,
           m_axCol[i_Col].m_i_CellOut,
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
