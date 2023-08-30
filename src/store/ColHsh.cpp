/*============================================================================*/
/* ColHsh.cpp -- Columns Required Metadata                               */
/*============================================================================*/

#include "ColHsh.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE,ColHsh,CN_COLHSH)
__aNewFunction(ColHsh,CN_COLHSH)
__vDeleteFunction(ColHsh,CN_COLHSH)
__aFooterFunction(ColHsh)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
ColHsh::ColHsh(void)
    : Object(CN_COLHSH)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_COLHSH, _FOOTER_);

  for (i8 i_Col = 0; i_Col < C_COLMAP; i_Col++) {
    m_axCols[i_Col].m_i_EmittedCol = C_UNKNOWN;
    m_axCols[i_Col].m_bPayload     = false;
    m_axCols[i_Col].m_i_Cell       = C_UNKNOWN;
  }
  m_nCols = 0;

  _postcondition_(_ADDRESS_,_MAGIC_,CN_COLHSH,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
ColHsh::~ColHsh(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_COLHSH,_FOOTER_);

}


/*----------------------------------------------------------------------------*/
/*-- static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/*-- public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* add column mapping                                                         */
/*----------------------------------------------------------------------------*/
void ColHsh::vAdd(i8 i_EmittedCol, bool bPayLoad, i8 i_Cell)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_COLHSH, _FOOTER_);

  m_axCols[m_nCols].m_i_EmittedCol = i_EmittedCol;
  m_axCols[m_nCols].m_bPayload     = bPayLoad;
  m_axCols[m_nCols].m_i_Cell       = i_Cell;

  m_nCols++;

  vValidate();
}

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void ColHsh::vDump(i8 i_Tab)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_COLHSH, _FOOTER_);
  UNUSED(i_Tab);

  for (i8 i_Col = 0; i_Col < m_nCols; i_Col++) {
    vTrace("               [%lld] %s %lld -> $%lld\n"_Stz,
           i_Col,
           m_axCols[i_Col].m_bPayload ? "payload cell" : "key cell",
           m_axCols[i_Col].m_i_Cell,
           m_axCols[i_Col].m_i_EmittedCol
           );
  }
}

/*----------------------------------------------------------------------------*/
/* validate                                                                   */
/*----------------------------------------------------------------------------*/
void ColHsh::vValidate(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_COLHSH, _FOOTER_);


}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
