/*============================================================================*/
/* MathOp.h -- math operators                                                 */
/*============================================================================*/
#pragma once

#include <float.h>
#include <limits.h>
#include <math.h>

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// AGGREGATE OPERATORS
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

i8 C_D8_MIN_VALUE = DBL_MIN + DBL_EPSILON;

static eDM eMathOp_AvgResult[DM_ENUMS] =
    //   NONE     BCD     BOL      DTE      FLT     INT     IVL           IVY           TME           TSP      VCH      UNK
    { DM_NONE, DM_BCD, DM_NONE, DM_NONE, DM_FLT, DM_BCD, DM_NONE/*X*/, DM_NONE/*X*/, DM_NONE/*X*/, DM_NONE, DM_NONE, DM_NONE };


static eDM eMathOp_MaxResult[DM_ENUMS] =
    //  NONE     BCD     BOL      DTE     FLT     INT     IVL           IVY       TME     TSP     VCH     UNK
    { DM_NONE, DM_BCD, DM_NONE, DM_DTE, DM_FLT, DM_INT, DM_NONE/*X*/, DM_NONE/*X*/, DM_TME, DM_TSP, DM_VCH, DM_NONE };

static eEO eMathOp_MaxCode[DM_ENUMS] =
    //  NONE     BCD           BOL           DTE           FLT           INT           IVL           IVY       TME           TSP           VCH           UNK
    { EO_NONE, EO_GT_BCDBCD, EO_NONE/*X*/, EO_GT_DTEDTE, EO_GT_FLTFLT, EO_GT_INTINT, EO_NONE/*X*/, EO_NONE/*X*/,EO_GT_TMETME, EO_GT_TSPTSP, EO_GT_VCHVCH, EO_NONE/*X*/ };


static eDM eMathOp_MinResult[DM_ENUMS] =
  //  NONE     BCD     BOL           DTE      FLT    INT     IVL           IVY          TME     TSP     VCH     UNK
    { DM_NONE, DM_BCD, DM_NONE/*X*/, DM_DTE, DM_FLT, DM_INT, DM_NONE/*X*/, DM_NONE/*X*/,DM_TME, DM_TSP, DM_VCH, DM_NONE/*X*/ };

static eEO eMathOp_MinCode[DM_ENUMS] =
  //  NONE     BCD           BOL           DTE           FLT           INT           IVL           IVY       TME           TSP           VCH           UNK
    { EO_NONE, EO_LT_BCDBCD, EO_NONE/*X*/, EO_LT_DTEDTE, EO_LT_FLTFLT, EO_LT_INTINT, EO_NONE/*X*/, EO_NONE/*X*/,EO_LT_TMETME, EO_LT_TSPTSP, EO_LT_VCHVCH, EO_NONE/*X*/ };




static eDM eMathOp_SumResult[DM_ENUMS] =
    //   NONE     BCD     BOL      DTE      FLT     INT     IVL           IVY          TME      TSP      VCH      UNK
       { DM_NONE, DM_BCD, DM_NONE, DM_NONE, DM_FLT, DM_INT, DM_NONE/*X*/, DM_NONE/*X*/,DM_NONE, DM_NONE, DM_NONE, DM_NONE};

static eEO eMathOp_SumCode[DM_ENUMS] =
    //   NONE     BCD            BOL      DTE      FLT            INT            IVL           IVY        TME      TSP      VCH      UNK
    {    EO_NONE, EO_ADD_BCDBCD, EO_NONE, EO_NONE, EO_ADD_FLTFLT, EO_ADD_INTINT, EO_NONE/*X*/, EO_NONE/*X*/, EO_NONE, EO_NONE, EO_NONE, EO_NONE};


// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// DATUMS

static eEO eMathOp_DatumCode[DM_ENUMS] =
    // NONE     BCD         BOL      DTE      FLT      INT         IVL      IVY,     TME      TSP           VCH        UNK
    {  EO_NONE, EO_INTEGER, EO_NONE, EO_DATE, EO_NONE, EO_INTEGER, EO_NONE, EO_NONE, EO_TIME, EO_TIMESTAMP, EO_STRING, EO_PUSHNULL    };

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// AND

static eDM eMathOp_CmpResult[DM_ENUMS][DM_ENUMS] = {
           //   NONE     BCD       BOL       DTE           FLT         INT           IVL         IVY,      TME          TSP            VCH      UNK
/*  0 NONE */ { DM_NONE, DM_NONE,  DM_NONE,  DM_NONE,      DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE,  DM_NONE,      DM_NONE,    DM_NONE, DM_NONE    },
/*  1 BCD  */ { DM_NONE, DM_BOL,   DM_NONE,  DM_NONE,      DM_BOL,     DM_BOL,       DM_NONE,    DM_NONE,  DM_NONE,      DM_NONE,    DM_NONE, DM_BOL    },
/*  2 BOL  */ { DM_NONE, DM_NONE,  DM_NONE,  DM_NONE,      DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE,  DM_NONE,      DM_NONE,    DM_NONE, DM_BOL     },
/*  3 DTE  */ { DM_NONE, DM_NONE,  DM_NONE,  DM_BOL,       DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE,  DM_NONE,      DM_BOL,     DM_NONE, DM_BOL    },
/*  4 FLT  */ { DM_NONE, DM_BOL,   DM_NONE,  DM_NONE,      DM_BOL,     DM_BOL,       DM_NONE,    DM_NONE,  DM_NONE,      DM_NONE,    DM_NONE, DM_BOL    },
/*  5 INT  */ { DM_NONE, DM_BOL,   DM_NONE,  DM_NONE,      DM_BOL,     DM_BOL,       DM_NONE,    DM_NONE,  DM_NONE,      DM_NONE,    DM_NONE, DM_BOL    },
/*  6 IVL  */ { DM_NONE, DM_NONE,  DM_NONE,  DM_NONE,      DM_NONE,    DM_NONE,      DM_BOL,     DM_BOL,   DM_NONE,      DM_NONE,    DM_NONE, DM_BOL    },
/*  6 IVY  */ { DM_NONE, DM_NONE,  DM_NONE,  DM_NONE,      DM_NONE,    DM_NONE,      DM_BOL,     DM_BOL,   DM_NONE,      DM_NONE,    DM_NONE, DM_BOL    },
/*  7 TME  */ { DM_NONE, DM_NONE,  DM_NONE,  DM_NONE,      DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE,  DM_BOL,       DM_BOL,     DM_NONE, DM_BOL    },
/*  8 TSP  */ { DM_NONE, DM_NONE,  DM_NONE,  DM_BOL,       DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE,  DM_BOL,       DM_BOL,     DM_NONE, DM_BOL    },
/*  9 VCH  */ { DM_NONE, DM_NONE,  DM_NONE,  DM_NONE,      DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE,  DM_NONE,      DM_NONE,    DM_BOL,  DM_BOL    },
/* 10 UNK  */ { DM_NONE, DM_BOL,   DM_BOL,   DM_BOL,       DM_BOL,     DM_BOL,       DM_BOL,     DM_BOL,   DM_BOL,       DM_BOL,     DM_BOL,  DM_BOL    }
};

static eEO eMathOp_EqCode[DM_ENUMS][DM_ENUMS] = {
           //   NONE     BCD            BOL           DTE           FLT            INT            IVL           IVY          TME           TSP           VCH           UNK
/*  0 NONE */ { EO_NONE, EO_NONE,       EO_NONE,      EO_NONE,      EO_NONE,       EO_NONE,       EO_NONE,      EO_NONE,     EO_NONE,      EO_NONE,      EO_NONE,      EO_EQ_XYZUNK    },
/*  1 BCD  */ { EO_NONE, EO_EQ_BCDBCD,  EO_NONE,      EO_NONE,      EO_EQ_BCDFLT,  EO_EQ_BCDINT,  EO_NONE,      EO_NONE,     EO_NONE,      EO_NONE,      EO_NONE,      EO_EQ_XYZUNK    },
/*  2 BOL  */ { EO_NONE, EO_NONE,       EO_EQ_BOLBOL, EO_NONE,      EO_NONE,       EO_NONE,       EO_NONE,      EO_NONE,     EO_NONE,      EO_NONE,      EO_NONE,      EO_EQ_XYZUNK    },
/*  3 DTE  */ { EO_NONE, EO_NONE,       EO_NONE,      EO_EQ_DTEDTE, EO_NONE,       EO_NONE,       EO_NONE,      EO_NONE,     EO_NONE,      EO_EQ_DTETSP, EO_NONE,      EO_EQ_XYZUNK    },
/*  4 FLT  */ { EO_NONE, EO_EQ_FLTBCD,  EO_NONE,      EO_NONE,      EO_EQ_FLTFLT,  EO_EQ_FLTINT,  EO_NONE,      EO_NONE,     EO_NONE,      EO_NONE,      EO_NONE,      EO_EQ_XYZUNK    },
/*  5 INT  */ { EO_NONE, EO_EQ_INTBCD,  EO_NONE,      EO_NONE,      EO_EQ_INTFLT,  EO_EQ_INTINT,  EO_NONE,      EO_NONE,     EO_NONE,      EO_NONE,      EO_NONE,      EO_EQ_XYZUNK    },
/*  6 IVL  */ { EO_NONE, EO_NONE,       EO_NONE,      EO_NONE,      EO_NONE,       EO_NONE,       EO_EQ_IVLIVL, EO_EQ_IVLIVL,EO_NONE,      EO_NONE,      EO_NONE,      EO_EQ_XYZUNK    },
/*  6 IVY  */ { EO_NONE, EO_NONE,       EO_NONE,      EO_NONE,      EO_NONE,       EO_NONE,       EO_EQ_IVLIVL, EO_EQ_IVLIVL,EO_NONE,      EO_NONE,      EO_NONE,      EO_EQ_XYZUNK    },
/*  7 TME  */ { EO_NONE, EO_NONE,       EO_NONE,      EO_NONE,      EO_NONE,       EO_NONE,       EO_NONE,      EO_NONE,     EO_EQ_TMETME, EO_NONE,      EO_NONE,      EO_EQ_XYZUNK    },
/*  8 TSP  */ { EO_NONE, EO_NONE,       EO_NONE,      EO_EQ_TSPDTE, EO_NONE,       EO_NONE,       EO_NONE,      EO_NONE,     EO_NONE,      EO_EQ_TSPTSP, EO_NONE,      EO_EQ_XYZUNK    },
/*  9 VCH  */ { EO_NONE, EO_NONE,       EO_NONE,      EO_NONE,      EO_NONE,       EO_NONE,       EO_NONE,      EO_NONE,     EO_NONE,      EO_NONE,      EO_EQ_VCHVCH, EO_EQ_XYZUNK    },
/* 10 UNK  */ { EO_NONE, EO_EQ_UNKXYZ,  EO_EQ_UNKXYZ, EO_EQ_UNKXYZ, EO_EQ_UNKXYZ,  EO_EQ_UNKXYZ,  EO_EQ_UNKXYZ, EO_EQ_UNKXYZ,EO_EQ_UNKXYZ, EO_EQ_UNKXYZ, EO_EQ_UNKXYZ, EO_EQ_UNKXYZ    }
};

static eEO eMathOp_GeCode[DM_ENUMS][DM_ENUMS] = {
           //   NONE     BCD            BOL           DTE           FLT            INT            IVL           IVY               TME           TSP           VCH           UNK
/*  0 NONE */ { EO_NONE, EO_NONE,       EO_NONE,      EO_NONE,      EO_NONE,       EO_NONE,       EO_NONE,      EO_NONE,          EO_NONE,      EO_NONE,      EO_NONE,      EO_GE_XYZUNK    },
/*  1 BCD  */ { EO_NONE, EO_GE_BCDBCD,  EO_NONE,      EO_NONE,      EO_GE_BCDFLT,  EO_GE_BCDINT,  EO_NONE,      EO_NONE,          EO_NONE,      EO_NONE,      EO_NONE,      EO_GE_XYZUNK    },
/*  2 BOL  */ { EO_NONE, EO_NONE,       EO_GE_BOLBOL, EO_NONE,      EO_NONE,       EO_NONE,       EO_NONE,      EO_NONE,          EO_NONE,      EO_NONE,      EO_NONE,      EO_GE_XYZUNK    },
/*  3 DTE  */ { EO_NONE, EO_NONE,       EO_NONE,      EO_GE_DTEDTE, EO_NONE,       EO_NONE,       EO_NONE,      EO_NONE,          EO_NONE,      EO_GE_DTETSP, EO_NONE,      EO_GE_XYZUNK    },
/*  4 FLT  */ { EO_NONE, EO_GE_FLTBCD,  EO_NONE,      EO_NONE,      EO_GE_FLTFLT,  EO_GE_FLTINT,  EO_NONE,      EO_NONE,          EO_NONE,      EO_NONE,      EO_NONE,      EO_GE_XYZUNK    },
/*  5 INT  */ { EO_NONE, EO_GE_INTBCD,  EO_NONE,      EO_NONE,      EO_GE_INTFLT,  EO_GE_INTINT,  EO_NONE,      EO_NONE,          EO_NONE,      EO_NONE,      EO_NONE,      EO_GE_XYZUNK    },
/*  6 IVL  */ { EO_NONE, EO_NONE,       EO_NONE,      EO_NONE,      EO_NONE,       EO_NONE,       EO_GE_IVLIVL, EO_GE_IVLIVL,     EO_NONE,      EO_NONE,      EO_NONE,      EO_GE_XYZUNK    },
/*  6 IVY  */ { EO_NONE, EO_NONE,       EO_NONE,      EO_NONE,      EO_NONE,       EO_NONE,       EO_GE_IVLIVL, EO_GE_IVLIVL,     EO_NONE,      EO_NONE,      EO_NONE,      EO_GE_XYZUNK    },
/*  7 TME  */ { EO_NONE, EO_NONE,       EO_NONE,      EO_NONE,      EO_NONE,       EO_NONE,       EO_NONE,      EO_NONE,          EO_GE_TMETME, EO_NONE,      EO_NONE,      EO_GE_XYZUNK    },
/*  8 TSP  */ { EO_NONE, EO_NONE,       EO_NONE,      EO_GE_TSPDTE, EO_NONE,       EO_NONE,       EO_NONE,      EO_NONE,          EO_NONE,      EO_GE_TSPTSP, EO_NONE,      EO_GE_XYZUNK    },
/*  9 VCH  */ { EO_NONE, EO_NONE,       EO_NONE,      EO_NONE,      EO_NONE,       EO_NONE,       EO_NONE,      EO_NONE,          EO_NONE,      EO_NONE,      EO_GE_VCHVCH, EO_GE_XYZUNK    },
/* 10 UNK  */ { EO_NONE, EO_GE_UNKXYZ,  EO_GE_UNKXYZ, EO_GE_UNKXYZ, EO_GE_UNKXYZ,  EO_GE_UNKXYZ,  EO_GE_UNKXYZ, EO_GE_UNKXYZ,EO_GE_UNKXYZ, EO_GE_UNKXYZ, EO_GE_UNKXYZ, EO_GE_UNKXYZ    }
};

static eEO eMathOp_GtCode[DM_ENUMS][DM_ENUMS] = {
           //   NONE     BCD            BOL           DTE           FLT            INT            IVL           IVY           TME           TSP           VCH           UNK
/*  0 NONE */ { EO_NONE, EO_NONE,       EO_NONE,      EO_NONE,      EO_NONE,       EO_NONE,       EO_NONE,      EO_NONE,      EO_NONE,      EO_NONE,      EO_NONE,      EO_GT_XYZUNK    },
/*  1 BCD  */ { EO_NONE, EO_GT_BCDBCD,  EO_NONE,      EO_NONE,      EO_GT_BCDFLT,  EO_GT_BCDINT,  EO_NONE,      EO_NONE,      EO_NONE,      EO_NONE,      EO_NONE,      EO_GT_XYZUNK    },
/*  2 BOL  */ { EO_NONE, EO_NONE,       EO_GT_BOLBOL, EO_NONE,      EO_NONE,       EO_NONE,       EO_NONE,      EO_NONE,      EO_NONE,      EO_NONE,      EO_NONE,      EO_GT_XYZUNK    },
/*  3 DTE  */ { EO_NONE, EO_NONE,       EO_NONE,      EO_GT_DTEDTE, EO_NONE,       EO_NONE,       EO_NONE,      EO_NONE,      EO_NONE,      EO_GT_DTETSP, EO_NONE,      EO_GT_XYZUNK    },
/*  4 FLT  */ { EO_NONE, EO_GT_FLTBCD,  EO_NONE,      EO_NONE,      EO_GT_FLTFLT,  EO_GT_FLTINT,  EO_NONE,      EO_NONE,      EO_NONE,      EO_NONE,      EO_NONE,      EO_GT_XYZUNK    },
/*  5 INT  */ { EO_NONE, EO_GT_INTBCD,  EO_NONE,      EO_NONE,      EO_GT_INTFLT,  EO_GT_INTINT,  EO_NONE,      EO_NONE,      EO_NONE,      EO_NONE,      EO_NONE,      EO_GT_XYZUNK    },
/*  6 IVL  */ { EO_NONE, EO_NONE,       EO_NONE,      EO_NONE,      EO_NONE,       EO_NONE,       EO_GT_IVLIVL, EO_GT_UNKXYZ, EO_NONE,      EO_NONE,      EO_NONE,      EO_GT_XYZUNK    },
/*  6 IVY  */ { EO_NONE, EO_NONE,       EO_NONE,      EO_NONE,      EO_NONE,       EO_NONE,       EO_GT_IVLIVL, EO_GT_UNKXYZ, EO_NONE,      EO_NONE,      EO_NONE,      EO_GT_XYZUNK    },
/*  7 TME  */ { EO_NONE, EO_NONE,       EO_NONE,      EO_NONE,      EO_NONE,       EO_NONE,       EO_NONE,      EO_NONE,      EO_GT_TMETME, EO_NONE,      EO_NONE,      EO_GT_XYZUNK    },
/*  8 TSP  */ { EO_NONE, EO_NONE,       EO_NONE,      EO_GT_TSPDTE, EO_NONE,       EO_NONE,       EO_NONE,      EO_NONE,      EO_NONE,      EO_GT_TSPTSP, EO_NONE,      EO_GT_XYZUNK    },
/*  9 VCH  */ { EO_NONE, EO_NONE,       EO_NONE,      EO_NONE,      EO_NONE,       EO_NONE,       EO_NONE,      EO_NONE,      EO_NONE,      EO_NONE,      EO_GT_VCHVCH, EO_GT_XYZUNK    },
/* 10 UNK  */ { EO_NONE, EO_GT_UNKXYZ,  EO_GT_UNKXYZ, EO_GT_UNKXYZ, EO_GT_UNKXYZ,  EO_GT_UNKXYZ,  EO_GT_UNKXYZ, EO_GT_UNKXYZ, EO_GT_UNKXYZ, EO_GT_UNKXYZ, EO_GT_UNKXYZ, EO_GT_UNKXYZ    }
};

static eEO eMathOp_LeCode[DM_ENUMS][DM_ENUMS] = {
           //   NONE     BCD            BOL           DTE           FLT            INT            IVL           IVY           TME           TSP           VCH           UNK
/*  0 NONE */ { EO_NONE, EO_NONE,       EO_NONE,      EO_NONE,      EO_NONE,       EO_NONE,       EO_NONE,      EO_NONE,      EO_NONE,      EO_NONE,      EO_NONE,      EO_LE_XYZUNK    },
/*  1 BCD  */ { EO_NONE, EO_LE_BCDBCD,  EO_NONE,      EO_NONE,      EO_LE_BCDFLT,  EO_LE_BCDINT,  EO_NONE,      EO_NONE,      EO_NONE,      EO_NONE,      EO_NONE,      EO_LE_XYZUNK    },
/*  2 BOL  */ { EO_NONE, EO_NONE,       EO_LE_BOLBOL, EO_NONE,      EO_NONE,       EO_NONE,       EO_NONE,      EO_NONE,      EO_NONE,      EO_NONE,      EO_NONE,      EO_LE_XYZUNK    },
/*  3 DTE  */ { EO_NONE, EO_NONE,       EO_NONE,      EO_LE_DTEDTE, EO_NONE,       EO_NONE,       EO_NONE,      EO_NONE,      EO_NONE,      EO_LE_DTETSP, EO_NONE,      EO_LE_XYZUNK    },
/*  4 FLT  */ { EO_NONE, EO_LE_FLTBCD,  EO_NONE,      EO_NONE,      EO_LE_FLTFLT,  EO_LE_FLTINT,  EO_NONE,      EO_NONE,      EO_NONE,      EO_NONE,      EO_NONE,      EO_LE_XYZUNK    },
/*  5 INT  */ { EO_NONE, EO_LE_INTBCD,  EO_NONE,      EO_NONE,      EO_LE_INTFLT,  EO_LE_INTINT,  EO_NONE,      EO_NONE,      EO_NONE,      EO_NONE,      EO_NONE,      EO_LE_XYZUNK    },
/*  6 IVL  */ { EO_NONE, EO_NONE,       EO_NONE,      EO_NONE,      EO_NONE,       EO_NONE,       EO_LE_IVLIVL, EO_NONE,      EO_NONE,      EO_NONE,      EO_NONE,      EO_LE_XYZUNK    },
/*  6 IVY  */ { EO_NONE, EO_NONE,       EO_NONE,      EO_NONE,      EO_NONE,       EO_NONE,       EO_LE_IVLIVL, EO_NONE,      EO_NONE,      EO_NONE,      EO_NONE,      EO_LE_XYZUNK    },
/*  7 TME  */ { EO_NONE, EO_NONE,       EO_NONE,      EO_NONE,      EO_NONE,       EO_NONE,       EO_NONE,      EO_NONE,      EO_LE_TMETME, EO_NONE,      EO_NONE,      EO_LE_XYZUNK    },
/*  8 TSP  */ { EO_NONE, EO_NONE,       EO_NONE,      EO_LE_TSPDTE, EO_NONE,       EO_NONE,       EO_NONE,      EO_NONE,      EO_NONE,      EO_LE_TSPTSP, EO_NONE,      EO_LE_XYZUNK    },
/*  9 VCH  */ { EO_NONE, EO_NONE,       EO_NONE,      EO_NONE,      EO_NONE,       EO_NONE,       EO_NONE,      EO_NONE,      EO_NONE,      EO_NONE,      EO_LE_VCHVCH, EO_LE_XYZUNK    },
/* 10 UNK  */ { EO_NONE, EO_LE_UNKXYZ,  EO_LE_UNKXYZ, EO_LE_UNKXYZ, EO_LE_UNKXYZ,  EO_LE_UNKXYZ,  EO_LE_UNKXYZ, EO_NONE,      EO_LE_UNKXYZ, EO_LE_UNKXYZ, EO_LE_UNKXYZ, EO_LE_UNKXYZ    }
};

static eEO eMathOp_LtCode[DM_ENUMS][DM_ENUMS] = {
           //   NONE     BCD            BOL           DTE           FLT            INT            IVL           IVY           TME           TSP           VCH           UNK
/*  0 NONE */ { EO_NONE, EO_NONE,       EO_NONE,      EO_NONE,      EO_NONE,       EO_NONE,       EO_NONE,      EO_NONE,      EO_NONE,      EO_NONE,      EO_NONE,      EO_LT_XYZUNK    },
/*  1 BCD  */ { EO_NONE, EO_LT_BCDBCD,  EO_NONE,      EO_NONE,      EO_LT_BCDFLT,  EO_LT_BCDINT,  EO_NONE,      EO_NONE,      EO_NONE,      EO_NONE,      EO_NONE,      EO_LT_XYZUNK    },
/*  2 BOL  */ { EO_NONE, EO_NONE,       EO_LT_BOLBOL, EO_NONE,      EO_NONE,       EO_NONE,       EO_NONE,      EO_NONE,      EO_NONE,      EO_NONE,      EO_NONE,      EO_LT_XYZUNK    },
/*  3 DTE  */ { EO_NONE, EO_NONE,       EO_NONE,      EO_LT_DTEDTE, EO_NONE,       EO_NONE,       EO_NONE,      EO_NONE,      EO_NONE,      EO_LT_DTETSP, EO_NONE,      EO_LT_XYZUNK    },
/*  4 FLT  */ { EO_NONE, EO_LT_FLTBCD,  EO_NONE,      EO_NONE,      EO_LT_FLTFLT,  EO_LT_FLTINT,  EO_NONE,      EO_NONE,      EO_NONE,      EO_NONE,      EO_NONE,      EO_LT_XYZUNK    },
/*  5 INT  */ { EO_NONE, EO_LT_INTBCD,  EO_NONE,      EO_NONE,      EO_LT_INTFLT,  EO_LT_INTINT,  EO_NONE,      EO_NONE,      EO_NONE,      EO_NONE,      EO_NONE,      EO_LT_XYZUNK    },
/*  6 IVL  */ { EO_NONE, EO_NONE,       EO_NONE,      EO_NONE,      EO_NONE,       EO_NONE,       EO_LT_IVLIVL, EO_NONE,      EO_NONE,      EO_NONE,      EO_NONE,      EO_LT_XYZUNK    },
/*  6 IVY  */ { EO_NONE, EO_NONE,       EO_NONE,      EO_NONE,      EO_NONE,       EO_NONE,       EO_LT_IVLIVL, EO_NONE,      EO_NONE,      EO_NONE,      EO_NONE,      EO_LT_XYZUNK    },
/*  7 TME  */ { EO_NONE, EO_NONE,       EO_NONE,      EO_NONE,      EO_NONE,       EO_NONE,       EO_NONE,      EO_NONE,      EO_LT_TMETME, EO_NONE,      EO_NONE,      EO_LT_XYZUNK    },
/*  8 TSP  */ { EO_NONE, EO_NONE,       EO_NONE,      EO_LT_TSPDTE, EO_NONE,       EO_NONE,       EO_NONE,      EO_NONE,      EO_NONE,      EO_LT_TSPTSP, EO_NONE,      EO_LT_XYZUNK    },
/*  9 VCH  */ { EO_NONE, EO_NONE,       EO_NONE,      EO_NONE,      EO_NONE,       EO_NONE,       EO_NONE,      EO_NONE,      EO_NONE,      EO_NONE,      EO_LT_VCHVCH, EO_LT_XYZUNK    },
/* 10 UNK  */ { EO_NONE, EO_LT_UNKXYZ,  EO_LT_UNKXYZ, EO_LT_UNKXYZ, EO_LT_UNKXYZ,  EO_LT_UNKXYZ,  EO_LT_UNKXYZ, EO_NONE,      EO_LT_UNKXYZ, EO_LT_UNKXYZ, EO_LT_UNKXYZ, EO_LT_UNKXYZ    }
};

static eEO eMathOp_NeCode[DM_ENUMS][DM_ENUMS] = {
           //   NONE     BCD            BOL           DTE           FLT            INT            IVL           IVY           TME           TSP           VCH           UNK
/*  0 NONE */ { EO_NONE, EO_NONE,       EO_NONE,      EO_NONE,      EO_NONE,       EO_NONE,       EO_NONE,      EO_NONE,      EO_NONE,      EO_NONE,      EO_NONE,      EO_NE_XYZUNK    },
/*  1 BCD  */ { EO_NONE, EO_NE_BCDBCD,  EO_NONE,      EO_NONE,      EO_NE_BCDFLT,  EO_NE_BCDINT,  EO_NONE,      EO_NONE,      EO_NONE,      EO_NONE,      EO_NONE,      EO_NE_XYZUNK    },
/*  2 BOL  */ { EO_NONE, EO_NONE,       EO_NE_BOLBOL, EO_NONE,      EO_NONE,       EO_NONE,       EO_NONE,      EO_NONE,      EO_NONE,      EO_NONE,      EO_NONE,      EO_NE_XYZUNK    },
/*  3 DTE  */ { EO_NONE, EO_NONE,       EO_NONE,      EO_NE_DTEDTE, EO_NONE,       EO_NONE,       EO_NONE,      EO_NONE,      EO_NONE,      EO_NE_DTETSP, EO_NONE,      EO_NE_XYZUNK    },
/*  4 FLT  */ { EO_NONE, EO_NE_FLTBCD,  EO_NONE,      EO_NONE,      EO_NE_FLTFLT,  EO_NE_FLTINT,  EO_NONE,      EO_NONE,      EO_NONE,      EO_NONE,      EO_NONE,      EO_NE_XYZUNK    },
/*  5 INT  */ { EO_NONE, EO_NE_INTBCD,  EO_NONE,      EO_NONE,      EO_NE_INTFLT,  EO_NE_INTINT,  EO_NONE,      EO_NONE,      EO_NONE,      EO_NONE,      EO_NONE,      EO_NE_XYZUNK    },
/*  6 IVL  */ { EO_NONE, EO_NONE,       EO_NONE,      EO_NONE,      EO_NONE,       EO_NONE,       EO_NE_IVLIVL, EO_NONE,      EO_NONE,      EO_NONE,      EO_NONE,      EO_NE_XYZUNK    },
/*  6 IVY  */ { EO_NONE, EO_NONE,       EO_NONE,      EO_NONE,      EO_NONE,       EO_NONE,       EO_NE_IVLIVL, EO_NONE,      EO_NONE,      EO_NONE,      EO_NONE,      EO_NE_XYZUNK    },
/*  7 TME  */ { EO_NONE, EO_NONE,       EO_NONE,      EO_NONE,      EO_NONE,       EO_NONE,       EO_NONE,      EO_NONE,      EO_NE_TMETME, EO_NONE,      EO_NONE,      EO_NE_XYZUNK    },
/*  8 TSP  */ { EO_NONE, EO_NONE,       EO_NONE,      EO_NE_TSPDTE, EO_NONE,       EO_NONE,       EO_NONE,      EO_NONE,      EO_NONE,      EO_NE_TSPTSP, EO_NONE,      EO_NE_XYZUNK    },
/*  9 VCH  */ { EO_NONE, EO_NONE,       EO_NONE,      EO_NONE,      EO_NONE,       EO_NONE,       EO_NONE,      EO_NONE,      EO_NONE,      EO_NONE,      EO_NE_VCHVCH, EO_NE_XYZUNK    },
/* 10 UNK  */ { EO_NONE, EO_NE_UNKXYZ,  EO_NE_UNKXYZ, EO_NE_UNKXYZ, EO_NE_UNKXYZ,  EO_NE_UNKXYZ,  EO_NE_UNKXYZ, EO_NONE,      EO_NE_UNKXYZ, EO_NE_UNKXYZ, EO_NE_UNKXYZ, EO_NE_UNKXYZ    }
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// MATHEMATICAL OPERATORS
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// ADD

static eDM eMathOp_AddResult[DM_ENUMS][DM_ENUMS] = {
           //   NONE     BCD       BOL       DTE           FLT         INT           IVL         IVY           TME          TSP            VCH      UNK
/*  0 NONE */ { DM_NONE, DM_NONE,  DM_NONE,  DM_NONE,      DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE, DM_NONE    },
/*  1 BCD  */ { DM_NONE, DM_BCD,   DM_NONE,  DM_NONE,      DM_FLT,     DM_BCD,       DM_NONE,    DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE, DM_UNK     },
/*  2 BOL  */ { DM_NONE, DM_NONE,  DM_NONE,  DM_NONE,      DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE, DM_NONE    },
/*  3 DTE  */ { DM_NONE, DM_NONE,  DM_NONE,  DM_NONE,      DM_NONE,    DM_NONE/*?*/, DM_DTE,     DM_NONE,    DM_NONE/*?*/, DM_NONE,    DM_NONE, DM_NONE    },
/*  4 FLT  */ { DM_NONE, DM_FLT,   DM_NONE,  DM_NONE,      DM_FLT,     DM_FLT,       DM_NONE,    DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE, DM_UNK     },
/*  5 INT  */ { DM_NONE, DM_BCD,   DM_NONE,  DM_NONE/*?*/, DM_FLT,     DM_INT,       DM_NONE,    DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE, DM_UNK     },
/*  6 IVL  */ { DM_NONE, DM_NONE,  DM_NONE,  DM_DTE,       DM_NONE,    DM_NONE,      DM_IVL,     DM_NONE,    DM_TME,       DM_TSP,     DM_NONE, DM_UNK     },
/*  6 IVY  */ { DM_NONE, DM_NONE,  DM_NONE,  DM_DTE,       DM_NONE,    DM_NONE,      DM_IVL,     DM_NONE,    DM_TME,       DM_TSP,     DM_NONE, DM_UNK     },
/*  7 TME  */ { DM_NONE, DM_NONE,  DM_NONE,  DM_NONE/*?*/, DM_NONE,    DM_NONE/*?*/, DM_TME,     DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE, DM_NONE    },
/*  8 TSP  */ { DM_NONE, DM_NONE,  DM_NONE,  DM_NONE,      DM_NONE,    DM_NONE,      DM_TSP,     DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE, DM_NONE    },
/*  9 VCH  */ { DM_NONE, DM_NONE,  DM_NONE,  DM_NONE,      DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE, DM_NONE    },
/* 10 UNK  */ { DM_NONE, DM_UNK,   DM_UNK,   DM_NONE,      DM_UNK,     DM_UNK,       DM_UNK,     DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE, DM_NONE    }
};

static eEO eMathOp_AddCode[DM_ENUMS][DM_ENUMS] = {
           //   NONE     BCD            BOL       DTE           FLT            INT            IVL           IVY           TME           TSP           VCH      UNK
/*  0 NONE */ { EO_NONE, EO_NONE,       EO_NONE,  EO_NONE,      EO_NONE,       EO_NONE,       EO_NONE,      EO_NONE,      EO_NONE,      EO_NONE,      EO_NONE, EO_NONE    },
/*  1 BCD  */ { EO_NONE, EO_ADD_BCDBCD, EO_NONE,  EO_NONE,      EO_ADD_BCDFLT, EO_ADD_BCDINT, EO_NONE,      EO_NONE,      EO_NONE,      EO_NONE,      EO_NONE, EO_ADD_XYZUNK  },
/*  2 BOL  */ { EO_NONE, EO_NONE,       EO_NONE,  EO_NONE,      EO_NONE,       EO_NONE,       EO_NONE,      EO_NONE,      EO_NONE,      EO_NONE,      EO_NONE, EO_NONE    },
/*  3 DTE  */ { EO_NONE, EO_NONE,       EO_NONE,  EO_NONE,      EO_NONE,       EO_NONE/*?*/,  EO_ADD_DTEIVL,EO_NONE,      EO_NONE/*?*/, EO_NONE,      EO_NONE, EO_NONE    },
/*  4 FLT  */ { EO_NONE, EO_ADD_FLTBCD, EO_NONE,  EO_NONE,      EO_ADD_FLTFLT, EO_ADD_FLTINT, EO_NONE,      EO_NONE,      EO_NONE,      EO_NONE,      EO_NONE, EO_ADD_XYZUNK  },
/*  5 INT  */ { EO_NONE, EO_ADD_INTBCD, EO_NONE,  EO_NONE/*?*/, EO_ADD_INTFLT, EO_ADD_INTINT, EO_NONE,      EO_NONE,      EO_NONE,      EO_NONE,      EO_NONE, EO_ADD_XYZUNK  },
/*  6 IVL  */ { EO_NONE, EO_NONE,       EO_NONE,  EO_ADD_IVLDTE,EO_NONE,       EO_NONE,       EO_ADD_IVLIVL,EO_NONE,      EO_ADD_IVLTME,EO_ADD_IVLTSP,EO_NONE, EO_ADD_XYZUNK  },
/*  6 IVY  */ { EO_NONE, EO_NONE,       EO_NONE,  EO_ADD_IVLDTE,EO_NONE,       EO_NONE,       EO_ADD_IVLIVL,EO_NONE,      EO_ADD_IVLTME,EO_ADD_IVLTSP,EO_NONE, EO_ADD_XYZUNK  },
/*  7 TME  */ { EO_NONE, EO_NONE,       EO_NONE,  EO_NONE/*?*/, EO_NONE,       EO_NONE/*?*/,  EO_ADD_TMEIVL,EO_NONE,      EO_NONE,      EO_NONE,      EO_NONE, EO_NONE    },
/*  8 TSP  */ { EO_NONE, EO_NONE,       EO_NONE,  EO_NONE,      EO_NONE,       EO_NONE,       EO_ADD_TSPIVL,EO_NONE,      EO_NONE,      EO_NONE,      EO_NONE, EO_NONE    },
/*  9 VCH  */ { EO_NONE, EO_NONE,       EO_NONE,  EO_NONE,      EO_NONE,       EO_NONE,       EO_NONE,      EO_NONE,      EO_NONE,      EO_NONE,      EO_NONE, EO_NONE    },
/* 10 UNK  */ { EO_NONE, EO_ADD_UNKXYZ, EO_NONE,  EO_NONE,      EO_ADD_UNKXYZ, EO_ADD_UNKXYZ, EO_ADD_UNKXYZ,EO_NONE,      EO_NONE,      EO_NONE,      EO_NONE, EO_NONE    }
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// DIVIDE

static eDM eMathOp_DivResult[DM_ENUMS][DM_ENUMS] = {
           //   NONE     BCD       BOL      DTE           FLT         INT           IVL         IVY           TMT          TSP            VCH      UNK
/*  0 NONE */ { DM_NONE, DM_NONE,  DM_NONE, DM_NONE,      DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE, DM_NONE    },
/*  1 BCD  */ { DM_NONE, DM_BCD,   DM_NONE, DM_NONE,      DM_FLT,     DM_BCD,       DM_NONE,    DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE, DM_UNK     },
/*  2 BOL  */ { DM_NONE, DM_NONE,  DM_NONE, DM_NONE,      DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE, DM_NONE    },
/*  3 DTE  */ { DM_NONE, DM_NONE,  DM_NONE, DM_NONE,      DM_NONE,    DM_NONE/*?*/, DM_DTE,     DM_NONE,    DM_NONE/*?*/, DM_NONE,    DM_NONE, DM_NONE    },
/*  5 FLT  */ { DM_NONE, DM_FLT,   DM_NONE, DM_NONE,      DM_FLT,     DM_FLT,       DM_NONE,    DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE, DM_UNK     },
/*  6 INT  */ { DM_NONE, DM_BCD,   DM_NONE, DM_NONE/*?*/, DM_FLT,     DM_INT,       DM_NONE,    DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE, DM_UNK     },
/*  7 IVL  */ { DM_NONE, DM_NONE,  DM_NONE, DM_DTE,       DM_NONE,    DM_NONE,      DM_IVL,     DM_NONE,    DM_TME,       DM_TSP,     DM_NONE, DM_UNK     },
/*  7 IVY  */ { DM_NONE, DM_NONE,  DM_NONE, DM_DTE,       DM_NONE,    DM_NONE,      DM_IVL,     DM_NONE,    DM_TME,       DM_TSP,     DM_NONE, DM_UNK     },
/*  9 TME  */ { DM_NONE, DM_NONE,  DM_NONE, DM_NONE/*?*/, DM_NONE,    DM_NONE/*?*/, DM_TME,     DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE, DM_NONE    },
/* 10 TSP  */ { DM_NONE, DM_NONE,  DM_NONE, DM_NONE,      DM_NONE,    DM_NONE,      DM_TSP,     DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE, DM_NONE    },
/* 11 VCH  */ { DM_NONE, DM_NONE,  DM_NONE, DM_NONE,      DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE, DM_NONE    },
/* 12 UNK  */ { DM_NONE, DM_UNK,   DM_NONE, DM_NONE,      DM_UNK,     DM_UNK,       DM_UNK,     DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE, DM_NONE    }
};

static eEO eMathOp_DivCode[DM_ENUMS][DM_ENUMS] = {
           //   NONE     BCD            BOL      DTE           FLT            INT            IVL          IVY           TME           TSP           VCH      UNK
/*  0 NONE */ { EO_NONE, EO_NONE,       EO_NONE, EO_NONE,      EO_NONE,       EO_NONE,      EO_NONE,      EO_NONE,      EO_NONE,      EO_NONE,      EO_NONE, EO_NONE    },
/*  1 BCD  */ { EO_NONE, EO_DIV_BCDBCD, EO_NONE, EO_NONE,      EO_DIV_BCDFLT, EO_DIV_BCDINT,EO_NONE,      EO_NONE,      EO_NONE,      EO_NONE,      EO_NONE, EO_DIV_UNKXYZ  },
/*  2 BOL  */ { EO_NONE, EO_NONE,       EO_NONE, EO_NONE,      EO_NONE,       EO_NONE,      EO_NONE,      EO_NONE,      EO_NONE,      EO_NONE,      EO_NONE, EO_NONE    },
/*  3 DTE  */ { EO_NONE, EO_NONE,       EO_NONE, EO_NONE,      EO_NONE,       EO_NONE/*?*/, EO_DIV_DTEIVL,EO_NONE,      EO_NONE/*?*/, EO_NONE,      EO_NONE, EO_NONE    },
/*  4 FLT  */ { EO_NONE, EO_DIV_FLTBCD, EO_NONE, EO_NONE,      EO_DIV_FLTFLT, EO_DIV_FLTINT,EO_NONE,      EO_NONE,      EO_NONE,      EO_NONE,      EO_NONE, EO_DIV_UNKXYZ  },
/*  5 INT  */ { EO_NONE, EO_DIV_INTBCD, EO_NONE, EO_NONE/*?*/, EO_DIV_INTFLT, EO_DIV_INTINT,EO_NONE,      EO_NONE,      EO_NONE,      EO_NONE,      EO_NONE, EO_DIV_UNKXYZ  },
/*  6 IVL  */ { EO_NONE, EO_NONE,       EO_NONE, EO_DIV_IVLDTE,EO_NONE,       EO_NONE,      EO_DIV_IVLIVL,EO_NONE,      EO_DIV_TMEIVL,EO_DIV_TSPIVL,EO_NONE, EO_DIV_UNKXYZ  },
/*  6 IVY  */ { EO_NONE, EO_NONE,       EO_NONE, EO_DIV_IVLDTE,EO_NONE,       EO_NONE,      EO_DIV_IVLIVL,EO_NONE,      EO_DIV_TMEIVL,EO_DIV_TSPIVL,EO_NONE, EO_DIV_UNKXYZ  },
/*  7 TME  */ { EO_NONE, EO_NONE,       EO_NONE, EO_NONE/*?*/, EO_NONE,       EO_NONE/*?*/, EO_DIV_TMEIVL,EO_NONE,      EO_NONE,      EO_NONE,      EO_NONE, EO_NONE    },
/*  8 TSP  */ { EO_NONE, EO_NONE,       EO_NONE, EO_NONE,      EO_NONE,       EO_NONE,      EO_DIV_TSPIVL,EO_NONE,      EO_NONE,      EO_NONE,      EO_NONE, EO_NONE    },
/*  9 VCH  */ { EO_NONE, EO_NONE,       EO_NONE, EO_NONE,      EO_NONE,       EO_NONE,      EO_NONE,      EO_NONE,      EO_NONE,      EO_NONE,      EO_NONE, EO_NONE    },
/* 10 UNK  */ { EO_NONE, EO_DIV_UNKXYZ, EO_NONE, EO_NONE,      EO_DIV_UNKXYZ, EO_DIV_UNKXYZ,EO_DIV_UNKXYZ,EO_NONE,      EO_NONE,      EO_NONE,      EO_NONE, EO_NONE    }
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// MODULUS

static eDM eMathOp_ModResult[DM_ENUMS][DM_ENUMS] = {

           //   NONE     BCD       BOL      DTE           FLT         INT           IVL         IVY           TMT          TSP            VCH      UNK
/*  0 NONE */ { DM_NONE, DM_NONE,  DM_NONE, DM_NONE,      DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE, DM_NONE    },
/*  1 BCD  */ { DM_NONE, DM_NONE,  DM_NONE, DM_NONE,      DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE, DM_NONE    },
/*  2 BOL  */ { DM_NONE, DM_NONE,  DM_NONE, DM_NONE,      DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE, DM_NONE    },
/*  3 DTE  */ { DM_NONE, DM_NONE,  DM_NONE, DM_NONE,      DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE, DM_NONE    },
/*  5 FLT  */ { DM_NONE, DM_NONE,  DM_NONE, DM_NONE,      DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE, DM_NONE    },
/*  6 INT  */ { DM_NONE, DM_NONE,  DM_NONE, DM_NONE,      DM_NONE,    DM_INT,       DM_NONE,    DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE, DM_UNK     },
/*  7 IVL  */ { DM_NONE, DM_NONE,  DM_NONE, DM_NONE,      DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE,    DM_TME,       DM_NONE,    DM_NONE, DM_NONE    },
/*  7 IVY  */ { DM_NONE, DM_NONE,  DM_NONE, DM_NONE,      DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE,    DM_TME,       DM_NONE,    DM_NONE, DM_NONE    },
/*  9 TME  */ { DM_NONE, DM_NONE,  DM_NONE, DM_NONE,      DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE, DM_NONE    },
/* 10 TSP  */ { DM_NONE, DM_NONE,  DM_NONE, DM_NONE,      DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE, DM_NONE    },
/* 11 VCH  */ { DM_NONE, DM_NONE,  DM_NONE, DM_NONE,      DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE, DM_NONE    },
/* 12 UNK  */ { DM_NONE, DM_NONE,  DM_NONE, DM_NONE,      DM_NONE,    DM_UNK,       DM_NONE,    DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE, DM_NONE    }
    };

static eEO eMathOp_ModCode[DM_ENUMS][DM_ENUMS] = {
           //   NONE     BCD      BOL      DTE       FLT      INT           IVL           IVY           TME           TSP           VCH      UNK
/*  0 NONE */ { EO_NONE, EO_NONE, EO_NONE, EO_NONE,  EO_NONE, EO_NONE,      EO_NONE,      EO_NONE,      EO_NONE,      EO_NONE,      EO_NONE, EO_NONE  },
/*  1 BCD  */ { EO_NONE, EO_NONE, EO_NONE, EO_NONE,  EO_NONE, EO_NONE,      EO_NONE,      EO_NONE,      EO_NONE,      EO_NONE,      EO_NONE, EO_NONE  },
/*  2 BOL  */ { EO_NONE, EO_NONE, EO_NONE, EO_NONE,  EO_NONE, EO_NONE,      EO_NONE,      EO_NONE,      EO_NONE,      EO_NONE,      EO_NONE, EO_NONE  },
/*  3 DTE  */ { EO_NONE, EO_NONE, EO_NONE, EO_NONE,  EO_NONE, EO_NONE,      EO_NONE,      EO_NONE,      EO_NONE,      EO_NONE,      EO_NONE, EO_NONE  },
/*  4 FLT  */ { EO_NONE, EO_NONE, EO_NONE, EO_NONE,  EO_NONE, EO_NONE,      EO_NONE,      EO_NONE,      EO_NONE,      EO_NONE,      EO_NONE, EO_NONE  },
/*  5 INT  */ { EO_NONE, EO_NONE, EO_NONE, EO_NONE,  EO_NONE, EO_MOD_INTINT,EO_NONE,      EO_NONE,      EO_NONE,      EO_NONE,      EO_NONE, EO_MOD_XYZUNK  },
/*  6 IVL  */ { EO_NONE, EO_NONE, EO_NONE, EO_NONE,  EO_NONE, EO_NONE,      EO_NONE,      EO_NONE,      EO_NONE,      EO_NONE,      EO_NONE, EO_NONE  },
/*  6 IVY  */ { EO_NONE, EO_NONE, EO_NONE, EO_NONE,  EO_NONE, EO_NONE,      EO_NONE,      EO_NONE,      EO_NONE,      EO_NONE,      EO_NONE, EO_NONE  },
/*  7 TME  */ { EO_NONE, EO_NONE, EO_NONE, EO_NONE,  EO_NONE, EO_NONE/*?*/, EO_NONE,      EO_NONE,      EO_NONE,      EO_NONE,      EO_NONE, EO_NONE  },
/*  8 TSP  */ { EO_NONE, EO_NONE, EO_NONE, EO_NONE,  EO_NONE, EO_NONE,      EO_NONE,      EO_NONE,      EO_NONE,      EO_NONE,      EO_NONE, EO_NONE  },
/*  9 VCH  */ { EO_NONE, EO_NONE, EO_NONE, EO_NONE,  EO_NONE, EO_NONE,      EO_NONE,      EO_NONE,      EO_NONE,      EO_NONE,      EO_NONE, EO_NONE  },
/* 10 UNK  */ { EO_NONE, EO_NONE, EO_NONE, EO_NONE,  EO_NONE, EO_MOD_UNKXYZ,EO_NONE,      EO_NONE,      EO_NONE,      EO_NONE,      EO_NONE, EO_NONE  }
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// MULTIPLY

static eDM eMathOp_MulResult[DM_ENUMS][DM_ENUMS] = {
           //   NONE     BCD      BOL       DTE           FLT         INT           IVL         IVY           TME         TSP            VCH      UNK
/*  0 NONE */ { DM_NONE, DM_NONE, DM_NONE,  DM_NONE,      DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE, DM_NONE    },
/*  1 BCD  */ { DM_NONE, DM_BCD,  DM_NONE,  DM_NONE,      DM_FLT,     DM_BCD,       DM_NONE,    DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE, DM_UNK     },
/*  2 BOL  */ { DM_NONE, DM_NONE, DM_NONE,  DM_NONE,      DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE, DM_NONE    },
/*  3 DTE  */ { DM_NONE, DM_NONE, DM_NONE,  DM_NONE,      DM_NONE,    DM_NONE/*?*/, DM_DTE,     DM_NONE,    DM_NONE/*?*/, DM_NONE,    DM_NONE, DM_NONE    },
/*  5 FLT  */ { DM_NONE, DM_FLT,  DM_NONE,  DM_NONE,      DM_FLT,     DM_FLT,       DM_NONE,    DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE, DM_UNK     },
/*  6 INT  */ { DM_NONE, DM_BCD,  DM_NONE,  DM_NONE/*?*/, DM_FLT,     DM_INT,       DM_NONE,    DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE, DM_UNK     },
/*  7 IVL  */ { DM_NONE, DM_NONE, DM_NONE,  DM_NONE,      DM_NONE,    DM_NONE,      DM_IVL,     DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE, DM_UNK     },
/*  7 IVY  */ { DM_NONE, DM_NONE, DM_NONE,  DM_NONE,      DM_NONE,    DM_NONE,      DM_IVL,     DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE, DM_UNK     },
/*  9 TME  */ { DM_NONE, DM_NONE, DM_NONE,  DM_NONE/*?*/, DM_NONE,    DM_NONE/*?*/, DM_TME,     DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE, DM_NONE    },
/* 10 TSP  */ { DM_NONE, DM_NONE, DM_NONE,  DM_NONE,      DM_NONE,    DM_NONE,      DM_TSP,     DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE, DM_NONE    },
/* 11 VCH  */ { DM_NONE, DM_NONE, DM_NONE,  DM_NONE,      DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE, DM_NONE    },
/* 12 UNK  */ { DM_NONE, DM_UNK,  DM_NONE,  DM_NONE,      DM_UNK,     DM_UNK,       DM_UNK,     DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE, DM_NONE    }
};

static eEO eMathOp_MulCode[DM_ENUMS][DM_ENUMS] = {
           //   NONE     BCD            BOL            DTE           FLT            INT           IVL           IVY           TME           TSP           VCH      UNK
/*  0 NONE */ { EO_NONE, EO_NONE,       EO_NONE,       EO_NONE,      EO_NONE,       EO_NONE,      EO_NONE,      EO_NONE,      EO_NONE,      EO_NONE,      EO_NONE, EO_NONE    },
/*  1 BCD  */ { EO_NONE, EO_MUL_BCDBCD, EO_NONE,       EO_NONE,      EO_MUL_BCDFLT, EO_MUL_BCDINT,EO_NONE,      EO_NONE,      EO_NONE,      EO_NONE,      EO_NONE, EO_MUL_XYZUNK  },
/*  2 BOL  */ { EO_NONE, EO_NONE,       EO_NONE,       EO_NONE,      EO_NONE,       EO_NONE,      EO_NONE,      EO_NONE,      EO_NONE,      EO_NONE,      EO_NONE, EO_NONE    },
/*  3 DTE  */ { EO_NONE, EO_NONE,       EO_NONE,       EO_NONE,      EO_NONE,       EO_NONE/*?*/, EO_MUL_DTEIVL,EO_NONE,      EO_NONE/*?*/, EO_NONE,      EO_NONE, EO_NONE    },
/*  4 FLT  */ { EO_NONE, EO_MUL_FLTBCD, EO_NONE,       EO_NONE,      EO_MUL_FLTFLT, EO_MUL_FLTINT,EO_NONE,      EO_NONE,      EO_NONE,      EO_NONE,      EO_NONE, EO_MUL_XYZUNK  },
/*  5 INT  */ { EO_NONE, EO_MUL_INTBCD, EO_NONE,       EO_NONE/*?*/, EO_MUL_INTFLT, EO_MUL_INTINT,EO_NONE,      EO_NONE,      EO_NONE,      EO_NONE,      EO_NONE, EO_MUL_XYZUNK  },
/*  6 IVL  */ { EO_NONE, EO_NONE,       EO_NONE,       EO_MUL_IVLDTE,EO_NONE,       EO_NONE,      EO_MUL_IVLIVL,EO_NONE,      EO_MUL_IVLTME,EO_MUL_IVLTSP,EO_NONE, EO_MUL_XYZUNK  },
/*  6 IVY  */ { EO_NONE, EO_NONE,       EO_NONE,       EO_MUL_IVLDTE,EO_NONE,       EO_NONE,      EO_MUL_IVLIVL,EO_NONE,      EO_MUL_IVLTME,EO_MUL_IVLTSP,EO_NONE, EO_MUL_XYZUNK  },
/*  7 TME  */ { EO_NONE, EO_NONE,       EO_NONE,       EO_NONE/*?*/, EO_NONE,       EO_NONE/*?*/, EO_MUL_TMEIVL,EO_NONE,      EO_NONE,      EO_NONE,      EO_NONE, EO_NONE    },
/*  8 TSP  */ { EO_NONE, EO_NONE,       EO_NONE,       EO_NONE,      EO_NONE,       EO_NONE,      EO_MUL_TSPIVL,EO_NONE,      EO_NONE,      EO_NONE,      EO_NONE, EO_NONE    },
/*  9 VCH  */ { EO_NONE, EO_NONE,       EO_NONE,       EO_NONE,      EO_NONE,       EO_NONE,      EO_NONE,      EO_NONE,      EO_NONE,      EO_NONE,      EO_NONE, EO_NONE    },
/* 10 UNK  */ { EO_NONE, EO_MUL_UNKXYZ, EO_NONE,       EO_NONE,      EO_MUL_UNKXYZ, EO_MUL_UNKXYZ,EO_MUL_UNKXYZ,EO_NONE,      EO_NONE,      EO_NONE,      EO_NONE, EO_NONE    }
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// NEGATE

static eDM eMathOp_NegResult[DM_ENUMS] =
   // NONE     BCD      BOL      DTE       FLT     INT     IVL     IVL     TME           TSP       VCH      UNK
    { DM_NONE, DM_BCD,  DM_NONE, DM_NONE,  DM_FLT, DM_INT, DM_IVL, DM_IVL, DM_NONE/*?*/, DM_NONE,  DM_NONE, DM_NONE };

static eEO eMathOp_NegCode[DM_ENUMS] =
   // NONE     BCD         BOL       DTE       FLT          INT          IVL         IVY,        TME      TSP       VCH      UNK
    { EO_NONE, EO_NEG_BCD, EO_NONE,  EO_NONE,  EO_NEG_FLT,  EO_NEG_INT,  EO_NEG_IVL, EO_NEG_IVL, EO_NONE, EO_NONE,  EO_NONE, EO_NONE    };

static eDM eMathOp_SubResult[DM_ENUMS][DM_ENUMS] = {
           //   NONE     BCD      BOL       DTE           FLT         INT           IVL         IVY           TME          TSP            VCH      UNK
/*  0 NONE */ { DM_NONE, DM_NONE, DM_NONE,  DM_NONE,      DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE, DM_NONE    },
/*  1 BCD  */ { DM_NONE, DM_BCD,  DM_NONE,  DM_NONE,      DM_FLT,     DM_BCD,       DM_NONE,    DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE, DM_UNK     },
/*  2 BOL  */ { DM_NONE, DM_NONE, DM_NONE,  DM_NONE,      DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE, DM_NONE    },
/*  3 DTE  */ { DM_NONE, DM_NONE, DM_NONE,  DM_NONE,      DM_NONE,    DM_NONE/*?*/, DM_DTE,     DM_NONE,    DM_NONE/*?*/, DM_NONE,    DM_NONE, DM_NONE    },
/*  4 FLT  */ { DM_NONE, DM_FLT,  DM_NONE,  DM_NONE,      DM_FLT,     DM_FLT,       DM_NONE,    DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE, DM_UNK     },
/*  5 INT  */ { DM_NONE, DM_BCD,  DM_NONE,  DM_NONE/*?*/, DM_FLT,     DM_INT,       DM_NONE,    DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE, DM_UNK     },
/*  6 IVL  */ { DM_NONE, DM_NONE, DM_NONE,  DM_DTE,       DM_NONE,    DM_NONE,      DM_IVL,     DM_NONE,    DM_TME,       DM_TSP,     DM_NONE, DM_UNK     },
/*  6 IVY  */ { DM_NONE, DM_NONE, DM_NONE,  DM_DTE,       DM_NONE,    DM_NONE,      DM_IVL,     DM_NONE,    DM_TME,       DM_TSP,     DM_NONE, DM_UNK     },
/*  7 TME  */ { DM_NONE, DM_NONE, DM_NONE,  DM_NONE/*?*/, DM_NONE,    DM_NONE/*?*/, DM_TME,     DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE, DM_NONE    },
/*  8 TSP  */ { DM_NONE, DM_NONE, DM_NONE,  DM_NONE,      DM_NONE,    DM_NONE,      DM_TSP,     DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE, DM_NONE    },
/*  9 VCH  */ { DM_NONE, DM_NONE, DM_NONE,  DM_NONE,      DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE, DM_NONE    },

/* 10 UNK  */ { DM_NONE, DM_NONE, DM_UNK,   DM_NONE,      DM_UNK,     DM_UNK,       DM_UNK,     DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE, DM_NONE    }
};

static eEO eMathOp_SubCode[DM_ENUMS][DM_ENUMS] = {
           //   NONE     BCD            BOL            DTE           FLT            INT           IVL           IVY           TME           TSP           VCH      UNK
/*  0 NONE */ { EO_NONE, EO_NONE,       EO_NONE,       EO_NONE,      EO_NONE,       EO_NONE,      EO_NONE,      EO_NONE,      EO_NONE,      EO_NONE,      EO_NONE, EO_NONE    },
/*  1 BCD  */ { EO_NONE, EO_SUB_BCDBCD, EO_NONE,       EO_NONE,      EO_SUB_BCDFLT, EO_SUB_BCDINT,EO_NONE,      EO_NONE,      EO_NONE,      EO_NONE,      EO_NONE, EO_SUB_XYZUNK  },
/*  2 BOL  */ { EO_NONE, EO_NONE,       EO_NONE,       EO_NONE,      EO_NONE,       EO_NONE,      EO_NONE,      EO_NONE,      EO_NONE,      EO_NONE,      EO_NONE, EO_NONE    },
/*  3 DTE  */ { EO_NONE, EO_NONE,       EO_NONE,       EO_NONE,      EO_NONE,       EO_NONE/*?*/, EO_SUB_DTEIVL,EO_NONE,      EO_NONE/*?*/, EO_NONE,      EO_NONE, EO_NONE    },
/*  4 FLT  */ { EO_NONE, EO_SUB_FLTBCD, EO_NONE,       EO_NONE,      EO_SUB_FLTFLT, EO_SUB_FLTINT,EO_NONE,      EO_NONE,      EO_NONE,      EO_NONE,      EO_NONE, EO_SUB_XYZUNK  },
/*  5 INT  */ { EO_NONE, EO_SUB_INTBCD, EO_NONE,       EO_NONE/*?*/, EO_SUB_INTFLT, EO_SUB_INTINT,EO_NONE,      EO_NONE,      EO_NONE,      EO_NONE,      EO_NONE, EO_SUB_XYZUNK  },
/*  6 IVL  */ { EO_NONE, EO_NONE,       EO_NONE,       EO_SUB_IVLDTE,EO_NONE,       EO_NONE,      EO_SUB_IVLIVL,EO_NONE,      EO_SUB_IVLTME,EO_SUB_IVLTSP,EO_NONE, EO_SUB_XYZUNK  },
/*  6 IVY  */ { EO_NONE, EO_NONE,       EO_NONE,       EO_SUB_IVLDTE,EO_NONE,       EO_NONE,      EO_SUB_IVLIVL,EO_NONE,      EO_SUB_IVLTME,EO_SUB_IVLTSP,EO_NONE, EO_SUB_XYZUNK  },
/*  7 TME  */ { EO_NONE, EO_NONE,       EO_NONE,       EO_NONE/*?*/, EO_NONE,       EO_NONE/*?*/, EO_SUB_TMEIVL,EO_NONE,      EO_NONE,      EO_NONE,      EO_NONE, EO_NONE    },
/*  8 TSP  */ { EO_NONE, EO_NONE,       EO_NONE,       EO_NONE,      EO_NONE,       EO_NONE,      EO_SUB_TSPIVL,EO_NONE,      EO_NONE,      EO_NONE,      EO_NONE, EO_NONE    },
/*  9 VCH  */ { EO_NONE, EO_NONE,       EO_NONE,       EO_NONE,      EO_NONE,       EO_NONE,      EO_NONE,      EO_NONE,      EO_NONE,      EO_NONE,      EO_NONE, EO_NONE    },
/* 10 UNK  */ { EO_NONE, EO_SUB_UNKXYZ, EO_NONE,       EO_NONE,      EO_SUB_UNKXYZ, EO_SUB_UNKXYZ,EO_SUB_UNKXYZ,EO_NONE,      EO_NONE,      EO_NONE,      EO_NONE, EO_NONE    }
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// LOGICAL OPERATORS
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// ISNULL

static eDM eMathOp_IsNullResult[DM_ENUMS] =
    // NONE    BOL     BCD      DTE       FLT      INT      IVL      IVY,     TME      TSP       VCH      UNK
    { DM_NONE, DM_BOL, DM_BOL,  DM_BOL,   DM_BOL,  DM_BOL,  DM_BOL,  DM_BOL,  DM_BOL,  DM_BOL,   DM_BOL,  DM_BOL };

static eEO eMathOp_IsNullCode[DM_ENUMS] =
    // NONE     BOL        BCD         DTE         FLT         INT         IVL        IVY        TME        TSP         VCH            UNK
    { EO_NONE,  EO_ISNULL, EO_ISNULL,  EO_ISNULL,  EO_ISNULL,  EO_ISNULL,  EO_ISNULL, EO_ISNULL, EO_ISNULL, EO_ISNULL,  EO_ISNULL_STR, EO_ISNULL    };

static eEO eMathOp_PopNullCode[DM_ENUMS] =
    // NONE     BOL        BCD         DTE         FLT         INT         IVL        IVY        TME        TSP         VCH            UNK
    { EO_NONE,  EO_POP,    EO_POP,     EO_POP,     EO_POP,     EO_POP,     EO_POP,    EO_POP,    EO_POP,    EO_POP,     EO_POP_STR,    EO_POP    };

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// MIN VALUE
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
i8 iMathOp_MinValue(eDM eDatum) {

  i8 iMinValue;

  switch(eDatum) {
    case DM_BCD: iMinValue = Bcd::iMinValue(); break;
    case DM_BOL: iMinValue = Bol::iMinValue(); break;
    case DM_DTE: iMinValue = Dte::iMinValue(); break;
    case DM_FLT: iMinValue = Flt::iMinValue(); break;
    case DM_INT: iMinValue = Int::iMinValue(); break;
    case DM_IVL: iMinValue = Ivl::iMinValue(); break;
    case DM_IVY: iMinValue = Ivl::iMinValue(); break;
    case DM_TME: iMinValue = Tme::iMinValue(); break;
    case DM_TSP: iMinValue = Tsp::iMinValue(); break;
    case DM_VCH: iMinValue = 0; break;
    case DM_UNK: iMinValue = Unk::iMinValue(); break;
      break;
    default:
      NEVER_REACH;

  }

  return iMinValue;
}

void vMathOp_MinValue(Stz* sMinValue) {
  sMinValue = ""_Stz;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// MAX VALUE
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
i8 iMathOp_MaxValue(eDM eDatum) {

  i8 iMaxValue;

  switch(eDatum) {
    case DM_BCD: iMaxValue = Bcd::iMaxValue(); break;
    case DM_BOL: iMaxValue = Bol::iMaxValue(); break;
    case DM_DTE: iMaxValue = Dte::iMaxValue(); break;
    case DM_FLT: iMaxValue = Flt::iMaxValue(); break;
    case DM_INT: iMaxValue = Int::iMaxValue(); break;
    case DM_IVL: iMaxValue = Ivl::iMaxValue(); break;
    case DM_IVY: iMaxValue = Ivl::iMaxValue(); break;
    case DM_TME: iMaxValue = Tme::iMaxValue(); break;
    case DM_TSP: iMaxValue = Tsp::iMaxValue(); break;
    case DM_VCH: RASSERT(0); break;
    case DM_UNK: iMaxValue = Unk::iMaxValue(); break;
      break;
    default:
      NEVER_REACH;

  }

  return iMaxValue;
}

void vMathOp_MaxValue(Stz* sMaxValue) {
  strcpy((char*)sMaxValue,"\255");
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
