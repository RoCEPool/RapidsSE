/*============================================================================*/
/* DbGenInfo.hpp -- Table name and column name strings                        */
/*============================================================================*/

#include "BASE.h"
#include "DbGenInfo.hpp"


Stz *aDBG_TblNme[DBG_MAX]
    {
        "NONE"_Stz,
        "PART"_Stz,
        "PARTSUPP"_Stz,
        "SUPPLIER"_Stz,
        "CUSTOMER"_Stz,
        "ORDERS"_Stz,
        "LINEITEM"_Stz,
        "NATION"_Stz,
        "REGION"_Stz,
    };

Stz *aDBG_Info[DBG_MAX][DBG_MAX_COL]
    {
        {
          "NONE"_Stz
        }
        ,
        {
            "P_PARTKEY"_Stz,   "P_NAME"_Stz,     "P_MFGR"_Stz,        "P_BRAND"_Stz,       "P_TYPE"_Stz,      "P_SIZE"_Stz,          "P_CONTAINER"_Stz,  "P_RETAILPRICE"_Stz,  "P_COMMENT"_Stz
        },
        {
            "PS_PARTKEY"_Stz,  "PS_SUPPKEY"_Stz, "PS_AVAILQTY"_Stz,   "PS_SUPPLYCOST"_Stz, "PS_COMMENT"_Stz
        },
        {
            "S_SUPPKEY"_Stz,   "S_NAME"_Stz,     "S_ADDRESS"_Stz,     "S_NATIONKEY"_Stz,   "S_PHONE"_Stz,     "S_ACCTBAL"_Stz,       "S_COMMENT"_Stz
        },
        {
            "C_CUSTKEY"_Stz,   "C_NAME"_Stz,     "C_ADDRESS"_Stz,     "C_NATIONKEY"_Stz,   "C_PHONE"_Stz,     "C_ACCTBAL"_Stz,       "C_MKTSEGMENT"_Stz, "C_COMMENT"_Stz
        },
        {
            "O_ORDERKEY"_Stz,  "O_CUSTKEY"_Stz,  "O_ORDERSTATUS"_Stz, "O_TOTALPRICE"_Stz,  "O_ORDERDATE"_Stz, "O_ORDERPRIORITY"_Stz, "O_CLERK"_Stz,      "O_SHIPPRIORITY"_Stz, "O_COMMENT"_Stz
        },
        {
            "L_ORDERKEY"_Stz,  "L_PARTKEY"_Stz,  "L_SUPPKEY"_Stz,     "L_LINENUMBER"_Stz,  "L_QUANTITY"_Stz,  "L_EXTENDEDPRICE"_Stz, "L_DISCOUNT"_Stz,   "L_TAX"_Stz,
            "L_RETURNFLAG"_Stz, "L_LINESTATUS"_Stz, "L_SHIPDATE"_Stz, "L_COMMITDATE"_Stz, "L_RECEIPTDATE"_Stz, "L_SHIPINSTRUCT"_Stz, "L_SHIPMODE"_Stz, "L_COMMENT"_Stz
        },
        {
            "N_NATIONKEY"_Stz, "N_NAME"_Stz,     "N_REGIONKEY"_Stz,   "N_COMMENT"_Stz
        },
        {
            "R_REGIONKEY"_Stz, "R_NAME"_Stz,     "R_COMMENT"_Stz
        }
    };

Stz* szGetDbgTblNme (i8 iTbl)
{
  return iTbl < DBG_MAX ? aDBG_TblNme[iTbl] : nullptr;
}

Stz* szGetDbgColNme (i8 iTbl, i8 iCol) {
  if (iTbl < DBG_MAX && iCol < DBG_MAX_COL) {
    return aDBG_Info[iTbl][iCol];
  } else {
    return nullptr;
  }
}