/*============================================================================*/
/* ColsReq_v2.cpp -- Columns Required Metadata                               */
/*============================================================================*/

#include "ColsReq_v2.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE,ColsReq_v2,CN_COLSREQ)
__aNewFunction(ColsReq_v2,CN_COLSREQ)
__vDeleteFunction(ColsReq_v2,CN_COLSREQ)
__aFooterFunction(ColsReq_v2)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
ColsReq_v2::ColsReq_v2(void)
    : Object(CN_COLSREQ)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_COLSREQ, _FOOTER_);

  for (i8 i_Col = 0; i_Col < C_COLSREQ; i_Col++) {
    m_axCols[i_Col].m_i_EmittedCol = C_UNKNOWN;
    m_axCols[i_Col].m_bPayload     = false;
    m_axCols[i_Col].m_i_Cell       = C_UNKNOWN;
  }
  m_nCols = 0;

  _postcondition_(_ADDRESS_,_MAGIC_,CN_COLSREQ,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
ColsReq_v2::~ColsReq_v2(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_COLSREQ,_FOOTER_);

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
void ColsReq_v2::vAddCol(i8 i_EmittedCol, bool bPayLoad, i8 i_Cell)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_COLSREQ, _FOOTER_);

  m_axCols[m_nCols].m_i_EmittedCol = i_EmittedCol;
  m_axCols[m_nCols].m_bPayload     = bPayLoad;
  m_axCols[m_nCols].m_i_Cell       = i_Cell;

  m_nCols++;

  vValidate();
}

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void ColsReq_v2::vDump(i8 i_Tab)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_COLSREQ, _FOOTER_);
  UNUSED(i_Tab);

  for (i8 i_Col = 0; i_Col < m_nCols; i_Col++) {
    vTrace("               [%lld] $%lld <- cell %lld \n"_Stz,
           i_Col,
           m_axCols[i_Col].m_i_EmittedCol,
           m_axCols[i_Col].m_i_Cell);
  }
}

/*----------------------------------------------------------------------------*/
/* validate                                                                   */
/*----------------------------------------------------------------------------*/
void ColsReq_v2::vValidate(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_COLSREQ, _FOOTER_);


}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
