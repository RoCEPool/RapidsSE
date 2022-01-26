/*============================================================================*/
/* FunMatrix.h -- Function Matrix                                             */
/*============================================================================*/
#pragma once

static eFM eMacro_Add[DM_ENUMS][DM_ENUMS] = {
    //   NONE     BCD            BOL       DTE            FLT            INT            IVL            IVY      TME            TSP            VCH      UNK
/*  0 NONE */ { FM_NONE, FM_NONE,       FM_NONE,  FM_NONE,       FM_NONE,       FM_NONE,       FM_NONE,       FM_NONE, FM_NONE,       FM_NONE,       FM_NONE, FM_NONE    },
/*  1 BCD  */ { FM_NONE, FM_ADD_BCDBCD, FM_NONE,  FM_NONE,       FM_ADD_BCDFLT, FM_ADD_BCDINT, FM_NONE,       FM_NONE, FM_NONE,       FM_NONE,       FM_NONE, FM_ADD_XYZUNK  },
/*  2 BOL  */ { FM_NONE, FM_NONE,       FM_NONE,  FM_NONE,       FM_NONE,       FM_NONE,       FM_NONE,       FM_NONE, FM_NONE,       FM_NONE,       FM_NONE, FM_NONE        },
/*  3 DTE  */ { FM_NONE, FM_NONE,       FM_NONE,  FM_NONE,       FM_NONE,       FM_NONE/*?*/,  FM_ADD_DTEIVL, FM_NONE, FM_NONE/*?*/,  FM_NONE,       FM_NONE, FM_NONE        },
/*  4 FLT  */ { FM_NONE, FM_ADD_FLTBCD, FM_NONE,  FM_NONE,       FM_ADD_FLTFLT, FM_ADD_FLTINT, FM_NONE,       FM_NONE, FM_NONE,       FM_NONE,       FM_NONE, FM_ADD_XYZUNK  },
/*  5 INT  */ { FM_NONE, FM_ADD_INTBCD, FM_NONE,  FM_NONE/*?*/,  FM_ADD_INTFLT, FM_ADD_INTINT, FM_NONE,       FM_NONE, FM_NONE,       FM_NONE,       FM_NONE, FM_ADD_XYZUNK  },
/*  6 IVL  */ { FM_NONE, FM_NONE,       FM_NONE,  FM_ADD_IVLDTE, FM_NONE,       FM_NONE,       FM_ADD_IVLIVL, FM_NONE, FM_ADD_IVLTME, FM_ADD_IVLTSP, FM_NONE, FM_ADD_XYZUNK  },
/*  6 IVY  */ { FM_NONE, FM_NONE,       FM_NONE,  FM_ADD_IVLDTE, FM_NONE,       FM_NONE,       FM_ADD_IVLIVL, FM_NONE, FM_ADD_IVLTME, FM_ADD_IVLTSP, FM_NONE, FM_ADD_XYZUNK  },
/*  7 TME  */ { FM_NONE, FM_NONE,       FM_NONE,  FM_NONE/*?*/,  FM_NONE,       FM_NONE/*?*/,  FM_ADD_TMEIVL, FM_NONE, FM_NONE,       FM_NONE,       FM_NONE, FM_NONE        },
/*  8 TSP  */ { FM_NONE, FM_NONE,       FM_NONE,  FM_NONE,       FM_NONE,       FM_NONE,       FM_ADD_TSPIVL, FM_NONE, FM_NONE,       FM_NONE,       FM_NONE, FM_NONE        },
/*  9 VCH  */ { FM_NONE, FM_NONE,       FM_NONE,  FM_NONE,       FM_NONE,       FM_NONE,       FM_NONE,       FM_NONE, FM_NONE,       FM_NONE,       FM_NONE, FM_NONE        },
/* 10 UNK  */ { FM_NONE, FM_ADD_UNKXYZ, FM_NONE,  FM_NONE,       FM_ADD_UNKXYZ, FM_ADD_UNKXYZ, FM_ADD_UNKXYZ, FM_NONE, FM_NONE,       FM_NONE,       FM_NONE, FM_NONE        }
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
/*  6 IVY  */ { DM_NONE, DM_NONE,  DM_NONE,  DM_DTE,       DM_NONE,    DM_NONE,      DM_IVL,     DM_NONE,    DM_TME,       DM_TSP,     DM_NONE, DM_UNK     },
/*  7 TME  */ { DM_NONE, DM_NONE,  DM_NONE,  DM_NONE/*?*/, DM_NONE,    DM_NONE/*?*/, DM_TME,     DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE, DM_NONE    },
/*  8 TSP  */ { DM_NONE, DM_NONE,  DM_NONE,  DM_NONE,      DM_NONE,    DM_NONE,      DM_TSP,     DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE, DM_NONE    },
/*  9 VCH  */ { DM_NONE, DM_NONE,  DM_NONE,  DM_NONE,      DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE, DM_NONE    },
/* 10 UNK  */ { DM_NONE, DM_UNK,   DM_UNK,   DM_NONE,      DM_UNK,     DM_UNK,       DM_UNK,     DM_NONE,    DM_NONE,      DM_NONE,    DM_NONE, DM_NONE    }
};


static eFM eMacro_Div[DM_ENUMS][DM_ENUMS] = {
    //   NONE     BCD            BOL       DTE            FLT            INT            IVL            IVY      TME            TSP            VCH      UNK
/*  0 NONE */ { FM_NONE, FM_NONE,       FM_NONE,  FM_NONE,       FM_NONE,       FM_NONE,       FM_NONE,       FM_NONE, FM_NONE,       FM_NONE,       FM_NONE, FM_NONE    },
/*  1 BCD  */ { FM_NONE, FM_DIV_BCDBCD, FM_NONE,  FM_NONE,       FM_DIV_BCDFLT, FM_DIV_BCDINT, FM_NONE,       FM_NONE, FM_NONE,       FM_NONE,       FM_NONE, FM_DIV_XYZUNK  },
/*  2 BOL  */ { FM_NONE, FM_NONE,       FM_NONE,  FM_NONE,       FM_NONE,       FM_NONE,       FM_NONE,       FM_NONE, FM_NONE,       FM_NONE,       FM_NONE, FM_NONE        },
/*  3 DTE  */ { FM_NONE, FM_NONE,       FM_NONE,  FM_NONE,       FM_NONE,       FM_NONE/*?*/,  FM_DIV_DTEIVL, FM_NONE, FM_NONE/*?*/,  FM_NONE,       FM_NONE, FM_NONE        },
/*  4 FLT  */ { FM_NONE, FM_DIV_FLTBCD, FM_NONE,  FM_NONE,       FM_DIV_FLTFLT, FM_DIV_FLTINT, FM_NONE,       FM_NONE, FM_NONE,       FM_NONE,       FM_NONE, FM_DIV_XYZUNK  },
/*  5 INT  */ { FM_NONE, FM_DIV_INTBCD, FM_NONE,  FM_NONE/*?*/,  FM_DIV_INTFLT, FM_DIV_INTINT, FM_NONE,       FM_NONE, FM_NONE,       FM_NONE,       FM_NONE, FM_DIV_XYZUNK  },
/*  6 IVL  */ { FM_NONE, FM_NONE,       FM_NONE,  FM_DIV_IVLDTE, FM_NONE,       FM_NONE,       FM_DIV_IVLIVL, FM_NONE, FM_DIV_IVLTME, FM_DIV_IVLTSP, FM_NONE, FM_DIV_XYZUNK  },
/*  6 IVY  */ { FM_NONE, FM_NONE,       FM_NONE,  FM_DIV_IVLDTE, FM_NONE,       FM_NONE,       FM_DIV_IVLIVL, FM_NONE, FM_DIV_IVLTME, FM_DIV_IVLTSP, FM_NONE, FM_DIV_XYZUNK  },
/*  7 TME  */ { FM_NONE, FM_NONE,       FM_NONE,  FM_NONE/*?*/,  FM_NONE,       FM_NONE/*?*/,  FM_DIV_TMEIVL, FM_NONE, FM_NONE,       FM_NONE,       FM_NONE, FM_NONE        },
/*  8 TSP  */ { FM_NONE, FM_NONE,       FM_NONE,  FM_NONE,       FM_NONE,       FM_NONE,       FM_DIV_TSPIVL, FM_NONE, FM_NONE,       FM_NONE,       FM_NONE, FM_NONE        },
/*  9 VCH  */ { FM_NONE, FM_NONE,       FM_NONE,  FM_NONE,       FM_NONE,       FM_NONE,       FM_NONE,       FM_NONE, FM_NONE,       FM_NONE,       FM_NONE, FM_NONE        },
/* 10 UNK  */ { FM_NONE, FM_DIV_UNKXYZ, FM_NONE,  FM_NONE,       FM_DIV_UNKXYZ, FM_DIV_UNKXYZ, FM_DIV_UNKXYZ, FM_NONE, FM_NONE,       FM_NONE,       FM_NONE, FM_NONE        }
};

static eDM eMacro_DivResult[DM_ENUMS][DM_ENUMS] = {
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

static eFM eMacro_Mul[DM_ENUMS][DM_ENUMS] = {
    //   NONE     BCD            BOL       DTE            FLT            INT            IVL            IVY      TME            TSP            VCH      UNK
/*  0 NONE */ { FM_NONE, FM_NONE,       FM_NONE,  FM_NONE,       FM_NONE,       FM_NONE,       FM_NONE,       FM_NONE, FM_NONE,       FM_NONE,       FM_NONE, FM_NONE    },
/*  1 BCD  */ { FM_NONE, FM_MUL_BCDBCD, FM_NONE,  FM_NONE,       FM_MUL_BCDFLT, FM_MUL_BCDINT, FM_NONE,       FM_NONE, FM_NONE,       FM_NONE,       FM_NONE, FM_MUL_XYZUNK  },
/*  2 BOL  */ { FM_NONE, FM_NONE,       FM_NONE,  FM_NONE,       FM_NONE,       FM_NONE,       FM_NONE,       FM_NONE, FM_NONE,       FM_NONE,       FM_NONE, FM_NONE        },
/*  3 DTE  */ { FM_NONE, FM_NONE,       FM_NONE,  FM_NONE,       FM_NONE,       FM_NONE/*?*/,  FM_MUL_DTEIVL, FM_NONE, FM_NONE/*?*/,  FM_NONE,       FM_NONE, FM_NONE        },
/*  4 FLT  */ { FM_NONE, FM_MUL_FLTBCD, FM_NONE,  FM_NONE,       FM_MUL_FLTFLT, FM_MUL_FLTINT, FM_NONE,       FM_NONE, FM_NONE,       FM_NONE,       FM_NONE, FM_MUL_XYZUNK  },
/*  5 INT  */ { FM_NONE, FM_MUL_INTBCD, FM_NONE,  FM_NONE/*?*/,  FM_MUL_INTFLT, FM_MUL_INTINT, FM_NONE,       FM_NONE, FM_NONE,       FM_NONE,       FM_NONE, FM_MUL_XYZUNK  },
/*  6 IVL  */ { FM_NONE, FM_NONE,       FM_NONE,  FM_MUL_IVLDTE, FM_NONE,       FM_NONE,       FM_MUL_IVLIVL, FM_NONE, FM_MUL_IVLTME, FM_MUL_IVLTSP, FM_NONE, FM_MUL_XYZUNK  },
/*  6 IVY  */ { FM_NONE, FM_NONE,       FM_NONE,  FM_MUL_IVLDTE, FM_NONE,       FM_NONE,       FM_MUL_IVLIVL, FM_NONE, FM_MUL_IVLTME, FM_MUL_IVLTSP, FM_NONE, FM_MUL_XYZUNK  },
/*  7 TME  */ { FM_NONE, FM_NONE,       FM_NONE,  FM_NONE/*?*/,  FM_NONE,       FM_NONE/*?*/,  FM_MUL_TMEIVL, FM_NONE, FM_NONE,       FM_NONE,       FM_NONE, FM_NONE        },
/*  8 TSP  */ { FM_NONE, FM_NONE,       FM_NONE,  FM_NONE,       FM_NONE,       FM_NONE,       FM_MUL_TSPIVL, FM_NONE, FM_NONE,       FM_NONE,       FM_NONE, FM_NONE        },
/*  9 VCH  */ { FM_NONE, FM_NONE,       FM_NONE,  FM_NONE,       FM_NONE,       FM_NONE,       FM_NONE,       FM_NONE, FM_NONE,       FM_NONE,       FM_NONE, FM_NONE        },
/* 10 UNK  */ { FM_NONE, FM_MUL_UNKXYZ, FM_NONE,  FM_NONE,       FM_MUL_UNKXYZ, FM_MUL_UNKXYZ, FM_MUL_UNKXYZ, FM_NONE, FM_NONE,       FM_NONE,       FM_NONE, FM_NONE        }
};

static eDM eMacro_MulResult[DM_ENUMS][DM_ENUMS] = {
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


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
