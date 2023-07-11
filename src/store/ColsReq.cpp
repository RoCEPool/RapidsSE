/*============================================================================*/
/* ColsReq.cpp -- Columns Required Metadata                               */
/*============================================================================*/

#include "ColsReq.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE,ColsReq,CN_COLSREQ)
__aNewFunction(ColsReq,CN_COLSREQ)
__vDeleteFunction(ColsReq,CN_COLSREQ)
__aFooterFunction(ColsReq)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
ColsReq::ColsReq(void)
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
ColsReq::~ColsReq(void) noexcept(false)
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
void ColsReq::vAddCol(i8 i_EmittedCol, bool bPayLoad, i8 i_Cell)
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
void ColsReq::vDump(i8 i_Tab)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_COLSREQ, _FOOTER_);

  for (i8 i_Col = 0; i_Col < m_nCols; i_Col++) {
    vTrace("               Col [%lld]: %lld %lld %s\n"_Stz,
           i_Col,
           m_axCols[i_Col].m_i_EmittedCol,
           m_axCols[i_Col].m_i_Cell,
           m_axCols[i_Col].m_bPayload ? "payload" : "key");
  }
}

/*----------------------------------------------------------------------------*/
/* validate                                                                   */
/*----------------------------------------------------------------------------*/
void ColsReq::vValidate(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_COLSREQ, _FOOTER_);


}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
