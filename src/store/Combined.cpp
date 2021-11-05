/*============================================================================*/
/* Combined.cpp -- Tub-Based Crw Combined using Dynamic Code                  */
/*============================================================================*/

#include "BASE.h"

#include "Combined.hpp"
//#include "IndexDef.hpp"
//#include "IndexTree.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE,Combined,CN_COMBINED)
__aNewFunction(Combined,CN_COMBINED)
__vDeleteFunction(Combined,CN_COMBINED)
__aFooterFunction(Combined)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Combined::Combined(void)
: Object(CN_COMBINED)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_COMBINED,_FOOTER_);
  
  m_xCombinedT.m_pTub        = nullptr;
  m_xCombinedT.m_bTrace      = false;

  // table scan T inflate (crwBuffer)
  m_xCombinedT.m_aHeadLine0  = nullptr;
  m_xCombinedT.m_iHeadSize   = 0;
  m_xCombinedT.m_eStrInflateRule = SI_NONE;

  // rowbuffer
  m_xCombinedT.m_aRowBuffer  = nullptr;

  // index scan T
  m_xCombinedT.m_aIndexT     = nullptr;
  m_xCombinedT.m_aIndexTreeT = nullptr;
  m_xCombinedT.m_aIndexDefT  = nullptr;
  m_xCombinedT.m_aEntKeyT    = nullptr;
  m_xCombinedT.m_aEntWrkT    = nullptr;
  m_xCombinedT.m_aEntUpperT  = nullptr;
  m_xCombinedT.m_aEntBufferT = nullptr;
  m_xCombinedT.m_aEntIntT    = nullptr;

  // common inflate
  m_xCombinedT.m_pRowSingleT = nullptr;
  m_xCombinedT.m_racStrAreaT = nullptr;
  m_xCombinedT.m_nRows       = 0;
  m_xCombinedT.m_nCols       = 0;

  // index join with J
  m_xCombinedT.m_aRowSingleJ = nullptr;
  m_xCombinedT.m_racStrAreaJ = nullptr;
  m_xCombinedT.m_aJoinRow    = nullptr;
  m_xCombinedT.m_racJoinArea = nullptr;
  m_xCombinedT.m_aIndexJ     = nullptr;
  m_xCombinedT.m_aIndexTreeJ = nullptr;
  m_xCombinedT.m_aIndexDefJ  = nullptr;
  m_xCombinedT.m_aEntKeyJ    = nullptr;
  m_xCombinedT.m_aEntWrkJ    = nullptr;
  m_xCombinedT.m_aEntUpperJ  = nullptr;
  m_xCombinedT.m_aEntBufferJ = nullptr;
  m_xCombinedT.m_aEntIntJ    = nullptr;

  // project (agg/payload)
  m_xCombinedT.m_aRowAggregate = nullptr;
  m_xCombinedT.m_aIndexTreeGby = nullptr;
  m_xCombinedT.m_aIndexDefGby  = nullptr;
  m_xCombinedT.m_aEntKeyPL     = nullptr;
  m_xCombinedT.m_aEntWrkPL     = nullptr;
  m_xCombinedT.m_aEntIntPL     = nullptr;

  _checkInitialised_(CN_COMBINED);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_COMBINED,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Combined::~Combined(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_COMBINED,_FOOTER_);

  RowSingle* pRowSingle = _pRowSingle(m_xCombinedT.m_pRowSingleT);
  delete pRowSingle;

  pRowSingle = _pRowSingle(m_xCombinedT.m_aRowSingleJ);
  delete pRowSingle;
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* repair tubby                                                               */
/*----------------------------------------------------------------------------*/
void Combined::vRepair(Object* pObject)
{
  _static_
  
  UNUSED(pObject);
}

/*----------------------------------------------------------------------------*/
/*-- Member Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get m_xCombinedT                                                           */
/*----------------------------------------------------------------------------*/
Combined_T* Combined::aCombinedT(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_COMBINED,_FOOTER_);

  return &m_xCombinedT;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
