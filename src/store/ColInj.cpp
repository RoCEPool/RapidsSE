/*============================================================================*/
/* ColInj.cpp -- Columns Required Metadata                                */
/*============================================================================*/

#include "ColInj.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE, ColInj, CN_COLINJ)
__aNewFunction(ColInj, CN_COLINJ)
__vDeleteFunction(ColInj, CN_COLINJ)
__aFooterFunction(ColInj)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
ColInj::ColInj(void)
    : Object(CN_COLINJ)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_COLINJ, _FOOTER_);

  for (i8 i_Col = 0; i_Col < C_COLINJ; i_Col++) {
    m_axCol[i_Col].m_i_CrwCol     = C_UNKNOWN;
    m_axCol[i_Col].m_i_StructCell = C_UNKNOWN;
    m_axCol[i_Col].m_bPriorCrwCol = false;

  }
  m_nCols = 0;

  _postcondition_(_ADDRESS_, _MAGIC_, CN_COLINJ, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
ColInj::~ColInj(void) noexcept(false)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_COLINJ, _FOOTER_);

}


/*----------------------------------------------------------------------------*/
/*-- static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/*-- public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* add injection of crwCol to structCol                                       */
/*----------------------------------------------------------------------------*/
void ColInj::vAdd(i8 i_CrwCol, i8 i_StructCell, Stz* szCn_memberNm,
                  Stz* szNote1, Stz* szNote2)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_COLINJ, _FOOTER_);

  m_axCol[m_nCols].m_i_CrwCol     = i_CrwCol;
  m_axCol[m_nCols].m_i_StructCell = i_StructCell;
  if (szCn_memberNm)
    m_axCol[m_nCols].m_sCn_memberNm = szCn_memberNm;

  m_axCol[m_nCols].m_bPriorCrwCol = false;

  if (szNote1)
    m_axCol[m_nCols].m_sNote1 = szNote1;
  if (szNote2)
    m_axCol[m_nCols].m_sNote2 = szNote2;

  m_nCols++;

  vValidate();
}

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void ColInj::vDump(i8 i_Tab)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_COLINJ, _FOOTER_);
  UNUSED(i_Tab);

  s256 sNotes;

  for (i8 i_Col = 0; i_Col < m_nCols; i_Col++) {
    if (*m_axCol[i_Col].m_sNote1.azChar() || *m_axCol[i_Col].m_sNote2.azChar()) {
      sNotes.vSprint("(%s %s)"_Stz,
                     m_axCol[i_Col].m_sNote1.azChar(),
                     m_axCol[i_Col].m_sNote2.azChar());
    }
    else {
      sNotes = ""_Stz;
    }

    vTrace("%s%sm_axCols[%lld] : m_i_CrwCol [%lld] %s-> m_i_StructCell [%lld] %s %s\n"_Stz,
           Stz::szTab(i_Tab),
           Stz::szSpaces(7),
           i_Col,

           m_axCol[i_Col].m_i_CrwCol,
           m_axCol[i_Col].m_bPriorCrwCol ? "(prior) " : "",
           m_axCol[i_Col].m_i_StructCell,
           m_axCol[i_Col].m_sCn_memberNm.szStz(),
           sNotes.azChar());
  }
}

/*----------------------------------------------------------------------------*/
/* prior m_i_CrwCol is used again                                             */
/*----------------------------------------------------------------------------*/
void ColInj::vRepeatsPrior(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_COLINJ, _FOOTER_);

  RASSERT(m_nCols >= 2);

  m_axCol[m_nCols - 1].m_bPriorCrwCol = true;
}

/*----------------------------------------------------------------------------*/
/* validate                                                                   */
/*----------------------------------------------------------------------------*/
void ColInj::vValidate(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_COLINJ, _FOOTER_);

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
