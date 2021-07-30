/*============================================================================*/
/* eJT.h -- Join Type                                                         */
/*============================================================================*/
#pragma once


enum eJT // Join Type
{
  /*  0 */ JT_NONE=0,
  /*  1 */ JT_INNER,
  /*  2 */ JT_CROSS,
  /*  3 */ JT_NATURAL,
  /*  4 */ JT_SEMI,
  /*  5 */ JT_ANTI,
  /*  6 */ JT_LEFT_OUTER,
  /*  7 */ JT_RIGHT_OUTER,
  /*  8 */ JT_FULL_OUTER,
  /*  9 */ JT_NENUMS
};

static Stz* aJT[]
{
    /*  0 */ "NONE"_Stz,
    /*  1 */ "INNER JOIN"_Stz,
    /*  2 */ "CROSS JOIN"_Stz,
    /*  3 */ "NATURAL JOIN"_Stz,
    /*  4 */ "SEMI JOIN"_Stz,
    /*  5 */ "ANTI JOIN"_Stz,
    /*  6 */ "LEFT OUTER JOIN"_Stz,
    /*  7 */ "RIGHT OUTER JOIN"_Stz,
    /*  8 */ "FULL OUTER JOIN"_Stz,
    /*  9 */ "NENUMS"_Stz
};

static Stz* aJT_pretty[]
{
  /*  0 */ "NONE"_Stz,
  /*  1 */ "Inner Join"_Stz,
  /*  2 */ "Cross Join"_Stz,
  /*  3 */ "Natural Join"_Stz,
  /*  4 */ "Semi Join"_Stz,
  /*  5 */ "Anti Join"_Stz,
  /*  6 */ "Left Outer Join"_Stz,
  /*  7 */ "Right Outer Join"_Stz,
  /*  8 */ "Full Outer Join"_Stz,
  /*  9 */ "NENUMS"_Stz
};


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
