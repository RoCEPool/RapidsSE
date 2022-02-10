/*============================================================================*/
/* FunMatrix.h -- Function Matrix                                             */
/*============================================================================*/
#pragma once

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// CONVERSION OPERATORS
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

static eFM eMacro_Cast[DM_ENUMS][DM_ENUMS] = {
           //   NONE     BCD           BOL          DTE           FLT           INT             IVL             IVY            TME           TSP           VCH           UNK
/*  0 NONE */ { FM_NONE, FM_NONE,      FM_NONE,     FM_NONE,      FM_NONE,      FM_NONE,        FM_NONE,        FM_NONE,       FM_NONE,      FM_NONE,      FM_NONE,      FM_NONE      },
/*  1 BCD  */ { FM_NONE, FM_NONE,      FM_NONE,     FM_NONE,      FM_BCD_TOFLT, FM_BCD_TOINT,   FM_NONE,/*?1*/  FM_NONE,/*?1*/ FM_NONE,      FM_NONE,      FM_BCD_TOVCH, FM_BCD_TOUNK },
/*  2 BOL  */ { FM_NONE, FM_NONE,      FM_NONE,     FM_NONE,      FM_NONE,      FM_NONE,        FM_NONE,        FM_NONE,       FM_NONE,      FM_NONE,      FM_BOL_TOVCH, FM_BOL_TOUNK },
/*  3 DTE  */ { FM_NONE, FM_NONE,      FM_NONE,     FM_NONE,      FM_NONE,      FM_NONE,        FM_NONE,        FM_NONE,       FM_NONE,      FM_DTE_TOTSP, FM_DTE_TOVCH, FM_DTE_TOUNK },
/*  4 FLT  */ { FM_NONE, FM_FLT_TOBCD, FM_NONE,     FM_NONE,      FM_NONE,      FM_FLT_TOINT,   FM_NONE,        FM_NONE,       FM_NONE,      FM_NONE,      FM_FLT_TOVCH, FM_FLT_TOUNK },
/*  5 INT  */ { FM_NONE, FM_INT_TOBCD, FM_NONE,     FM_NONE,      FM_INT_TOFLT, FM_NONE,        FM_NONE,/*?1*/  FM_NONE,/*?1*/ FM_NONE,      FM_NONE,      FM_INT_TOVCH, FM_INT_TOUNK },
/*  6 IVL  */ { FM_NONE, FM_NONE/*?2*/,FM_NONE,     FM_NONE,      FM_NONE,      FM_NONE/*?2*/,  FM_NONE,        FM_NONE,       FM_NONE,      FM_NONE,      FM_IVL_TOVCH, FM_IVL_TOUNK },
/*  7 IVY  */ { FM_NONE, FM_NONE/*?2*/,FM_NONE,     FM_NONE,      FM_NONE,      FM_NONE/*?2*/,  FM_NONE,        FM_NONE,       FM_NONE,      FM_NONE,      FM_IVY_TOVCH, FM_IVY_TOUNK },
/*  8 TME  */ { FM_NONE, FM_NONE,      FM_NONE,     FM_NONE,      FM_NONE,      FM_NONE,        FM_NONE,        FM_NONE,       FM_NONE,      FM_TME_TOTSP, FM_TME_TOVCH, FM_TME_TOUNK },
/*  9 TSP  */ { FM_NONE, FM_NONE,      FM_NONE,     FM_TSP_TODTE, FM_NONE,      FM_NONE,        FM_NONE,        FM_NONE,       FM_TSP_TOTME, FM_NONE,      FM_TSP_TOVCH, FM_TSP_TOUNK },
/* 10 VCH  */ { FM_NONE, FM_VCH_TOBCD, FM_NONE,     FM_VCH_TODTE, FM_VCH_TOFLT, FM_VCH_TOINT,   FM_VCH_TOIVL,   FM_VCH_TOIVY,  FM_VCH_TOTME, FM_VCH_TOTSP, FM_NONE,      FM_VCH_TOUNK },
/* 11 UNK  */ { FM_NONE, FM_UNK_TOBCD, FM_UNK_TOBOL,FM_UNK_TOFLT, FM_UNK_TOINT, FM_UNK_TOIVL,   FM_UNK_TOIVY,  FM_UNK_TOTME, FM_UNK_TOTSP, FM_UNK_TOVCH,FM_NONE ,     FM_NONE      }
};

static eDM eMacro_CastResult[DM_ENUMS][DM_ENUMS] = {
           //   NONE     BCD           BOL         DTE      FLT      INT            IVL            IVY            TME      TSP      VCH      UNK
/*  0 NONE */ { DM_NONE, DM_NONE,      DM_NONE,    DM_NONE, DM_NONE, DM_NONE,       DM_NONE,       DM_NONE,       DM_NONE, DM_NONE, DM_NONE, DM_NONE },
/*  1 BCD  */ { DM_NONE, DM_BCD,       DM_NONE,    DM_NONE, DM_FLT,  DM_INT,        DM_NONE,/*?1*/ DM_NONE,/*?1*/ DM_NONE, DM_NONE, DM_VCH,  DM_UNK  },
/*  2 BOL  */ { DM_NONE, DM_NONE,      DM_NONE,    DM_NONE, DM_NONE, DM_NONE,       DM_NONE,       DM_NONE,       DM_NONE, DM_NONE, DM_VCH,  DM_UNK  },
/*  3 DTE  */ { DM_NONE, DM_NONE,      DM_NONE,    DM_DTE,  DM_NONE, DM_NONE,       DM_NONE,       DM_NONE,       DM_NONE, DM_TSP,  DM_VCH,  DM_UNK  },
/*  4 FLT  */ { DM_NONE, DM_BCD,       DM_NONE,    DM_NONE, DM_FLT,  DM_INT,        DM_NONE,       DM_NONE,       DM_NONE, DM_NONE, DM_VCH,  DM_UNK  },
/*  5 INT  */ { DM_NONE, DM_BCD,       DM_NONE,    DM_NONE, DM_FLT,  DM_INT,        DM_NONE,/*?1*/ DM_NONE,/*?1*/ DM_NONE, DM_NONE, DM_VCH,  DM_UNK  },
/*  6 IVL  */ { DM_NONE, DM_NONE/*?2*/,DM_NONE,    DM_NONE, DM_NONE, DM_NONE/*?2*/, DM_IVL,        DM_NONE,       DM_NONE, DM_NONE, DM_VCH,  DM_UNK  },
/*  7 IVY  */ { DM_NONE, DM_NONE/*?2*/,DM_NONE,    DM_NONE, DM_NONE, DM_NONE/*?2*/, DM_NONE,       DM_IVY,        DM_NONE, DM_NONE, DM_VCH,  DM_UNK  },
/*  8 TME  */ { DM_NONE, DM_NONE,      DM_NONE,    DM_NONE, DM_NONE, DM_NONE,       DM_NONE,       DM_NONE,       DM_TME,  DM_TSP,  DM_VCH,  DM_UNK  },
/*  9 TSP  */ { DM_NONE, DM_NONE,      DM_NONE,    DM_DTE,  DM_NONE, DM_NONE,       DM_NONE,       DM_NONE,       DM_TME,  DM_TSP,  DM_VCH,  DM_UNK  },
/* 10 VCH  */ { DM_NONE, DM_BCD,       DM_NONE,    DM_DTE,  DM_FLT,  DM_INT,        DM_IVL,        DM_IVY,        DM_TME,  DM_TSP,  DM_VCH,  DM_UNK  },
/* 11 UNK  */ { DM_NONE, DM_BCD,       DM_BOL,     DM_DTE,  DM_FLT,  DM_INT,        DM_IVL,        DM_IVY,        DM_TME,  DM_TSP,  DM_VCH,  DM_UNK  }
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// MATHEMATICAL OPERATORS
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// ADD

static eFM eMacro_Add[DM_ENUMS][DM_ENUMS] = {
           //   NONE     BCD            BOL       DTE            FLT            INT            IVL        IVY      TME            TSP            VCH      UNK
/*  0 NONE */ { FM_NONE, FM_NONE,       FM_NONE,  FM_NONE,       FM_NONE,       FM_NONE,       FM_NONE,   FM_NONE, FM_NONE,       FM_NONE,       FM_NONE, FM_NONE  },
/*  1 BCD  */ { FM_NONE, FM_ADD_BCDBCD, FM_NONE,  FM_NONE,       FM_NONE,       FM_NONE,       FM_NONE,   FM_NONE, FM_NONE,       FM_NONE,       FM_NONE, FM_NONE  },
/*  2 BOL  */ { FM_NONE, FM_NONE,       FM_NONE,  FM_NONE,       FM_NONE,       FM_NONE,       FM_NONE,   FM_NONE, FM_NONE,       FM_NONE,       FM_NONE, FM_NONE  },
/*  3 DTE  */ { FM_NONE, FM_NONE,       FM_NONE,  FM_NONE,       FM_NONE,       FM_NONE/*?*/,  FM_NONE,   FM_NONE, FM_NONE/*?*/,  FM_NONE,       FM_NONE, FM_NONE  },
/*  4 FLT  */ { FM_NONE, FM_NONE,       FM_NONE,  FM_NONE,       FM_ADD_FLTFLT, FM_NONE,       FM_NONE,   FM_NONE, FM_NONE,       FM_NONE,       FM_NONE, FM_NONE  },
/*  5 INT  */ { FM_NONE, FM_NONE,       FM_NONE,  FM_NONE/*?*/,  FM_NONE,       FM_ADD_INTINT, FM_NONE,   FM_NONE, FM_NONE,       FM_NONE,       FM_NONE, FM_NONE  },
/*  6 IVL  */ { FM_NONE, FM_NONE,       FM_NONE,  FM_NONE,       FM_NONE,       FM_NONE,       FM_NONE,   FM_NONE, FM_NONE,       FM_NONE,       FM_NONE, FM_NONE  },
/*  7 IVY  */ { FM_NONE, FM_NONE,       FM_NONE,  FM_NONE,       FM_NONE,       FM_NONE,       FM_NONE,   FM_NONE, FM_NONE,       FM_NONE,       FM_NONE, FM_NONE  },
/*  8 TME  */ { FM_NONE, FM_NONE,       FM_NONE,  FM_NONE/*?*/,  FM_NONE,       FM_NONE/*?*/,  FM_NONE,   FM_NONE, FM_NONE,       FM_NONE,       FM_NONE, FM_NONE  },
/*  9 TSP  */ { FM_NONE, FM_NONE,       FM_NONE,  FM_NONE,       FM_NONE,       FM_NONE,       FM_NONE,   FM_NONE, FM_NONE,       FM_NONE,       FM_NONE, FM_NONE  },
/* 10 VCH  */ { FM_NONE, FM_NONE,       FM_NONE,  FM_NONE,       FM_NONE,       FM_NONE,       FM_NONE,   FM_NONE, FM_NONE,       FM_NONE,       FM_NONE, FM_NONE  },
/* 11 UNK  */ { FM_NONE, FM_NONE,       FM_NONE,  FM_NONE,       FM_NONE,       FM_NONE,       FM_NONE,   FM_NONE, FM_NONE,       FM_NONE,       FM_NONE, FM_NONE  }
};

static eDM eMacro_AddResult[DM_ENUMS][DM_ENUMS] = {
           //   NONE     BCD       BOL       DTE           FLT         INT           IVL         IVY           TME          TSP            VCH      UNK
/*  0 NONE */ { DM_NONE, DM_NONE,  DM_NONE,  DM_NONE,      DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE, DM_NONE    },
/*  1 BCD  */ { DM_NONE, DM_BCD,   DM_NONE,  DM_NONE,      DM_FLT,     DM_BCD,       DM_NONE,    DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE, DM_UNK     },
/*  2 BOL  */ { DM_NONE, DM_NONE,  DM_NONE,  DM_NONE,      DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE, DM_NONE    },
/*  3 DTE  */ { DM_NONE, DM_NONE,  DM_NONE,  DM_NONE,      DM_NONE,    DM_NONE/*?*/, DM_NONE,    DM_NONE,    DM_NONE/*?*/, DM_NONE,    DM_NONE, DM_NONE    },
/*  4 FLT  */ { DM_NONE, DM_FLT,   DM_NONE,  DM_NONE,      DM_FLT,     DM_FLT,       DM_NONE,    DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE, DM_UNK     },
/*  5 INT  */ { DM_NONE, DM_BCD,   DM_NONE,  DM_NONE/*?*/, DM_FLT,     DM_INT,       DM_NONE,    DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE, DM_UNK     },
/*  6 IVL  */ { DM_NONE, DM_NONE,  DM_NONE,  DM_NONE,      DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE,    DM_TME,       DM_TSP,     DM_NONE, DM_UNK     },
/*  7 IVY  */ { DM_NONE, DM_NONE,  DM_NONE,  DM_NONE,      DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE,    DM_TME,       DM_TSP,     DM_NONE, DM_UNK     },
/*  8 TME  */ { DM_NONE, DM_NONE,  DM_NONE,  DM_NONE/*?*/, DM_NONE,    DM_NONE/*?*/, DM_NONE,    DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE, DM_NONE    },
/*  9 TSP  */ { DM_NONE, DM_NONE,  DM_NONE,  DM_NONE,      DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE, DM_NONE    },
/* 10 VCH  */ { DM_NONE, DM_NONE,  DM_NONE,  DM_NONE,      DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE, DM_NONE    },
/* 11 UNK  */ { DM_NONE, DM_UNK,   DM_UNK,   DM_NONE,      DM_UNK,     DM_UNK,       DM_NONE,    DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE, DM_NONE    }
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// DIVIDE

static eFM eMacro_Div[DM_ENUMS][DM_ENUMS] = {
           //   NONE     BCD            BOL       DTE            FLT            INT            IVL            IVY            TME            TSP        VCH      UNK
/*  0 NONE */ { FM_NONE, FM_NONE,       FM_NONE,  FM_NONE,       FM_NONE,       FM_NONE,       FM_NONE,       FM_NONE,       FM_NONE,       FM_NONE,   FM_NONE, FM_NONE  },
/*  1 BCD  */ { FM_NONE, FM_DIV_BCDBCD, FM_NONE,  FM_NONE,       FM_NONE,       FM_NONE,       FM_NONE,       FM_NONE,       FM_NONE,       FM_NONE,   FM_NONE, FM_NONE  },
/*  2 BOL  */ { FM_NONE, FM_NONE,       FM_NONE,  FM_NONE,       FM_NONE,       FM_NONE,       FM_NONE,       FM_NONE,       FM_NONE,       FM_NONE,   FM_NONE, FM_NONE  },
/*  3 DTE  */ { FM_NONE, FM_NONE,       FM_NONE,  FM_NONE,       FM_NONE,       FM_NONE/*?*/,  FM_NONE,       FM_NONE,       FM_NONE/*?*/,  FM_NONE,   FM_NONE, FM_NONE  },
/*  4 FLT  */ { FM_NONE, FM_NONE,       FM_NONE,  FM_NONE,       FM_DIV_FLTFLT, FM_NONE,       FM_NONE,       FM_NONE,       FM_NONE,       FM_NONE,   FM_NONE, FM_NONE  },
/*  5 INT  */ { FM_NONE, FM_NONE,       FM_NONE,  FM_NONE/*?*/,  FM_NONE,       FM_DIV_INTINT, FM_NONE,       FM_NONE,       FM_NONE,       FM_NONE,   FM_NONE, FM_NONE  },
/*  6 IVL  */ { FM_NONE, FM_NONE,       FM_NONE,  FM_NONE,       FM_NONE,       FM_NONE,       FM_NONE,       FM_NONE,       FM_NONE,       FM_NONE,   FM_NONE, FM_NONE  },
/*  7 IVY  */ { FM_NONE, FM_NONE,       FM_NONE,  FM_NONE,       FM_NONE,       FM_NONE,       FM_NONE,       FM_NONE,       FM_NONE,       FM_NONE,   FM_NONE, FM_NONE  },
/*  8 TME  */ { FM_NONE, FM_NONE,       FM_NONE,  FM_NONE/*?*/,  FM_NONE,       FM_NONE/*?*/,  FM_NONE,       FM_NONE,       FM_NONE,       FM_NONE,   FM_NONE, FM_NONE  },
/*  9 TSP  */ { FM_NONE, FM_NONE,       FM_NONE,  FM_NONE,       FM_NONE,       FM_NONE,       FM_NONE,       FM_NONE,       FM_NONE,       FM_NONE,   FM_NONE, FM_NONE  },
/* 10 VCH  */ { FM_NONE, FM_NONE,       FM_NONE,  FM_NONE,       FM_NONE,       FM_NONE,       FM_NONE,       FM_NONE,       FM_NONE,       FM_NONE,   FM_NONE, FM_NONE  },
/* 11 UNK  */ { FM_NONE, FM_NONE,       FM_NONE,  FM_NONE,       FM_NONE,       FM_NONE,       FM_NONE,       FM_NONE,       FM_NONE,       FM_NONE,   FM_NONE, FM_NONE  }
};

static eDM eMacro_DivResult[DM_ENUMS][DM_ENUMS] = {
           //   NONE     BCD       BOL      DTE           FLT         INT           IVL         IVY         TME           TSP         VCH      UNK
/*  0 NONE */ { DM_NONE, DM_NONE,  DM_NONE, DM_NONE,      DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE, DM_NONE    },
/*  1 BCD  */ { DM_NONE, DM_BCD,   DM_NONE, DM_NONE,      DM_FLT,     DM_BCD,       DM_NONE,    DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE, DM_NONE    },
/*  2 BOL  */ { DM_NONE, DM_NONE,  DM_NONE, DM_NONE,      DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE, DM_NONE    },
/*  3 DTE  */ { DM_NONE, DM_NONE,  DM_NONE, DM_NONE,      DM_NONE,    DM_NONE/*?*/, DM_NONE,    DM_NONE,    DM_NONE/*?*/, DM_NONE,    DM_NONE, DM_NONE    },
/*  4 FLT  */ { DM_NONE, DM_NONE,  DM_NONE, DM_NONE,      DM_FLT,     DM_NONE,      DM_NONE,    DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE, DM_NONE    },
/*  5 INT  */ { DM_NONE, DM_NONE,  DM_NONE, DM_NONE/*?*/, DM_NONE,    DM_INT,       DM_NONE,    DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE, DM_NONE    },
/*  6 IVL  */ { DM_NONE, DM_NONE,  DM_NONE, DM_NONE,      DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE, DM_NONE    },
/*  7 IVY  */ { DM_NONE, DM_NONE,  DM_NONE, DM_NONE,      DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE, DM_NONE    },
/*  8 TME  */ { DM_NONE, DM_NONE,  DM_NONE, DM_NONE/*?*/, DM_NONE,    DM_NONE/*?*/, DM_NONE,    DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE, DM_NONE    },
/*  9 TSP  */ { DM_NONE, DM_NONE,  DM_NONE, DM_NONE,      DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE, DM_NONE    },
/* 10 VCH  */ { DM_NONE, DM_NONE,  DM_NONE, DM_NONE,      DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE, DM_NONE    },
/* 11 UNK  */ { DM_NONE, DM_NONE,  DM_NONE, DM_NONE,      DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE, DM_NONE    }
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// MODULUS

static eDM eMacro_ModResult[DM_ENUMS][DM_ENUMS] = {

           //   NONE     BCD       BOL      DTE           FLT         INT           IVL         IVY           TMT          TSP            VCH      UNK
/*  0 NONE */ { DM_NONE, DM_NONE,  DM_NONE, DM_NONE,      DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE, DM_NONE    },
/*  1 BCD  */ { DM_NONE, DM_NONE,  DM_NONE, DM_NONE,      DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE, DM_NONE    },
/*  2 BOL  */ { DM_NONE, DM_NONE,  DM_NONE, DM_NONE,      DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE, DM_NONE    },
/*  3 DTE  */ { DM_NONE, DM_NONE,  DM_NONE, DM_NONE,      DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE, DM_NONE    },
/*  4 FLT  */ { DM_NONE, DM_NONE,  DM_NONE, DM_NONE,      DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE, DM_NONE    },
/*  5 INT  */ { DM_NONE, DM_NONE,  DM_NONE, DM_NONE,      DM_NONE,    DM_INT,       DM_NONE,    DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE, DM_UNK     },
/*  6 IVL  */ { DM_NONE, DM_NONE,  DM_NONE, DM_NONE,      DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE,    DM_TME,       DM_NONE,    DM_NONE, DM_NONE    },
/*  7 IVY  */ { DM_NONE, DM_NONE,  DM_NONE, DM_NONE,      DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE,    DM_TME,       DM_NONE,    DM_NONE, DM_NONE    },
/*  8 TME  */ { DM_NONE, DM_NONE,  DM_NONE, DM_NONE,      DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE, DM_NONE    },
/*  9 TSP  */ { DM_NONE, DM_NONE,  DM_NONE, DM_NONE,      DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE, DM_NONE    },
/* 10 VCH  */ { DM_NONE, DM_NONE,  DM_NONE, DM_NONE,      DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE, DM_NONE    },
/* 11 UNK  */ { DM_NONE, DM_NONE,  DM_NONE, DM_NONE,      DM_NONE,    DM_UNK,       DM_NONE,    DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE, DM_NONE    }
};

static eFM eMacro_Mod[DM_ENUMS][DM_ENUMS] = {
           //   NONE     BCD      BOL      DTE       FLT      INT           IVL           IVY           TME           TSP           VCH      UNK
/*  0 NONE */ { FM_NONE, FM_NONE, FM_NONE, FM_NONE,  FM_NONE, FM_NONE,      FM_NONE,      FM_NONE,      FM_NONE,      FM_NONE,      FM_NONE, FM_NONE  },
/*  1 BCD  */ { FM_NONE, FM_NONE, FM_NONE, FM_NONE,  FM_NONE, FM_NONE,      FM_NONE,      FM_NONE,      FM_NONE,      FM_NONE,      FM_NONE, FM_NONE  },
/*  2 BOL  */ { FM_NONE, FM_NONE, FM_NONE, FM_NONE,  FM_NONE, FM_NONE,      FM_NONE,      FM_NONE,      FM_NONE,      FM_NONE,      FM_NONE, FM_NONE  },
/*  3 DTE  */ { FM_NONE, FM_NONE, FM_NONE, FM_NONE,  FM_NONE, FM_NONE,      FM_NONE,      FM_NONE,      FM_NONE,      FM_NONE,      FM_NONE, FM_NONE  },
/*  4 FLT  */ { FM_NONE, FM_NONE, FM_NONE, FM_NONE,  FM_NONE, FM_NONE,      FM_NONE,      FM_NONE,      FM_NONE,      FM_NONE,      FM_NONE, FM_NONE  },
/*  5 INT  */ { FM_NONE, FM_NONE, FM_NONE, FM_NONE,  FM_NONE, FM_MOD_INTINT,FM_NONE,      FM_NONE,      FM_NONE,      FM_NONE,      FM_NONE, FM_NONE  },
/*  6 IVL  */ { FM_NONE, FM_NONE, FM_NONE, FM_NONE,  FM_NONE, FM_NONE,      FM_NONE,      FM_NONE,      FM_NONE,      FM_NONE,      FM_NONE, FM_NONE  },
/*  7 IVY  */ { FM_NONE, FM_NONE, FM_NONE, FM_NONE,  FM_NONE, FM_NONE,      FM_NONE,      FM_NONE,      FM_NONE,      FM_NONE,      FM_NONE, FM_NONE  },
/*  8 TME  */ { FM_NONE, FM_NONE, FM_NONE, FM_NONE,  FM_NONE, FM_NONE/*?*/, FM_NONE,      FM_NONE,      FM_NONE,      FM_NONE,      FM_NONE, FM_NONE  },
/*  9 TSP  */ { FM_NONE, FM_NONE, FM_NONE, FM_NONE,  FM_NONE, FM_NONE,      FM_NONE,      FM_NONE,      FM_NONE,      FM_NONE,      FM_NONE, FM_NONE  },
/* 10 VCH  */ { FM_NONE, FM_NONE, FM_NONE, FM_NONE,  FM_NONE, FM_NONE,      FM_NONE,      FM_NONE,      FM_NONE,      FM_NONE,      FM_NONE, FM_NONE  },
/* 11 UNK  */ { FM_NONE, FM_NONE, FM_NONE, FM_NONE,  FM_NONE, FM_NONE,      FM_NONE,      FM_NONE,      FM_NONE,      FM_NONE,      FM_NONE, FM_NONE  }
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// MULTIPLY

static eFM eMacro_Mul[DM_ENUMS][DM_ENUMS] = {
           //   NONE     BCD            BOL       DTE              FLT            INT            IVL            IVY            TME            TSP            VCH      UNK
/*  0 NONE */ { FM_NONE, FM_NONE,       FM_NONE,  FM_NONE,         FM_NONE,       FM_NONE,       FM_NONE,       FM_NONE,       FM_NONE,       FM_NONE,       FM_NONE, FM_NONE  },
/*  1 BCD  */ { FM_NONE, FM_MUL_BCDBCD, FM_NONE,  FM_NONE,         FM_NONE,       FM_NONE,       FM_NONE,       FM_NONE,       FM_NONE,       FM_NONE,       FM_NONE, FM_NONE  },
/*  2 BOL  */ { FM_NONE, FM_NONE,       FM_NONE,  FM_NONE,         FM_NONE,       FM_NONE,       FM_NONE,       FM_NONE,       FM_NONE,       FM_NONE,       FM_NONE, FM_NONE  },
/*  3 DTE  */ { FM_NONE, FM_NONE,       FM_NONE,  FM_NONE,         FM_NONE,       FM_NONE/*?*/,  FM_NONE,       FM_NONE,       FM_NONE/*?*/,  FM_NONE,       FM_NONE, FM_NONE  },
/*  4 FLT  */ { FM_NONE, FM_NONE,       FM_NONE,  FM_NONE,         FM_MUL_FLTFLT, FM_NONE,       FM_NONE,       FM_NONE,       FM_NONE,       FM_NONE,       FM_NONE, FM_NONE  },
/*  5 INT  */ { FM_NONE, FM_NONE,       FM_NONE,  FM_NONE/*?*/,    FM_NONE,       FM_MUL_INTINT, FM_NONE,       FM_NONE,       FM_NONE,       FM_NONE,       FM_NONE, FM_NONE  },
/*  6 IVL  */ { FM_NONE, FM_NONE,       FM_NONE,  FM_NONE,         FM_NONE,       FM_NONE,       FM_NONE,       FM_NONE,       FM_NONE,       FM_NONE,       FM_NONE, FM_NONE  },
/*  7 IVY  */ { FM_NONE, FM_NONE,       FM_NONE,  FM_NONE,         FM_NONE,       FM_NONE,       FM_NONE,       FM_NONE,       FM_NONE,       FM_NONE,       FM_NONE, FM_NONE  },
/*  8 TME  */ { FM_NONE, FM_NONE,       FM_NONE,  FM_NONE/*?*/,    FM_NONE,       FM_NONE/*?*/,  FM_NONE,       FM_NONE,       FM_NONE,       FM_NONE,       FM_NONE, FM_NONE  },
/*  9 TSP  */ { FM_NONE, FM_NONE,       FM_NONE,  FM_NONE,         FM_NONE,       FM_NONE,       FM_NONE,       FM_NONE,       FM_NONE,       FM_NONE,       FM_NONE, FM_NONE  },
/* 10 VCH  */ { FM_NONE, FM_NONE,       FM_NONE,  FM_NONE,         FM_NONE,       FM_NONE,       FM_NONE,       FM_NONE,       FM_NONE,       FM_NONE,       FM_NONE, FM_NONE  },
/* 11 UNK  */ { FM_NONE, FM_NONE,       FM_NONE,  FM_NONE,         FM_NONE,       FM_NONE,       FM_NONE,       FM_NONE,       FM_NONE,       FM_NONE,       FM_NONE, FM_NONE  }
};

static eDM eMacro_MulResult[DM_ENUMS][DM_ENUMS] = {
           //   NONE     BCD      BOL       DTE           FLT         INT           IVL         IVY           TME         TSP            VCH      UNK
/*  0 NONE */ { DM_NONE, DM_NONE, DM_NONE,  DM_NONE,      DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE, DM_NONE    },
/*  1 BCD  */ { DM_NONE, DM_BCD,  DM_NONE,  DM_NONE,      DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE, DM_UNK     },
/*  2 BOL  */ { DM_NONE, DM_NONE, DM_NONE,  DM_NONE,      DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE, DM_NONE    },
/*  3 DTE  */ { DM_NONE, DM_NONE, DM_NONE,  DM_NONE,      DM_NONE,    DM_NONE/*?*/, DM_NONE,    DM_NONE,    DM_NONE/*?*/, DM_NONE,    DM_NONE, DM_NONE    },
/*  4 FLT  */ { DM_NONE, DM_NONE, DM_NONE,  DM_NONE,      DM_FLT,     DM_NONE,      DM_NONE,    DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE, DM_UNK     },
/*  5 INT  */ { DM_NONE, DM_NONE, DM_NONE,  DM_NONE/*?*/, DM_NONE,    DM_INT,       DM_NONE,    DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE, DM_UNK     },
/*  6 IVL  */ { DM_NONE, DM_NONE, DM_NONE,  DM_NONE,      DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE, DM_UNK     },
/*  7 IVY  */ { DM_NONE, DM_NONE, DM_NONE,  DM_NONE,      DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE, DM_UNK     },
/*  8 TME  */ { DM_NONE, DM_NONE, DM_NONE,  DM_NONE/*?*/, DM_NONE,    DM_NONE/*?*/, DM_NONE,    DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE, DM_NONE    },
/*  9 TSP  */ { DM_NONE, DM_NONE, DM_NONE,  DM_NONE,      DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE, DM_NONE    },
/* 10 VCH  */ { DM_NONE, DM_NONE, DM_NONE,  DM_NONE,      DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE, DM_NONE    },
/* 11 UNK  */ { DM_NONE, DM_UNK,  DM_NONE,  DM_NONE,      DM_UNK,     DM_UNK,       DM_NONE,    DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE, DM_NONE    }
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// NEGATE

static eDM eMacro_NegResult[DM_ENUMS] =
    // NONE     BCD      BOL      DTE       FLT     INT     IVL     IVL     TME           TSP       VCH      UNK
    { DM_NONE, DM_BCD,  DM_NONE, DM_NONE,  DM_FLT, DM_INT, DM_IVL, DM_IVL, DM_NONE/*?*/, DM_NONE,  DM_NONE, DM_NONE };

static eFM eMacro_Neg[DM_ENUMS] =
    // NONE    BCD         BOL       DTE       FLT          INT          IVL         IVY,        TME      TSP       VCH      UNK
    { FM_NONE, FM_NEG_BCD, FM_NONE,  FM_NONE,  FM_NEG_FLT,  FM_NEG_INT,  FM_NONE,    FM_NONE, FM_NONE, FM_NONE,  FM_NONE, FM_NONE    };

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// PSH

static eFM eMacro_Push[DM_ENUMS] =
    // NONE     BCD          BOL           DTE           FLT           INT           IVL          IVY,         TME          TSP           VCH          UNK
    { FM_NONE,  FM_PSH_BCD,  FM_PSH_BOL,   FM_PSH_DTE,   FM_PSH_FLT,   FM_PSH_INT,   FM_PSH_IVL,  FM_PSH_IVY,  FM_PSH_TME,  FM_PSH_TSP,   FM_PSH_VCH,  FM_PSH_UNK    };

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// SUBTRACT

static eFM eMacro_Sub[DM_ENUMS][DM_ENUMS] = {
           //   NONE     BCD            BOL       DTE            FLT            INT            IVL        IVY      TME            TSP            VCH      UNK
/*  0 NONE */ { FM_NONE, FM_NONE,       FM_NONE,  FM_NONE,       FM_NONE,       FM_NONE,       FM_NONE,   FM_NONE, FM_NONE,       FM_NONE,       FM_NONE, FM_NONE  },
/*  1 BCD  */ { FM_NONE, FM_SUB_BCDBCD, FM_NONE,  FM_NONE,       FM_NONE,       FM_NONE,       FM_NONE,   FM_NONE, FM_NONE,       FM_NONE,       FM_NONE, FM_NONE  },
/*  2 BOL  */ { FM_NONE, FM_NONE,       FM_NONE,  FM_NONE,       FM_NONE,       FM_NONE,       FM_NONE,   FM_NONE, FM_NONE,       FM_NONE,       FM_NONE, FM_NONE  },
/*  3 DTE  */ { FM_NONE, FM_NONE,       FM_NONE,  FM_NONE,       FM_NONE,       FM_NONE/*?*/,  FM_NONE,   FM_NONE, FM_NONE/*?*/,  FM_NONE,       FM_NONE, FM_NONE  },
/*  4 FLT  */ { FM_NONE, FM_NONE,       FM_NONE,  FM_NONE,       FM_SUB_FLTFLT, FM_NONE,       FM_NONE,   FM_NONE, FM_NONE,       FM_NONE,       FM_NONE, FM_NONE  },
/*  5 INT  */ { FM_NONE, FM_NONE,       FM_NONE,  FM_NONE/*?*/,  FM_NONE,       FM_SUB_INTINT, FM_NONE,   FM_NONE, FM_NONE,       FM_NONE,       FM_NONE, FM_NONE  },
/*  6 IVL  */ { FM_NONE, FM_NONE,       FM_NONE,  FM_NONE,       FM_NONE,       FM_NONE,       FM_NONE,   FM_NONE, FM_NONE,       FM_NONE,       FM_NONE, FM_NONE  },
/*  6 IVY  */ { FM_NONE, FM_NONE,       FM_NONE,  FM_NONE,       FM_NONE,       FM_NONE,       FM_NONE,   FM_NONE, FM_NONE,       FM_NONE,       FM_NONE, FM_NONE  },
/*  7 TME  */ { FM_NONE, FM_NONE,       FM_NONE,  FM_NONE/*?*/,  FM_NONE,       FM_NONE/*?*/,  FM_NONE,   FM_NONE, FM_NONE,       FM_NONE,       FM_NONE, FM_NONE  },
/*  8 TSP  */ { FM_NONE, FM_NONE,       FM_NONE,  FM_NONE,       FM_NONE,       FM_NONE,       FM_NONE,   FM_NONE, FM_NONE,       FM_NONE,       FM_NONE, FM_NONE  },
/*  9 VCH  */ { FM_NONE, FM_NONE,       FM_NONE,  FM_NONE,       FM_NONE,       FM_NONE,       FM_NONE,   FM_NONE, FM_NONE,       FM_NONE,       FM_NONE, FM_NONE  },
/* 10 UNK  */ { FM_NONE, FM_NONE,       FM_NONE,  FM_NONE,       FM_NONE,       FM_NONE,       FM_NONE,   FM_NONE, FM_NONE,       FM_NONE,       FM_NONE, FM_NONE  }
};

static eDM eMacro_SubResult[DM_ENUMS][DM_ENUMS] = {
           //   NONE     BCD      BOL       DTE           FLT         INT           IVL         IVY           TME          TSP            VCH      UNK
/*  0 NONE */ { DM_NONE, DM_NONE, DM_NONE,  DM_NONE,      DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE, DM_NONE    },
/*  1 BCD  */ { DM_NONE, DM_BCD,  DM_NONE,  DM_NONE,      DM_FLT,     DM_BCD,       DM_NONE,    DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE, DM_NONE     },
/*  2 BOL  */ { DM_NONE, DM_NONE, DM_NONE,  DM_NONE,      DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE, DM_NONE    },
/*  3 DTE  */ { DM_NONE, DM_NONE, DM_NONE,  DM_NONE,      DM_NONE,    DM_NONE/*?*/, DM_NONE,    DM_NONE,    DM_NONE/*?*/, DM_NONE,    DM_NONE, DM_NONE    },
/*  4 FLT  */ { DM_NONE, DM_NONE, DM_NONE,  DM_NONE,      DM_FLT,     DM_FLT,       DM_NONE,    DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE, DM_NONE     },
/*  5 INT  */ { DM_NONE, DM_NONE, DM_NONE,  DM_NONE/*?*/, DM_NONE,     DM_INT,      DM_NONE,    DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE, DM_NONE     },
/*  6 IVL  */ { DM_NONE, DM_NONE, DM_NONE,  DM_NONE,      DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE,    DM_TME,       DM_TSP,     DM_NONE, DM_NONE     },
/*  6 IVY  */ { DM_NONE, DM_NONE, DM_NONE,  DM_NONE,      DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE,    DM_TME,       DM_TSP,     DM_NONE, DM_UNK     },
/*  7 TME  */ { DM_NONE, DM_NONE, DM_NONE,  DM_NONE/*?*/, DM_NONE,    DM_NONE/*?*/, DM_TME,     DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE, DM_NONE    },
/*  8 TSP  */ { DM_NONE, DM_NONE, DM_NONE,  DM_NONE,      DM_NONE,    DM_NONE,      DM_TSP,     DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE, DM_NONE    },
/*  9 VCH  */ { DM_NONE, DM_NONE, DM_NONE,  DM_NONE,      DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE, DM_NONE    },

/* 10 UNK  */ { DM_NONE, DM_NONE, DM_UNK,   DM_NONE,      DM_UNK,     DM_UNK,       DM_UNK,     DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE, DM_NONE    }
};



// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// RELATIONAL OPERATORS
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

static eFM eMacro_Eq[DM_ENUMS][DM_ENUMS] = {
           //   NONE     BCD            BOL           DTE           FLT            INT            IVL           IVY          TME           TSP           VCH           UNK
/*  0 NONE */ { FM_NONE, FM_NONE,       FM_NONE,      FM_NONE,      FM_NONE,       FM_NONE,       FM_NONE,      FM_NONE,     FM_NONE,      FM_NONE,      FM_NONE,      FM_NONE    },
/*  1 BCD  */ { FM_NONE, FM_EQ_BCDBCD,  FM_NONE,      FM_NONE,      FM_NONE,       FM_NONE,       FM_NONE,      FM_NONE,     FM_NONE,      FM_NONE,      FM_NONE,      FM_NONE    },
/*  2 BOL  */ { FM_NONE, FM_NONE,       FM_EQ_BOLBOL, FM_NONE,      FM_NONE,       FM_NONE,       FM_NONE,      FM_NONE,     FM_NONE,      FM_NONE,      FM_NONE,      FM_NONE    },
/*  3 DTE  */ { FM_NONE, FM_NONE,       FM_NONE,      FM_EQ_DTEDTE, FM_NONE,       FM_NONE,       FM_NONE,      FM_NONE,     FM_NONE,      FM_NONE,      FM_NONE,      FM_NONE    },
/*  4 FLT  */ { FM_NONE, FM_NONE,       FM_NONE,      FM_NONE,      FM_EQ_FLTFLT,  FM_NONE,       FM_NONE,      FM_NONE,     FM_NONE,      FM_NONE,      FM_NONE,      FM_NONE    },
/*  5 INT  */ { FM_NONE, FM_NONE,       FM_NONE,      FM_NONE,      FM_NONE,       FM_EQ_INTINT,  FM_NONE,      FM_NONE,     FM_NONE,      FM_NONE,      FM_NONE,      FM_NONE    },
/*  6 IVL  */ { FM_NONE, FM_NONE,       FM_NONE,      FM_NONE,      FM_NONE,       FM_NONE,       FM_EQ_IVLIVL, FM_NONE,     FM_NONE,      FM_NONE,      FM_NONE,      FM_NONE    },
/*  6 IVY  */ { FM_NONE, FM_NONE,       FM_NONE,      FM_NONE,      FM_NONE,       FM_NONE,       FM_NONE,      FM_EQ_IVYIVY,FM_NONE,      FM_NONE,      FM_NONE,      FM_NONE    },
/*  7 TME  */ { FM_NONE, FM_NONE,       FM_NONE,      FM_NONE,      FM_NONE,       FM_NONE,       FM_NONE,      FM_NONE,     FM_EQ_TMETME, FM_NONE,      FM_NONE,      FM_NONE    },
/*  8 TSP  */ { FM_NONE, FM_NONE,       FM_NONE,      FM_NONE,      FM_NONE,       FM_NONE,       FM_NONE,      FM_NONE,     FM_NONE,      FM_EQ_TSPTSP, FM_NONE,      FM_NONE    },
/*  9 VCH  */ { FM_NONE, FM_NONE,       FM_NONE,      FM_NONE,      FM_NONE,       FM_NONE,       FM_NONE,      FM_NONE,     FM_NONE,      FM_NONE,      FM_EQ_VCHVCH, FM_NONE    },
/* 10 UNK  */ { FM_NONE, FM_NONE,       FM_NONE,      FM_NONE,      FM_NONE,       FM_NONE,       FM_NONE,      FM_NONE,     FM_NONE,      FM_NONE,      FM_NONE,      FM_NONE    }
};

static eFM eMacro_Ge[DM_ENUMS][DM_ENUMS] = {
           //   NONE     BCD            BOL           DTE           FLT            INT            IVL           IVY               TME           TSP           VCH           UNK
/*  0 NONE */ { FM_NONE, FM_NONE,       FM_NONE,      FM_NONE,      FM_NONE,       FM_NONE,       FM_NONE,      FM_NONE,          FM_NONE,      FM_NONE,      FM_NONE,      FM_NONE    },
/*  1 BCD  */ { FM_NONE, FM_GE_BCDBCD,  FM_NONE,      FM_NONE,      FM_NONE,       FM_NONE,       FM_NONE,      FM_NONE,          FM_NONE,      FM_NONE,      FM_NONE,      FM_NONE    },
/*  2 BOL  */ { FM_NONE, FM_NONE,       FM_GE_BOLBOL, FM_NONE,      FM_NONE,       FM_NONE,       FM_NONE,      FM_NONE,          FM_NONE,      FM_NONE,      FM_NONE,      FM_NONE    },
/*  3 DTE  */ { FM_NONE, FM_NONE,       FM_NONE,      FM_GE_DTEDTE, FM_NONE,       FM_NONE,       FM_NONE,      FM_NONE,          FM_NONE,      FM_NONE,      FM_NONE,      FM_NONE    },
/*  4 FLT  */ { FM_NONE, FM_NONE,       FM_NONE,      FM_NONE,      FM_GE_FLTFLT,  FM_NONE,       FM_NONE,      FM_NONE,          FM_NONE,      FM_NONE,      FM_NONE,      FM_NONE    },
/*  5 INT  */ { FM_NONE, FM_NONE,       FM_NONE,      FM_NONE,      FM_NONE,       FM_GE_INTINT,  FM_NONE,      FM_NONE,          FM_NONE,      FM_NONE,      FM_NONE,      FM_NONE    },
/*  6 IVL  */ { FM_NONE, FM_NONE,       FM_NONE,      FM_NONE,      FM_NONE,       FM_NONE,       FM_GE_IVLIVL, FM_NONE,          FM_NONE,      FM_NONE,      FM_NONE,      FM_NONE    },
/*  6 IVY  */ { FM_NONE, FM_NONE,       FM_NONE,      FM_NONE,      FM_NONE,       FM_NONE,       FM_NONE,      FM_GE_IVYIVY,     FM_NONE,      FM_NONE,      FM_NONE,      FM_NONE    },
/*  7 TME  */ { FM_NONE, FM_NONE,       FM_NONE,      FM_NONE,      FM_NONE,       FM_NONE,       FM_NONE,      FM_NONE,          FM_GE_TMETME, FM_NONE,      FM_NONE,      FM_NONE    },
/*  8 TSP  */ { FM_NONE, FM_NONE,       FM_NONE,      FM_NONE,      FM_NONE,       FM_NONE,       FM_NONE,      FM_NONE,          FM_NONE,      FM_GE_TSPTSP, FM_NONE,      FM_NONE    },
/*  9 VCH  */ { FM_NONE, FM_NONE,       FM_NONE,      FM_NONE,      FM_NONE,       FM_NONE,       FM_NONE,      FM_NONE,          FM_NONE,      FM_NONE,      FM_GE_VCHVCH, FM_NONE    },
/* 10 UNK  */ { FM_NONE, FM_NONE,       FM_NONE,      FM_NONE,      FM_NONE,       FM_NONE,       FM_NONE,      FM_NONE,          FM_NONE,      FM_NONE,      FM_NONE,      FM_NONE    }
};

static eFM eMacro_Gt[DM_ENUMS][DM_ENUMS] = {
           //   NONE     BCD            BOL           DTE           FLT            INT            IVL           IVY           TME           TSP           VCH           UNK
/*  0 NONE */ { FM_NONE, FM_NONE,       FM_NONE,      FM_NONE,      FM_NONE,       FM_NONE,       FM_NONE,      FM_NONE,      FM_NONE,      FM_NONE,      FM_NONE,      FM_NONE    },
/*  1 BCD  */ { FM_NONE, FM_GT_BCDBCD,  FM_NONE,      FM_NONE,      FM_NONE,       FM_NONE,       FM_NONE,      FM_NONE,      FM_NONE,      FM_NONE,      FM_NONE,      FM_NONE    },
/*  2 BOL  */ { FM_NONE, FM_NONE,       FM_GT_BOLBOL, FM_NONE,      FM_NONE,       FM_NONE,       FM_NONE,      FM_NONE,      FM_NONE,      FM_NONE,      FM_NONE,      FM_NONE    },
/*  3 DTE  */ { FM_NONE, FM_NONE,       FM_NONE,      FM_GT_DTEDTE, FM_NONE,       FM_NONE,       FM_NONE,      FM_NONE,      FM_NONE,      FM_NONE,      FM_NONE,      FM_NONE    },
/*  4 FLT  */ { FM_NONE, FM_NONE,       FM_NONE,      FM_NONE,      FM_GT_FLTFLT,  FM_NONE,       FM_NONE,      FM_NONE,      FM_NONE,      FM_NONE,      FM_NONE,      FM_NONE    },
/*  5 INT  */ { FM_NONE, FM_NONE,       FM_NONE,      FM_NONE,      FM_NONE,       FM_GT_INTINT,  FM_NONE,      FM_NONE,      FM_NONE,      FM_NONE,      FM_NONE,      FM_NONE    },
/*  6 IVL  */ { FM_NONE, FM_NONE,       FM_NONE,      FM_NONE,      FM_NONE,       FM_NONE,       FM_GT_IVLIVL, FM_NONE,      FM_NONE,      FM_NONE,      FM_NONE,      FM_NONE    },
/*  6 IVY  */ { FM_NONE, FM_NONE,       FM_NONE,      FM_NONE,      FM_NONE,       FM_NONE,       FM_NONE,      FM_GT_IVYIVY, FM_NONE,      FM_NONE,      FM_NONE,      FM_NONE    },
/*  7 TME  */ { FM_NONE, FM_NONE,       FM_NONE,      FM_NONE,      FM_NONE,       FM_NONE,       FM_NONE,      FM_NONE,      FM_GT_TMETME, FM_NONE,      FM_NONE,      FM_NONE    },
/*  8 TSP  */ { FM_NONE, FM_NONE,       FM_NONE,      FM_NONE,      FM_NONE,       FM_NONE,       FM_NONE,      FM_NONE,      FM_NONE,      FM_GT_TSPTSP, FM_NONE,      FM_NONE    },
/*  9 VCH  */ { FM_NONE, FM_NONE,       FM_NONE,      FM_NONE,      FM_NONE,       FM_NONE,       FM_NONE,      FM_NONE,      FM_NONE,      FM_NONE,      FM_GT_VCHVCH, FM_NONE    },
/* 10 UNK  */ { FM_NONE, FM_NONE,       FM_NONE,      FM_NONE,      FM_NONE,       FM_NONE,       FM_NONE,      FM_NONE,      FM_NONE,      FM_NONE,      FM_NONE,      FM_NONE    }
};

static eFM eMacro_Le[DM_ENUMS][DM_ENUMS] = {
           //   NONE     BCD            BOL           DTE           FLT            INT            IVL           IVY           TME           TSP           VCH           UNK
/*  0 NONE */ { FM_NONE, FM_NONE,       FM_NONE,      FM_NONE,      FM_NONE,       FM_NONE,       FM_NONE,      FM_NONE,      FM_NONE,      FM_NONE,      FM_NONE,      FM_NONE   },
/*  1 BCD  */ { FM_NONE, FM_LE_BCDBCD,  FM_NONE,      FM_NONE,      FM_NONE,       FM_NONE,       FM_NONE,      FM_NONE,      FM_NONE,      FM_NONE,      FM_NONE,      FM_NONE   },
/*  2 BOL  */ { FM_NONE, FM_NONE,       FM_LE_BOLBOL, FM_NONE,      FM_NONE,       FM_NONE,       FM_NONE,      FM_NONE,      FM_NONE,      FM_NONE,      FM_NONE,      FM_NONE   },
/*  3 DTE  */ { FM_NONE, FM_NONE,       FM_NONE,      FM_LE_DTEDTE, FM_NONE,       FM_NONE,       FM_NONE,      FM_NONE,      FM_NONE,      FM_NONE,      FM_NONE,      FM_NONE   },
/*  4 FLT  */ { FM_NONE, FM_NONE,       FM_NONE,      FM_NONE,      FM_LE_FLTFLT,  FM_NONE,       FM_NONE,      FM_NONE,      FM_NONE,      FM_NONE,      FM_NONE,      FM_NONE   },
/*  5 INT  */ { FM_NONE, FM_NONE,       FM_NONE,      FM_NONE,      FM_NONE,       FM_LE_INTINT,  FM_NONE,      FM_NONE,      FM_NONE,      FM_NONE,      FM_NONE,      FM_NONE   },
/*  6 IVL  */ { FM_NONE, FM_NONE,       FM_NONE,      FM_NONE,      FM_NONE,       FM_NONE,       FM_LE_IVLIVL, FM_NONE,      FM_NONE,      FM_NONE,      FM_NONE,      FM_NONE   },
/*  6 IVY  */ { FM_NONE, FM_NONE,       FM_NONE,      FM_NONE,      FM_NONE,       FM_NONE,       FM_NONE,      FM_LE_IVYIVY, FM_NONE,      FM_NONE,      FM_NONE,      FM_NONE   },
/*  7 TME  */ { FM_NONE, FM_NONE,       FM_NONE,      FM_NONE,      FM_NONE,       FM_NONE,       FM_NONE,      FM_NONE,      FM_LE_TMETME, FM_NONE,      FM_NONE,      FM_NONE   },
/*  8 TSP  */ { FM_NONE, FM_NONE,       FM_NONE,      FM_NONE,      FM_NONE,       FM_NONE,       FM_NONE,      FM_NONE,      FM_NONE,      FM_LE_TSPTSP, FM_NONE,      FM_NONE   },
/*  9 VCH  */ { FM_NONE, FM_NONE,       FM_NONE,      FM_NONE,      FM_NONE,       FM_NONE,       FM_NONE,      FM_NONE,      FM_NONE,      FM_NONE,      FM_LE_VCHVCH, FM_NONE   },
/* 10 UNK  */ { FM_NONE, FM_NONE,       FM_NONE,      FM_NONE,      FM_NONE,       FM_NONE,       FM_NONE,      FM_NONE,      FM_NONE,      FM_NONE,      FM_NONE,      FM_NONE   }
};

static eFM eMacro_Lt[DM_ENUMS][DM_ENUMS] = {
          //   NONE     BCD            BOL           DTE           FLT            INT            IVL           IVY           TME           TSP            VCH           UNK
/*  0 NONE */ { FM_NONE, FM_NONE,       FM_NONE,      FM_NONE,      FM_NONE,       FM_NONE,       FM_NONE,      FM_NONE,      FM_NONE,      FM_NONE,      FM_NONE,      FM_NONE    },
/*  1 BCD  */ { FM_NONE, FM_LT_BCDBCD,  FM_NONE,      FM_NONE,      FM_NONE,       FM_NONE,       FM_NONE,      FM_NONE,      FM_NONE,      FM_NONE,      FM_NONE,      FM_NONE    },
/*  2 BOL  */ { FM_NONE, FM_NONE,       FM_LT_BOLBOL, FM_NONE,      FM_NONE,       FM_NONE,       FM_NONE,      FM_NONE,      FM_NONE,      FM_NONE,      FM_NONE,      FM_NONE    },
/*  3 DTE  */ { FM_NONE, FM_NONE,       FM_NONE,      FM_LT_DTEDTE, FM_NONE,       FM_NONE,       FM_NONE,      FM_NONE,      FM_NONE,      FM_NONE,      FM_NONE,      FM_NONE    },
/*  4 FLT  */ { FM_NONE, FM_NONE,       FM_NONE,      FM_NONE,      FM_LT_FLTFLT,  FM_NONE,       FM_NONE,       FM_NONE,      FM_NONE,      FM_NONE,      FM_NONE,      FM_NONE    },
/*  5 INT  */ { FM_NONE, FM_NONE,       FM_NONE,      FM_NONE,      FM_NONE,       FM_LT_INTINT,  FM_NONE,      FM_NONE,      FM_NONE,      FM_NONE,      FM_NONE,      FM_NONE    },
/*  6 IVL  */ { FM_NONE, FM_NONE,       FM_NONE,      FM_NONE,      FM_NONE,       FM_NONE,       FM_LT_IVLIVL, FM_NONE,      FM_NONE,      FM_NONE,      FM_NONE,      FM_NONE    },
/*  6 IVY  */ { FM_NONE, FM_NONE,       FM_NONE,      FM_NONE,      FM_NONE,       FM_NONE,       FM_NONE,      FM_LT_IVYIVY, FM_NONE,      FM_NONE,      FM_NONE,      FM_NONE    },
/*  7 TME  */ { FM_NONE, FM_NONE,       FM_NONE,      FM_NONE,      FM_NONE,       FM_NONE,       FM_NONE,      FM_NONE,      FM_LT_TMETME, FM_NONE,      FM_NONE,      FM_NONE    },
/*  8 TSP  */ { FM_NONE, FM_NONE,       FM_NONE,      FM_NONE,      FM_NONE,       FM_NONE,       FM_NONE,      FM_NONE,      FM_NONE,      FM_LT_TSPTSP, FM_NONE,      FM_NONE    },
/*  9 VCH  */ { FM_NONE, FM_NONE,       FM_NONE,      FM_NONE,      FM_NONE,       FM_NONE,       FM_NONE,      FM_NONE,      FM_NONE,      FM_NONE,      FM_LT_VCHVCH,      FM_NONE    },
/* 10 UNK  */ { FM_NONE, FM_NONE,       FM_NONE,      FM_NONE,      FM_NONE,       FM_NONE,       FM_NONE,      FM_NONE,      FM_NONE,      FM_NONE,      FM_NONE,      FM_NONE    }
};

static eFM eMacro_Ne[DM_ENUMS][DM_ENUMS] = {
           //   NONE     BCD            BOL           DTE           FLT            INT            IVL           IVY           TME           TSP           VCH           UNK
/*  0 NONE */ { FM_NONE, FM_NONE,       FM_NONE,      FM_NONE,      FM_NONE,       FM_NONE,       FM_NONE,      FM_NONE,      FM_NONE,      FM_NONE,      FM_NONE,      FM_NONE   },
/*  1 BCD  */ { FM_NONE, FM_NE_BCDBCD,  FM_NONE,      FM_NONE,      FM_NONE,       FM_NONE,       FM_NONE,      FM_NONE,      FM_NONE,      FM_NONE,      FM_NONE,      FM_NONE   },
/*  2 BOL  */ { FM_NONE, FM_NONE,       FM_NE_BOLBOL, FM_NONE,      FM_NONE,       FM_NONE,       FM_NONE,      FM_NONE,      FM_NONE,      FM_NONE,      FM_NONE,      FM_NONE   },
/*  3 DTE  */ { FM_NONE, FM_NONE,       FM_NONE,      FM_NE_DTEDTE, FM_NONE,       FM_NONE,       FM_NONE,      FM_NONE,      FM_NONE,      FM_NONE,      FM_NONE,      FM_NONE   },
/*  4 FLT  */ { FM_NONE, FM_NONE,       FM_NONE,      FM_NONE,      FM_NE_FLTFLT,  FM_NONE,       FM_NONE,      FM_NONE,      FM_NONE,      FM_NONE,      FM_NONE,      FM_NONE   },
/*  5 INT  */ { FM_NONE, FM_NONE,       FM_NONE,      FM_NONE,      FM_NONE,       FM_NE_INTINT,  FM_NONE,      FM_NONE,      FM_NONE,      FM_NONE,      FM_NONE,      FM_NONE   },
/*  6 IVL  */ { FM_NONE, FM_NONE,       FM_NONE,      FM_NONE,      FM_NONE,       FM_NONE,       FM_NE_IVLIVL, FM_NONE,      FM_NONE,      FM_NONE,      FM_NONE,      FM_NONE   },
/*  6 IVY  */ { FM_NONE, FM_NONE,       FM_NONE,      FM_NONE,      FM_NONE,       FM_NONE,       FM_NONE,      FM_NE_IVYIVY, FM_NONE,      FM_NONE,      FM_NONE,      FM_NONE   },
/*  7 TME  */ { FM_NONE, FM_NONE,       FM_NONE,      FM_NONE,      FM_NONE,       FM_NONE,       FM_NONE,      FM_NONE,      FM_NE_TMETME, FM_NONE,      FM_NONE,      FM_NONE   },
/*  8 TSP  */ { FM_NONE, FM_NONE,       FM_NONE,      FM_NONE,      FM_NONE,       FM_NONE,       FM_NONE,      FM_NONE,      FM_NONE,      FM_NE_TSPTSP, FM_NONE,      FM_NONE   },
/*  9 VCH  */ { FM_NONE, FM_NONE,       FM_NONE,      FM_NONE,      FM_NONE,       FM_NONE,       FM_NONE,      FM_NONE,      FM_NONE,      FM_NONE,      FM_NE_VCHVCH, FM_NONE   },
/* 10 UNK  */ { FM_NONE, FM_NONE,       FM_NONE,      FM_NONE,      FM_NONE,       FM_NONE,       FM_NONE,      FM_NONE,      FM_NONE,      FM_NONE,      FM_NONE,      FM_NONE   }
};

static eDM eMacro_CmpResult[DM_ENUMS][DM_ENUMS] = {
           //   NONE     BCD       BOL       DTE           FLT         INT           IVL         IVY,      TME          TSP            VCH      UNK
/*  0 NONE */ { DM_NONE, DM_NONE,  DM_NONE,  DM_NONE,      DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE,  DM_NONE,      DM_NONE,    DM_NONE, DM_NONE    },
/*  1 BCD  */ { DM_NONE, DM_BOL,   DM_NONE,  DM_NONE,      DM_BOL,     DM_BOL,       DM_NONE,    DM_NONE,  DM_NONE,      DM_NONE,    DM_NONE, DM_BOL    },
/*  2 BOL  */ { DM_NONE, DM_NONE,  DM_NONE,  DM_NONE,      DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE,  DM_NONE,      DM_NONE,    DM_NONE, DM_BOL     },
/*  3 DTE  */ { DM_NONE, DM_NONE,  DM_NONE,  DM_BOL,       DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE,  DM_NONE,      DM_BOL,     DM_NONE, DM_BOL    },
/*  4 FLT  */ { DM_NONE, DM_BOL,   DM_NONE,  DM_NONE,      DM_BOL,     DM_BOL,       DM_NONE,    DM_NONE,  DM_NONE,      DM_NONE,    DM_NONE, DM_BOL    },
/*  5 INT  */ { DM_NONE, DM_BOL,   DM_NONE,  DM_NONE,      DM_BOL,     DM_BOL,       DM_NONE,    DM_NONE,  DM_NONE,      DM_NONE,    DM_NONE, DM_BOL    },
/*  6 IVL  */ { DM_NONE, DM_NONE,  DM_NONE,  DM_NONE,      DM_NONE,    DM_NONE,      DM_BOL,     DM_NONE,  DM_NONE,      DM_NONE,    DM_NONE, DM_BOL    },
/*  6 IVY  */ { DM_NONE, DM_NONE,  DM_NONE,  DM_NONE,      DM_NONE,    DM_NONE,      DM_NONE,    DM_BOL,   DM_NONE,      DM_NONE,    DM_NONE, DM_BOL    },
/*  7 TME  */ { DM_NONE, DM_NONE,  DM_NONE,  DM_NONE,      DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE,  DM_BOL,       DM_NONE,    DM_NONE, DM_BOL    },
/*  8 TSP  */ { DM_NONE, DM_NONE,  DM_NONE,  DM_BOL,       DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE,  DM_NONE,      DM_BOL,     DM_NONE, DM_BOL    },
/*  9 VCH  */ { DM_NONE, DM_NONE,  DM_NONE,  DM_NONE,      DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE,  DM_NONE,      DM_NONE,    DM_BOL,  DM_BOL    },
/* 10 UNK  */ { DM_NONE, DM_BOL,   DM_BOL,   DM_BOL,       DM_BOL,     DM_BOL,       DM_BOL,     DM_BOL,   DM_BOL,       DM_BOL,     DM_BOL,  DM_BOL    }
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
