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
  m_xDynamicT.m_bProcessRow = true;
  m_xDynamicT.m_iRowsProcessed = 0;

	m_xDynamicT.m_bHasAggRow = false;
	m_xDynamicT.m_bHasRowToAgg = false;
  m_xDynamicT.m_bIndexScan = false;

  // limit
  m_xDynamicT.m_iLimitRows       = 0;

	// rowbuffer T
	m_xDynamicT.m_aPdsRowBuffer = nullptr;

	// inflate T (SR_IT)
  m_xDynamicT.m_nRows       = 0;
  m_xDynamicT.m_nCols       = 0;
  m_xDynamicT.m_iHeadSize   = 0;
  m_xDynamicT.m_eStrInflateRule = SI_NONE;
  m_xDynamicT.m_aHeadLine0      = nullptr;

  m_xDynamicT.m_xRowInfoIT.m_bInitialize = true;
  for (i8 i = 0; i < 256; i++)
    m_xDynamicT.m_xRowInfoIT.m_iCell[i] = 0;
#ifdef WITH_CHECK_INITIALISED
  MemLib::vZero(m_xDynamicT.m_xRowInfoIT.m_cStringArea,_C_16KB,CN_BYTES_T);
#else
  m_xDynamicT.m_xRowInfoIT.m_cStringArea[0] = '\0';
#endif
	m_xDynamicT.m_xRowInfoIT.m_nStringAreaSize = _C_16KB;

  m_xDynamicT.m_xRowInfoIT.m_aRowCellArray = (void*)&m_xDynamicT.m_xRowInfoIT.m_iCell[0];
  m_xDynamicT.m_xRowInfoIT.m_acStrArea     = (char*)&m_xDynamicT.m_xRowInfoIT.m_cStringArea[0];
  m_xDynamicT.m_xRowInfoIT.m_acStrAreaSav  = (char*)&m_xDynamicT.m_xRowInfoIT.m_cStringArea[0];

  // inflate J (SR_IJ)
  m_xDynamicT.m_xRowInfoIJ.m_bInitialize = true;
  for (i8 i = 0; i < 256; i++)
    m_xDynamicT.m_xRowInfoIJ.m_iCell[i] = 0;
#ifdef WITH_CHECK_INITIALISED
  MemLib::vZero(m_xDynamicT.m_xRowInfoIJ.m_cStringArea,_C_16KB,CN_BYTES_T);
#else
  m_xDynamicT.m_xRowInfoIJ.m_cStringArea[0] = '\0';
#endif
	m_xDynamicT.m_xRowInfoIJ.m_nStringAreaSize = _C_16KB;

  m_xDynamicT.m_xRowInfoIJ.m_aRowCellArray = (void*)&m_xDynamicT.m_xRowInfoIJ.m_iCell[0];
  m_xDynamicT.m_xRowInfoIJ.m_acStrArea     = (char*)&m_xDynamicT.m_xRowInfoIJ.m_cStringArea[0];
  m_xDynamicT.m_xRowInfoIJ.m_acStrAreaSav  = (char*)&m_xDynamicT.m_xRowInfoIJ.m_cStringArea[0];

  // project PL (SR_PL)
  m_xDynamicT.m_xRowInfoPL.m_bInitialize = true;
  for (i8 i = 0; i < 256; i++)
    m_xDynamicT.m_xRowInfoPL.m_iCell[i] = 0;
#ifdef WITH_CHECK_INITIALISED
  MemLib::vZero(m_xDynamicT.m_xRowInfoPL.m_cStringArea,_C_16KB,CN_BYTES_T);
#else
  m_xDynamicT.m_xRowInfoPL.m_cStringArea[0] = '\0';
#endif
	m_xDynamicT.m_xRowInfoPL.m_nStringAreaSize = _C_16KB;

  m_xDynamicT.m_xRowInfoPL.m_aRowCellArray = (void*)&m_xDynamicT.m_xRowInfoPL.m_iCell[0];
  m_xDynamicT.m_xRowInfoPL.m_acStrArea     = (char*)&m_xDynamicT.m_xRowInfoPL.m_cStringArea[0];
  m_xDynamicT.m_xRowInfoPL.m_acStrAreaSav  = (char*)&m_xDynamicT.m_xRowInfoPL.m_cStringArea[0];

  // project T (SR_PT)
  m_xDynamicT.m_xRowInfoPT.m_bInitialize = true;
  for (i8 i = 0; i < 256; i++)
    m_xDynamicT.m_xRowInfoPT.m_iCell[i] = 0;
#ifdef WITH_CHECK_INITIALISED
  MemLib::vZero(m_xDynamicT.m_xRowInfoPT.m_cStringArea,_C_16KB,CN_BYTES_T);
#else
  m_xDynamicT.m_xRowInfoPT.m_cStringArea[0] = '\0';
#endif
	m_xDynamicT.m_xRowInfoPT.m_nStringAreaSize = _C_16KB;

  m_xDynamicT.m_xRowInfoPT.m_aRowCellArray = (void*)&m_xDynamicT.m_xRowInfoPT.m_iCell[0];
  m_xDynamicT.m_xRowInfoPT.m_acStrArea     = (char*)&m_xDynamicT.m_xRowInfoPT.m_cStringArea[0];
  m_xDynamicT.m_xRowInfoPT.m_acStrAreaSav  = (char*)&m_xDynamicT.m_xRowInfoPT.m_cStringArea[0];


  // join T & J (SR_TJ)
  m_xDynamicT.m_xRowInfoTJ.m_bInitialize = true;
  for (i8 i = 0; i < 256; i++)
    m_xDynamicT.m_xRowInfoTJ.m_iCell[i] = 0;
#ifdef WITH_CHECK_INITIALISED
  MemLib::vZero(m_xDynamicT.m_xRowInfoTJ.m_cStringArea,_C_16KB,CN_BYTES_T);
#else
  m_xDynamicT.m_xRowInfoTJ.m_cStringArea[0] = '\0';
#endif
	m_xDynamicT.m_xRowInfoTJ.m_nStringAreaSize = _C_16KB;

  m_xDynamicT.m_xRowInfoTJ.m_aRowCellArray = (void*)&m_xDynamicT.m_xRowInfoTJ.m_iCell[0];
  m_xDynamicT.m_xRowInfoTJ.m_acStrArea     = (char*)&m_xDynamicT.m_xRowInfoTJ.m_cStringArea[0];
  m_xDynamicT.m_xRowInfoTJ.m_acStrAreaSav  = (char*)&m_xDynamicT.m_xRowInfoTJ.m_cStringArea[0];


  // aggregate T (SR_AT)
  m_xDynamicT.m_xRowInfoAT.m_bInitialize = true;
  for (i8 i = 0; i < 256; i++)
    m_xDynamicT.m_xRowInfoAT.m_iCell[i] = 0;
#ifdef WITH_CHECK_INITIALISED
  MemLib::vZero(m_xDynamicT.m_xRowInfoAT.m_cStringArea,_C_16KB,CN_BYTES_T);
#else
  m_xDynamicT.m_xRowInfoAT.m_cStringArea[0] = '\0';
#endif
	m_xDynamicT.m_xRowInfoAT.m_nStringAreaSize = _C_16KB;

  m_xDynamicT.m_xRowInfoAT.m_aRowCellArray = (void*)&m_xDynamicT.m_xRowInfoAT.m_iCell[0];
  m_xDynamicT.m_xRowInfoAT.m_acStrArea     = (char*)&m_xDynamicT.m_xRowInfoAT.m_cStringArea[0];
  m_xDynamicT.m_xRowInfoAT.m_acStrAreaSav  = (char*)&m_xDynamicT.m_xRowInfoAT.m_cStringArea[0];

  // tub output (SR_TO)
  m_xDynamicT.m_pTub        = nullptr;
  m_xDynamicT.m_xRowInfoTO.m_bInitialize = true;
  for (i8 i = 0; i < 256; i++)
    m_xDynamicT.m_xRowInfoTO.m_iCell[i] = 0;
#ifdef WITH_CHECK_INITIALISED
  MemLib::vZero(m_xDynamicT.m_xRowInfoTO.m_cStringArea,_C_16KB,CN_BYTES_T);
#else
  m_xDynamicT.m_xRowInfoTO.m_cStringArea[0] = '\0';
#endif
	m_xDynamicT.m_xRowInfoTO.m_nStringAreaSize = _C_16KB;

  m_xDynamicT.m_xRowInfoTO.m_aRowCellArray = (void*)&m_xDynamicT.m_xRowInfoTO.m_iCell[0];
  m_xDynamicT.m_xRowInfoTO.m_acStrArea     = (char*)&m_xDynamicT.m_xRowInfoTO.m_cStringArea[0];
  m_xDynamicT.m_xRowInfoTO.m_acStrAreaSav  = (char*)&m_xDynamicT.m_xRowInfoTO.m_cStringArea[0];

  // pot output (SR_PO)
  m_xDynamicT.m_bGeneratePot = false;
  m_xDynamicT.m_pPot         = nullptr;
  m_xDynamicT.m_xRowInfoPO.m_bInitialize = true;
  for (i8 i = 0; i < 256; i++)
    m_xDynamicT.m_xRowInfoPO.m_iCell[i] = 0;
#ifdef WITH_CHECK_INITIALISED
  MemLib::vZero(m_xDynamicT.m_xRowInfoPO.m_cStringArea,_C_16KB,CN_BYTES_T);
#else
  m_xDynamicT.m_xRowInfoPO.m_cStringArea[0] = '\0';
#endif
	m_xDynamicT.m_xRowInfoPO.m_nStringAreaSize = _C_16KB;

  m_xDynamicT.m_xRowInfoPO.m_aRowCellArray = (void*)&m_xDynamicT.m_xRowInfoPO.m_iCell[0];
  m_xDynamicT.m_xRowInfoPO.m_acStrArea     = (char*)&m_xDynamicT.m_xRowInfoPO.m_cStringArea[0];
  m_xDynamicT.m_xRowInfoPO.m_acStrAreaSav  = (char*)&m_xDynamicT.m_xRowInfoPO.m_cStringArea[0];


  // index scan T
  m_xDynamicT.m_xIndexInfoT.m_bInitialize = true;
#ifdef WITH_CHECK_INITIALISED
  MemLib::vZero(m_xDynamicT.m_xIndexInfoT.m_cTableName,C_MAX_TABLE_NAME_LENGTH,CN_BYTES_T);
  MemLib::vZero(m_xDynamicT.m_xIndexInfoT.m_cIndexName,C_MAX_INDEX_NAME_LENGTH,CN_BYTES_T);
#else
  m_xDynamicT.m_xIndexInfoT.m_cTableName[0] = '\0';
  m_xDynamicT.m_xIndexInfoT.m_cIndexName[0] = '\0';
#endif

  m_xDynamicT.m_xIndexInfoT.m_aTable      = nullptr;
  m_xDynamicT.m_xIndexInfoT.m_aIndex      = nullptr;
  m_xDynamicT.m_xIndexInfoT.m_aIndexTree  = nullptr;
  m_xDynamicT.m_xIndexInfoT.m_aIndexDef   = nullptr;
  m_xDynamicT.m_xIndexInfoT.m_aEntWrk     = (Ent_int*)(void*)&pLogicUnit->m_oEntWrkT;
  m_xDynamicT.m_xIndexInfoT.m_aEntKey     = ((EntWrk*)m_xDynamicT.m_xIndexInfoT.m_aEntWrk)->aKey();
  m_xDynamicT.m_xIndexInfoT.m_aEntBottom  = nullptr;
  m_xDynamicT.m_xIndexInfoT.m_aEntUpper   = nullptr;
  m_xDynamicT.m_xIndexInfoT.m_aEntBuffer  = nullptr;
  m_xDynamicT.m_xIndexInfoT.m_aEntInt     = (Ent_int*)pLogicUnit->m_oEntWrkT.aEnt();

  // index join with J
  m_xDynamicT.m_iJoinedRows    = 0;
  m_xDynamicT.m_xIndexInfoJ.m_bInitialize = true;

#ifdef WITH_CHECK_INITIALISED
  MemLib::vZero(m_xDynamicT.m_xIndexInfoJ.m_cTableName,C_MAX_TABLE_NAME_LENGTH,CN_BYTES_T);
  MemLib::vZero(m_xDynamicT.m_xIndexInfoJ.m_cIndexName,C_MAX_INDEX_NAME_LENGTH,CN_BYTES_T);
#else
  m_xDynamicT.m_xIndexInfoJ.m_cTableName[0] = '\0';
  m_xDynamicT.m_xIndexInfoJ.m_cIndexName[0] = '\0';
#endif

  m_xDynamicT.m_xIndexInfoJ.m_cIndexName[0] = '\0';
  m_xDynamicT.m_xIndexInfoJ.m_aTable      = nullptr;
  m_xDynamicT.m_xIndexInfoJ.m_aIndex      = nullptr;
  m_xDynamicT.m_xIndexInfoJ.m_aIndexTree  = nullptr;
  m_xDynamicT.m_xIndexInfoJ.m_aIndexDef   = nullptr;
  m_xDynamicT.m_xIndexInfoJ.m_aEntWrk     = (void*)&pLogicUnit->m_oEntWrkJ;
  m_xDynamicT.m_xIndexInfoJ.m_aEntKey     = ((EntWrk*)m_xDynamicT.m_xIndexInfoJ.m_aEntWrk)->aKey();
  m_xDynamicT.m_xIndexInfoJ.m_aEntBottom  = nullptr;
  m_xDynamicT.m_xIndexInfoJ.m_aEntUpper   = nullptr;
  m_xDynamicT.m_xIndexInfoJ.m_aEntBuffer  = nullptr;
  m_xDynamicT.m_xIndexInfoJ.m_aEntInt     = (Ent_int*)pLogicUnit->m_oEntWrkJ.aEnt();

  // payload (gby/srt/top)
  m_xDynamicT.m_aiPayload      = nullptr;
  m_xDynamicT.m_xIndexInfoPL.m_bInitialize = true;

#ifdef WITH_CHECK_INITIALISED
  MemLib::vZero(m_xDynamicT.m_xIndexInfoPL.m_cTableName,C_MAX_TABLE_NAME_LENGTH,CN_BYTES_T);
  MemLib::vZero(m_xDynamicT.m_xIndexInfoPL.m_cIndexName,C_MAX_INDEX_NAME_LENGTH,CN_BYTES_T);
#else
  m_xDynamicT.m_xIndexInfoPL.m_cTableName[0] = '\0';
  m_xDynamicT.m_xIndexInfoPL.m_cIndexName[0] = '\0';
#endif

  m_xDynamicT.m_xIndexInfoPL.m_aTable       = nullptr;
  m_xDynamicT.m_xIndexInfoPL.m_aIndex       = nullptr;
  m_xDynamicT.m_xIndexInfoPL.m_aIndexTree   = nullptr;
  m_xDynamicT.m_xIndexInfoPL.m_aIndexDef    = nullptr;
  m_xDynamicT.m_xIndexInfoPL.m_aEntWrk      = (void*)&pLogicUnit->m_oEntWrkPL;
  m_xDynamicT.m_xIndexInfoPL.m_aEntKey      = ((EntWrk*)m_xDynamicT.m_xIndexInfoPL.m_aEntWrk)->aKey();
  m_xDynamicT.m_xIndexInfoPL.m_aEntBottom   = nullptr;
  m_xDynamicT.m_xIndexInfoPL.m_aEntUpper    = nullptr;
  m_xDynamicT.m_xIndexInfoPL.m_aEntBuffer   = nullptr;
  m_xDynamicT.m_xIndexInfoPL.m_aEntInt      = (Ent_int*)pLogicUnit->m_oEntWrkPL.aEnt();

  // ordAgg
//  m_xDynamicT.m_iIndexScan  = 0;
//  m_xDynamicT.m_iHasOrdAggRow = 0;
//  m_xDynamicT.m_iOrdAggDone = 0;

  m_xDynamicT.m_aStreamDefOut = nullptr;

  m_xDynamicT.m_iaPow10[0] = 1;
  for (i8 iIndex = 1; iIndex < 16; iIndex++) {
    m_xDynamicT.m_iaPow10[iIndex] = 10*m_xDynamicT.m_iaPow10[iIndex-1];
  }

	//
	// extern function pointers
	//
	m_xDynamicT.m_fp_vExtern_EntWrkAppendElementStz = nullptr;

	m_xDynamicT.m_fp_aExtern_EntBufferNextEntry = nullptr;
	m_xDynamicT.m_fp_vExtern_EntPayloadCreate = nullptr;

	m_xDynamicT.m_fp_vExtern_IndexFind = nullptr;
	m_xDynamicT.m_fp_vExtern_IndexNew = nullptr;
	m_xDynamicT.m_fp_aExtern_IndexTreeBegin = nullptr;
	m_xDynamicT.m_fp_aExtern_IndexTreeFindEntryAsc = nullptr;
	m_xDynamicT.m_fp_aExtern_IndexTreeFindInsertGenAsc = nullptr;
	m_xDynamicT.m_fp_aExtern_IndexTreeInsertEnt = nullptr;

	m_xDynamicT.m_fp_vExtern_IndexTreePositionGE_gen = nullptr;
	m_xDynamicT.m_fp_vExtern_IndexTreePositionGT_gen = nullptr;
	m_xDynamicT.m_fp_vExtern_IndexTreePositionLE_gen = nullptr;
	m_xDynamicT.m_fp_vExtern_IndexTreePositionLT_gen = nullptr;

	m_xDynamicT.m_fp_vExtern_IndexTreePositionGE_int = nullptr;
	m_xDynamicT.m_fp_vExtern_IndexTreePositionGT_int = nullptr;
	m_xDynamicT.m_fp_vExtern_IndexTreePositionLE_int = nullptr;
	m_xDynamicT.m_fp_vExtern_IndexTreePositionLT_int = nullptr;

	m_xDynamicT.m_fp_aExtern_PdsRowBufferCellArray = nullptr;

	m_xDynamicT.m_fp_aExtern_RowPixNew = nullptr;

	m_xDynamicT.m_fp_aExtern_StreamDefNew = nullptr;

	m_xDynamicT.m_fp_aExtern_TubAllotRowPds = nullptr;
	m_xDynamicT.m_fp_vExtern_TubRowPix = nullptr;
	m_xDynamicT.m_fp_vExtern_TubRowStrSize = nullptr;

  _checkInitialised_(CN_DYNAMIC);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_DYNAMIC,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Dynamic::~Dynamic(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DYNAMIC,_FOOTER_);

  if (m_xDynamicT.m_xIndexInfoPL.m_aIndexTree) {
    IndexTree* pIndexTree = _pIndexTree(m_xDynamicT.m_xIndexInfoPL.m_aIndexTree);
    delete pIndexTree;
  }

  if (m_xDynamicT.m_xIndexInfoPL.m_aIndexDef) {
    IndexDef* pIndexDef = _pIndexDef(m_xDynamicT.m_xIndexInfoPL.m_aIndexDef);
    delete pIndexDef;
  }
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
