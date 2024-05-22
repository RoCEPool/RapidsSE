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

  for (i8 iIndex = 0; iIndex < C_MAX_DUP_OPS; iIndex++) {

    m_xDynamicT.m_xRowInfoIT[iIndex].m_bInitialize = true;

    for (i8 i = 0; i < 256; i++)
      m_xDynamicT.m_xRowInfoIT[iIndex].m_iCell[i] = 0;
#ifdef WITH_CHECK_INITIALISED
    MemLib::vZero(m_xDynamicT.m_xRowInfoIT[iIndex].m_cStringArea,_C_16KB,CN_BYTES_T);
#else
    m_xDynamicT.m_xRowInfoIT[iIndex].m_cStringArea[0] = '\0';
#endif
    m_xDynamicT.m_xRowInfoIT[iIndex].m_nStringAreaSize = _C_16KB;

    m_xDynamicT.m_xRowInfoIT[iIndex].m_aPdsRowBuffer = nullptr;
    m_xDynamicT.m_xRowInfoIT[iIndex].m_aRowCellArray = (void *) &m_xDynamicT.m_xRowInfoIT[iIndex].m_iCell[0];
    m_xDynamicT.m_xRowInfoIT[iIndex].m_acStrArea = (char *) &m_xDynamicT.m_xRowInfoIT[iIndex].m_cStringArea[0];
    m_xDynamicT.m_xRowInfoIT[iIndex].m_acStrAreaSav = (char *) &m_xDynamicT.m_xRowInfoIT[iIndex].m_cStringArea[0];

    // inflate J (SR_IJ)
    m_xDynamicT.m_xRowInfoIJ[iIndex].m_bInitialize = true;

    for (i8 i = 0; i < 256; i++)
      m_xDynamicT.m_xRowInfoIJ[iIndex].m_iCell[i] = 0;
#ifdef WITH_CHECK_INITIALISED
    MemLib::vZero(m_xDynamicT.m_xRowInfoIJ[iIndex].m_cStringArea,_C_16KB,CN_BYTES_T);
#else
    m_xDynamicT.m_xRowInfoIJ[iIndex].m_cStringArea[0] = '\0';
#endif
    m_xDynamicT.m_xRowInfoIJ[iIndex].m_nStringAreaSize = _C_16KB;

    m_xDynamicT.m_xRowInfoIJ[iIndex].m_aPdsRowBuffer = nullptr;
    m_xDynamicT.m_xRowInfoIJ[iIndex].m_aRowCellArray = (void *) &m_xDynamicT.m_xRowInfoIJ[iIndex].m_iCell[0];
    m_xDynamicT.m_xRowInfoIJ[iIndex].m_acStrArea = (char *) &m_xDynamicT.m_xRowInfoIJ[iIndex].m_cStringArea[0];
    m_xDynamicT.m_xRowInfoIJ[iIndex].m_acStrAreaSav = (char *) &m_xDynamicT.m_xRowInfoIJ[iIndex].m_cStringArea[0];

    // project PL (SR_PL)
    m_xDynamicT.m_xRowInfoPL[iIndex].m_bInitialize = true;

    for (i8 i = 0; i < 256; i++)
      m_xDynamicT.m_xRowInfoPL[iIndex].m_iCell[i] = 0;
#ifdef WITH_CHECK_INITIALISED
    MemLib::vZero(m_xDynamicT.m_xRowInfoPL[iIndex].m_cStringArea,_C_16KB,CN_BYTES_T);
#else
    m_xDynamicT.m_xRowInfoPL[iIndex].m_cStringArea[0] = '\0';
#endif
    m_xDynamicT.m_xRowInfoPL[iIndex].m_nStringAreaSize = _C_16KB;

    m_xDynamicT.m_xRowInfoPL[iIndex].m_aPdsRowBuffer = nullptr;
    m_xDynamicT.m_xRowInfoPL[iIndex].m_aRowCellArray = (void *) &m_xDynamicT.m_xRowInfoPL[iIndex].m_iCell[0];
    m_xDynamicT.m_xRowInfoPL[iIndex].m_acStrArea = (char *) &m_xDynamicT.m_xRowInfoPL[iIndex].m_cStringArea[0];
    m_xDynamicT.m_xRowInfoPL[iIndex].m_acStrAreaSav = (char *) &m_xDynamicT.m_xRowInfoPL[iIndex].m_cStringArea[0];

    // project T (SR_PT)
    m_xDynamicT.m_xRowInfoPT[iIndex].m_bInitialize = true;

    for (i8 i = 0; i < 256; i++)
      m_xDynamicT.m_xRowInfoPT[iIndex].m_iCell[i] = 0;

#ifdef WITH_CHECK_INITIALISED
    MemLib::vZero(m_xDynamicT.m_xRowInfoPT[iIndex].m_cStringArea,_C_16KB,CN_BYTES_T);
#else
    m_xDynamicT.m_xRowInfoPT[iIndex].m_cStringArea[0] = '\0';
#endif
    m_xDynamicT.m_xRowInfoPT[iIndex].m_nStringAreaSize = _C_16KB;

    m_xDynamicT.m_xRowInfoPT[iIndex].m_aPdsRowBuffer = nullptr;
    m_xDynamicT.m_xRowInfoPT[iIndex].m_aRowCellArray = (void *) &m_xDynamicT.m_xRowInfoPT[iIndex].m_iCell[0];
    m_xDynamicT.m_xRowInfoPT[iIndex].m_acStrArea = (char *) &m_xDynamicT.m_xRowInfoPT[iIndex].m_cStringArea[0];
    m_xDynamicT.m_xRowInfoPT[iIndex].m_acStrAreaSav = (char *) &m_xDynamicT.m_xRowInfoPT[iIndex].m_cStringArea[0];

    // join T & J (SR_TJ)

    m_xDynamicT.m_xRowInfoTJ[iIndex].m_bInitialize = true;

    for (i8 i = 0; i < 256; i++)
      m_xDynamicT.m_xRowInfoTJ[iIndex].m_iCell[i] = 0;
#ifdef WITH_CHECK_INITIALISED
    MemLib::vZero(m_xDynamicT.m_xRowInfoTJ[iIndex].m_cStringArea,_C_16KB,CN_BYTES_T);
#else
    m_xDynamicT.m_xRowInfoTJ[iIndex].m_cStringArea[0] = '\0';
#endif
    m_xDynamicT.m_xRowInfoTJ[iIndex].m_nStringAreaSize = _C_16KB;

    m_xDynamicT.m_xRowInfoTJ[iIndex].m_aPdsRowBuffer = nullptr;
    m_xDynamicT.m_xRowInfoTJ[iIndex].m_aRowCellArray = (void *) &m_xDynamicT.m_xRowInfoTJ[iIndex].m_iCell[0];
    m_xDynamicT.m_xRowInfoTJ[iIndex].m_acStrArea = (char *) &m_xDynamicT.m_xRowInfoTJ[iIndex].m_cStringArea[0];
    m_xDynamicT.m_xRowInfoTJ[iIndex].m_acStrAreaSav = (char *) &m_xDynamicT.m_xRowInfoTJ[iIndex].m_cStringArea[0];

    // aggregate T (SR_AT)
    m_xDynamicT.m_xRowInfoAT[iIndex].m_bInitialize = true;

    for (i8 i = 0; i < 256; i++)
      m_xDynamicT.m_xRowInfoAT[iIndex].m_iCell[i] = 0;
#ifdef WITH_CHECK_INITIALISED
    MemLib::vZero(m_xDynamicT.m_xRowInfoAT[iIndex].m_cStringArea,_C_16KB,CN_BYTES_T);
#else
    m_xDynamicT.m_xRowInfoAT[iIndex].m_cStringArea[0] = '\0';
#endif
    m_xDynamicT.m_xRowInfoAT[iIndex].m_nStringAreaSize = _C_16KB;

    m_xDynamicT.m_xRowInfoAT[iIndex].m_aPdsRowBuffer = nullptr;
    m_xDynamicT.m_xRowInfoAT[iIndex].m_aRowCellArray = (void *) &m_xDynamicT.m_xRowInfoAT[iIndex].m_iCell[0];
    m_xDynamicT.m_xRowInfoAT[iIndex].m_acStrArea = (char *) &m_xDynamicT.m_xRowInfoAT[iIndex].m_cStringArea[0];
    m_xDynamicT.m_xRowInfoAT[iIndex].m_acStrAreaSav = (char *) &m_xDynamicT.m_xRowInfoAT[iIndex].m_cStringArea[0];

    // tub output (SR_TO)
    m_xDynamicT.m_pTub = nullptr;
    m_xDynamicT.m_xRowInfoTO[iIndex].m_bInitialize = true;

    for (i8 i = 0; i < 256; i++)
      m_xDynamicT.m_xRowInfoTO[iIndex].m_iCell[i] = 0;
#ifdef WITH_CHECK_INITIALISED
    MemLib::vZero(m_xDynamicT.m_xRowInfoTO[iIndex].m_cStringArea,_C_16KB,CN_BYTES_T);
#else
    m_xDynamicT.m_xRowInfoTO[iIndex].m_cStringArea[0] = '\0';
#endif
    m_xDynamicT.m_xRowInfoTO[iIndex].m_nStringAreaSize = _C_16KB;

    m_xDynamicT.m_xRowInfoTO[iIndex].m_aPdsRowBuffer = nullptr;
    m_xDynamicT.m_xRowInfoTO[iIndex].m_aRowCellArray = (void *) &m_xDynamicT.m_xRowInfoTO[iIndex].m_iCell[0];
    m_xDynamicT.m_xRowInfoTO[iIndex].m_acStrArea = (char *) &m_xDynamicT.m_xRowInfoTO[iIndex].m_cStringArea[0];
    m_xDynamicT.m_xRowInfoTO[iIndex].m_acStrAreaSav = (char *) &m_xDynamicT.m_xRowInfoTO[iIndex].m_cStringArea[0];

    // pot output (SR_PO)
    m_xDynamicT.m_bGeneratePot = false;
    m_xDynamicT.m_pPot = nullptr;
    m_xDynamicT.m_xRowInfoPO[iIndex].m_bInitialize = true;

    for (i8 i = 0; i < 256; i++)
      m_xDynamicT.m_xRowInfoPO[iIndex].m_iCell[i] = 0;
#ifdef WITH_CHECK_INITIALISED
    MemLib::vZero(m_xDynamicT.m_xRowInfoPO[iIndex].m_cStringArea,_C_16KB,CN_BYTES_T);
#else
    m_xDynamicT.m_xRowInfoPO[iIndex].m_cStringArea[0] = '\0';
#endif
    m_xDynamicT.m_xRowInfoPO[iIndex].m_nStringAreaSize = _C_16KB;

    m_xDynamicT.m_xRowInfoPO[iIndex].m_aPdsRowBuffer = nullptr;
    m_xDynamicT.m_xRowInfoPO[iIndex].m_aRowCellArray = (void *) &m_xDynamicT.m_xRowInfoPO[iIndex].m_iCell[0];
    m_xDynamicT.m_xRowInfoPO[iIndex].m_acStrArea = (char *) &m_xDynamicT.m_xRowInfoPO[iIndex].m_cStringArea[0];
    m_xDynamicT.m_xRowInfoPO[iIndex].m_acStrAreaSav = (char *) &m_xDynamicT.m_xRowInfoPO[iIndex].m_cStringArea[0];


    // index scan T
    m_xDynamicT.m_xIndexInfoT[iIndex].m_bInitialize = true;
#ifdef WITH_CHECK_INITIALISED
    MemLib::vZero(m_xDynamicT.m_xIndexInfoT[iIndex].m_cTableName,C_MAX_TABLE_NAME_LENGTH,CN_BYTES_T);
    MemLib::vZero(m_xDynamicT.m_xIndexInfoT[iIndex].m_cIndexName,C_MAX_INDEX_NAME_LENGTH,CN_BYTES_T);
#else
    m_xDynamicT.m_xIndexInfoT[iIndex].m_cTableName[0] = '\0';
    m_xDynamicT.m_xIndexInfoT[iIndex].m_cIndexName[0] = '\0';
#endif

    m_xDynamicT.m_xIndexInfoT[iIndex].m_aTable = nullptr;
    m_xDynamicT.m_xIndexInfoT[iIndex].m_aIndex = nullptr;
    m_xDynamicT.m_xIndexInfoT[iIndex].m_aIndexTree = nullptr;
    m_xDynamicT.m_xIndexInfoT[iIndex].m_aIndexDef = nullptr;
    m_xDynamicT.m_xIndexInfoT[iIndex].m_aEntWrk = (Ent_int * )(void * ) & pLogicUnit->m_oEntWrkT;
    m_xDynamicT.m_xIndexInfoT[iIndex].m_aEntKey = ((EntWrk *) m_xDynamicT.m_xIndexInfoT[iIndex].m_aEntWrk)->aKey();
    m_xDynamicT.m_xIndexInfoT[iIndex].m_aEntBottom = nullptr;
    m_xDynamicT.m_xIndexInfoT[iIndex].m_aEntUpper = nullptr;
    m_xDynamicT.m_xIndexInfoT[iIndex].m_aEntBuffer = nullptr;
    m_xDynamicT.m_xIndexInfoT[iIndex].m_aEntInt = (Ent_int *) pLogicUnit->m_oEntWrkT.aEnt();

    // index join with J
    m_xDynamicT.m_iJoinedRows = 0;
    m_xDynamicT.m_xIndexInfoJ[iIndex].m_bInitialize = true;

#ifdef WITH_CHECK_INITIALISED
    MemLib::vZero(m_xDynamicT.m_xIndexInfoJ[iIndex].m_cTableName,C_MAX_TABLE_NAME_LENGTH,CN_BYTES_T);
    MemLib::vZero(m_xDynamicT.m_xIndexInfoJ[iIndex].m_cIndexName,C_MAX_INDEX_NAME_LENGTH,CN_BYTES_T);
#else
    m_xDynamicT.m_xIndexInfoJ[iIndex].m_cTableName[0] = '\0';
    m_xDynamicT.m_xIndexInfoJ[iIndex].m_cIndexName[0] = '\0';
#endif

    m_xDynamicT.m_xIndexInfoJ[iIndex].m_cIndexName[0] = '\0';
    m_xDynamicT.m_xIndexInfoJ[iIndex].m_aTable = nullptr;
    m_xDynamicT.m_xIndexInfoJ[iIndex].m_aIndex = nullptr;
    m_xDynamicT.m_xIndexInfoJ[iIndex].m_aIndexTree = nullptr;
    m_xDynamicT.m_xIndexInfoJ[iIndex].m_aIndexDef = nullptr;
    m_xDynamicT.m_xIndexInfoJ[iIndex].m_aEntWrk = (void *) &pLogicUnit->m_oEntWrkJ;
    m_xDynamicT.m_xIndexInfoJ[iIndex].m_aEntKey = ((EntWrk *) m_xDynamicT.m_xIndexInfoJ[iIndex].m_aEntWrk)->aKey();
    m_xDynamicT.m_xIndexInfoJ[iIndex].m_aEntBottom = nullptr;
    m_xDynamicT.m_xIndexInfoJ[iIndex].m_aEntUpper = nullptr;
    m_xDynamicT.m_xIndexInfoJ[iIndex].m_aEntBuffer = nullptr;
    m_xDynamicT.m_xIndexInfoJ[iIndex].m_aEntInt = (Ent_int *) pLogicUnit->m_oEntWrkJ.aEnt();

    // payload (gby/srt/top)
    m_xDynamicT.m_aiPayload = nullptr;
    m_xDynamicT.m_xIndexInfoPL[iIndex].m_bInitialize = true;

#ifdef WITH_CHECK_INITIALISED
    MemLib::vZero(m_xDynamicT.m_xIndexInfoPL[iIndex].m_cTableName,C_MAX_TABLE_NAME_LENGTH,CN_BYTES_T);
    MemLib::vZero(m_xDynamicT.m_xIndexInfoPL[iIndex].m_cIndexName,C_MAX_INDEX_NAME_LENGTH,CN_BYTES_T);
#else
    m_xDynamicT.m_xIndexInfoPL[iIndex].m_cTableName[0] = '\0';
    m_xDynamicT.m_xIndexInfoPL[iIndex].m_cIndexName[0] = '\0';
#endif

    m_xDynamicT.m_xIndexInfoPL[iIndex].m_aTable = nullptr;
    m_xDynamicT.m_xIndexInfoPL[iIndex].m_aIndex = nullptr;
    m_xDynamicT.m_xIndexInfoPL[iIndex].m_aIndexTree = nullptr;
    m_xDynamicT.m_xIndexInfoPL[iIndex].m_aIndexDef = nullptr;
    m_xDynamicT.m_xIndexInfoPL[iIndex].m_aEntWrk = (void *) &pLogicUnit->m_oEntWrkPL;
    m_xDynamicT.m_xIndexInfoPL[iIndex].m_aEntKey = ((EntWrk *) m_xDynamicT.m_xIndexInfoPL[iIndex].m_aEntWrk)->aKey();
    m_xDynamicT.m_xIndexInfoPL[iIndex].m_aEntBottom = nullptr;
    m_xDynamicT.m_xIndexInfoPL[iIndex].m_aEntUpper = nullptr;
    m_xDynamicT.m_xIndexInfoPL[iIndex].m_aEntBuffer = nullptr;
    m_xDynamicT.m_xIndexInfoPL[iIndex].m_aEntInt = (Ent_int *) pLogicUnit->m_oEntWrkPL.aEnt();
  }

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
	m_xDynamicT.m_fp_vExtern_EntWrkReset = nullptr;

	m_xDynamicT.m_fp_aExtern_IndexTreeAllotPayStr = nullptr;
	m_xDynamicT.m_fp_vExtern_IndexFind = nullptr;
	m_xDynamicT.m_fp_vExtern_IndexNew = nullptr;
	m_xDynamicT.m_fp_aExtern_IndexTreeBegin = nullptr;
	m_xDynamicT.m_fp_aExtern_IndexTreeFindEntryAsc = nullptr;
	m_xDynamicT.m_fp_aExtern_IndexTreeFindEntryAscDsc = nullptr;
	m_xDynamicT.m_fp_aExtern_IndexTreeFindInsertGenAsc = nullptr;
	m_xDynamicT.m_fp_aExtern_IndexTreeInsertEnt    = nullptr;
	m_xDynamicT.m_fp_aExtern_IndexTreeInsertEntTop = nullptr;

	m_xDynamicT.m_fp_vExtern_IndexTreePositionGE_gen = nullptr;
	m_xDynamicT.m_fp_vExtern_IndexTreePositionGT_gen = nullptr;
	m_xDynamicT.m_fp_vExtern_IndexTreePositionLE_gen = nullptr;
	m_xDynamicT.m_fp_vExtern_IndexTreePositionLT_gen = nullptr;

	m_xDynamicT.m_fp_vExtern_IndexTreePositionGE_int = nullptr;
	m_xDynamicT.m_fp_vExtern_IndexTreePositionGT_int = nullptr;
	m_xDynamicT.m_fp_vExtern_IndexTreePositionLE_int = nullptr;
	m_xDynamicT.m_fp_vExtern_IndexTreePositionLT_int = nullptr;

	m_xDynamicT.m_fp_aExtern_PdsFirstRowCellArray  = nullptr;
	m_xDynamicT.m_fp_aExtern_PdsNextRowCellArray   = nullptr;
	m_xDynamicT.m_fp_vExtern_PotAllotRowPds   = nullptr;
	m_xDynamicT.m_fp_vExtern_PotRowStrSize    = nullptr;

	m_xDynamicT.m_fp_aExtern_StreamDefNew = nullptr;

	m_xDynamicT.m_fp_vExtern_TubAllotRowPds = nullptr;
	m_xDynamicT.m_fp_vExtern_TubRowStrSize  = nullptr;

  _checkInitialised_(CN_DYNAMIC);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_DYNAMIC,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Dynamic::~Dynamic(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DYNAMIC,_FOOTER_);

  StreamDef* pStreamDef = _pStreamDef(m_xDynamicT.m_aStreamDefOut);
  delete pStreamDef;

  if (m_xDynamicT.m_xIndexInfoPL[0].m_aIndexTree) {
    IndexTree* pIndexTree = _pIndexTree(m_xDynamicT.m_xIndexInfoPL[0].m_aIndexTree);
    delete pIndexTree;
  }

  if (m_xDynamicT.m_xIndexInfoPL[0].m_aIndexDef) {
    IndexDef* pIndexDef = _pIndexDef(m_xDynamicT.m_xIndexInfoPL[0].m_aIndexDef);
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
