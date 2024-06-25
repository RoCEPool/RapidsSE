/*============================================================================*/
/* RrfJoin.cpp -- Code Fragments                                              */
/*============================================================================*/

#include "BASE.h"
#include "Dynamic.hpp"
#include "IndexDef.hpp"
#include "CodeSynthCommon.h"
#include "ClangDynamic.h"
#include "ClangStuff.h"
#include "Pay.hpp"
#include "Key.hpp"
#include "Ent.hpp"
#include "Ent_int.h"
#include "eKE.h"

/*----------------------------------------------------------------------------*/
/*-- Code Fragments ----------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*-- vRrfJoinBegin ----------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
// function-vRrfJoinBegin
void vRrfJoinBegin(Dynamic_T* aDynamicT) {


  // marker-start
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  struct RowRef {
      union {
          struct {
              u8 m_uBuffer: 32; // which buffer (2^32 * 1MB is 4PB)
              u8 m_uItem: 16; // item in that buffer (2^16 is max of 64K items)
              u8 m_uLevels: 3; // number of skip levels in next entry array
              u8 m_uPayload: 1; // is there is a payload? (not used yet)
              u8 m_uKind: 4; // entry kind
              u8 m_uFree: 1; // free entry?
              u8 m_uDesc: 1; // has descending key?
              u8 m_uSpare: 5; // spare
              u8 m_uGlobal: 1; // is global?
          } Std;        // 64b

          struct {
              u8 m_uBuffer: 32; // which buffer (2^32 * 1MB is 4PB)
              u8 m_uItem: 16; // item in that buffer (2^16 is max of 64K items)
              u8 m_uLevels: 3; // number of skip levels in next entry array
              u8 m_uKind: 4; // entry kind
              u8 m_uFree: 1; // free entry?
              u8 m_uPtn: 7;  // number of skip levels in next entry array
              u8 m_uGlobal: 1; // is global?
          } Glb;        // 64b

          i8 m_iAsI8;
      } Rrf;
  };
  struct RowRef xRrf;

// marker-end
}

/*----------------------------------------------------------------------------*/
/*-- vRrfJoinDereference -----------------------------------------------------*/
/*----------------------------------------------------------------------------*/
// function-vRrfJoinDereference
void vRrfJoinDereference(Dynamic_T* aDynamicT) {

#include "vFLAT_Parameters.h"

  i8 iIndexBuffer;
  i8 iIndexItem;
  i8 i_BaseOffsetDataX;
  i8 CURRENT_DUP_OP;
  char* aIndexCrw;

  IndexInfo __INDEX_INFO__[C_MAX_DUP_OPS];
  RowInfo   __ROW_INFO_TO__[C_MAX_DUP_OPS];
  RowInfo   __ROW_INFO_FROM__[C_MAX_DUP_OPS];
  i8 __COLUMNS_TO_INFLATE_;
  i8 __COLUMNS_INFLATED__;

  static const void* C_ADR_REGION = (void*)0x204800000000;
  static const i8 C_CRW_HEADBYTES = 32;
  static const i8 C_CRW_FIRSTHEAD = 640;

  struct RowRef {
      union {
          struct {
              u8 m_uBuffer: 32; // which buffer (2^32 * 1MB is 4PB)
              u8 m_uItem: 16; // item in that buffer (2^16 is max of 64K items)
              u8 m_uLevels: 3; // number of skip levels in next entry array
              u8 m_uPayload: 1; // is there is a payload? (not used yet)
              u8 m_uKind: 4; // entry kind
              u8 m_uFree: 1; // free entry?
              u8 m_uDesc: 1; // has descending key?
              u8 m_uSpare: 5; // spare
              u8 m_uGlobal: 1; // is global?
          } Std;        // 64b

          struct {
              u8 m_uBuffer: 32; // which buffer (2^32 * 1MB is 4PB)
              u8 m_uItem: 16; // item in that buffer (2^16 is max of 64K items)
              u8 m_uLevels: 3; // number of skip levels in next entry array
              u8 m_uKind: 4; // entry kind
              u8 m_uFree: 1; // free entry?
              u8 m_uPtn: 7;  // number of skip levels in next entry array
              u8 m_uGlobal: 1; // is global?
          } Glb;        // 64b

          i8 m_iAsI8;
      } Rrf;
  };

  struct RowRef xRrf;

  L_InflateAColumn:

  // marker-start

  // process the row
	aDynamicT->m_bProcessRow = true;

  aHeadLineSav = aHeadLine;

  // dereference the entry

  iIndexBuffer = xRrf.Rrf.Glb.m_uBuffer;
  iIndexItem   = xRrf.Rrf.Glb.m_uItem;
  aIndexCrw    = ( (char*)C_ADR_REGION + iIndexBuffer * C_1MB + C_CRW_FIRSTHEAD + iIndexItem * C_CRW_HEADBYTES );

  // inflate the derefenced entry
  aHeadLine = (struct HeadLine_T *)aIndexCrw;
  auMetaCode = META_AREA(aHeadLine);
  ayDstByte = (byte*)__ROW_INFO_TO__[CURRENT_DUP_OP].m_aRowCellArray + 8*__COLUMNS_INFLATED__;
  acStrArea = (char*)__ROW_INFO_TO__[CURRENT_DUP_OP].m_acStrArea;
  racStrArea = acStrArea;
  eStrInflateRule = SI_STRAREA_ADR;
  i_BaseOffsetDataX = C_1B  + C_8B  + __COLUMNS_TO_INFLATE_;
  ayDataByte = ADDR_ADD(byte*,aHeadLine,i_BaseOffsetDataX);

  // marker-end

  __L_LABEL_NEXT__:

  L_Return:
  return;

}

/*----------------------------------------------------------------------------*/
/*-- vRrfJoinReturn ----------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
// function-vRrfJoinReturn
void vRrfJoinReturn(Dynamic_T* aDynamicT) {

  void* aHeadLine;
  void* aHeadLineSav;
  void* aSubReturnLabel;

  i8 CURRENT_DUP_OP;

  IndexInfo __INDEX_INFO__[C_MAX_DUP_OPS];
  RowInfo __ROW_INFO_TO__[C_MAX_DUP_OPS];

  bool bLeftOuterRow;

  // marker-start
  goto __L_LABEL_INDEX_RETURN__;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // INFLATE-CRW-RET
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  __L_LABEL_INDEX_RETURN__:

  aHeadLine = aHeadLineSav;
  aDynamicT->m_iJoinedRows++;

  goto __L_LABEL_NEXT__;

  __L_LABEL_LOOP__:

  __ROW_INFO_TO__[CURRENT_DUP_OP].m_acStrArea = __ROW_INFO_TO__[CURRENT_DUP_OP].m_acStrAreaSav;

  goto __L_LABEL_OUTER_LOOP__;

  // marker-end

  __L_LABEL_NEXT__:
  __L_LABEL_OUTER_LOOP__:

  return;
}

/*----------------------------------------------------------------------------*/
/*-- vCopyRrfCol -------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
// function-vCopyRrfCol
void vCopyRrfCol(Dynamic_T* aDynamicT) {

  struct RowRef {
      union {
          struct {
              u8 m_uBuffer: 32; // which buffer (2^32 * 1MB is 4PB)
              u8 m_uItem: 16; // item in that buffer (2^16 is max of 64K items)
              u8 m_uLevels: 3; // number of skip levels in next entry array
              u8 m_uPayload: 1; // is there is a payload? (not used yet)
              u8 m_uKind: 4; // entry kind
              u8 m_uFree: 1; // free entry?
              u8 m_uDesc: 1; // has descending key?
              u8 m_uSpare: 5; // spare
              u8 m_uGlobal: 1; // is global?
          } Std;        // 64b

          struct {
              u8 m_uBuffer: 32; // which buffer (2^32 * 1MB is 4PB)
              u8 m_uItem: 16; // item in that buffer (2^16 is max of 64K items)
              u8 m_uLevels: 3; // number of skip levels in next entry array
              u8 m_uKind: 4; // entry kind
              u8 m_uFree: 1; // free entry?
              u8 m_uPtn: 7;  // number of skip levels in next entry array
              u8 m_uGlobal: 1; // is global?
          } Glb;        // 64b

          i8 m_iAsI8;
      } Rrf;
  };

  struct RowRef xRrf;

  struct __STRUCT_FROM___PREV_OP {
      union Vch_T __COLUMN_FROM__;
  };

  struct __STRUCT_FROM___PREV_OP* p__STRUCT_FROM___PREV_OP;

  // marker-start
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // column __COLUMN_NO__

  xRrf.Rrf.m_iAsI8 = p__STRUCT_FROM___PREV_OP->__COLUMN_FROM__.m_iAsI8;

  // marker-end
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
