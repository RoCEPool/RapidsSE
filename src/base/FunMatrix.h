/*============================================================================*/
/* FunMatrix.h -- Function Matrix                                             */
/*============================================================================*/
#pragma once

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// CONVERSION OPERATORS
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

static eFM eMacro_Cast[DM_ENUMS][DM_ENUMS] = {
           //   NONE     BCD           BOL          DTE           FLT           INT             IVY             IVL            TME           TSP           VCH           UNK
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
/* 10 VCH  */ { FM_NONE, FM_VCH_TOBCD, FM_UNK_TOBOL,FM_VCH_TODTE, FM_VCH_TOFLT, FM_VCH_TOINT,   FM_VCH_TOIVL,   FM_VCH_TOIVY,  FM_VCH_TOTME, FM_VCH_TOTSP, FM_NONE,      FM_VCH_TOUNK },
/* 11 UNK  */ { FM_NONE, FM_UNK_TOBCD, FM_UNK_TODTE, FM_UNK_TOFLT, FM_UNK_TOINT,   FM_UNK_TOIVL,   FM_UNK_TOIVY,  FM_UNK_TOTME, FM_UNK_TOTSP, FM_UNK_TOVCH,FM_NONE ,     FM_NONE      }
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
           //   NONE     BCD            BOL       DTE            FLT            INT            IVL            IVY      TME            TSP            VCH      UNK
/*  0 NONE */ { FM_NONE, FM_NONE,       FM_NONE,  FM_NONE,       FM_NONE,       FM_NONE,       FM_NONE,       FM_NONE, FM_NONE,       FM_NONE,       FM_NONE, FM_NONE        },
/*  1 BCD  */ { FM_NONE, FM_ADD_BCDBCD, FM_NONE,  FM_NONE,       FM_ADD_BCDFLT, FM_ADD_BCDINT, FM_NONE,       FM_NONE, FM_NONE,       FM_NONE,       FM_NONE, FM_ADD_XYZUNK  },
/*  2 BOL  */ { FM_NONE, FM_NONE,       FM_NONE,  FM_NONE,       FM_NONE,       FM_NONE,       FM_NONE,       FM_NONE, FM_NONE,       FM_NONE,       FM_NONE, FM_NONE        },
/*  3 DTE  */ { FM_NONE, FM_NONE,       FM_NONE,  FM_NONE,       FM_NONE,       FM_NONE/*?*/,  FM_ADD_DTEIVL, FM_NONE, FM_NONE/*?*/,  FM_NONE,       FM_NONE, FM_NONE        },
/*  4 FLT  */ { FM_NONE, FM_ADD_FLTBCD, FM_NONE,  FM_NONE,       FM_ADD_FLTFLT, FM_ADD_FLTINT, FM_NONE,       FM_NONE, FM_NONE,       FM_NONE,       FM_NONE, FM_ADD_XYZUNK  },
/*  5 INT  */ { FM_NONE, FM_ADD_INTBCD, FM_NONE,  FM_NONE/*?*/,  FM_ADD_INTFLT, FM_ADD_INTINT, FM_NONE,       FM_NONE, FM_NONE,       FM_NONE,       FM_NONE, FM_ADD_XYZUNK  },
/*  6 IVL  */ { FM_NONE, FM_NONE,       FM_NONE,  FM_ADD_IVLDTE, FM_NONE,       FM_NONE,       FM_ADD_IVLIVL, FM_NONE, FM_ADD_IVLTME, FM_ADD_IVLTSP, FM_NONE, FM_ADD_XYZUNK  },
/*  7 IVY  */ { FM_NONE, FM_NONE,       FM_NONE,  FM_ADD_IVLDTE, FM_NONE,       FM_NONE,       FM_ADD_IVLIVL, FM_NONE, FM_ADD_IVLTME, FM_ADD_IVLTSP, FM_NONE, FM_ADD_XYZUNK  },
/*  8 TME  */ { FM_NONE, FM_NONE,       FM_NONE,  FM_NONE/*?*/,  FM_NONE,       FM_NONE/*?*/,  FM_ADD_TMEIVL, FM_NONE, FM_NONE,       FM_NONE,       FM_NONE, FM_NONE        },
/*  9 TSP  */ { FM_NONE, FM_NONE,       FM_NONE,  FM_NONE,       FM_NONE,       FM_NONE,       FM_ADD_TSPIVL, FM_NONE, FM_NONE,       FM_NONE,       FM_NONE, FM_NONE        },
/* 10 VCH  */ { FM_NONE, FM_NONE,       FM_NONE,  FM_NONE,       FM_NONE,       FM_NONE,       FM_NONE,       FM_NONE, FM_NONE,       FM_NONE,       FM_NONE, FM_NONE        },
/* 11 UNK  */ { FM_NONE, FM_ADD_UNKXYZ, FM_NONE,  FM_NONE,       FM_ADD_UNKXYZ, FM_ADD_UNKXYZ, FM_ADD_UNKXYZ, FM_NONE, FM_NONE,       FM_NONE,       FM_NONE, FM_NONE        }
};

static eDM eMacro_AddResult[DM_ENUMS][DM_ENUMS] = {
           //   NONE     BCD       BOL       DTE           FLT         INT           IVL         IVY           TME          TSP            VCH      UNK
/*  0 NONE */ { DM_NONE, DM_NONE,  DM_NONE,  DM_NONE,      DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE, DM_NONE    },
/*  1 BCD  */ { DM_NONE, DM_BCD,   DM_NONE,  DM_NONE,      DM_FLT,     DM_BCD,       DM_NONE,    DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE, DM_UNK     },
/*  2 BOL  */ { DM_NONE, DM_NONE,  DM_NONE,  DM_NONE,      DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE, DM_NONE    },
/*  3 DTE  */ { DM_NONE, DM_NONE,  DM_NONE,  DM_NONE,      DM_NONE,    DM_NONE/*?*/, DM_DTE,     DM_NONE,    DM_NONE/*?*/, DM_NONE,    DM_NONE, DM_NONE    },
/*  4 FLT  */ { DM_NONE, DM_FLT,   DM_NONE,  DM_NONE,      DM_FLT,     DM_FLT,       DM_NONE,    DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE, DM_UNK     },
/*  5 INT  */ { DM_NONE, DM_BCD,   DM_NONE,  DM_NONE/*?*/, DM_FLT,     DM_INT,       DM_NONE,    DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE, DM_UNK     },
/*  6 IVL  */ { DM_NONE, DM_NONE,  DM_NONE,  DM_DTE,       DM_NONE,    DM_NONE,      DM_IVL,     DM_NONE,    DM_TME,       DM_TSP,     DM_NONE, DM_UNK     },
/*  7 IVY  */ { DM_NONE, DM_NONE,  DM_NONE,  DM_DTE,       DM_NONE,    DM_NONE,      DM_IVL,     DM_NONE,    DM_TME,       DM_TSP,     DM_NONE, DM_UNK     },
/*  8 TME  */ { DM_NONE, DM_NONE,  DM_NONE,  DM_NONE/*?*/, DM_NONE,    DM_NONE/*?*/, DM_TME,     DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE, DM_NONE    },
/*  9 TSP  */ { DM_NONE, DM_NONE,  DM_NONE,  DM_NONE,      DM_NONE,    DM_NONE,      DM_TSP,     DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE, DM_NONE    },
/* 10 VCH  */ { DM_NONE, DM_NONE,  DM_NONE,  DM_NONE,      DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE, DM_NONE    },
/* 11 UNK  */ { DM_NONE, DM_UNK,   DM_UNK,   DM_NONE,      DM_UNK,     DM_UNK,       DM_UNK,     DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE, DM_NONE    }
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// DIVIDE

static eFM eMacro_Div[DM_ENUMS][DM_ENUMS] = {
           //   NONE     BCD            BOL       DTE            FLT            INT            IVL            IVY      TME            TSP            VCH      UNK
/*  0 NONE */ { FM_NONE, FM_NONE,       FM_NONE,  FM_NONE,       FM_NONE,       FM_NONE,       FM_NONE,       FM_NONE, FM_NONE,       FM_NONE,       FM_NONE, FM_NONE    },
/*  1 BCD  */ { FM_NONE, FM_DIV_BCDBCD, FM_NONE,  FM_NONE,       FM_DIV_BCDFLT, FM_DIV_BCDINT, FM_NONE,       FM_NONE, FM_NONE,       FM_NONE,       FM_NONE, FM_DIV_XYZUNK  },
/*  2 BOL  */ { FM_NONE, FM_NONE,       FM_NONE,  FM_NONE,       FM_NONE,       FM_NONE,       FM_NONE,       FM_NONE, FM_NONE,       FM_NONE,       FM_NONE, FM_NONE        },
/*  3 DTE  */ { FM_NONE, FM_NONE,       FM_NONE,  FM_NONE,       FM_NONE,       FM_NONE/*?*/,  FM_DIV_DTEIVL, FM_NONE, FM_NONE/*?*/,  FM_NONE,       FM_NONE, FM_NONE        },
/*  4 FLT  */ { FM_NONE, FM_DIV_FLTBCD, FM_NONE,  FM_NONE,       FM_DIV_FLTFLT, FM_DIV_FLTINT, FM_NONE,       FM_NONE, FM_NONE,       FM_NONE,       FM_NONE, FM_DIV_XYZUNK  },
/*  5 INT  */ { FM_NONE, FM_DIV_INTBCD, FM_NONE,  FM_NONE/*?*/,  FM_DIV_INTFLT, FM_DIV_INTINT, FM_NONE,       FM_NONE, FM_NONE,       FM_NONE,       FM_NONE, FM_DIV_XYZUNK  },
/*  6 IVL  */ { FM_NONE, FM_NONE,       FM_NONE,  FM_DIV_IVLDTE, FM_NONE,       FM_NONE,       FM_DIV_IVLIVL, FM_NONE, FM_DIV_IVLTME, FM_DIV_IVLTSP, FM_NONE, FM_DIV_XYZUNK  },
/*  7 IVY  */ { FM_NONE, FM_NONE,       FM_NONE,  FM_DIV_IVLDTE, FM_NONE,       FM_NONE,       FM_DIV_IVLIVL, FM_NONE, FM_DIV_IVLTME, FM_DIV_IVLTSP, FM_NONE, FM_DIV_XYZUNK  },
/*  8 TME  */ { FM_NONE, FM_NONE,       FM_NONE,  FM_NONE/*?*/,  FM_NONE,       FM_NONE/*?*/,  FM_DIV_TMEIVL, FM_NONE, FM_NONE,       FM_NONE,       FM_NONE, FM_NONE        },
/*  9 TSP  */ { FM_NONE, FM_NONE,       FM_NONE,  FM_NONE,       FM_NONE,       FM_NONE,       FM_DIV_TSPIVL, FM_NONE, FM_NONE,       FM_NONE,       FM_NONE, FM_NONE        },
/* 10 VCH  */ { FM_NONE, FM_NONE,       FM_NONE,  FM_NONE,       FM_NONE,       FM_NONE,       FM_NONE,       FM_NONE, FM_NONE,       FM_NONE,       FM_NONE, FM_NONE        },
/* 11 UNK  */ { FM_NONE, FM_DIV_UNKXYZ, FM_NONE,  FM_NONE,       FM_DIV_UNKXYZ, FM_DIV_UNKXYZ, FM_DIV_UNKXYZ, FM_NONE, FM_NONE,       FM_NONE,       FM_NONE, FM_NONE        }
};

static eDM eMacro_DivResult[DM_ENUMS][DM_ENUMS] = {
           //   NONE     BCD       BOL      DTE           FLT         INT           IVL         IVY           TMT          TSP            VCH      UNK
/*  0 NONE */ { DM_NONE, DM_NONE,  DM_NONE, DM_NONE,      DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE, DM_NONE    },
/*  1 BCD  */ { DM_NONE, DM_BCD,   DM_NONE, DM_NONE,      DM_FLT,     DM_BCD,       DM_NONE,    DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE, DM_UNK     },
/*  2 BOL  */ { DM_NONE, DM_NONE,  DM_NONE, DM_NONE,      DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE, DM_NONE    },
/*  3 DTE  */ { DM_NONE, DM_NONE,  DM_NONE, DM_NONE,      DM_NONE,    DM_NONE/*?*/, DM_DTE,     DM_NONE,    DM_NONE/*?*/, DM_NONE,    DM_NONE, DM_NONE    },
/*  4 FLT  */ { DM_NONE, DM_FLT,   DM_NONE, DM_NONE,      DM_FLT,     DM_FLT,       DM_NONE,    DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE, DM_UNK     },
/*  5 INT  */ { DM_NONE, DM_BCD,   DM_NONE, DM_NONE/*?*/, DM_FLT,     DM_INT,       DM_NONE,    DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE, DM_UNK     },
/*  6 IVL  */ { DM_NONE, DM_NONE,  DM_NONE, DM_DTE,       DM_NONE,    DM_NONE,      DM_IVL,     DM_NONE,    DM_TME,       DM_TSP,     DM_NONE, DM_UNK     },
/*  7 IVY  */ { DM_NONE, DM_NONE,  DM_NONE, DM_DTE,       DM_NONE,    DM_NONE,      DM_IVL,     DM_NONE,    DM_TME,       DM_TSP,     DM_NONE, DM_UNK     },
/*  8 TME  */ { DM_NONE, DM_NONE,  DM_NONE, DM_NONE/*?*/, DM_NONE,    DM_NONE/*?*/, DM_TME,     DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE, DM_NONE    },
/*  9 TSP  */ { DM_NONE, DM_NONE,  DM_NONE, DM_NONE,      DM_NONE,    DM_NONE,      DM_TSP,     DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE, DM_NONE    },
/* 10 VCH  */ { DM_NONE, DM_NONE,  DM_NONE, DM_NONE,      DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE, DM_NONE    },
/* 11 UNK  */ { DM_NONE, DM_UNK,   DM_NONE, DM_NONE,      DM_UNK,     DM_UNK,       DM_UNK,     DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE, DM_NONE    }
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
/*  5 INT  */ { FM_NONE, FM_NONE, FM_NONE, FM_NONE,  FM_NONE, FM_MOD_INTINT,FM_NONE,      FM_NONE,      FM_NONE,      FM_NONE,      FM_NONE, FM_MOD_XYZUNK  },
/*  6 IVL  */ { FM_NONE, FM_NONE, FM_NONE, FM_NONE,  FM_NONE, FM_NONE,      FM_NONE,      FM_NONE,      FM_NONE,      FM_NONE,      FM_NONE, FM_NONE  },
/*  7 IVY  */ { FM_NONE, FM_NONE, FM_NONE, FM_NONE,  FM_NONE, FM_NONE,      FM_NONE,      FM_NONE,      FM_NONE,      FM_NONE,      FM_NONE, FM_NONE  },
/*  8 TME  */ { FM_NONE, FM_NONE, FM_NONE, FM_NONE,  FM_NONE, FM_NONE/*?*/, FM_NONE,      FM_NONE,      FM_NONE,      FM_NONE,      FM_NONE, FM_NONE  },
/*  9 TSP  */ { FM_NONE, FM_NONE, FM_NONE, FM_NONE,  FM_NONE, FM_NONE,      FM_NONE,      FM_NONE,      FM_NONE,      FM_NONE,      FM_NONE, FM_NONE  },
/* 10 VCH  */ { FM_NONE, FM_NONE, FM_NONE, FM_NONE,  FM_NONE, FM_NONE,      FM_NONE,      FM_NONE,      FM_NONE,      FM_NONE,      FM_NONE, FM_NONE  },
/* 11 UNK  */ { FM_NONE, FM_NONE, FM_NONE, FM_NONE,  FM_NONE, FM_MOD_UNKXYZ,FM_NONE,      FM_NONE,      FM_NONE,      FM_NONE,      FM_NONE, FM_NONE  }
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// MULTIPLY

static eFM eMacro_Mul[DM_ENUMS][DM_ENUMS] = {
           //   NONE     BCD            BOL       DTE            FLT            INT            IVL            IVY      TME            TSP            VCH      UNK
/*  0 NONE */ { FM_NONE, FM_NONE,       FM_NONE,  FM_NONE,       FM_NONE,       FM_NONE,       FM_NONE,       FM_NONE, FM_NONE,       FM_NONE,       FM_NONE, FM_NONE    },
/*  1 BCD  */ { FM_NONE, FM_MUL_BCDBCD, FM_NONE,  FM_NONE,       FM_MUL_BCDFLT, FM_MUL_BCDINT, FM_NONE,       FM_NONE, FM_NONE,       FM_NONE,       FM_NONE, FM_MUL_XYZUNK  },
/*  2 BOL  */ { FM_NONE, FM_NONE,       FM_NONE,  FM_NONE,       FM_NONE,       FM_NONE,       FM_NONE,       FM_NONE, FM_NONE,       FM_NONE,       FM_NONE, FM_NONE        },
/*  3 DTE  */ { FM_NONE, FM_NONE,       FM_NONE,  FM_NONE,       FM_NONE,       FM_NONE/*?*/,  FM_MUL_DTEIVL, FM_NONE, FM_NONE/*?*/,  FM_NONE,       FM_NONE, FM_NONE        },
/*  4 FLT  */ { FM_NONE, FM_MUL_FLTBCD, FM_NONE,  FM_NONE,       FM_MUL_FLTFLT, FM_MUL_FLTINT, FM_NONE,       FM_NONE, FM_NONE,       FM_NONE,       FM_NONE, FM_MUL_XYZUNK  },
/*  5 INT  */ { FM_NONE, FM_MUL_INTBCD, FM_NONE,  FM_NONE/*?*/,  FM_MUL_INTFLT, FM_MUL_INTINT, FM_NONE,       FM_NONE, FM_NONE,       FM_NONE,       FM_NONE, FM_MUL_XYZUNK  },
/*  6 IVL  */ { FM_NONE, FM_NONE,       FM_NONE,  FM_MUL_IVLDTE, FM_NONE,       FM_NONE,       FM_MUL_IVLIVL, FM_NONE, FM_MUL_IVLTME, FM_MUL_IVLTSP, FM_NONE, FM_MUL_XYZUNK  },
/*  7 IVY  */ { FM_NONE, FM_NONE,       FM_NONE,  FM_MUL_IVLDTE, FM_NONE,       FM_NONE,       FM_MUL_IVLIVL, FM_NONE, FM_MUL_IVLTME, FM_MUL_IVLTSP, FM_NONE, FM_MUL_XYZUNK  },
/*  8 TME  */ { FM_NONE, FM_NONE,       FM_NONE,  FM_NONE/*?*/,  FM_NONE,       FM_NONE/*?*/,  FM_MUL_TMEIVL, FM_NONE, FM_NONE,       FM_NONE,       FM_NONE, FM_NONE        },
/*  9 TSP  */ { FM_NONE, FM_NONE,       FM_NONE,  FM_NONE,       FM_NONE,       FM_NONE,       FM_MUL_TSPIVL, FM_NONE, FM_NONE,       FM_NONE,       FM_NONE, FM_NONE        },
/* 10 VCH  */ { FM_NONE, FM_NONE,       FM_NONE,  FM_NONE,       FM_NONE,       FM_NONE,       FM_NONE,       FM_NONE, FM_NONE,       FM_NONE,       FM_NONE, FM_NONE        },
/* 11 UNK  */ { FM_NONE, FM_MUL_UNKXYZ, FM_NONE,  FM_NONE,       FM_MUL_UNKXYZ, FM_MUL_UNKXYZ, FM_MUL_UNKXYZ, FM_NONE, FM_NONE,       FM_NONE,       FM_NONE, FM_NONE        }
};

static eDM eMacro_MulResult[DM_ENUMS][DM_ENUMS] = {
           //   NONE     BCD      BOL       DTE           FLT         INT           IVL         IVY           TME         TSP            VCH      UNK
/*  0 NONE */ { DM_NONE, DM_NONE, DM_NONE,  DM_NONE,      DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE, DM_NONE    },
/*  1 BCD  */ { DM_NONE, DM_BCD,  DM_NONE,  DM_NONE,      DM_FLT,     DM_BCD,       DM_NONE,    DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE, DM_UNK     },
/*  2 BOL  */ { DM_NONE, DM_NONE, DM_NONE,  DM_NONE,      DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE, DM_NONE    },
/*  3 DTE  */ { DM_NONE, DM_NONE, DM_NONE,  DM_NONE,      DM_NONE,    DM_NONE/*?*/, DM_DTE,     DM_NONE,    DM_NONE/*?*/, DM_NONE,    DM_NONE, DM_NONE    },
/*  4 FLT  */ { DM_NONE, DM_FLT,  DM_NONE,  DM_NONE,      DM_FLT,     DM_FLT,       DM_NONE,    DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE, DM_UNK     },
/*  5 INT  */ { DM_NONE, DM_BCD,  DM_NONE,  DM_NONE/*?*/, DM_FLT,     DM_INT,       DM_NONE,    DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE, DM_UNK     },
/*  6 IVL  */ { DM_NONE, DM_NONE, DM_NONE,  DM_NONE,      DM_NONE,    DM_NONE,      DM_IVL,     DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE, DM_UNK     },
/*  7 IVY  */ { DM_NONE, DM_NONE, DM_NONE,  DM_NONE,      DM_NONE,    DM_NONE,      DM_IVL,     DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE, DM_UNK     },
/*  8 TME  */ { DM_NONE, DM_NONE, DM_NONE,  DM_NONE/*?*/, DM_NONE,    DM_NONE/*?*/, DM_TME,     DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE, DM_NONE    },
/*  9 TSP  */ { DM_NONE, DM_NONE, DM_NONE,  DM_NONE,      DM_NONE,    DM_NONE,      DM_TSP,     DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE, DM_NONE    },
/* 10 VCH  */ { DM_NONE, DM_NONE, DM_NONE,  DM_NONE,      DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE, DM_NONE    },
/* 11 UNK  */ { DM_NONE, DM_UNK,  DM_NONE,  DM_NONE,      DM_UNK,     DM_UNK,       DM_UNK,     DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE, DM_NONE    }
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// NEGATE

static eDM eMacro_NegResult[DM_ENUMS] =
    // NONE     BCD      BOL      DTE       FLT     INT     IVL     IVL     TME           TSP       VCH      UNK
    { DM_NONE, DM_BCD,  DM_NONE, DM_NONE,  DM_FLT, DM_INT, DM_IVL, DM_IVL, DM_NONE/*?*/, DM_NONE,  DM_NONE, DM_NONE };

static eFM eMacro_Neg[DM_ENUMS] =
    // NONE     BCD         BOL       DTE       FLT          INT          IVL         IVY,        TME      TSP       VCH      UNK
    { FM_NONE, FM_NEG_BCD, FM_NONE,  FM_NONE,  FM_NEG_FLT,  FM_NEG_INT,  FM_NEG_IVL, FM_NEG_IVL, FM_NONE, FM_NONE,  FM_NONE, FM_NONE    };

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// SUBTRACT

static eFM eMacro_Sub[DM_ENUMS][DM_ENUMS] = {
           //   NONE     BCD            BOL       DTE            FLT            INT            IVL            IVY      TME            TSP            VCH      UNK
/*  0 NONE */ { FM_NONE, FM_NONE,       FM_NONE,  FM_NONE,       FM_NONE,       FM_NONE,       FM_NONE,       FM_NONE, FM_NONE,       FM_NONE,       FM_NONE, FM_NONE    },
/*  1 BCD  */ { FM_NONE, FM_SUB_BCDBCD, FM_NONE,  FM_NONE,       FM_SUB_BCDFLT, FM_SUB_BCDINT, FM_NONE,       FM_NONE, FM_NONE,       FM_NONE,       FM_NONE, FM_SUB_XYZUNK  },
/*  2 BOL  */ { FM_NONE, FM_NONE,       FM_NONE,  FM_NONE,       FM_NONE,       FM_NONE,       FM_NONE,       FM_NONE, FM_NONE,       FM_NONE,       FM_NONE, FM_NONE        },
/*  3 DTE  */ { FM_NONE, FM_NONE,       FM_NONE,  FM_NONE,       FM_NONE,       FM_NONE/*?*/,  FM_SUB_DTEIVL, FM_NONE, FM_NONE/*?*/,  FM_NONE,       FM_NONE, FM_NONE        },
/*  4 FLT  */ { FM_NONE, FM_SUB_FLTBCD, FM_NONE,  FM_NONE,       FM_SUB_FLTFLT, FM_SUB_FLTINT, FM_NONE,       FM_NONE, FM_NONE,       FM_NONE,       FM_NONE, FM_SUB_XYZUNK  },
/*  5 INT  */ { FM_NONE, FM_SUB_INTBCD, FM_NONE,  FM_NONE/*?*/,  FM_SUB_INTFLT, FM_SUB_INTINT, FM_NONE,       FM_NONE, FM_NONE,       FM_NONE,       FM_NONE, FM_SUB_XYZUNK  },
/*  6 IVL  */ { FM_NONE, FM_NONE,       FM_NONE,  FM_SUB_IVLDTE, FM_NONE,       FM_NONE,       FM_SUB_IVLIVL, FM_NONE, FM_SUB_IVLTME, FM_SUB_IVLTSP, FM_NONE, FM_SUB_XYZUNK  },
/*  6 IVY  */ { FM_NONE, FM_NONE,       FM_NONE,  FM_SUB_IVLDTE, FM_NONE,       FM_NONE,       FM_SUB_IVLIVL, FM_NONE, FM_SUB_IVLTME, FM_SUB_IVLTSP, FM_NONE, FM_SUB_XYZUNK  },
/*  7 TME  */ { FM_NONE, FM_NONE,       FM_NONE,  FM_NONE/*?*/,  FM_NONE,       FM_NONE/*?*/,  FM_SUB_TMEIVL, FM_NONE, FM_NONE,       FM_NONE,       FM_NONE, FM_NONE        },
/*  8 TSP  */ { FM_NONE, FM_NONE,       FM_NONE,  FM_NONE,       FM_NONE,       FM_NONE,       FM_SUB_TSPIVL, FM_NONE, FM_NONE,       FM_NONE,       FM_NONE, FM_NONE        },
/*  9 VCH  */ { FM_NONE, FM_NONE,       FM_NONE,  FM_NONE,       FM_NONE,       FM_NONE,       FM_NONE,       FM_NONE, FM_NONE,       FM_NONE,       FM_NONE, FM_NONE        },
/* 10 UNK  */ { FM_NONE, FM_SUB_UNKXYZ, FM_NONE,  FM_NONE,       FM_SUB_UNKXYZ, FM_SUB_UNKXYZ, FM_SUB_UNKXYZ, FM_NONE, FM_NONE,       FM_NONE,       FM_NONE, FM_NONE        }
};

static eDM eMacro_SubResult[DM_ENUMS][DM_ENUMS] = {
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



// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// RELATIONAL OPERATORS
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

static eFM eMacro_EqCode[DM_ENUMS][DM_ENUMS] = {
           //   NONE     BCD            BOL           DTE           FLT            INT            IVL           IVY          TME           TSP           VCH           UNK
/*  0 NONE */ { FM_NONE, FM_NONE,       FM_NONE,      FM_NONE,      FM_NONE,       FM_NONE,       FM_NONE,      FM_NONE,     FM_NONE,      FM_NONE,      FM_NONE,      FM_EQ_XYZUNK    },
/*  1 BCD  */ { FM_NONE, FM_EQ_BCDBCD,  FM_NONE,      FM_NONE,      FM_EQ_BCDFLT,  FM_EQ_BCDINT,  FM_NONE,      FM_NONE,     FM_NONE,      FM_NONE,      FM_NONE,      FM_EQ_XYZUNK    },
/*  2 BOL  */ { FM_NONE, FM_NONE,       FM_EQ_BOLBOL, FM_NONE,      FM_NONE,       FM_NONE,       FM_NONE,      FM_NONE,     FM_NONE,      FM_NONE,      FM_NONE,      FM_EQ_XYZUNK    },
/*  3 DTE  */ { FM_NONE, FM_NONE,       FM_NONE,      FM_EQ_DTEDTE, FM_NONE,       FM_NONE,       FM_NONE,      FM_NONE,     FM_NONE,      FM_EQ_DTETSP, FM_NONE,      FM_EQ_XYZUNK    },
/*  4 FLT  */ { FM_NONE, FM_EQ_FLTBCD,  FM_NONE,      FM_NONE,      FM_EQ_FLTFLT,  FM_EQ_FLTINT,  FM_NONE,      FM_NONE,     FM_NONE,      FM_NONE,      FM_NONE,      FM_EQ_XYZUNK    },
/*  5 INT  */ { FM_NONE, FM_EQ_INTBCD,  FM_NONE,      FM_NONE,      FM_EQ_INTFLT,  FM_EQ_INTINT,  FM_NONE,      FM_NONE,     FM_NONE,      FM_NONE,      FM_NONE,      FM_EQ_XYZUNK    },
/*  6 IVL  */ { FM_NONE, FM_NONE,       FM_NONE,      FM_NONE,      FM_NONE,       FM_NONE,       FM_EQ_IVLIVL, FM_EQ_IVLIVL,FM_NONE,      FM_NONE,      FM_NONE,      FM_EQ_XYZUNK    },
/*  6 IVY  */ { FM_NONE, FM_NONE,       FM_NONE,      FM_NONE,      FM_NONE,       FM_NONE,       FM_EQ_IVLIVL, FM_EQ_IVLIVL,FM_NONE,      FM_NONE,      FM_NONE,      FM_EQ_XYZUNK    },
/*  7 TME  */ { FM_NONE, FM_NONE,       FM_NONE,      FM_NONE,      FM_NONE,       FM_NONE,       FM_NONE,      FM_NONE,     FM_EQ_TMETME, FM_NONE,      FM_NONE,      FM_EQ_XYZUNK    },
/*  8 TSP  */ { FM_NONE, FM_NONE,       FM_NONE,      FM_EQ_TSPDTE, FM_NONE,       FM_NONE,       FM_NONE,      FM_NONE,     FM_NONE,      FM_EQ_TSPTSP, FM_NONE,      FM_EQ_XYZUNK    },
/*  9 VCH  */ { FM_NONE, FM_NONE,       FM_NONE,      FM_NONE,      FM_NONE,       FM_NONE,       FM_NONE,      FM_NONE,     FM_NONE,      FM_NONE,      FM_EQ_VCHVCH, FM_EQ_XYZUNK    },
/* 10 UNK  */ { FM_NONE, FM_EQ_UNKXYZ,  FM_EQ_UNKXYZ, FM_EQ_UNKXYZ, FM_EQ_UNKXYZ,  FM_EQ_UNKXYZ,  FM_EQ_UNKXYZ, FM_EQ_UNKXYZ,FM_EQ_UNKXYZ, FM_EQ_UNKXYZ, FM_EQ_UNKXYZ, FM_EQ_UNKXYZ    }
};

static eFM eMacro_GeCode[DM_ENUMS][DM_ENUMS] = {
           //   NONE     BCD            BOL           DTE           FLT            INT            IVL           IVY               TME           TSP           VCH           UNK
/*  0 NONE */ { FM_NONE, FM_NONE,       FM_NONE,      FM_NONE,      FM_NONE,       FM_NONE,       FM_NONE,      FM_NONE,          FM_NONE,      FM_NONE,      FM_NONE,      FM_GE_XYZUNK    },
/*  1 BCD  */ { FM_NONE, FM_GE_BCDBCD,  FM_NONE,      FM_NONE,      FM_GE_BCDFLT,  FM_GE_BCDINT,  FM_NONE,      FM_NONE,          FM_NONE,      FM_NONE,      FM_NONE,      FM_GE_XYZUNK    },
/*  2 BOL  */ { FM_NONE, FM_NONE,       FM_GE_BOLBOL, FM_NONE,      FM_NONE,       FM_NONE,       FM_NONE,      FM_NONE,          FM_NONE,      FM_NONE,      FM_NONE,      FM_GE_XYZUNK    },
/*  3 DTE  */ { FM_NONE, FM_NONE,       FM_NONE,      FM_GE_DTEDTE, FM_NONE,       FM_NONE,       FM_NONE,      FM_NONE,          FM_NONE,      FM_GE_DTETSP, FM_NONE,      FM_GE_XYZUNK    },
/*  4 FLT  */ { FM_NONE, FM_GE_FLTBCD,  FM_NONE,      FM_NONE,      FM_GE_FLTFLT,  FM_GE_FLTINT,  FM_NONE,      FM_NONE,          FM_NONE,      FM_NONE,      FM_NONE,      FM_GE_XYZUNK    },
/*  5 INT  */ { FM_NONE, FM_GE_INTBCD,  FM_NONE,      FM_NONE,      FM_GE_INTFLT,  FM_GE_INTINT,  FM_NONE,      FM_NONE,          FM_NONE,      FM_NONE,      FM_NONE,      FM_GE_XYZUNK    },
/*  6 IVL  */ { FM_NONE, FM_NONE,       FM_NONE,      FM_NONE,      FM_NONE,       FM_NONE,       FM_GE_IVLIVL, FM_GE_IVLIVL,     FM_NONE,      FM_NONE,      FM_NONE,      FM_GE_XYZUNK    },
/*  6 IVY  */ { FM_NONE, FM_NONE,       FM_NONE,      FM_NONE,      FM_NONE,       FM_NONE,       FM_GE_IVLIVL, FM_GE_IVLIVL,     FM_NONE,      FM_NONE,      FM_NONE,      FM_GE_XYZUNK    },
/*  7 TME  */ { FM_NONE, FM_NONE,       FM_NONE,      FM_NONE,      FM_NONE,       FM_NONE,       FM_NONE,      FM_NONE,          FM_GE_TMETME, FM_NONE,      FM_NONE,      FM_GE_XYZUNK    },
/*  8 TSP  */ { FM_NONE, FM_NONE,       FM_NONE,      FM_GE_TSPDTE, FM_NONE,       FM_NONE,       FM_NONE,      FM_NONE,          FM_NONE,      FM_GE_TSPTSP, FM_NONE,      FM_GE_XYZUNK    },
/*  9 VCH  */ { FM_NONE, FM_NONE,       FM_NONE,      FM_NONE,      FM_NONE,       FM_NONE,       FM_NONE,      FM_NONE,          FM_NONE,      FM_NONE,      FM_GE_VCHVCH, FM_GE_XYZUNK    },
/* 10 UNK  */ { FM_NONE, FM_GE_UNKXYZ,  FM_GE_UNKXYZ, FM_GE_UNKXYZ, FM_GE_UNKXYZ,  FM_GE_UNKXYZ,  FM_GE_UNKXYZ, FM_GE_UNKXYZ,FM_GE_UNKXYZ, FM_GE_UNKXYZ, FM_GE_UNKXYZ, FM_GE_UNKXYZ    }
};

static eFM eMacro_GtCode[DM_ENUMS][DM_ENUMS] = {
           //   NONE     BCD            BOL           DTE           FLT            INT            IVL           IVY           TME           TSP           VCH           UNK
/*  0 NONE */ { FM_NONE, FM_NONE,       FM_NONE,      FM_NONE,      FM_NONE,       FM_NONE,       FM_NONE,      FM_NONE,      FM_NONE,      FM_NONE,      FM_NONE,      FM_GT_XYZUNK    },
/*  1 BCD  */ { FM_NONE, FM_GT_BCDBCD,  FM_NONE,      FM_NONE,      FM_GT_BCDFLT,  FM_GT_BCDINT,  FM_NONE,      FM_NONE,      FM_NONE,      FM_NONE,      FM_NONE,      FM_GT_XYZUNK    },
/*  2 BOL  */ { FM_NONE, FM_NONE,       FM_GT_BOLBOL, FM_NONE,      FM_NONE,       FM_NONE,       FM_NONE,      FM_NONE,      FM_NONE,      FM_NONE,      FM_NONE,      FM_GT_XYZUNK    },
/*  3 DTE  */ { FM_NONE, FM_NONE,       FM_NONE,      FM_GT_DTEDTE, FM_NONE,       FM_NONE,       FM_NONE,      FM_NONE,      FM_NONE,      FM_GT_DTETSP, FM_NONE,      FM_GT_XYZUNK    },
/*  4 FLT  */ { FM_NONE, FM_GT_FLTBCD,  FM_NONE,      FM_NONE,      FM_GT_FLTFLT,  FM_GT_FLTINT,  FM_NONE,      FM_NONE,      FM_NONE,      FM_NONE,      FM_NONE,      FM_GT_XYZUNK    },
/*  5 INT  */ { FM_NONE, FM_GT_INTBCD,  FM_NONE,      FM_NONE,      FM_GT_INTFLT,  FM_GT_INTINT,  FM_NONE,      FM_NONE,      FM_NONE,      FM_NONE,      FM_NONE,      FM_GT_XYZUNK    },
/*  6 IVL  */ { FM_NONE, FM_NONE,       FM_NONE,      FM_NONE,      FM_NONE,       FM_NONE,       FM_GT_IVLIVL, FM_GT_UNKXYZ, FM_NONE,      FM_NONE,      FM_NONE,      FM_GT_XYZUNK    },
/*  6 IVY  */ { FM_NONE, FM_NONE,       FM_NONE,      FM_NONE,      FM_NONE,       FM_NONE,       FM_GT_IVLIVL, FM_GT_UNKXYZ, FM_NONE,      FM_NONE,      FM_NONE,      FM_GT_XYZUNK    },
/*  7 TME  */ { FM_NONE, FM_NONE,       FM_NONE,      FM_NONE,      FM_NONE,       FM_NONE,       FM_NONE,      FM_NONE,      FM_GT_TMETME, FM_NONE,      FM_NONE,      FM_GT_XYZUNK    },
/*  8 TSP  */ { FM_NONE, FM_NONE,       FM_NONE,      FM_GT_TSPDTE, FM_NONE,       FM_NONE,       FM_NONE,      FM_NONE,      FM_NONE,      FM_GT_TSPTSP, FM_NONE,      FM_GT_XYZUNK    },
/*  9 VCH  */ { FM_NONE, FM_NONE,       FM_NONE,      FM_NONE,      FM_NONE,       FM_NONE,       FM_NONE,      FM_NONE,      FM_NONE,      FM_NONE,      FM_GT_VCHVCH, FM_GT_XYZUNK    },
/* 10 UNK  */ { FM_NONE, FM_GT_UNKXYZ,  FM_GT_UNKXYZ, FM_GT_UNKXYZ, FM_GT_UNKXYZ,  FM_GT_UNKXYZ,  FM_GT_UNKXYZ, FM_GT_UNKXYZ, FM_GT_UNKXYZ, FM_GT_UNKXYZ, FM_GT_UNKXYZ, FM_GT_UNKXYZ    }
};

static eFM eMacro_LeCode[DM_ENUMS][DM_ENUMS] = {
           //   NONE     BCD            BOL           DTE           FLT            INT            IVL           IVY           TME           TSP           VCH           UNK
/*  0 NONE */ { FM_NONE, FM_NONE,       FM_NONE,      FM_NONE,      FM_NONE,       FM_NONE,       FM_NONE,      FM_NONE,      FM_NONE,      FM_NONE,      FM_NONE,      FM_LE_XYZUNK    },
/*  1 BCD  */ { FM_NONE, FM_LE_BCDBCD,  FM_NONE,      FM_NONE,      FM_LE_BCDFLT,  FM_LE_BCDINT,  FM_NONE,      FM_NONE,      FM_NONE,      FM_NONE,      FM_NONE,      FM_LE_XYZUNK    },
/*  2 BOL  */ { FM_NONE, FM_NONE,       FM_LE_BOLBOL, FM_NONE,      FM_NONE,       FM_NONE,       FM_NONE,      FM_NONE,      FM_NONE,      FM_NONE,      FM_NONE,      FM_LE_XYZUNK    },
/*  3 DTE  */ { FM_NONE, FM_NONE,       FM_NONE,      FM_LE_DTEDTE, FM_NONE,       FM_NONE,       FM_NONE,      FM_NONE,      FM_NONE,      FM_LE_DTETSP, FM_NONE,      FM_LE_XYZUNK    },
/*  4 FLT  */ { FM_NONE, FM_LE_FLTBCD,  FM_NONE,      FM_NONE,      FM_LE_FLTFLT,  FM_LE_FLTINT,  FM_NONE,      FM_NONE,      FM_NONE,      FM_NONE,      FM_NONE,      FM_LE_XYZUNK    },
/*  5 INT  */ { FM_NONE, FM_LE_INTBCD,  FM_NONE,      FM_NONE,      FM_LE_INTFLT,  FM_LE_INTINT,  FM_NONE,      FM_NONE,      FM_NONE,      FM_NONE,      FM_NONE,      FM_LE_XYZUNK    },
/*  6 IVL  */ { FM_NONE, FM_NONE,       FM_NONE,      FM_NONE,      FM_NONE,       FM_NONE,       FM_LE_IVLIVL, FM_NONE,      FM_NONE,      FM_NONE,      FM_NONE,      FM_LE_XYZUNK    },
/*  6 IVY  */ { FM_NONE, FM_NONE,       FM_NONE,      FM_NONE,      FM_NONE,       FM_NONE,       FM_LE_IVLIVL, FM_NONE,      FM_NONE,      FM_NONE,      FM_NONE,      FM_LE_XYZUNK    },
/*  7 TME  */ { FM_NONE, FM_NONE,       FM_NONE,      FM_NONE,      FM_NONE,       FM_NONE,       FM_NONE,      FM_NONE,      FM_LE_TMETME, FM_NONE,      FM_NONE,      FM_LE_XYZUNK    },
/*  8 TSP  */ { FM_NONE, FM_NONE,       FM_NONE,      FM_LE_TSPDTE, FM_NONE,       FM_NONE,       FM_NONE,      FM_NONE,      FM_NONE,      FM_LE_TSPTSP, FM_NONE,      FM_LE_XYZUNK    },
/*  9 VCH  */ { FM_NONE, FM_NONE,       FM_NONE,      FM_NONE,      FM_NONE,       FM_NONE,       FM_NONE,      FM_NONE,      FM_NONE,      FM_NONE,      FM_LE_VCHVCH, FM_LE_XYZUNK    },
/* 10 UNK  */ { FM_NONE, FM_LE_UNKXYZ,  FM_LE_UNKXYZ, FM_LE_UNKXYZ, FM_LE_UNKXYZ,  FM_LE_UNKXYZ,  FM_LE_UNKXYZ, FM_NONE,      FM_LE_UNKXYZ, FM_LE_UNKXYZ, FM_LE_UNKXYZ, FM_LE_UNKXYZ    }
};

static eFM eMacro_LtCode[DM_ENUMS][DM_ENUMS] = {
          //   NONE     BCD            BOL           DTE           FLT            INT            IVL           IVY           TME           TSP           VCH           UNK
/*  0 NONE */ { FM_NONE, FM_NONE,       FM_NONE,      FM_NONE,      FM_NONE,       FM_NONE,       FM_NONE,      FM_NONE,      FM_NONE,      FM_NONE,      FM_NONE,      FM_LT_XYZUNK    },
/*  1 BCD  */ { FM_NONE, FM_LT_BCDBCD,  FM_NONE,      FM_NONE,      FM_LT_BCDFLT,  FM_LT_BCDINT,  FM_NONE,      FM_NONE,      FM_NONE,      FM_NONE,      FM_NONE,      FM_LT_XYZUNK    },
/*  2 BOL  */ { FM_NONE, FM_NONE,       FM_LT_BOLBOL, FM_NONE,      FM_NONE,       FM_NONE,       FM_NONE,      FM_NONE,      FM_NONE,      FM_NONE,      FM_NONE,      FM_LT_XYZUNK    },
/*  3 DTE  */ { FM_NONE, FM_NONE,       FM_NONE,      FM_LT_DTEDTE, FM_NONE,       FM_NONE,       FM_NONE,      FM_NONE,      FM_NONE,      FM_LT_DTETSP, FM_NONE,      FM_LT_XYZUNK    },
/*  4 FLT  */ { FM_NONE, FM_LT_FLTBCD,  FM_NONE,      FM_NONE,      FM_LT_FLTFLT,  FM_LT_FLTINT,  FM_NONE,      FM_NONE,      FM_NONE,      FM_NONE,      FM_NONE,      FM_LT_XYZUNK    },
/*  5 INT  */ { FM_NONE, FM_LT_INTBCD,  FM_NONE,      FM_NONE,      FM_LT_INTFLT,  FM_LT_INTINT,  FM_NONE,      FM_NONE,      FM_NONE,      FM_NONE,      FM_NONE,      FM_LT_XYZUNK    },
/*  6 IVL  */ { FM_NONE, FM_NONE,       FM_NONE,      FM_NONE,      FM_NONE,       FM_NONE,       FM_LT_IVLIVL, FM_NONE,      FM_NONE,      FM_NONE,      FM_NONE,      FM_LT_XYZUNK    },
/*  6 IVY  */ { FM_NONE, FM_NONE,       FM_NONE,      FM_NONE,      FM_NONE,       FM_NONE,       FM_LT_IVLIVL, FM_NONE,      FM_NONE,      FM_NONE,      FM_NONE,      FM_LT_XYZUNK    },
/*  7 TME  */ { FM_NONE, FM_NONE,       FM_NONE,      FM_NONE,      FM_NONE,       FM_NONE,       FM_NONE,      FM_NONE,      FM_LT_TMETME, FM_NONE,      FM_NONE,      FM_LT_XYZUNK    },
/*  8 TSP  */ { FM_NONE, FM_NONE,       FM_NONE,      FM_LT_TSPDTE, FM_NONE,       FM_NONE,       FM_NONE,      FM_NONE,      FM_NONE,      FM_LT_TSPTSP, FM_NONE,      FM_LT_XYZUNK    },
/*  9 VCH  */ { FM_NONE, FM_NONE,       FM_NONE,      FM_NONE,      FM_NONE,       FM_NONE,       FM_NONE,      FM_NONE,      FM_NONE,      FM_NONE,      FM_LT_VCHVCH, FM_LT_XYZUNK    },
/* 10 UNK  */ { FM_NONE, FM_LT_UNKXYZ,  FM_LT_UNKXYZ, FM_LT_UNKXYZ, FM_LT_UNKXYZ,  FM_LT_UNKXYZ,  FM_LT_UNKXYZ, FM_NONE,      FM_LT_UNKXYZ, FM_LT_UNKXYZ, FM_LT_UNKXYZ, FM_LT_UNKXYZ    }
};

static eFM eMacro_NeCode[DM_ENUMS][DM_ENUMS] = {
           //   NONE     BCD            BOL           DTE           FLT            INT            IVL           IVY           TME           TSP           VCH           UNK
/*  0 NONE */ { FM_NONE, FM_NONE,       FM_NONE,      FM_NONE,      FM_NONE,       FM_NONE,       FM_NONE,      FM_NONE,      FM_NONE,      FM_NONE,      FM_NONE,      FM_NE_XYZUNK    },
/*  1 BCD  */ { FM_NONE, FM_NE_BCDBCD,  FM_NONE,      FM_NONE,      FM_NE_BCDFLT,  FM_NE_BCDINT,  FM_NONE,      FM_NONE,      FM_NONE,      FM_NONE,      FM_NONE,      FM_NE_XYZUNK    },
/*  2 BOL  */ { FM_NONE, FM_NONE,       FM_NE_BOLBOL, FM_NONE,      FM_NONE,       FM_NONE,       FM_NONE,      FM_NONE,      FM_NONE,      FM_NONE,      FM_NONE,      FM_NE_XYZUNK    },
/*  3 DTE  */ { FM_NONE, FM_NONE,       FM_NONE,      FM_NE_DTEDTE, FM_NONE,       FM_NONE,       FM_NONE,      FM_NONE,      FM_NONE,      FM_NE_DTETSP, FM_NONE,      FM_NE_XYZUNK    },
/*  4 FLT  */ { FM_NONE, FM_NE_FLTBCD,  FM_NONE,      FM_NONE,      FM_NE_FLTFLT,  FM_NE_FLTINT,  FM_NONE,      FM_NONE,      FM_NONE,      FM_NONE,      FM_NONE,      FM_NE_XYZUNK    },
/*  5 INT  */ { FM_NONE, FM_NE_INTBCD,  FM_NONE,      FM_NONE,      FM_NE_INTFLT,  FM_NE_INTINT,  FM_NONE,      FM_NONE,      FM_NONE,      FM_NONE,      FM_NONE,      FM_NE_XYZUNK    },
/*  6 IVL  */ { FM_NONE, FM_NONE,       FM_NONE,      FM_NONE,      FM_NONE,       FM_NONE,       FM_NE_IVLIVL, FM_NONE,      FM_NONE,      FM_NONE,      FM_NONE,      FM_NE_XYZUNK    },
/*  6 IVY  */ { FM_NONE, FM_NONE,       FM_NONE,      FM_NONE,      FM_NONE,       FM_NONE,       FM_NE_IVLIVL, FM_NONE,      FM_NONE,      FM_NONE,      FM_NONE,      FM_NE_XYZUNK    },
/*  7 TME  */ { FM_NONE, FM_NONE,       FM_NONE,      FM_NONE,      FM_NONE,       FM_NONE,       FM_NONE,      FM_NONE,      FM_NE_TMETME, FM_NONE,      FM_NONE,      FM_NE_XYZUNK    },
/*  8 TSP  */ { FM_NONE, FM_NONE,       FM_NONE,      FM_NE_TSPDTE, FM_NONE,       FM_NONE,       FM_NONE,      FM_NONE,      FM_NONE,      FM_NE_TSPTSP, FM_NONE,      FM_NE_XYZUNK    },
/*  9 VCH  */ { FM_NONE, FM_NONE,       FM_NONE,      FM_NONE,      FM_NONE,       FM_NONE,       FM_NONE,      FM_NONE,      FM_NONE,      FM_NONE,      FM_NE_VCHVCH, FM_NE_XYZUNK    },
/* 10 UNK  */ { FM_NONE, FM_NE_UNKXYZ,  FM_NE_UNKXYZ, FM_NE_UNKXYZ, FM_NE_UNKXYZ,  FM_NE_UNKXYZ,  FM_NE_UNKXYZ, FM_NONE,      FM_NE_UNKXYZ, FM_NE_UNKXYZ, FM_NE_UNKXYZ, FM_NE_UNKXYZ    }
};

static eDM eMacro_CmpResult[DM_ENUMS][DM_ENUMS] = {
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

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
