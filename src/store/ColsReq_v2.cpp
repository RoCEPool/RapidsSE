/*============================================================================*/
/* ColsReq_v2.cpp -- Columns Required Metadata                                */
/*============================================================================*/

#include "ColsReq_v2.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE,ColsReq_v2,CN_COLSREQ_V2)
__aNewFunction(ColsReq_v2,CN_COLSREQ_V2)
__vDeleteFunction(ColsReq_v2,CN_COLSREQ_V2)
__aFooterFunction(ColsReq_v2)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
ColsReq_v2::ColsReq_v2(void)
    : Object(CN_COLSREQ_V2)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_COLSREQ_V2, _FOOTER_);

  for (i8 i_Col = 0; i_Col < C_COLSREQ; i_Col++) {
    m_axCols[i_Col].m_i_CrwCol = C_UNKNOWN;
    m_axCols[i_Col].m_i_StructCell = C_UNKNOWN;
  }
  m_nCols = 0;

  _postcondition_(_ADDRESS_,_MAGIC_,CN_COLSREQ_V2,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
ColsReq_v2::~ColsReq_v2(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_COLSREQ_V2,_FOOTER_);

}


/*----------------------------------------------------------------------------*/
/*-- static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/*-- public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* add mapping of crwCol to structCol                                         */
/*----------------------------------------------------------------------------*/
void ColsReq_v2::vAddMapping(i8 i_CrwCol, i8 i_StructCell)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_COLSREQ_V2, _FOOTER_);

  m_axCols[m_nCols].m_i_CrwCol     = i_CrwCol;
  m_axCols[m_nCols].m_i_StructCell = i_StructCell;

  m_nCols++;

  vValidate();
}

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void ColsReq_v2::vDump(i8 i_Tab)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_COLSREQ_V2, _FOOTER_);
  UNUSED(i_Tab);

  for (i8 i_Col = 0; i_Col < m_nCols; i_Col++) {
    vTrace("            m_axCols[%lld] : m_i_CrwCol [%lld] -> m_i_StructCell [%lld] \n"_Stz,
           i_Col,
           m_axCols[i_Col].m_i_CrwCol,
           m_axCols[i_Col].m_i_StructCell);
  }
}

/*----------------------------------------------------------------------------*/
/* validate                                                                   */
/*----------------------------------------------------------------------------*/
void ColsReq_v2::vValidate(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_COLSREQ_V2, _FOOTER_);


}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
