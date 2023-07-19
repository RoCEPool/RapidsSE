/*============================================================================*/
/* ColMap.cpp -- Columns Required Metadata                               */
/*============================================================================*/

#include "ColMap.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE,ColMap,CN_COLMAP)
__aNewFunction(ColMap,CN_COLMAP)
__vDeleteFunction(ColMap,CN_COLMAP)
__aFooterFunction(ColMap)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
ColMap::ColMap(void)
    : Object(CN_COLMAP)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_COLMAP, _FOOTER_);

  for (i8 i_Col = 0; i_Col < C_COLMAP; i_Col++) {
    m_axCols[i_Col].m_i_EmittedCol = C_UNKNOWN;
    m_axCols[i_Col].m_bPayload     = false;
    m_axCols[i_Col].m_i_Cell       = C_UNKNOWN;
  }
  m_nCols = 0;

  _postcondition_(_ADDRESS_,_MAGIC_,CN_COLMAP,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
ColMap::~ColMap(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_COLMAP,_FOOTER_);

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
void ColMap::vAdd(i8 i_EmittedCol, bool bPayLoad, i8 i_Cell)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_COLMAP, _FOOTER_);

  m_axCols[m_nCols].m_i_EmittedCol = i_EmittedCol;
  m_axCols[m_nCols].m_bPayload     = bPayLoad;
  m_axCols[m_nCols].m_i_Cell       = i_Cell;

  m_nCols++;

  vValidate();
}

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void ColMap::vDump(i8 i_Tab)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_COLMAP, _FOOTER_);

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
void ColMap::vValidate(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_COLMAP, _FOOTER_);


}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
