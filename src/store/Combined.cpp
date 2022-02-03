/*============================================================================*/
/* Combined.cpp -- Tub-Based Crw Combined using Dynamic Code                  */
/*============================================================================*/

#include "BASE.h"

#include "Combined.hpp"
#include "LogicUnit.hpp"

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
Combined::Combined(LogicUnit* pLogicUnit)
: Object(CN_COMBINED)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_COMBINED,_FOOTER_);
  
  m_xCombinedT.m_pTub        = nullptr;
  m_xCombinedT.m_pPot        = nullptr;
  m_xCombinedT.m_bTrace      = false;
  m_xCombinedT.m_iErrorCode  = (i8)EC_NONE;
  m_xCombinedT.m_iHashRows   = 0;

  // table scan T inflate (crwBuffer)
  m_xCombinedT.m_aHeadLine0  = nullptr;
  m_xCombinedT.m_iHeadSize   = 0;
  m_xCombinedT.m_eStrInflateRule = SI_NONE;

  // rowbuffer
  m_xCombinedT.m_aRowBuffer    = nullptr;

  // index scan T
  m_xCombinedT.m_aIndexT     = nullptr;
  m_xCombinedT.m_aIndexTreeT = nullptr;
  m_xCombinedT.m_aIndexDefT  = nullptr;
  m_xCombinedT.m_aEntWrkT    = (Ent_int*)(void*)&pLogicUnit->m_oEntWrkT;
  m_xCombinedT.m_aEntKeyT    = ((EntWrk*)m_xCombinedT.m_aEntWrkT)->aKey();

  m_xCombinedT.m_aEntUpperT  = nullptr;
  m_xCombinedT.m_aEntBufferT = nullptr;
  m_xCombinedT.m_aEntIntT    = (Ent_int*)pLogicUnit->m_oEntWrkT.aEnt();;

  // common inflate
  m_xCombinedT.m_pRowSingleT    = nullptr;
  m_xCombinedT.m_aRowCellArrayT = nullptr;
  m_xCombinedT.m_racStrAreaT = nullptr;
  m_xCombinedT.m_racStrAreaTSav = nullptr;
  m_xCombinedT.m_nRows       = 0;
  m_xCombinedT.m_nCols       = 0;

  // index join with J
  m_xCombinedT.m_iIndexJoinInit = 0;
  m_xCombinedT.m_aRowSingleJ = nullptr;
  m_xCombinedT.m_aRowCellArrayJ = nullptr;
  m_xCombinedT.m_racStrAreaJ    = nullptr;
  m_xCombinedT.m_racStrAreaJSav = nullptr;
  m_xCombinedT.m_aIndexJ     = nullptr;
  m_xCombinedT.m_aIndexTreeJ = nullptr;
  m_xCombinedT.m_aIndexDefJ  = nullptr;
  m_xCombinedT.m_aEntWrkJ    = (void*)&pLogicUnit->m_oEntWrkJ;
  m_xCombinedT.m_aEntKeyJ    = ((EntWrk*)m_xCombinedT.m_aEntWrkJ)->aKey();
  m_xCombinedT.m_aEntUpperJ  = nullptr;
  m_xCombinedT.m_aEntBufferJ = nullptr;
  m_xCombinedT.m_aEntIntJ    = (Ent_int*)pLogicUnit->m_oEntWrkJ.aEnt();

  // join row T and J
  m_xCombinedT.m_aRowSingleTJ = nullptr;
  m_xCombinedT.m_aRowCellArrayTJ = nullptr;
  m_xCombinedT.m_racStrAreaTJ    = nullptr;
  m_xCombinedT.m_racStrAreaTJSav = nullptr;

  // project (col/exp)
  m_xCombinedT.m_iProjectInit = 0;
  m_xCombinedT.m_aRowSingleP    = nullptr;
  m_xCombinedT.m_aRowCellArrayP = nullptr;
  m_xCombinedT.m_racStrAreaP    = nullptr;
  m_xCombinedT.m_racStrAreaSavP = nullptr;

  // project (agg)
  m_xCombinedT.m_nAggRows = 0;
  m_xCombinedT.m_iProcessRow    = 1;
  m_xCombinedT.m_iRowsProcessed = 0;
  m_xCombinedT.m_aRowSingleAgg  = nullptr;
  m_xCombinedT.m_aRowCellArrayAgg = nullptr;
  m_xCombinedT.m_racStrAreaAgg  = nullptr;

  // project (gby/payload)
  m_xCombinedT.m_aiPayload      = nullptr;
  m_xCombinedT.m_aPayloadRow    = nullptr;
  m_xCombinedT.m_racPayloadArea = nullptr;
  m_xCombinedT.m_aIndexTreeGby  = nullptr;
  m_xCombinedT.m_aIndexDefGby   = nullptr;
  m_xCombinedT.m_aEntBufferPL   = nullptr;
  m_xCombinedT.m_aEntWrkPL      = (void*)&pLogicUnit->m_oEntWrkPL;
  m_xCombinedT.m_aEntKeyPL      = ((EntWrk*)m_xCombinedT.m_aEntWrkPL)->aKey();
  m_xCombinedT.m_aEntIntPL      = (Ent_int*)pLogicUnit->m_oEntWrkPL.aEnt();;

  // top
  m_xCombinedT.m_iTopInit         = 0;
  m_xCombinedT.m_aiTopPayload     = nullptr;
  m_xCombinedT.m_racTopPayloadArea = nullptr;
  m_xCombinedT.m_aTopPayloadRow   = nullptr;
  m_xCombinedT.m_aIndexDefTOP     = nullptr;
  m_xCombinedT.m_aIndexTreeTOP    = nullptr;
  m_xCombinedT.m_aEntBufferTOP    = nullptr;
  m_xCombinedT.m_aEntBottomTOP    = nullptr;
  m_xCombinedT.m_aEntWrkTOP       = (void*)&pLogicUnit->m_oEntWrkPL;
  m_xCombinedT.m_aEntKeyTOP       = ((EntWrk*)m_xCombinedT.m_aEntWrkPL)->aKey();
  m_xCombinedT.m_aEntIntTOP       = (Ent_int*)pLogicUnit->m_oEntWrkPL.aEnt();;
  m_xCombinedT.m_iInsertCntTOP    = 0;

  // limit
  m_xCombinedT.m_iLimitRows       = 0;

  // tub
  m_xCombinedT.m_aRowTub          = nullptr;
  m_xCombinedT.m_aRowTubCellArray = nullptr;
  m_xCombinedT.m_racTubStrArea    = nullptr;
  m_xCombinedT.m_iTubRowSize      = 0;

  // pot
  m_xCombinedT.m_aRowPot          = nullptr;
  m_xCombinedT.m_aRowPotCellArray = nullptr;
  m_xCombinedT.m_racPotStrArea    = nullptr;

  _checkInitialised_(CN_COMBINED);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_COMBINED,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Combined::~Combined(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_COMBINED,_FOOTER_);

  RowSingle* pRowSingle;

  pRowSingle = _pRowSingle(m_xCombinedT.m_aRowSingleJ);
  delete pRowSingle;

  pRowSingle = _pRowSingle(m_xCombinedT.m_aRowSingleTJ);
  delete pRowSingle;

  pRowSingle = _pRowSingle(m_xCombinedT.m_aRowSingleP);
  delete pRowSingle;

  pRowSingle = _pRowSingle(m_xCombinedT.m_pRowSingleT);
  delete pRowSingle;

  pRowSingle = _pRowSingle(m_xCombinedT.m_aRowSingleAgg);
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
