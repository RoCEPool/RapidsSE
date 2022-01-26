/*============================================================================*/
/* CodeSynth.hpp -- Code Synthesizer                                          */
/*============================================================================*/
#pragma once

#include "TinyC.hpp"
#include "Tub.hpp"
#include "eAG.h"
#include "ePJ.h"
#include "eFM.h"
#include "IndexDef.hpp"

namespace STORE {

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

  static const i8 C_FLATTEN_SOURCE = C_SIZE_LARGE_OBJECT - C_1KB;

  //----------------------------------------------------------------------------

  class CodeSynth : public Object {
  private:

    char m_caCode[C_FLATTEN_SOURCE];
    char *m_azCodeCrs;

    TokenVec *m_pTokenVec;
    TypedStack *m_pExprStack;
    TypedStack *m_pPostStack;
    TypedStack *m_pLowerStack;
    TypedStack *m_pUpperStack;
    TypedStack *m_pInitStack;

    s256 m_sLowerPosition;
    s256 m_sUpperPosition;

    StreamDef *m_pStreamDefLft;
    StreamDef *m_pStreamDefOut;
    eSR m_eStencilFrom;
    eSR m_eStencilTo;

    i8 m_iLimitRows;

    // must be last
    Footer m_xFooter;

  public:
    void *operator new(size_t);

    void operator delete(void *);

    CodeSynth(void);

    CodeSynth(StreamDef *pStreamDefOut, eSR eStencilTo);

    CodeSynth(StreamDef *pStreamDefLeft, StreamDef *pStreamDefOut, TypedStack *pPostStack,
              eSR eStencilFrom, eSR eStencilTo);

    ~CodeSynth(void) noexcept(false);

    static void vFormColumn(Stz *szColumn, StreamDef *pStreamDef, eSR eStencilRow, i8 i_Column);

    void vPostStack(TypedStack *pPostStack);

    TypedStack *pPostStack(void);

    void vAppendFragment(Stz *szFragment);

    void vAppendFragment(Stz *szFragment, i8 *piLineNumber);

    void vAppendFragment(CodeSynth *pFragment);

    void vAppendFragment(CodeSynth *pFragment, i8 *piLineNumber);

    char *azCode(void);

    void vPrint(void);

    void vReset(void);

    bool bSynthAggGby(IndexDef *pIndexDef, IntStack *pAggTypeStack,
                      Stz *sProjectExpr, Stz *sLabel, Stz *sLabelLoop, bool bHashRows);

    void vSynthAllocTubRow(IntStack *pAggTypeStack, bool bEosHandler, Stz *sLabel, Stz *sLabelNext);

    void vSynthAllocPotRow(IntStack *pAggTypeStack, Stz *sHashExpr, bool bEosHandler,
                           Stz *sLabel, Stz *sLabelNext);

    void vSynthEosPayload(IndexDef *pIndexDef, IntStack *pAggTypeStack);

    void vSynthEosTop(IndexDef *pIndexDef);

    void vSynthFilter(Stz *sFilterExpr, Stz *sLabel);

    void vSynthFilterOrdAgg(Stz *sFilterExpr, Stz *sLabel, Stz *sLabelPrev);

    void vSynthIndexJoin(StreamDef *pStreamDefRht, IndexDef *pIndexDefJ,
                         Stz *sJoinExpr, Stz *sLabel, Stz *sLabelNext, u8 uSelectColumns);

    void vSynthIndexScan(IndexDef *pIndexDef, Stz *sIndexScanExpr, Stz *sLabel, Stz *sNextLabel, u8 uSelectColumns);

    void vSynthInflateCrw(Stz *sTableNm, Stz *sLabel, Stz *sReturnLabel, u8 *puSelectColumns);

    void vSynthInflateRow(Stz *sLabel);

    void vSynthKeyExists(StreamDef *pStreamDefRht, IndexDef *pIndexDefJ,
                         Stz *sFilterExpr, Stz *sKeyExpr, Stz *sLabel, u8 uSelectColumns);

    void vSynthLimit(i8 iLimit, Stz *sLabel, Stz *sLabelNext, Stz *sLabelLoop);

    void vSynthOrdAgg(IndexDef *pIndexDef, ePJ eProjectKind, IntStack *pAggTypeStack,
                      Stz *sProjectExpr, Stz *sLabel, Stz *sLabelNext, Stz *sLabelLoop, bool bHashRows);

    void vSynthSmlAgg(IndexDef *pIndexDef, ePJ eProjectKind, IntStack *pAggTypeStack,
                      Stz *sProjectExpr, Stz *sLabel, Stz *sLabelLoop, bool bHashRows);

    bool bSynthPrjAgg(IntStack *pAggTypeStack, Stz *sProjectExpr, Stz *sLabel, Stz *sLabelLoop, bool bHashRows);

    bool bSynthPrjExp(IntStack *pAggTypeStack, Stz *sProjectExpr, Stz *sLabel, Stz *sLabelNext, Stz *sLabelLoop,
                      bool bHashRows);

    void vSynthStruct(u8 uSelectColumns);

    void vSynthStructArray(u8 uSelectColumns);

    void vSynthTop(IndexDef *pIndexDef, Stz *sKeyExpr, i8 iTopCount, Stz *sLabel, Stz *sLabelLoop);

  private:
    void vFindKey(IndexDef *pIndexDef, IntStack *pAggTypeStack);

    void vGenerateAggInitCode(IntStack *pAggTypeStack);

    void vGenerateAssign(u8 uSelectColumns);

    void vGenerateFilter(Stz *sExpr);

    void vGenerateGoto(Stz *sLabel);

    void vGenerateInflate(StreamDef *pStreamDef, Stz *szReturnPrefix, i8 nColumndBefore, u8 uSelectColumns);

    void vGenerateInitStub(Stz *szComment, Stz *sLabel);

    void vGenerateKeyExistsFilter(Stz *sExpr);

    void vGenerateProcessRow(Stz *sLabel);

    bool bGeneratePrjAgg(IntStack *pAggTypeStack);

    void vGeneratePrjExp(void);

    void vGeneratePrjGby(IndexDef *pIndexDef, IntStack *pAggTypeStack);

    void vGenerateStencilCast(void);

    i8 iGetValue(eDM eDatum, char *sValue);

    void vInitAgg(eAG eAggType, eDM eExprType);

    void vOrdAggGby(IndexDef *pIndexDef, IntStack *pAggTypeStack, Stz *sLabelNext, Stz *sLabelLoop);

    void vPayloadAgg(IndexDef *pIndexDef, IntStack *pAggTypeStack);

    void vPayloadKey(IndexDef *pIndexDef, Stz *szEntWrk, Stz *szEntKey);

    void vPayloadStore(IndexDef *pIndexDef, IntStack *pAggTypeStack);

    void vPayloadToBuffer(IndexDef *pIndexDef, IntStack *pAggTypeStack, bool bTop);

    void vPayloadTraversal(IndexDef *pIndexDef, IntStack *pAggTypeStack);

    void vPushAggCol(TypedStack *pExprStack, ePJ eProjectKind, bool bHashRows, i8 i_Col);

    void vPushArithmetic(TypedStack *pExprStack, IntStack *pTypeStack, Token *pTkn);

    void vPushOrdCol(TypedStack *pExprStack, IntStack* pTypeStack, Token *pTkn);

    void vSmlAggLoad(IntStack *pAggTypeStack);

    void vSmlAggStore(IntStack *pAggTypeStack);

    void vToInfix(char *sExpr, IntStack *pAggTypeStack,
                  ePJ eProjectKind, bool bHashRows, StreamDef *pStreamDef);

    void vTopIndexTraversal(IndexDef *pIndexDef);

    Stz *szInfix(TypedStack *pExprStack, i8 iProject);

    void vGetInfix(s256 &sInfix, TypedStack *pExprStack, i8 iProject);

    Footer *aFooter(void);
  };

  CodeSynth *_pCodeSynth(void *aAddr);

}
/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
