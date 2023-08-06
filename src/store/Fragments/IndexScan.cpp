/*============================================================================*/
/* IndexScan.cpp -- Code Fragments                                            */
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

/*----------------------------------------------------------------------------*/
/*-- Code Fragments ----------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*-- vIndexScanBegin ---------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
// function-vIndexScanBegin
void vIndexScanBegin(Dynamic_T* aDynamicT) {

  i8 i_CurrentRow;
  i8*   paW;

  char* __INDEXNAME__;
  IndexInfo __INDEX_INFO__;

  __L_LABEL_LOOP__:

  // marker-start
  struct __STRUCT_TO__* p__STRUCT_TO__;

  __L_LABEL__:

  p__STRUCT_TO__ = (struct __STRUCT_TO__*)aDynamicT->m_xRowInfoIT.m_aRowCellArray;

  paW = &aDynamicT->m_aW[0];

    strcpy(&aDynamicT->m_xIndexInfoT.m_cTableName[0], "__TABLENAME__");
    strcpy(&aDynamicT->m_xIndexInfoT.m_cIndexName[0], "__INDEXNAME__");

  #ifdef WITH_CLANG
    aDynamicT->m_xIndexInfoT.m_aTable = aExtern_FindTable(aDynamicT->m_xIndexInfoT.m_cTableName);
    aDynamicT->m_xIndexInfoT.m_aIndex = aExtern_FindIndex(aDynamicT->m_xIndexInfoT.m_aTable, aDynamicT->m_xIndexInfoT.m_cIndexName);
    aDynamicT->m_xIndexInfoT.m_aIndexDef = aExtern_GetIndexDef(aDynamicT->m_xIndexInfoT.m_aIndex);
    aDynamicT->m_xIndexInfoT.m_aIndexTree = aExtern_GetIndexTree(aDynamicT->m_xIndexInfoT.m_aTable, aDynamicT->m_xIndexInfoT.m_aIndex);
  #endif

    i_CurrentRow = -1;

  // marker-end

  L_Return:
  return;

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
