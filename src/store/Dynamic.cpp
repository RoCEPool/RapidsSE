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

  for (i8 i_Stack = 0; i_Stack < 256; i_Stack++)
    m_xDynamicT.m_aW[i_Stack] = 0;

  m_xDynamicT.m_bTrace      = false;
  m_xDynamicT.m_iErrorCode  = (i8)EC_NONE;
  m_xDynamicT.m_iProcessRow = 1;
  m_xDynamicT.m_iRowsProcessed = 0;

  // rowbuffer T
  m_xDynamicT.m_aRowBuffer    = nullptr;

  // inflate T (SR_IT)
  m_xDynamicT.m_nRows       = 0;
  m_xDynamicT.m_nCols       = 0;
  m_xDynamicT.m_iHeadSize   = 0;
  m_xDynamicT.m_eStrInflateRule = SI_NONE;
  m_xDynamicT.m_aHeadLine0      = nullptr;
  m_xDynamicT.m_xRowInfoIT.m_aRowSingle    = nullptr;
  m_xDynamicT.m_xRowInfoIT.m_aRowCellArray = nullptr;
  m_xDynamicT.m_xRowInfoIT.m_acStrArea    = nullptr;
  m_xDynamicT.m_xRowInfoIT.m_acStrAreaSav = nullptr;

  // inflate J (SR_IJ)
  m_xDynamicT.m_xRowInfoIJ.m_aRowSingle    = nullptr;
  m_xDynamicT.m_xRowInfoIJ.m_aRowCellArray = nullptr;
  m_xDynamicT.m_xRowInfoIJ.m_acStrArea    = nullptr;
  m_xDynamicT.m_xRowInfoIJ.m_acStrAreaSav = nullptr;

  // project PL (SR_PL)
  m_xDynamicT.m_xRowInfoPL.m_aRowSingle    = nullptr;
  m_xDynamicT.m_xRowInfoPL.m_aRowCellArray = nullptr;
  m_xDynamicT.m_xRowInfoPL.m_acStrArea    = nullptr;
  m_xDynamicT.m_xRowInfoPL.m_acStrAreaSav = nullptr;

  // project T (SR_PT)
  m_xDynamicT.m_xRowInfoPT.m_aRowSingle    = nullptr;
  m_xDynamicT.m_xRowInfoPT.m_aRowCellArray = nullptr;
  m_xDynamicT.m_xRowInfoPT.m_acStrArea    = nullptr;
  m_xDynamicT.m_xRowInfoPT.m_acStrAreaSav = nullptr;

  // join T & J (SR_TJ)
  m_xDynamicT.m_xRowInfoTJ.m_aRowSingle    = nullptr;
  m_xDynamicT.m_xRowInfoTJ.m_aRowCellArray = nullptr;
  m_xDynamicT.m_xRowInfoTJ.m_acStrArea    = nullptr;
  m_xDynamicT.m_xRowInfoTJ.m_acStrAreaSav = nullptr;

  // aggregate T (SR_AT)
  m_xDynamicT.m_iAggregInit = 0;
  m_xDynamicT.m_xRowInfoAT.m_aRowSingle  = nullptr;
  m_xDynamicT.m_xRowInfoAT.m_aRowCellArray = nullptr;
  m_xDynamicT.m_xRowInfoAT.m_acStrArea  = nullptr;
  m_xDynamicT.m_xRowInfoAT.m_acStrAreaSav  = nullptr;

  // tub output (SR_TO)
  m_xDynamicT.m_pTub        = nullptr;
  m_xDynamicT.m_xRowInfoTO.m_aRowSingle    = nullptr;
  m_xDynamicT.m_xRowInfoTO.m_aRowCellArray = nullptr;
  m_xDynamicT.m_xRowInfoTO.m_acStrArea    = nullptr;
  m_xDynamicT.m_xRowInfoTO.m_acStrAreaSav  = nullptr;

  // pot output (SR_PO)
  m_xDynamicT.m_iPot        = 0;
  m_xDynamicT.m_pPot        = nullptr;
  m_xDynamicT.m_xRowInfoPO.m_aRowSingle    = nullptr;
  m_xDynamicT.m_xRowInfoPO.m_aRowCellArray = nullptr;
  m_xDynamicT.m_xRowInfoPO.m_acStrArea    = nullptr;
  m_xDynamicT.m_xRowInfoPO.m_acStrAreaSav = nullptr;

  // index scan T
  m_xDynamicT.m_xIndexInfoT.m_aIndex     = nullptr;
  m_xDynamicT.m_xIndexInfoT.m_aIndexTree = nullptr;
  m_xDynamicT.m_xIndexInfoT.m_aIndexDef  = nullptr;
  m_xDynamicT.m_xIndexInfoT.m_aEntWrk    = (Ent_int*)(void*)&pLogicUnit->m_oEntWrkT;
  m_xDynamicT.m_xIndexInfoT.m_aEntKey    = ((EntWrk*)m_xDynamicT.m_xIndexInfoT.m_aEntWrk)->aKey();
  m_xDynamicT.m_xIndexInfoT.m_aEntBottom = nullptr;
  m_xDynamicT.m_xIndexInfoT.m_aEntUpper  = nullptr;
  m_xDynamicT.m_xIndexInfoT.m_aEntBuffer = nullptr;
  m_xDynamicT.m_xIndexInfoT.m_aEntInt    = (Ent_int*)pLogicUnit->m_oEntWrkT.aEnt();

  // index join with J
  m_xDynamicT.m_iIndexJoinInit = 0;
  m_xDynamicT.m_iJoinedRows = 0;
  m_xDynamicT.m_xIndexInfoJ.m_aIndex     = nullptr;
  m_xDynamicT.m_xIndexInfoJ.m_aIndexTree = nullptr;
  m_xDynamicT.m_xIndexInfoJ.m_aIndexDef  = nullptr;
  m_xDynamicT.m_xIndexInfoJ.m_aEntWrk    = (void*)&pLogicUnit->m_oEntWrkJ;
  m_xDynamicT.m_xIndexInfoJ.m_aEntKey    = ((EntWrk*)m_xDynamicT.m_xIndexInfoJ.m_aEntWrk)->aKey();
  m_xDynamicT.m_xIndexInfoJ.m_aEntBottom = nullptr;
  m_xDynamicT.m_xIndexInfoJ.m_aEntUpper  = nullptr;
  m_xDynamicT.m_xIndexInfoJ.m_aEntBuffer = nullptr;
  m_xDynamicT.m_xIndexInfoJ.m_aEntInt    = (Ent_int*)pLogicUnit->m_oEntWrkJ.aEnt();

  // payload (gby/srt/top)
  m_xDynamicT.m_aiPayload      = nullptr;
  m_xDynamicT.m_xIndexInfoPL.m_aIndex       = nullptr;
  m_xDynamicT.m_xIndexInfoPL.m_aIndexTree   = nullptr;
  m_xDynamicT.m_xIndexInfoPL.m_aIndexDef    = nullptr;
  m_xDynamicT.m_xIndexInfoPL.m_aEntWrk      = (void*)&pLogicUnit->m_oEntWrkPL;
  m_xDynamicT.m_xIndexInfoPL.m_aEntKey      = ((EntWrk*)m_xDynamicT.m_xIndexInfoPL.m_aEntWrk)->aKey();
  m_xDynamicT.m_xIndexInfoPL.m_aEntBottom   = nullptr;
  m_xDynamicT.m_xIndexInfoPL.m_aEntUpper    = nullptr;
  m_xDynamicT.m_xIndexInfoPL.m_aEntBuffer   = nullptr;
  m_xDynamicT.m_xIndexInfoPL.m_aEntInt      = (Ent_int*)pLogicUnit->m_oEntWrkPL.aEnt();

  // limit
  m_xDynamicT.m_iLimitRows       = 0;

  // ordAgg
//  m_xDynamicT.m_iIndexScan  = 0;
//  m_xDynamicT.m_iHasOrdAggRow = 0;
//  m_xDynamicT.m_iOrdAggDone = 0;

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

  if (m_xDynamicT.m_xIndexInfoPL.m_aIndexTree) {
    IndexTree* pIndexTree = _pIndexTree(m_xDynamicT.m_xIndexInfoPL.m_aIndexTree);
    delete pIndexTree;
  }

  if (m_xDynamicT.m_xIndexInfoPL.m_aIndexDef) {
    IndexDef* pIndexDef = _pIndexDef(m_xDynamicT.m_xIndexInfoPL.m_aIndexDef);
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
