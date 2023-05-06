/*============================================================================*/
/* Dynamic.cpp -- Dynamic Code                                                */
/*============================================================================*/

#include "BASE.h"

#include "Dynamic.hpp"
#include "LogicUnit.hpp"

//#include "IndexDef.hpp"
//#include "IndexTree.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE,Dynamic,CN_DYNAMIC)
__aNewFunction(Dynamic,CN_DYNAMIC)
__vDeleteFunction(Dynamic,CN_DYNAMIC)
__aFooterFunction(Dynamic)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Dynamic::Dynamic(LogicUnit* pLogicUnit)
: Object(CN_DYNAMIC)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DYNAMIC,_FOOTER_);

  // inflate T (SR_IT)
  m_xDynamicT.m_nRows       = 0;
  m_xDynamicT.m_nCols       = 0;
  m_xDynamicT.m_xRowInfoIT.m_aRowSingle    = nullptr;
  m_xDynamicT.m_xRowInfoIT.m_aRowCellArray = nullptr;
  m_xDynamicT.m_xRowInfoIT.m_racStrArea = nullptr;
  m_xDynamicT.m_xRowInfoIT.m_racStrAreaSav = nullptr;

  // inflate J (SR_IJ)
  m_xDynamicT.m_xRowInfoIJ.m_aRowSingle    = nullptr;
  m_xDynamicT.m_xRowInfoIJ.m_aRowCellArray = nullptr;
  m_xDynamicT.m_xRowInfoIJ.m_racStrArea    = nullptr;
  m_xDynamicT.m_xRowInfoIJ.m_racStrAreaSav = nullptr;

  // project PL (SR_PL)
  m_xDynamicT.m_xRowInfoPL.m_aRowSingle    = nullptr;
  m_xDynamicT.m_xRowInfoPL.m_aRowCellArray = nullptr;
  m_xDynamicT.m_xRowInfoPL.m_racStrArea    = nullptr;
  m_xDynamicT.m_xRowInfoPL.m_racStrAreaSav = nullptr;

  // project T (SR_PT)
  m_xDynamicT.m_xRowInfoPT.m_aRowSingle    = nullptr;
  m_xDynamicT.m_xRowInfoPT.m_aRowCellArray = nullptr;
  m_xDynamicT.m_xRowInfoPT.m_racStrArea    = nullptr;
  m_xDynamicT.m_xRowInfoPT.m_racStrAreaSav = nullptr;

  // join T & J (SR_TJ)
  m_xDynamicT.m_xRowInfoTJ.m_aRowSingle    = nullptr;
  m_xDynamicT.m_xRowInfoTJ.m_aRowCellArray = nullptr;
  m_xDynamicT.m_xRowInfoTJ.m_racStrArea    = nullptr;
  m_xDynamicT.m_xRowInfoTJ.m_racStrAreaSav = nullptr;

  // aggregate T (SR_AT)
  m_xDynamicT.m_xRowInfoAT.m_aRowSingle  = nullptr;
  m_xDynamicT.m_xRowInfoAT.m_aRowCellArray = nullptr;
  m_xDynamicT.m_xRowInfoAT.m_racStrArea  = nullptr;
  m_xDynamicT.m_xRowInfoAT.m_racStrAreaSav  = nullptr;

  // tub output (SR_TO)
  m_xDynamicT.m_pTub        = nullptr;
  m_xDynamicT.m_iTubRowSize = 0;
  m_xDynamicT.m_xRowInfoTO.m_aRowSingle    = nullptr;
  m_xDynamicT.m_xRowInfoTO.m_aRowCellArray = nullptr;
  m_xDynamicT.m_xRowInfoTO.m_racStrArea    = nullptr;
  m_xDynamicT.m_xRowInfoTO.m_racStrAreaSav  = nullptr;

  // pot output (SR_PO)
  m_xDynamicT.m_pPot        = nullptr;
  m_xDynamicT.m_xRowInfoPO.m_aRowSingle    = nullptr;
  m_xDynamicT.m_xRowInfoPO.m_aRowCellArray = nullptr;
  m_xDynamicT.m_xRowInfoPO.m_racStrArea    = nullptr;
  m_xDynamicT.m_xRowInfoPO.m_racStrAreaSav = nullptr;

  m_xDynamicT.m_bTrace      = false;
  m_xDynamicT.m_iErrorCode  = (i8)EC_NONE;
  m_xDynamicT.m_iHashRows   = 0;

  // table scan T inflate (crwBuffer)
  m_xDynamicT.m_aHeadLine0  = nullptr;
  m_xDynamicT.m_iHeadSize   = 0;
  m_xDynamicT.m_eStrInflateRule = SI_NONE;

  for (i8 i_Stack = 0; i_Stack < 256; i_Stack++)
    m_xDynamicT.m_aW[i_Stack] = 0;

  // rowbuffer
  m_xDynamicT.m_aRowBuffer    = nullptr;

  // index scan T
  m_xDynamicT.m_iIndexScan  = 0;
  m_xDynamicT.m_xIndexInfoT.m_aIndex     = nullptr;
  m_xDynamicT.m_xIndexInfoT.m_aIndexTree = nullptr;
  m_xDynamicT.m_xIndexInfoT.m_aIndexDef  = nullptr;
  m_xDynamicT.m_xIndexInfoT.m_aEntWrk    = (Ent_int*)(void*)&pLogicUnit->m_oEntWrkT;
  m_xDynamicT.m_xIndexInfoT.m_aEntKey    = ((EntWrk*)m_xDynamicT.m_xIndexInfoT.m_aEntWrk)->aKey();
  m_xDynamicT.m_xIndexInfoT.m_aEntUpper  = nullptr;
  m_xDynamicT.m_xIndexInfoT.m_aEntBuffer = nullptr;
  m_xDynamicT.m_xIndexInfoT.m_aEntInt    = (Ent_int*)pLogicUnit->m_oEntWrkT.aEnt();;

  // index join with J
  m_xDynamicT.m_iIndexJoinInit = 0;
  m_xDynamicT.m_iJoinedRows = 0;
  m_xDynamicT.m_aIndexJ     = nullptr;
  m_xDynamicT.m_aIndexTreeJ = nullptr;
  m_xDynamicT.m_aIndexDefJ  = nullptr;
  m_xDynamicT.m_aEntWrkJ    = (void*)&pLogicUnit->m_oEntWrkJ;
  m_xDynamicT.m_aEntKeyJ    = ((EntWrk*)m_xDynamicT.m_aEntWrkJ)->aKey();
  m_xDynamicT.m_aEntUpperJ  = nullptr;
  m_xDynamicT.m_aEntBufferJ = nullptr;
  m_xDynamicT.m_aEntIntJ    = (Ent_int*)pLogicUnit->m_oEntWrkJ.aEnt();

  // project (col/exp)
  m_xDynamicT.m_iProjectInit = 0;

  // project (agg)
  m_xDynamicT.m_iRowsProcessed = 0;

  // project (gby/payload)
  m_xDynamicT.m_nAggRows = 0;
  m_xDynamicT.m_iProcessRow    = 1;

  // project (ordAgg)
  m_xDynamicT.m_iHasOrdAggRow = 0;
  m_xDynamicT.m_iOrdAggDone = 0;

  // payload (gby/srt/top)
  m_xDynamicT.m_aiPayload      = nullptr;
  m_xDynamicT.m_aIndexTreePL   = nullptr;
  m_xDynamicT.m_aIndexDefPL    = nullptr;
  m_xDynamicT.m_aEntBufferPL   = nullptr;
  m_xDynamicT.m_aEntWrkPL      = (void*)&pLogicUnit->m_oEntWrkPL;
  m_xDynamicT.m_aEntKeyPL      = ((EntWrk*)m_xDynamicT.m_aEntWrkPL)->aKey();
  m_xDynamicT.m_aEntBottomPL     = nullptr;
  m_xDynamicT.m_aEntIntPL      = (Ent_int*)pLogicUnit->m_oEntWrkPL.aEnt();

  // top
  m_xDynamicT.m_iTopInit         = 0;
  m_xDynamicT.m_iInsertCntTOP    = 0;

  // limit
  m_xDynamicT.m_iLimitRows       = 0;

  _checkInitialised_(CN_DYNAMIC);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_DYNAMIC,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Dynamic::~Dynamic(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DYNAMIC,_FOOTER_);

  RowSingle* pRowSingle;

  if (m_xDynamicT.m_aIndexTreePL) {
    IndexTree* pIndexTree = _pIndexTree(m_xDynamicT.m_aIndexTreePL);
    delete pIndexTree;
  }

  if (m_xDynamicT.m_aIndexDefPL) {
    IndexDef* pIndexDef = _pIndexDef(m_xDynamicT.m_aIndexDefPL);
    delete pIndexDef;
  }

  pRowSingle = _pRowSingle(m_xDynamicT.m_xRowInfoIJ.m_aRowSingle);
  delete pRowSingle;

  pRowSingle = _pRowSingle(m_xDynamicT.m_xRowInfoTJ.m_aRowSingle);
  delete pRowSingle;

  pRowSingle = _pRowSingle(m_xDynamicT.m_xRowInfoPT.m_aRowSingle);
  delete pRowSingle;

  pRowSingle = _pRowSingle(m_xDynamicT.m_xRowInfoIT.m_aRowSingle);
  delete pRowSingle;

  pRowSingle = _pRowSingle(m_xDynamicT.m_xRowInfoAT.m_aRowSingle);
  delete pRowSingle;

  pRowSingle = _pRowSingle(m_xDynamicT.m_xRowInfoPL.m_aRowSingle);
  delete pRowSingle;
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* repair tubby                                                               */
/*----------------------------------------------------------------------------*/
void Dynamic::vRepair(Object* pObject)
{
  _static_
  
  UNUSED(pObject);
}

/*----------------------------------------------------------------------------*/
/*-- Member Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get m_xDynamicT                                                             */
/*----------------------------------------------------------------------------*/
Dynamic_T* Dynamic::aDynamicT(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DYNAMIC,_FOOTER_);

  return &m_xDynamicT;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
