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
  m_xCombinedT.m_aRowBuffer  = nullptr;
  m_xCombinedT.m_pRowSingleT = nullptr;
  m_xCombinedT.m_pRowSingleJ = nullptr;
  m_xCombinedT.m_pRowAggregate = nullptr;
  m_xCombinedT.m_aHeadLine0  = nullptr;
  m_xCombinedT.m_iHeadSize   = 0;
  m_xCombinedT.m_nRows       = 0;
  m_xCombinedT.m_nCols       = 0;

  m_xCombinedT.m_eStrInflateRule = SI_NONE;

  m_xCombinedT.m_pJoinIndex = nullptr;
  m_xCombinedT.m_pIndexTree = nullptr;
  m_xCombinedT.m_pIndexDef  = nullptr;
  m_xCombinedT.m_pEntWrk    = nullptr;
  m_xCombinedT.m_pEntWrkUpper = nullptr;
  m_xCombinedT.m_aEntKey      = nullptr;
  m_xCombinedT.m_aEntInt      = nullptr;
  m_xCombinedT.m_aEntIntUpper = nullptr;
  m_xCombinedT.m_bTrace       = false;

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

  pRowSingle = _pRowSingle(m_xCombinedT.m_pRowSingleJ);
  delete pRowSingle;

//  IndexTree* pIndexTree = _pIndexTree(m_xCombinedT.m_pIndexTree);
//  delete pIndexTree;
//
//  IndexDef* pIndexDef = _pIndexDef(m_xCombinedT.m_pIndexDef);
//  delete pIndexDef;
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
