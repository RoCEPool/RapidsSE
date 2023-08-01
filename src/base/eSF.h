/*============================================================================*/
/* eSF.h - symbol form                                                        */
/*============================================================================*/
#pragma once

enum eSF // symbol form
{
  /* */ SF_NONE = 0,   // none

  /* */ SF_AGGFN,
  /* */ SF_ASNAME,
  /* */ SF_COLNM,
  /* */ SF_DASCNE,
  /* */ SF_IDXCOL,
  /* */ SF_KEYCOL,
  /* */ SF_NUMBER,

  /* */ SF_OTHER,

  /* */ SF_PRMCOL,
  /* */ SF_SASCNM,
  /* */ SF_SQCKEY,
  /* */ SF_SQRYNO,
  /* */ SF_STMCOL,
  /* */ SF_TBLCOL,
  /* */ SF_TBLSYN,
  /* */ SF_UNKNOWN,
  /* */ SF_XREFNO,

  /* */ SF_ENUMS
};

static Stz* aSF[]
{
  /* */ "SF_NONE"_Stz,

  /* */ "SF_AGGFN"_Stz,
  /* */ "SF_ASNAME"_Stz,
  /* */ "SF_COLNM"_Stz,
  /* */ "SF_DASCNM"_Stz,
  /* */ "SF_IDXCOL"_Stz,
  /* */ "SF_KEYCOL"_Stz,
  /* */ "SF_NUMBER"_Stz,

  /* */ "SF_OTHER"_Stz,

  /* */ "SF_PRMCOL"_Stz,
  /* */ "SF_SASCNM"_Stz,
  /* */ "SF_SQCKEY"_Stz,
  /* */ "SF_SQRYNO"_Stz,
  /* */ "SF_STMCOL"_Stz,
  /* */ "SF_STMCOL"_Stz,
  /* */ "SF_TBLCOL"_Stz,
  /* */ "SF_TBLSYN"_Stz,
  /* */ "SF_UNKNOWN"_Stz,
  /* */ "SF_XREFNO"_Stz,

  /* */ "SF_ENUMS"_Stz
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
