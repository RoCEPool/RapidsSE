/*============================================================================*/
/* MathOp.h -- math operators                                                 */
/*============================================================================*/
#pragma once

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// MATHEMATICAL OPERATORS
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// ADD

static eDM eMathOp_AddResult[DM_ENUMS][DM_ENUMS] = {
           //   NONE     BCD       BOL       DTE           FLT         INT           IVL         TME          TSP            VCH      UNK
/*  0 NONE */ { DM_NONE, DM_NONE,  DM_NONE,  DM_NONE,      DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE, DM_NONE    },
/*  1 BCD  */ { DM_NONE, DM_BCD,   DM_NONE,  DM_NONE,      DM_FLT,     DM_BCD,       DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE, DM_UNK     },
/*  2 BOL  */ { DM_NONE, DM_NONE,  DM_NONE,  DM_NONE,      DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE, DM_NONE    },
/*  3 DTE  */ { DM_NONE, DM_NONE,  DM_NONE,  DM_NONE,      DM_NONE,    DM_NONE/*?*/, DM_DTE,     DM_NONE/*?*/, DM_NONE,    DM_NONE, DM_NONE    },
/*  4 FLT  */ { DM_NONE, DM_FLT,   DM_NONE,  DM_NONE,      DM_FLT,     DM_FLT,       DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE, DM_UNK     },
/*  5 INT  */ { DM_NONE, DM_BCD,   DM_NONE,  DM_NONE/*?*/, DM_FLT,     DM_INT,       DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE, DM_UNK     },
/*  6 IVL  */ { DM_NONE, DM_NONE,  DM_NONE,  DM_DTE,       DM_NONE,    DM_NONE,      DM_IVL,     DM_TME,       DM_TSP,     DM_NONE, DM_UNK     },
/*  7 TME  */ { DM_NONE, DM_NONE,  DM_NONE,  DM_NONE/*?*/, DM_NONE,    DM_NONE/*?*/, DM_TME,     DM_NONE,      DM_NONE,    DM_NONE, DM_NONE    },
/*  8 TSP  */ { DM_NONE, DM_NONE,  DM_NONE,  DM_NONE,      DM_NONE,    DM_NONE,      DM_TSP,     DM_NONE,      DM_NONE,    DM_NONE, DM_NONE    },
/*  9 VCH  */ { DM_NONE, DM_NONE,  DM_NONE,  DM_NONE,      DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE, DM_NONE    },

/* 10 UNK  */ { DM_NONE, DM_NONE, DM_UNK,   DM_NONE,      DM_UNK,     DM_UNK,       DM_UNK,     DM_NONE,      DM_NONE,    DM_NONE, DM_NONE    }
};

static eEO eMathOp_AddCode[DM_ENUMS][DM_ENUMS] = {
           //   NONE     BCD            BOL       DTE           FLT            INT            IVL           TME           TSP           VCH      UNK
/*  0 NONE */ { EO_NONE, EO_NONE,       EO_NONE,  EO_NONE,      EO_NONE,       EO_NONE,       EO_NONE,      EO_NONE,      EO_NONE,      EO_NONE, EO_NONE    },
/*  1 BCD  */ { EO_NONE, EO_ADD_BCDBCD, EO_NONE,  EO_NONE,      EO_ADD_BCDFLT, EO_ADD_BCDINT, EO_NONE,      EO_NONE,      EO_NONE,      EO_NONE, EO_ADD_XYZUNK  },
/*  2 BOL  */ { EO_NONE, EO_NONE,       EO_NONE,  EO_NONE,      EO_NONE,       EO_NONE,       EO_NONE,      EO_NONE,      EO_NONE,      EO_NONE, EO_NONE    },
/*  3 DTE  */ { EO_NONE, EO_NONE,       EO_NONE,  EO_NONE,      EO_NONE,       EO_NONE/*?*/,  EO_ADD_DTEIVL,EO_NONE/*?*/, EO_NONE,      EO_NONE, EO_NONE    },
/*  4 FLT  */ { EO_NONE, EO_ADD_FLTBCD, EO_NONE,  EO_NONE,      EO_ADD_FLTFLT, EO_ADD_FLTINT, EO_NONE,      EO_NONE,      EO_NONE,      EO_NONE, EO_ADD_XYZUNK  },
/*  5 INT  */ { EO_NONE, EO_ADD_INTBCD, EO_NONE,  EO_NONE/*?*/, EO_ADD_INTFLT, EO_ADD_INTINT, EO_NONE,      EO_NONE,      EO_NONE,      EO_NONE, EO_ADD_XYZUNK  },
/*  6 IVL  */ { EO_NONE, EO_NONE,       EO_NONE,  EO_ADD_IVLDTE,EO_NONE,       EO_NONE,       EO_ADD_IVLIVL,EO_ADD_IVLTME,EO_ADD_IVLTSP,EO_NONE, EO_ADD_XYZUNK  },
/*  7 TME  */ { EO_NONE, EO_NONE,       EO_NONE,  EO_NONE/*?*/, EO_NONE,       EO_NONE/*?*/,  EO_ADD_TMEIVL,EO_NONE,      EO_NONE,      EO_NONE, EO_NONE    },
/*  8 TSP  */ { EO_NONE, EO_NONE,       EO_NONE,  EO_NONE,      EO_NONE,       EO_NONE,       EO_ADD_TSPIVL,EO_NONE,      EO_NONE,      EO_NONE, EO_NONE    },
/*  9 VCH  */ { EO_NONE, EO_NONE,       EO_NONE,  EO_NONE,      EO_NONE,       EO_NONE,       EO_NONE,      EO_NONE,      EO_NONE,      EO_NONE, EO_NONE    },
/* 10 UNK  */ { EO_NONE, EO_ADD_UNKXYZ, EO_NONE,  EO_NONE,      EO_ADD_UNKXYZ, EO_ADD_UNKXYZ, EO_ADD_UNKXYZ,EO_NONE,      EO_NONE,      EO_NONE, EO_NONE    }
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// DIVIDE

static eDM eMathOp_DivResult[DM_ENUMS][DM_ENUMS] = {
           //   NONE     BCD       BOL      DTE           FLT         INT           IVL         TMT          TSP            VCH      UNK
/*  0 NONE */ { DM_NONE, DM_NONE,  DM_NONE, DM_NONE,      DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE, DM_NONE    },
/*  1 BCD  */ { DM_NONE, DM_BCD,   DM_NONE, DM_NONE,      DM_FLT,     DM_BCD,       DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE, DM_UNK     },
/*  2 BOL  */ { DM_NONE, DM_NONE,  DM_NONE, DM_NONE,      DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE, DM_NONE    },
/*  3 DTE  */ { DM_NONE, DM_NONE,  DM_NONE, DM_NONE,      DM_NONE,    DM_NONE/*?*/, DM_DTE,     DM_NONE/*?*/, DM_NONE,    DM_NONE, DM_NONE    },
/*  5 FLT  */ { DM_NONE, DM_FLT,   DM_NONE, DM_NONE,      DM_FLT,     DM_FLT,       DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE, DM_UNK     },
/*  6 INT  */ { DM_NONE, DM_BCD,   DM_NONE, DM_NONE/*?*/, DM_FLT,     DM_INT,       DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE, DM_UNK     },
/*  7 IVL  */ { DM_NONE, DM_NONE,  DM_NONE, DM_DTE,       DM_NONE,    DM_NONE,      DM_IVL,     DM_TME,       DM_TSP,     DM_NONE, DM_UNK     },
/*  9 TME  */ { DM_NONE, DM_NONE,  DM_NONE, DM_NONE/*?*/, DM_NONE,    DM_NONE/*?*/, DM_TME,     DM_NONE,      DM_NONE,    DM_NONE, DM_NONE    },
/* 10 TSP  */ { DM_NONE, DM_NONE,  DM_NONE, DM_NONE,      DM_NONE,    DM_NONE,      DM_TSP,     DM_NONE,      DM_NONE,    DM_NONE, DM_NONE    },
/* 11 VCH  */ { DM_NONE, DM_NONE,  DM_NONE, DM_NONE,      DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE, DM_NONE    },
/* 12 UNK  */ { DM_NONE, DM_UNK,   DM_NONE, DM_NONE,      DM_UNK,     DM_UNK,       DM_UNK,     DM_NONE,      DM_NONE,    DM_NONE, DM_NONE    }
};

static eEO eMathOp_DivCode[DM_ENUMS][DM_ENUMS] = {
           //   NONE     BCD            BOL      DTE           FLT            INT            IVL           TME           TSP           VCH      UNK
/*  0 NONE */ { EO_NONE, EO_NONE,       EO_NONE, EO_NONE,      EO_NONE,       EO_NONE,      EO_NONE,      EO_NONE,      EO_NONE,      EO_NONE, EO_NONE    },
/*  1 BCD  */ { EO_NONE, EO_DIV_BCDBCD, EO_NONE, EO_NONE,      EO_DIV_BCDFLT, EO_DIV_BCDINT,EO_NONE,      EO_NONE,      EO_NONE,      EO_NONE, EO_DIV_XYZUNK  },
/*  2 BOL  */ { EO_NONE, EO_NONE,       EO_NONE, EO_NONE,      EO_NONE,       EO_NONE,      EO_NONE,      EO_NONE,      EO_NONE,      EO_NONE, EO_NONE    },
/*  3 DTE  */ { EO_NONE, EO_NONE,       EO_NONE, EO_NONE,      EO_NONE,       EO_NONE/*?*/, EO_DIV_DTEIVL,EO_NONE/*?*/, EO_NONE,      EO_NONE, EO_NONE    },
/*  4 FLT  */ { EO_NONE, EO_DIV_FLTBCD, EO_NONE, EO_NONE,      EO_DIV_FLTFLT, EO_DIV_FLTINT,EO_NONE,      EO_NONE,      EO_NONE,      EO_NONE, EO_DIV_XYZUNK  },
/*  5 INT  */ { EO_NONE, EO_DIV_INTBCD, EO_NONE, EO_NONE/*?*/, EO_DIV_INTFLT, EO_DIV_INTINT,EO_NONE,      EO_NONE,      EO_NONE,      EO_NONE, EO_DIV_XYZUNK  },
/*  6 IVL  */ { EO_NONE, EO_NONE,       EO_NONE, EO_DIV_IVLDTE,EO_NONE,       EO_NONE,      EO_DIV_IVLIVL,EO_DIV_IVLTME,EO_DIV_IVLTSP,EO_NONE, EO_DIV_XYZUNK  },
/*  7 TME  */ { EO_NONE, EO_NONE,       EO_NONE, EO_NONE/*?*/, EO_NONE,       EO_NONE/*?*/, EO_DIV_TMEIVL,EO_NONE,      EO_NONE,      EO_NONE, EO_NONE    },
/*  8 TSP  */ { EO_NONE, EO_NONE,       EO_NONE, EO_NONE,      EO_NONE,       EO_NONE,      EO_DIV_TSPIVL,EO_NONE,      EO_NONE,      EO_NONE, EO_NONE    },
/*  9 VCH  */ { EO_NONE, EO_NONE,       EO_NONE, EO_NONE,      EO_NONE,       EO_NONE,      EO_NONE,      EO_NONE,      EO_NONE,      EO_NONE, EO_NONE    },
/* 10 UNK  */ { EO_NONE, EO_DIV_UNKXYZ, EO_NONE, EO_NONE,      EO_DIV_UNKXYZ, EO_DIV_UNKXYZ,EO_DIV_XYZUNK,EO_NONE,      EO_NONE,      EO_NONE, EO_NONE    }
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// MODULUS

static eDM eMathOp_ModResult[DM_ENUMS][DM_ENUMS] = {

           //   NONE     BCD       BOL      DTE           FLT         INT           IVL         TMT          TSP            VCH      UNK
/*  0 NONE */ { DM_NONE, DM_NONE,  DM_NONE, DM_NONE,      DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE, DM_NONE    },
/*  1 BCD  */ { DM_NONE, DM_NONE,  DM_NONE, DM_NONE,      DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE, DM_NONE    },
/*  2 BOL  */ { DM_NONE, DM_NONE,  DM_NONE, DM_NONE,      DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE, DM_NONE    },
/*  3 DTE  */ { DM_NONE, DM_NONE,  DM_NONE, DM_NONE,      DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE, DM_NONE    },
/*  5 FLT  */ { DM_NONE, DM_NONE,  DM_NONE, DM_NONE,      DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE, DM_NONE    },
/*  6 INT  */ { DM_NONE, DM_NONE,  DM_NONE, DM_NONE,      DM_NONE,    DM_INT,       DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE, DM_UNK     },
/*  7 IVL  */ { DM_NONE, DM_NONE,  DM_NONE, DM_NONE,      DM_NONE,    DM_NONE,      DM_NONE,    DM_TME,       DM_NONE,    DM_NONE, DM_NONE    },
/*  9 TME  */ { DM_NONE, DM_NONE,  DM_NONE, DM_NONE,      DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE, DM_NONE    },
/* 10 TSP  */ { DM_NONE, DM_NONE,  DM_NONE, DM_NONE,      DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE, DM_NONE    },
/* 11 VCH  */ { DM_NONE, DM_NONE,  DM_NONE, DM_NONE,      DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE, DM_NONE    },
/* 12 UNK  */ { DM_NONE, DM_NONE,  DM_NONE, DM_NONE,      DM_NONE,    DM_UNK,       DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE, DM_NONE    }
    };

static eEO eMathOp_ModCode[DM_ENUMS][DM_ENUMS] = {
           //   NONE     BCD      BOL      DTE       FLT      INT           IVL           TME           TSP           VCH      UNK
/*  0 NONE */ { EO_NONE, EO_NONE, EO_NONE, EO_NONE,  EO_NONE, EO_NONE,      EO_NONE,      EO_NONE,      EO_NONE,      EO_NONE, EO_NONE    },
/*  1 BCD  */ { EO_NONE, EO_NONE, EO_NONE, EO_NONE,  EO_NONE, EO_NONE,      EO_NONE,      EO_NONE,      EO_NONE,      EO_NONE, EO_NONE  },
/*  2 BOL  */ { EO_NONE, EO_NONE, EO_NONE, EO_NONE,  EO_NONE, EO_NONE,      EO_NONE,      EO_NONE,      EO_NONE,      EO_NONE, EO_NONE    },
/*  3 DTE  */ { EO_NONE, EO_NONE, EO_NONE, EO_NONE,  EO_NONE, EO_NONE,      EO_NONE,      EO_NONE,      EO_NONE,      EO_NONE, EO_NONE    },
/*  4 FLT  */ { EO_NONE, EO_NONE, EO_NONE, EO_NONE,  EO_NONE, EO_NONE,      EO_NONE,      EO_NONE,      EO_NONE,      EO_NONE, EO_NONE  },
/*  5 INT  */ { EO_NONE, EO_NONE, EO_NONE, EO_NONE,  EO_NONE, EO_MOD_INTINT,EO_NONE,      EO_NONE,      EO_NONE,      EO_NONE, EO_MOD_XYZUNK  },
/*  6 IVL  */ { EO_NONE, EO_NONE, EO_NONE, EO_NONE,  EO_NONE, EO_NONE,      EO_NONE,      EO_NONE,      EO_NONE,      EO_NONE, EO_NONE  },
/*  7 TME  */ { EO_NONE, EO_NONE, EO_NONE, EO_NONE,  EO_NONE, EO_NONE/*?*/, EO_NONE,      EO_NONE,      EO_NONE,      EO_NONE, EO_NONE    },
/*  8 TSP  */ { EO_NONE, EO_NONE, EO_NONE, EO_NONE,  EO_NONE, EO_NONE,      EO_NONE,      EO_NONE,      EO_NONE,      EO_NONE, EO_NONE    },
/*  9 VCH  */ { EO_NONE, EO_NONE, EO_NONE, EO_NONE,  EO_NONE, EO_NONE,      EO_NONE,      EO_NONE,      EO_NONE,      EO_NONE, EO_NONE    },
/* 10 UNK  */ { EO_NONE, EO_NONE, EO_NONE, EO_NONE,  EO_NONE, EO_MOD_UNKXYZ,EO_NONE,      EO_NONE,      EO_NONE,      EO_NONE, EO_NONE    }
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// MULTIPLY

static eDM eMathOp_MulResult[DM_ENUMS][DM_ENUMS] = {
           //   NONE     BOL      BCD       DTE           FLT         INT           IVL         TMT          TSP            VCH      UNK
/*  0 NONE */ { DM_NONE, DM_NONE, DM_NONE,  DM_NONE,      DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE, DM_NONE    },
/*  1 BCD  */ { DM_NONE, DM_NONE, DM_BCD,   DM_NONE,      DM_FLT,     DM_BCD,       DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE, DM_UNK     },
/*  2 BOL  */ { DM_NONE, DM_NONE, DM_NONE,  DM_NONE,      DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE, DM_NONE    },
/*  3 DTE  */ { DM_NONE, DM_NONE, DM_NONE,  DM_NONE,      DM_NONE,    DM_NONE/*?*/, DM_DTE,     DM_NONE/*?*/, DM_NONE,    DM_NONE, DM_NONE    },
/*  5 FLT  */ { DM_NONE, DM_NONE, DM_FLT,   DM_NONE,      DM_FLT,     DM_FLT,       DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE, DM_UNK     },
/*  6 INT  */ { DM_NONE, DM_NONE, DM_BCD,   DM_NONE/*?*/, DM_FLT,     DM_INT,       DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE, DM_UNK     },
/*  7 IVL  */ { DM_NONE, DM_NONE, DM_NONE,  DM_NONE,      DM_NONE,    DM_NONE,      DM_IVL,     DM_NONE,      DM_NONE,    DM_NONE, DM_UNK     },
/*  9 TME  */ { DM_NONE, DM_NONE, DM_NONE,  DM_NONE/*?*/, DM_NONE,    DM_NONE/*?*/, DM_TME,     DM_NONE,      DM_NONE,    DM_NONE, DM_NONE    },
/* 10 TSP  */ { DM_NONE, DM_NONE, DM_NONE,  DM_NONE,      DM_NONE,    DM_NONE,      DM_TSP,     DM_NONE,      DM_NONE,    DM_NONE, DM_NONE    },
/* 11 VCH  */ { DM_NONE, DM_NONE, DM_NONE,  DM_NONE,      DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE, DM_NONE    },
/* 12 UNK  */ { DM_NONE, DM_NONE, DM_UNK,   DM_NONE,      DM_UNK,     DM_UNK,       DM_UNK,     DM_NONE,      DM_NONE,    DM_NONE, DM_NONE    }
};

static eEO eMathOp_MulCode[DM_ENUMS][DM_ENUMS] = {
           //   NONE     BOL      BCD            DTE           FLT            INT           IVL           TME           TSP           VCH      UNK
/*  0 NONE */ { EO_NONE, EO_NONE, EO_NONE,       EO_NONE,      EO_NONE,       EO_NONE,      EO_NONE,      EO_NONE,      EO_NONE,      EO_NONE, EO_NONE    },
/*  1 BCD  */ { EO_NONE, EO_NONE, EO_MUL_BCDBCD, EO_NONE,      EO_MUL_BCDFLT, EO_MUL_BCDINT,EO_NONE,      EO_NONE,      EO_NONE,      EO_NONE, EO_MUL_XYZUNK  },
/*  2 BOL  */ { EO_NONE, EO_NONE, EO_NONE,       EO_NONE,      EO_NONE,       EO_NONE,      EO_NONE,      EO_NONE,      EO_NONE,      EO_NONE, EO_NONE    },
/*  3 DTE  */ { EO_NONE, EO_NONE, EO_NONE,       EO_NONE,      EO_NONE,       EO_NONE/*?*/, EO_MUL_DTEIVL,EO_NONE/*?*/, EO_NONE,      EO_NONE, EO_NONE    },
/*  4 FLT  */ { EO_NONE, EO_NONE, EO_MUL_FLTBCD, EO_NONE,      EO_MUL_FLTFLT, EO_MUL_FLTINT,EO_NONE,      EO_NONE,      EO_NONE,      EO_NONE, EO_MUL_XYZUNK  },
/*  5 INT  */ { EO_NONE, EO_NONE, EO_MUL_INTBCD, EO_NONE/*?*/, EO_MUL_INTFLT, EO_MUL_INTINT,EO_NONE,      EO_NONE,      EO_NONE,      EO_NONE, EO_MUL_XYZUNK  },
/*  6 IVL  */ { EO_NONE, EO_NONE, EO_NONE,       EO_MUL_IVLDTE,EO_NONE,       EO_NONE,      EO_MUL_IVLIVL,EO_MUL_IVLTME,EO_MUL_IVLTSP,EO_NONE, EO_MUL_XYZUNK  },
/*  7 TME  */ { EO_NONE, EO_NONE, EO_NONE,       EO_NONE/*?*/, EO_NONE,       EO_NONE/*?*/, EO_MUL_TMEIVL,EO_NONE,      EO_NONE,      EO_NONE, EO_NONE    },
/*  8 TSP  */ { EO_NONE, EO_NONE, EO_NONE,       EO_NONE,      EO_NONE,       EO_NONE,      EO_MUL_TSPIVL,EO_NONE,      EO_NONE,      EO_NONE, EO_NONE    },
/*  9 VCH  */ { EO_NONE, EO_NONE, EO_NONE,       EO_NONE,      EO_NONE,       EO_NONE,      EO_NONE,      EO_NONE,      EO_NONE,      EO_NONE, EO_NONE    },
/* 10 UNK  */ { EO_NONE, EO_NONE, EO_MUL_UNKXYZ, EO_NONE,      EO_MUL_UNKXYZ, EO_MUL_UNKXYZ,EO_MUL_UNKXYZ,EO_NONE,      EO_NONE,      EO_NONE, EO_NONE    }
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// NEGATE

static eDM eMathOp_NegResult[DM_ENUMS] =
   // NONE     BOL      BCD     DTE       FLT     INT     IVL     TME           TSP       VCH      UNK
    { DM_NONE, DM_NONE, DM_BCD, DM_NONE,  DM_FLT, DM_INT, DM_IVL, DM_NONE/*?*/, DM_NONE,  DM_NONE, DM_NONE };

static eEO eMathOp_NegCode[DM_ENUMS][DM_ENUMS] =
   // NONE     BOL      BCD          DTE       FLT          INT          IVL         TME      TSP       VCH      UNK
    { EO_NONE, EO_NONE, EO_NEG_BCD,  EO_NONE,  EO_NEG_FLT,  EO_NEG_INT,  EO_NEG_IVL, EO_NONE, EO_NONE,  EO_NONE, EO_NONE    };

static eDM eMathOp_SubResult[DM_ENUMS][DM_ENUMS] = {
           //   NONE     BOL      BCD       DTE           FLT         INT           IVL         TME          TSP            VCH      UNK
/*  0 NONE */ { DM_NONE, DM_NONE, DM_NONE,  DM_NONE,      DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE, DM_NONE    },
/*  1 BCD  */ { DM_NONE, DM_NONE, DM_BCD,   DM_NONE,      DM_FLT,     DM_BCD,       DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE, DM_UNK     },
/*  2 BOL  */ { DM_NONE, DM_NONE, DM_NONE,  DM_NONE,      DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE, DM_NONE    },
/*  3 DTE  */ { DM_NONE, DM_NONE, DM_NONE,  DM_NONE,      DM_NONE,    DM_NONE/*?*/, DM_DTE,     DM_NONE/*?*/, DM_NONE,    DM_NONE, DM_NONE    },
/*  4 FLT  */ { DM_NONE, DM_NONE, DM_FLT,   DM_NONE,      DM_FLT,     DM_FLT,       DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE, DM_UNK     },
/*  5 INT  */ { DM_NONE, DM_NONE, DM_BCD,   DM_NONE/*?*/, DM_FLT,     DM_INT,       DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE, DM_UNK     },
/*  6 IVL  */ { DM_NONE, DM_NONE, DM_NONE,  DM_DTE,       DM_NONE,    DM_NONE,      DM_IVL,     DM_TME,       DM_TSP,     DM_NONE, DM_UNK     },
/*  7 TME  */ { DM_NONE, DM_NONE, DM_NONE,  DM_NONE/*?*/, DM_NONE,    DM_NONE/*?*/, DM_TME,     DM_NONE,      DM_NONE,    DM_NONE, DM_NONE    },
/*  8 TSP  */ { DM_NONE, DM_NONE, DM_NONE,  DM_NONE,      DM_NONE,    DM_NONE,      DM_TSP,     DM_NONE,      DM_NONE,    DM_NONE, DM_NONE    },
/*  9 VCH  */ { DM_NONE, DM_NONE, DM_NONE,  DM_NONE,      DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE, DM_NONE    },

/* 10 UNK  */ { DM_NONE, DM_NONE, DM_UNK,   DM_NONE,      DM_UNK,     DM_UNK,       DM_UNK,     DM_NONE,      DM_NONE,    DM_NONE, DM_NONE    }
};

static eEO eMathOp_SubCode[DM_ENUMS][DM_ENUMS] = {
    //   NONE     BOL      BCD            DTE           FLT            INT            IVL           TME           TSP           VCH      UNK
/*  0 NONE */ { EO_NONE, EO_NONE, EO_NONE,       EO_NONE,      EO_NONE,       EO_NONE,      EO_NONE,      EO_NONE,      EO_NONE,      EO_NONE, EO_NONE    },
/*  1 BCD  */ { EO_NONE, EO_NONE, EO_SUB_BCDBCD, EO_NONE,      EO_SUB_BCDFLT, EO_SUB_BCDINT,EO_NONE,      EO_NONE,      EO_NONE,      EO_NONE, EO_SUB_XYZUNK  },
/*  2 BOL  */ { EO_NONE, EO_NONE, EO_NONE,       EO_NONE,      EO_NONE,       EO_NONE,      EO_NONE,      EO_NONE,      EO_NONE,      EO_NONE, EO_NONE    },
/*  3 DTE  */ { EO_NONE, EO_NONE, EO_NONE,       EO_NONE,      EO_NONE,       EO_NONE/*?*/, EO_SUB_DTEIVL,EO_NONE/*?*/, EO_NONE,      EO_NONE, EO_NONE    },
/*  4 FLT  */ { EO_NONE, EO_NONE, EO_SUB_FLTBCD, EO_NONE,      EO_SUB_FLTFLT, EO_SUB_FLTINT,EO_NONE,      EO_NONE,      EO_NONE,      EO_NONE, EO_SUB_XYZUNK  },
/*  5 INT  */ { EO_NONE, EO_NONE, EO_SUB_INTBCD, EO_NONE/*?*/, EO_SUB_INTFLT, EO_SUB_INTINT,EO_NONE,      EO_NONE,      EO_NONE,      EO_NONE, EO_SUB_XYZUNK  },
/*  6 IVL  */ { EO_NONE, EO_NONE, EO_NONE,       EO_SUB_IVLDTE,EO_NONE,       EO_NONE,      EO_SUB_IVLIVL,EO_SUB_IVLTME,EO_SUB_IVLTSP,EO_NONE, EO_SUB_XYZUNK  },
/*  7 TME  */ { EO_NONE, EO_NONE, EO_NONE,       EO_NONE/*?*/, EO_NONE,       EO_NONE/*?*/, EO_SUB_TMEIVL,EO_NONE,      EO_NONE,      EO_NONE, EO_NONE    },
/*  8 TSP  */ { EO_NONE, EO_NONE, EO_NONE,       EO_NONE,      EO_NONE,       EO_NONE,      EO_SUB_TSPIVL,EO_NONE,      EO_NONE,      EO_NONE, EO_NONE    },
/*  9 VCH  */ { EO_NONE, EO_NONE, EO_NONE,       EO_NONE,      EO_NONE,       EO_NONE,      EO_NONE,      EO_NONE,      EO_NONE,      EO_NONE, EO_NONE    },
/* 10 UNK  */ { EO_NONE, EO_NONE, EO_SUB_UNKXYZ, EO_NONE,      EO_SUB_UNKXYZ, EO_SUB_UNKXYZ,EO_SUB_UNKXYZ,EO_NONE,      EO_NONE,      EO_NONE, EO_NONE    }
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// LOGICAL OPERATORS
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// AND

static eDM eMathOp_AndResult[DM_ENUMS][DM_ENUMS] = {
           //   NONE     BCD       BOL       DTE           FLT         INT           IVL         TME          TSP            VCH      UNK
/*  0 NONE */ { DM_NONE, DM_NONE,  DM_NONE,  DM_NONE,      DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE, DM_NONE    },
/*  1 BCD  */ { DM_NONE, DM_NONE,  DM_NONE,  DM_NONE,      DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE, DM_NONE    },
/*  2 BOL  */ { DM_NONE, DM_BOL,   DM_NONE,  DM_NONE,      DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE, DM_BOL     },
/*  3 DTE  */ { DM_NONE, DM_NONE,  DM_NONE,  DM_NONE,      DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE, DM_NONE    },
/*  4 FLT  */ { DM_NONE, DM_NONE,  DM_NONE,  DM_NONE,      DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE, DM_NONE    },
/*  5 INT  */ { DM_NONE, DM_NONE,  DM_NONE,  DM_NONE,      DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE, DM_NONE    },
/*  6 IVL  */ { DM_NONE, DM_NONE,  DM_NONE,  DM_NONE,      DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE, DM_NONE    },
/*  7 TME  */ { DM_NONE, DM_NONE,  DM_NONE,  DM_NONE,      DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE, DM_NONE    },
/*  8 TSP  */ { DM_NONE, DM_NONE,  DM_NONE,  DM_NONE,      DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE, DM_NONE    },
/*  9 VCH  */ { DM_NONE, DM_NONE,  DM_NONE,  DM_NONE,      DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE, DM_NONE    },

/* 10 UNK  */ { DM_NONE, DM_NONE,  DM_BOL,   DM_NONE,      DM_UNK,     DM_UNK,       DM_UNK,     DM_NONE,      DM_NONE,    DM_NONE, DM_NONE    }
};

static eEO eMathOp_AndCode[DM_ENUMS][DM_ENUMS] = {
           //   NONE     BCD            BOL           DTE           FLT            INT            IVL           TME           TSP           VCH      UNK
/*  0 NONE */ { EO_NONE, EO_NONE,       EO_NONE,      EO_NONE,      EO_NONE,       EO_NONE,       EO_NONE,      EO_NONE,      EO_NONE,      EO_NONE, EO_NONE    },
/*  1 BCD  */ { EO_NONE, EO_NONE,       EO_NONE,      EO_NONE,      EO_NONE,       EO_NONE,       EO_NONE,      EO_NONE,      EO_NONE,      EO_NONE, EO_NONE    },
/*  2 BOL  */ { EO_NONE, EO_NONE,       EO_AND_BOLBOL,EO_NONE,      EO_NONE,       EO_NONE,       EO_NONE,      EO_NONE,      EO_NONE,      EO_NONE, EO_AND_BOLUNK    },
/*  3 DTE  */ { EO_NONE, EO_NONE,       EO_NONE,      EO_NONE,      EO_NONE,       EO_NONE,       EO_NONE,      EO_NONE,      EO_NONE,      EO_NONE, EO_NONE    },
/*  4 FLT  */ { EO_NONE, EO_NONE,       EO_NONE,      EO_NONE,      EO_NONE,       EO_NONE,       EO_NONE,      EO_NONE,      EO_NONE,      EO_NONE, EO_NONE    },
/*  5 INT  */ { EO_NONE, EO_NONE,       EO_NONE,      EO_NONE,      EO_NONE,       EO_NONE,       EO_NONE,      EO_NONE,      EO_NONE,      EO_NONE, EO_NONE    },
/*  6 IVL  */ { EO_NONE, EO_NONE,       EO_NONE,      EO_NONE,      EO_NONE,       EO_NONE,       EO_NONE,      EO_NONE,      EO_NONE,      EO_NONE, EO_NONE    },
/*  7 TME  */ { EO_NONE, EO_NONE,       EO_NONE,      EO_NONE,      EO_NONE,       EO_NONE,       EO_NONE,      EO_NONE,      EO_NONE,      EO_NONE, EO_NONE    },
/*  8 TSP  */ { EO_NONE, EO_NONE,       EO_NONE,      EO_NONE,      EO_NONE,       EO_NONE,       EO_NONE,      EO_NONE,      EO_NONE,      EO_NONE, EO_NONE    },
/*  9 VCH  */ { EO_NONE, EO_NONE,       EO_NONE,      EO_NONE,      EO_NONE,       EO_NONE,       EO_NONE,      EO_NONE,      EO_NONE,      EO_NONE, EO_NONE    },
/* 10 UNK  */ { EO_NONE, EO_NONE,       EO_AND_UNKBOL,EO_NONE,      EO_NONE,       EO_NONE,       EO_NONE,      EO_NONE,      EO_NONE,      EO_NONE, EO_NONE    }
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// NOT

static eDM eMathOp_NotResult[DM_ENUMS] =
    // NONE    BOL     BCD      DTE       FLT      INT      IVL      TME      TSP       VCH      UNK
    { DM_NONE, DM_BOL, DM_NONE, DM_NONE,  DM_NONE, DM_NONE, DM_NONE, DM_NONE, DM_NONE,  DM_NONE, DM_BOL };

static eEO eMathOp_NotCode[DM_ENUMS][DM_ENUMS] =
    // NONE     BOL        BCD       DTE       FLT       INT       IVL      TME      TSP       VCH      UNK
    { EO_NONE, EO_NOT_BOL, EO_NONE,  EO_NONE,  EO_NONE,  EO_NONE,  EO_NONE, EO_NONE, EO_NONE,  EO_NONE, EO_NOT_UNK    };


// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// OR

static eDM eMathOp_OrResult[DM_ENUMS][DM_ENUMS] = {
           //   NONE     BCD       BOL       DTE           FLT         INT           IVL         TME          TSP            VCH      UNK
/*  0 NONE */ { DM_NONE, DM_NONE,  DM_NONE,  DM_NONE,      DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE, DM_NONE    },
/*  1 BCD  */ { DM_NONE, DM_NONE,  DM_NONE,  DM_NONE,      DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE, DM_NONE    },
/*  2 BOL  */ { DM_NONE, DM_BOL,   DM_NONE,  DM_NONE,      DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE, DM_BOL     },
/*  3 DTE  */ { DM_NONE, DM_NONE,  DM_NONE,  DM_NONE,      DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE, DM_NONE    },
/*  4 FLT  */ { DM_NONE, DM_NONE,  DM_NONE,  DM_NONE,      DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE, DM_NONE    },
/*  5 INT  */ { DM_NONE, DM_NONE,  DM_NONE,  DM_NONE,      DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE, DM_NONE    },
/*  6 IVL  */ { DM_NONE, DM_NONE,  DM_NONE,  DM_NONE,      DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE, DM_NONE    },
/*  7 TME  */ { DM_NONE, DM_NONE,  DM_NONE,  DM_NONE,      DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE, DM_NONE    },
/*  8 TSP  */ { DM_NONE, DM_NONE,  DM_NONE,  DM_NONE,      DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE, DM_NONE    },
/*  9 VCH  */ { DM_NONE, DM_NONE,  DM_NONE,  DM_NONE,      DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE, DM_NONE    },

/* 10 UNK  */ { DM_NONE, DM_NONE,  DM_BOL,   DM_NONE,      DM_UNK,     DM_UNK,       DM_UNK,     DM_NONE,      DM_NONE,    DM_NONE, DM_NONE    }
};

static eEO eMathOp_OrCode[DM_ENUMS][DM_ENUMS] = {
    //   NONE     BCD            BOL           DTE           FLT            INT            IVL           TME           TSP           VCH      UNK
/*  0 NONE */ { EO_NONE, EO_NONE,       EO_NONE,      EO_NONE,      EO_NONE,       EO_NONE,       EO_NONE,      EO_NONE,      EO_NONE,      EO_NONE, EO_NONE    },
/*  1 BCD  */ { EO_NONE, EO_NONE,       EO_NONE,      EO_NONE,      EO_NONE,       EO_NONE,       EO_NONE,      EO_NONE,      EO_NONE,      EO_NONE, EO_NONE    },
/*  2 BOL  */ { EO_NONE, EO_NONE,       EO_OR_BOLBOL, EO_NONE,      EO_NONE,       EO_NONE,       EO_NONE,      EO_NONE,      EO_NONE,      EO_NONE, EO_OR_BOLUNK    },
/*  3 DTE  */ { EO_NONE, EO_NONE,       EO_NONE,      EO_NONE,      EO_NONE,       EO_NONE,       EO_NONE,      EO_NONE,      EO_NONE,      EO_NONE, EO_NONE    },
/*  4 FLT  */ { EO_NONE, EO_NONE,       EO_NONE,      EO_NONE,      EO_NONE,       EO_NONE,       EO_NONE,      EO_NONE,      EO_NONE,      EO_NONE, EO_NONE    },
/*  5 INT  */ { EO_NONE, EO_NONE,       EO_NONE,      EO_NONE,      EO_NONE,       EO_NONE,       EO_NONE,      EO_NONE,      EO_NONE,      EO_NONE, EO_NONE    },
/*  6 IVL  */ { EO_NONE, EO_NONE,       EO_NONE,      EO_NONE,      EO_NONE,       EO_NONE,       EO_NONE,      EO_NONE,      EO_NONE,      EO_NONE, EO_NONE    },
/*  7 TME  */ { EO_NONE, EO_NONE,       EO_NONE,      EO_NONE,      EO_NONE,       EO_NONE,       EO_NONE,      EO_NONE,      EO_NONE,      EO_NONE, EO_NONE    },
/*  8 TSP  */ { EO_NONE, EO_NONE,       EO_NONE,      EO_NONE,      EO_NONE,       EO_NONE,       EO_NONE,      EO_NONE,      EO_NONE,      EO_NONE, EO_NONE    },
/*  9 VCH  */ { EO_NONE, EO_NONE,       EO_NONE,      EO_NONE,      EO_NONE,       EO_NONE,       EO_NONE,      EO_NONE,      EO_NONE,      EO_NONE, EO_NONE    },
/* 10 UNK  */ { EO_NONE, EO_NONE,       EO_OR_UNKBOL,EO_NONE,      EO_NONE,       EO_NONE,       EO_NONE,      EO_NONE,      EO_NONE,      EO_NONE, EO_NONE    }
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// XOR

static eDM eMathOp_XorResult[DM_ENUMS][DM_ENUMS] = {
    //   NONE     BCD       BOL       DTE           FLT         INT           IVL         TME          TSP            VCH      UNK
/*  0 NONE */ { DM_NONE, DM_NONE,  DM_NONE,  DM_NONE,      DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE, DM_NONE    },
/*  1 BCD  */ { DM_NONE, DM_NONE,  DM_NONE,  DM_NONE,      DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE, DM_NONE    },
/*  2 BOL  */ { DM_NONE, DM_BOL,   DM_NONE,  DM_NONE,      DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE, DM_BOL     },
/*  3 DTE  */ { DM_NONE, DM_NONE,  DM_NONE,  DM_NONE,      DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE, DM_NONE    },
/*  4 FLT  */ { DM_NONE, DM_NONE,  DM_NONE,  DM_NONE,      DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE, DM_NONE    },
/*  5 INT  */ { DM_NONE, DM_NONE,  DM_NONE,  DM_NONE,      DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE, DM_NONE    },
/*  6 IVL  */ { DM_NONE, DM_NONE,  DM_NONE,  DM_NONE,      DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE, DM_NONE    },
/*  7 TME  */ { DM_NONE, DM_NONE,  DM_NONE,  DM_NONE,      DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE, DM_NONE    },
/*  8 TSP  */ { DM_NONE, DM_NONE,  DM_NONE,  DM_NONE,      DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE, DM_NONE    },
/*  9 VCH  */ { DM_NONE, DM_NONE,  DM_NONE,  DM_NONE,      DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE, DM_NONE    },

/* 10 UNK  */ { DM_NONE, DM_NONE,  DM_BOL,   DM_NONE,      DM_UNK,     DM_UNK,       DM_UNK,     DM_NONE,      DM_NONE,    DM_NONE, DM_NONE    }
};

static eEO eMathOp_XorCode[DM_ENUMS][DM_ENUMS] = {
    //   NONE     BCD            BOL           DTE           FLT            INT            IVL           TME           TSP           VCH      UNK
/*  0 NONE */ { EO_NONE, EO_NONE,       EO_NONE,      EO_NONE,      EO_NONE,       EO_NONE,       EO_NONE,      EO_NONE,      EO_NONE,      EO_NONE, EO_NONE    },
/*  1 BCD  */ { EO_NONE, EO_NONE,       EO_NONE,      EO_NONE,      EO_NONE,       EO_NONE,       EO_NONE,      EO_NONE,      EO_NONE,      EO_NONE, EO_NONE    },
/*  2 BOL  */ { EO_NONE, EO_NONE,       EO_XOR_BOLBOL,EO_NONE,      EO_NONE,       EO_NONE,       EO_NONE,      EO_NONE,      EO_NONE,      EO_NONE, EO_XOR_BOLUNK    },
/*  3 DTE  */ { EO_NONE, EO_NONE,       EO_NONE,      EO_NONE,      EO_NONE,       EO_NONE,       EO_NONE,      EO_NONE,      EO_NONE,      EO_NONE, EO_NONE    },
/*  4 FLT  */ { EO_NONE, EO_NONE,       EO_NONE,      EO_NONE,      EO_NONE,       EO_NONE,       EO_NONE,      EO_NONE,      EO_NONE,      EO_NONE, EO_NONE    },
/*  5 INT  */ { EO_NONE, EO_NONE,       EO_NONE,      EO_NONE,      EO_NONE,       EO_NONE,       EO_NONE,      EO_NONE,      EO_NONE,      EO_NONE, EO_NONE    },
/*  6 IVL  */ { EO_NONE, EO_NONE,       EO_NONE,      EO_NONE,      EO_NONE,       EO_NONE,       EO_NONE,      EO_NONE,      EO_NONE,      EO_NONE, EO_NONE    },
/*  7 TME  */ { EO_NONE, EO_NONE,       EO_NONE,      EO_NONE,      EO_NONE,       EO_NONE,       EO_NONE,      EO_NONE,      EO_NONE,      EO_NONE, EO_NONE    },
/*  8 TSP  */ { EO_NONE, EO_NONE,       EO_NONE,      EO_NONE,      EO_NONE,       EO_NONE,       EO_NONE,      EO_NONE,      EO_NONE,      EO_NONE, EO_NONE    },
/*  9 VCH  */ { EO_NONE, EO_NONE,       EO_NONE,      EO_NONE,      EO_NONE,       EO_NONE,       EO_NONE,      EO_NONE,      EO_NONE,      EO_NONE, EO_NONE    },
/* 10 UNK  */ { EO_NONE, EO_NONE,       EO_XOR_UNKBOL,EO_NONE,      EO_NONE,       EO_NONE,       EO_NONE,      EO_NONE,      EO_NONE,      EO_NONE, EO_NONE    }
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
