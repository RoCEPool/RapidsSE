/*============================================================================*/
/* eQC.h -- SubQuery called from which Clause                                 */
/*============================================================================*/
#pragma once

enum eQC // subquery calling clause
{
  /*   */ QC_NONE=0,

  /*   */ QC_FROM_CLAUSE,
  /*   */ QC_HAVING_CLAUSE,
  /*   */ QC_WHERE_CLAUSE,

  /*   */ QC_ENUMS
};

static Stz* aQC[]
{
  /*   */ "QC_NONE"_Stz,

  /*   */ "QC_FROM_CLAUSE"_Stz,
  /*   */ "QC_HAVING_CLAUSE"_Stz,
  /*   */ "QC_WHERE_CLAUSE"_Stz,

  /*   */ "QC_ENUMS"_Stz,
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
