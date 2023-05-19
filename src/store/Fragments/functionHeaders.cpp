
/*----------------------------------------------------------------------------*/
/*-- Function header Fragments -----------------------------------------------*/
/*----------------------------------------------------------------------------*/

#include <stdio.h>
#include <string.h>

#include "../src/primitives/BaseTypes_tcc.h"
#include "../src/base/TccStuff.h"

// function-__FUNCTION_CRW__

// marker-start

#ifdef WITH_TINYC
  #include "tcclib.h" // include libc header for TCC
#endif

/*--------------------------------------------------------------------*/
/* flattened function -- CRWBUFFER HANDLER                            */
/*--------------------------------------------------------------------*/
int __FUNCTION_CRW__(struct Dynamic_T* aDynamicT) {

  // marker-end

}

// function-vSetColumns
/*--------------------------------------------------------------------*/
/* vSetColumns --                                                     */
/*--------------------------------------------------------------------*/
int vSetColumns(struct Dynamic_T* aDynamicT) {

  i8 __COLUMNS_IN__;

  // marker-start
  aDynamicT->m_nCols = __COLUMNS_IN__;

  // marker-end
}

// function-vCrwContinue
/*--------------------------------------------------------------------*/
/* vCrwContinue --                                                    */
/*--------------------------------------------------------------------*/
int vCrwContinue(struct Dynamic_T* aDynamicT) {

  // marker-start
  #include "../src/store/vFLAT_Parameters_v2.h"
  #include "../src/store/vFLAT_PreliminaryCrw_v2.h"


  char *szSrcStz;
  char *szDstStz;
  char *szStrArea;
  char *szArg1;
  char *szArg2;
  char *szArg3;
  m8 *amCellAddress;
  i8 bFilterResult;
  i8 bKeyMatch;
  i8 bNullResult;
  i8 bFound;
  i8 *piColumn;
  i8 iIndex;
  i8 nSrcSize;
  i8 nDstSize;
  i8 iOffset;
  i8 iMixIn;
  i8 iBufferJ;
  i8 iItemJ;
  i8 iLeftOuterRow;
  i8 iJoinedRows;
  i8 i_BaseOffsetDataX;
  i8 iIndexBuffer;
  i8 iIndexItem;
  i8 i_CurrentRow;
  i8 iElemSize;
  i8 iArg1;
  i8 iArg2;
  i8 iArg3;
  i8 iScale;
  i8 iInCount;
  i8 *paW;
  union Bcd_T bcdArg1;
  union Bcd_T bcdArg2;
  union Bcd_T bcdArg3;
  union Bcd_T bcdReScale1;
  union Bcd_T bcdReScale2;
  union Bcd_T bcdRound;
  i8 iaPow10[15];
  i8 iScaleDiff;
  union Dte_T xDte1;
  union Dte_T xDte2;
  union Dte_T xDte3;
  union Flt_T fltArg1;
  union Flt_T fltArg2;
  union Flt_T fltArg3;
  union Tsp_T xTsp1;
  union Tsp_T xTsp2;
  union Tsp_T xTsp3;
  union Tme_T xTme1;
  union Tme_T xTme2;
  union Tme_T xTme3;
  void *aIndexCrw;
  void *aEntInserted;
  void *aEntFound;
  void *aCrwJ;

  printf("__FUNCTION_CRW__\n");

  // marker-end

  L_InflateCrwT:
  L_Return:
    return 0;
}


//function-__FUNCTION_EOS__

// marker-start

/*--------------------------------------------------------------------*/
/* flattened function -- EOS Handler                                  */
/*--------------------------------------------------------------------*/
int __FUNCTION_EOS__(struct Dynamic_T* aDynamicT) {

  char *szSrcStz;
  char *szDstStz;
  char *szStrArea;
  char *szArg1;
  char *szArg2;
  char *szArg3;
  m8 *amCellAddress;
  i8 bFilterResult;
  i8 bKeyMatch;
  i8 bNullResult;
  i8 bFound;
  i8 *piColumn;
  i8 iIndex;
  i8 nSrcSize;
  i8 nDstSize;
  i8 iOffset;
  i8 iMixIn;
  i8 iBufferJ;
  i8 iItemJ;
  i8 iLeftOuterRow;
  i8 iJoinedRows;
  i8 i_BaseOffsetDataX;
  i8 iIndexBuffer;
  i8 iIndexItem;
  i8 i_CurrentRow;
  i8 iElemSize;
  i8 iArg1;
  i8 iArg2;
  i8 iArg3;
  i8 iScale;
  i8 iInCount;
  i8 *paW;
  union Bcd_T bcdArg1;
  union Bcd_T bcdArg2;
  union Bcd_T bcdArg3;
  union Bcd_T bcdReScale1;
  union Bcd_T bcdReScale2;
  union Bcd_T bcdRound;
  i8 iaPow10[15];
  i8 iScaleDiff;
  union Dte_T xDte1;
  union Dte_T xDte2;
  union Dte_T xDte3;
  union Flt_T fltArg1;
  union Flt_T fltArg2;
  union Flt_T fltArg3;
  union Tsp_T xTsp1;
  union Tsp_T xTsp2;
  union Tsp_T xTsp3;
  union Tme_T xTme1;
  union Tme_T xTme2;
  union Tme_T xTme3;
  void *aIndexCrw;
  void *aEntInserted;
  void *aEntFound;
  void *aCrwJ;

  printf("__FUNCTION_EOS__\n");

  // marker-end
}

//function-__FUNCTION_INDEX__

// marker-start

/*--------------------------------------------------------------------*/
/* flattened function -- INDEXBUFFER HANDLER                          */
/*--------------------------------------------------------------------*/
int __FUNCTION_INDEX__(struct Dynamic_T* aDynamicT) {

  // marker-end

}

// function-vIndexContinue
/*--------------------------------------------------------------------*/
/* vIndexContinue --                                                  */
/*--------------------------------------------------------------------*/
int vIndexContinue(struct Dynamic_T* aDynamicT) {

  // marker-start
#include "../src/store/vFLAT_Parameters_v2.h"


   char* szSrcStz;
   char* szDstStz;
   char* szStrArea;
   char* szArg1;
   char* szArg2;
   char* szArg3;
   m8*   amCellAddress;
   i8    bFilterResult;
   i8    bKeyMatch;
   i8    bNullResult;
   i8    bFound;
   i8*   piColumn;
   i8    iIndex;
   i8    nSrcSize;
   i8    nDstSize;
   i8    iOffset;
   i8    iMixIn;
   i8    iBufferJ;
   i8    iItemJ;
   i8    iLeftOuterRow;
   i8    iJoinedRows;
   i8    i_BaseOffsetDataX;
   i8    iIndexBuffer;
   i8    iIndexItem;
   i8    i_CurrentRow;
   i8    iElemSize;
   i8    iArg1;
   i8    iArg2;
   i8    iArg3;
   i8    iScale;
   i8    iInCount;
   i8*   paW;
   union Bcd_T bcdArg1;
   union Bcd_T bcdArg2;
   union Bcd_T bcdArg3;
   union Bcd_T bcdReScale1;
   union Bcd_T bcdReScale2;
   union Bcd_T bcdRound;
   i8    iaPow10[15];
   i8    iScaleDiff;
   union Dte_T xDte1;
   union Dte_T xDte2;
   union Dte_T xDte3;
   union Flt_T fltArg1;
   union Flt_T fltArg2;
   union Flt_T fltArg3;
   union Ivl_T ivlArg1;
   union Ivl_T ivlArg2;
   union Ivl_T ivlArg3;
   union Ivl_T wIvlResult;
   union Tsp_T xTsp1;
   union Tsp_T xTsp2;
   union Tsp_T xTsp3;
   union Tme_T xTme1;
   union Tme_T xTme2;
   union Tme_T xTme3;
   void* aIndexCrw;
   void* aEntInserted;
   void* aEntFound;
   void* aCrwJ;
   struct tm* pDateTime;

  printf("__FUNCTION_INDEX__\n");

  // marker-end
}

//function-__FUNCTION_ROW__

// marker-start

/*--------------------------------------------------------------------*/
/* flattened function -- ROWBUFFER HANDLER                            */
/*--------------------------------------------------------------------*/
int __FUNCTION_ROW__(struct Dynamic_T* aDynamicT)
{
  // marker-end
}

// function-vRowContinue
/*--------------------------------------------------------------------*/
/* vRowContinue --                                                    */
/*--------------------------------------------------------------------*/
int vRowContinue(struct Dynamic_T* aDynamicT) {

  // marker-start

#include "../src/store/vFLAT_Parameters_v2.h"

  char *szSrcStz;
  char *szDstStz;
  char *szStrArea;
  char *szArg1;
  char *szArg2;
  char *szArg3;
  m8 *amCellAddress;
  i8 bFilterResult;
  i8 bKeyMatch;
  i8 bNullResult;
  i8 bFound;
  i8 *piColumn;
  i8 iIndex;
  i8 nSrcSize;
  i8 nDstSize;
  i8 iOffset;
  i8 iMixIn;
  i8 iBufferJ;
  i8 iItemJ;
  i8 iLeftOuterRow;
  i8 iJoinedRows;
  i8 i_BaseOffsetDataX;
  i8 iIndexBuffer;
  i8 iIndexItem;
  i8 i_CurrentRow;
  i8 iElemSize;
  i8 iArg1;
  i8 iArg2;
  i8 iArg3;
  i8 iScale;
  i8 iInCount;
  i8 *paW;
  union Bcd_T bcdArg1;
  union Bcd_T bcdArg2;
  union Bcd_T bcdArg3;
  union Bcd_T bcdReScale1;
  union Bcd_T bcdReScale2;
  union Bcd_T bcdRound;
  i8 iaPow10[15];
  i8 iScaleDiff;
  union Dte_T xDte1;
  union Dte_T xDte2;
  union Dte_T xDte3;
  union Flt_T fltArg1;
  union Flt_T fltArg2;
  union Flt_T fltArg3;
  union Tsp_T xTsp1;
  union Tsp_T xTsp2;
  union Tsp_T xTsp3;
  union Tme_T xTme1;
  union Tme_T xTme2;
  union Tme_T xTme3;
  void *aIndexCrw;
  void *aEntInserted;
  void *aEntFound;
  void *aCrwJ;

  printf("__FUNCTION_ROW__\n");

  // marker-end

  return 0;
}
