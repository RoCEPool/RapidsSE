/*============================================================================*/
/* IndexScan.cpp -- Code Fragments                                            */
/*============================================================================*/

#include "BASE.h"
#include "Dynamic.hpp"
#include "IndexDef.hpp"
#include "TccStuff.h"
#include "Pay.hpp"
#include "Key.hpp"
#include "Ent.hpp"
#include "Ent_int.h"

/*----------------------------------------------------------------------------*/
/*-- Code Fragments ----------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*-- vIndexPositionBegin -----------------------------------------------------*/
/*----------------------------------------------------------------------------*/
// function-vIndexPositionBegin
void vIndexPositionBegin(Dynamic_T* aDynamicT) {

  IndexInfo __INDEX_INFO__;

  // marker-start

#ifdef WITH_TINYC
  // position at beginning of the index
  __INDEX_INFO__.m_aEntInt =(struct Ent_int*)aIndexTree_begin(
        __INDEX_INFO__.m_aIndexTree, &__INDEX_INFO__.m_aEntBuffer);
#endif

  if ( ! aDynamicT->m_xIndexInfoT.m_aEntInt )
    goto L_Return;

  // marker-end

  L_Return:
  return;

}


/*----------------------------------------------------------------------------*/
/*-- vIndexPositionLower -----------------------------------------------------*/
/*----------------------------------------------------------------------------*/
// function-vIndexPositionLower
void vIndexPositionLower(Dynamic_T* aDynamicT) {

  IndexInfo __INDEX_INFO__;

  // marker-start

#ifdef WITH_TINYC
  // position at lower bound
  __INDEX_INFO__.m_aEntInt =  (struct Ent_int *)__INDEX_POSITION_LOWER__(
      __INDEX_INFO__.m_aIndexTree, &__INDEX_INFO__.m_aEntBuffer, __INDEX_INFO__.m_aEntKey);
#endif

  if ( ! __INDEX_INFO__.m_aEntInt )
    goto L_Return;

  // marker-end

  L_Return:
  return;

}

/*----------------------------------------------------------------------------*/
/*-- vIndexPositionUpper -----------------------------------------------------*/
/*----------------------------------------------------------------------------*/
// function-vIndexPositionUpper
void vIndexPositionUpper(Dynamic_T* aDynamicT) {

  IndexInfo __INDEX_INFO__;

  // marker-start

#ifdef WITH_TINYC
  // position at upper bound
  __INDEX_INFO__.m_aEntUpper =  (struct Ent_int *)__INDEX_POSITION_UPPER__(
      __INDEX_INFO__.m_aIndexTree, __INDEX_INFO__.m_aEntKey);
#endif

  // reached upper bound ?
  if (__INDEX_INFO__.m_aEntInt == __INDEX_INFO__.m_aEntUpper) {
    goto L_Return;
  }

  // process the row
  aDynamicT->m_iProcessRow = 1;

  // marker-end

  L_Return:
  return;

}

/*----------------------------------------------------------------------------*/
/*-- vIndexCheckUpper --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
// function-vIndexCheckUpper
void vIndexCheckUpper(Dynamic_T* aDynamicT) {

  IndexInfo __INDEX_INFO__;

  // marker-start

  L_IndexScanDeref:

  // reached upper bound ?
  if (__INDEX_INFO__.m_aEntInt == __INDEX_INFO__.m_aEntUpper) {
    goto L_Return;
  }

  // process the row
  aDynamicT->m_iProcessRow = 1;

  // marker-end

  L_Return:
  return;

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
