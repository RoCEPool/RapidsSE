/*============================================================================*/
/* MathOp.h -- math operators                                                 */
/*============================================================================*/
#pragma once

static eDM eMathOp_AddResult[DM_ENUMS][DM_ENUMS] = {
           //   NONE     BOL      BCD       DTE           FLT         INT           IVL         TMT          TSP            VCH      UNK
/*  0 NONE */ { DM_NONE, DM_NONE, DM_NONE,  DM_NONE,      DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE, DM_NONE    },
/*  1 BCD  */ { DM_NONE, DM_NONE, DM_BCD,   DM_NONE,      DM_FLT,     DM_BCD,       DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE, DM_UNK     },
/*  2 BOL  */ { DM_NONE, DM_NONE, DM_NONE,  DM_NONE,      DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE, DM_NONE    },
/*  3 DTE  */ { DM_NONE, DM_NONE, DM_NONE,  DM_NONE,      DM_NONE,    DM_NONE/*?*/, DM_DTE,     DM_NONE/*?*/, DM_NONE,    DM_NONE, DM_NONE    },
/*  5 FLT  */ { DM_NONE, DM_NONE, DM_FLT,   DM_NONE,      DM_FLT,     DM_FLT,       DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE, DM_UNK     },
/*  6 INT  */ { DM_NONE, DM_NONE, DM_BCD,   DM_NONE/*?*/, DM_FLT,     DM_INT,       DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE, DM_UNK     },
/*  7 IVL  */ { DM_NONE, DM_NONE, DM_NONE,  DM_DTE,       DM_NONE,    DM_NONE,      DM_IVL,     DM_TME,       DM_TSP,     DM_NONE, DM_UNK     },
/*  9 TME  */ { DM_NONE, DM_NONE, DM_NONE,  DM_NONE/*?*/, DM_NONE,    DM_NONE/*?*/, DM_TME,     DM_NONE,      DM_NONE,    DM_NONE, DM_NONE    },
/* 10 TSP  */ { DM_NONE, DM_NONE, DM_NONE,  DM_NONE,      DM_NONE,    DM_NONE,      DM_TSP,     DM_NONE,      DM_NONE,    DM_NONE, DM_NONE    },
/* 11 VCH  */ { DM_NONE, DM_NONE, DM_NONE,  DM_NONE,      DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE, DM_NONE    },
/* 12 UNK  */ { DM_NONE, DM_NONE, DM_UNK,   DM_NONE,      DM_UNK,     DM_UNK,       DM_UNK,     DM_NONE,      DM_NONE,    DM_NONE, DM_NONE    }
};

static eEO eMathOp_AddCode[DM_ENUMS][DM_ENUMS] = {
                  //   NONE     BOL      BCD       DTE           FLT         INT           IVL         TMT          TSP            VCH      UNK
/*  0 NONE       */ { EO_NONE, EO_NONE, EO_NONE,   EO_NONE,      EO_NONE,    EO_NONE,      EO_NONE,    EO_NONE,      EO_NONE,      EO_NONE, EO_NONE    },
/*  1 BCD        */ { EO_NONE, EO_NONE, EO_NONE,   EO_NONE,      EO_NONE,    EO_NONE,      EO_NONE,    EO_NONE,      EO_NONE,      EO_NONE, EO_NONE    },
/*  2 BOL        */ { EO_NONE, EO_NONE, EO_NONE,   EO_NONE,      EO_NONE,    EO_NONE,      EO_NONE,    EO_NONE,      EO_NONE,      EO_NONE, EO_NONE    },
/*  3 DTE        */ { EO_NONE, EO_NONE, EO_NONE,   EO_NONE,      EO_NONE,    EO_NONE/*?*/, EO_NONE,    EO_NONE/*?*/, EO_NONE,      EO_NONE, EO_NONE    },
/*  5 FLT        */ { EO_NONE, EO_NONE, EO_NONE,   EO_NONE,      EO_NONE,    EO_NONE,      EO_NONE,    EO_NONE,      EO_NONE,      EO_NONE, EO_NONE    },
/*  6 INT        */ { EO_NONE, EO_NONE, EO_ADD_ID, EO_NONE/*?*/, EO_ADD_IF,  EO_ADD_II,    EO_NONE,    EO_NONE,      EO_NONE,      EO_NONE, EO_ADD_IU  },
/*  7 IVL        */ { EO_NONE, EO_NONE, EO_NONE,   EO_NONE,      EO_NONE,    EO_NONE,      EO_NONE,    EO_NONE,      EO_NONE,      EO_NONE, EO_NONE    },
/*  9 TME        */ { EO_NONE, EO_NONE, EO_NONE,   EO_NONE/*?*/, EO_NONE,    EO_NONE/*?*/, EO_NONE,    EO_NONE,      EO_NONE,      EO_NONE, EO_NONE    },
/* 10 TSP        */ { EO_NONE, EO_NONE, EO_NONE,   EO_NONE,      EO_NONE,    EO_NONE,      EO_NONE,    EO_NONE,      EO_NONE,      EO_NONE, EO_NONE    },
/* 11 VCH        */ { EO_NONE, EO_NONE, EO_NONE,   EO_NONE,      EO_NONE,    EO_NONE,      EO_NONE,    EO_NONE,      EO_NONE,      EO_NONE, EO_NONE    },
/* 12 UNK        */ { EO_NONE, EO_NONE, EO_NONE,   EO_NONE,      EO_NONE,    EO_NONE,      EO_NONE,    EO_NONE,      EO_NONE,      EO_NONE, EO_NONE    }
};

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

           //   NONE     BOL      BCD       DTE           FLT         INT           IVL         TMT          TSP            VCH      UNK
/*  0 NONE */ { EO_NONE, EO_NONE, EO_NONE,   EO_NONE,      EO_NONE,    EO_NONE,      EO_NONE,    EO_NONE,      EO_NONE,      EO_NONE, EO_NONE    },
/*  1 BCD  */ { EO_NONE, EO_NONE, EO_MUL_II, EO_NONE,      EO_MUL_II,  EO_MUL_II,    EO_NONE,    EO_NONE,      EO_NONE,      EO_NONE, EO_NONE    },
/*  2 BOL  */ { EO_NONE, EO_NONE, EO_NONE,   EO_NONE,      EO_NONE,    EO_NONE,      EO_NONE,    EO_NONE,      EO_NONE,      EO_NONE, EO_NONE    },
/*  3 DTE  */ { EO_NONE, EO_NONE, EO_NONE,   EO_NONE,      EO_NONE,    EO_NONE/*?*/, EO_NONE,    EO_NONE/*?*/, EO_NONE,      EO_NONE, EO_NONE    },
/*  5 FLT  */ { EO_NONE, EO_NONE, EO_NONE,   EO_NONE,      EO_NONE,    EO_NONE,      EO_NONE,    EO_NONE,      EO_NONE,      EO_NONE, EO_NONE    },
/*  6 INT  */ { EO_NONE, EO_NONE, EO_MUL_II, EO_NONE/*?*/, EO_MUL_II,  EO_MUL_II,    EO_NONE,    EO_NONE,      EO_NONE,      EO_NONE, EO_MUL_II  },
/*  7 IVL  */ { EO_NONE, EO_NONE, EO_NONE,   EO_NONE,      EO_NONE,    EO_NONE,      EO_NONE,    EO_NONE,      EO_NONE,      EO_NONE, EO_NONE    },
/*  9 TME  */ { EO_NONE, EO_NONE, EO_NONE,   EO_NONE/*?*/, EO_NONE,    EO_NONE/*?*/, EO_NONE,    EO_NONE,      EO_NONE,      EO_NONE, EO_NONE    },
/* 10 TSP  */ { EO_NONE, EO_NONE, EO_NONE,   EO_NONE,      EO_NONE,    EO_NONE,      EO_NONE,    EO_NONE,      EO_NONE,      EO_NONE, EO_NONE    },
/* 11 VCH  */ { EO_NONE, EO_NONE, EO_NONE,   EO_NONE,      EO_NONE,    EO_NONE,      EO_NONE,    EO_NONE,      EO_NONE,      EO_NONE, EO_NONE    },
/* 12 UNK  */ { EO_NONE, EO_NONE, EO_NONE,   EO_NONE,      EO_NONE,    EO_NONE,      EO_NONE,    EO_NONE,      EO_NONE,      EO_NONE, EO_NONE    }
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
