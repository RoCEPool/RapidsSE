/*============================================================================*/
/* eJT.cpp -- Join Type                                                       */
/*============================================================================*/
#include "BASE.h"
#include "eJT.h"

Stz* aJT[JT_NENUMS]
{
    /*  0 */ "NONE"_Stz,
    /*  1 */ "INNER JOIN"_Stz,
    /*  2 */ "CROSS JOIN"_Stz,
    /*  3 */ "SEMI JOIN"_Stz,
    /*  4 */ "ANTI JOIN"_Stz,
    /*  5 */ "LEFT OUTER JOIN"_Stz,
    /*  6 */ "RIGHT OUTER JOIN"_Stz,
    /*  7 */ "FULL OUTER JOIN"_Stz,
    /*  8 */ "NATURAL INNER JOIN"_Stz,
    /*  9 */ "NATURAL LEFT OUTER JOIN"_Stz,
    /* 10 */ "NATURAL RIGHT OUTER JOIN"_Stz,
    /* 11 */ "NATURAL FULL OUTER JOIN"_Stz,
//    /* 12 */ "NENUMS"_Stz
};

Stz* aJT_pretty[JT_NENUMS]
{
  /*  0 */ "NONE"_Stz,
  /*  1 */ "Inner Join"_Stz,
  /*  2 */ "Cross Join"_Stz,
  /*  3 */ "Semi Join"_Stz,
  /*  4 */ "Anti Join"_Stz,
  /*  5 */ "Left Outer Join"_Stz,
  /*  6 */ "Right Outer Join"_Stz,
  /*  7 */ "Full Outer Join"_Stz,
  /*  8 */ "Natural Inner Join"_Stz,
  /*  9 */ "Natural Left Outer Join"_Stz,
  /* 10 */ "Natural Right Outer Join"_Stz,
  /* 11 */ "Natural Full Outer Join"_Stz,
//  /* 12 */ "NENUMS"_Stz
};


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
