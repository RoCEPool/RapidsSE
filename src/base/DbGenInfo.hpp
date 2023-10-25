/*============================================================================*/
/* DbGenInfo.hpp -- Table name and column name strings                        */
/*============================================================================*/
#pragma once

enum eDBG // DB GEN Strings identification
{
  DBG_NONE=0, // none
  DBG_PART,
  DBG_PART_PSUPP,
  DBG_SUPP,
  DBG_CUST,
  DBG_ORDER,
  DBG_ORDER_LINE,
  DBG_NATION,
  DBG_REGION,
  DBG_MAX
};

static const i8 DBG_MAX_COL = 20;

extern Stz *aDBG_TblNme[DBG_MAX];
extern Stz *aDBG_Info[DBG_MAX][DBG_MAX_COL];

Stz* szGetDbgTblNme (i8 iTbl);
Stz* szGetDbgColNme (i8 iTbl, i8 iCol);

/* Sample usage:
 * #include "DbGenInfo.hpp"
 *
 * vTrace("%s\n"_Stz, szGetDbgTblNme(DBG_CUST));
 * vTrace("%s\n"_Stz, szGetDbgColNme(DBG_CUST, 2));
 *
 * */



/*============================================================================*/
/* end                                                                        */
/*============================================================================*/