/*============================================================================*/
/* eQA.h -- Queue Actions                                                     */
/*============================================================================*/
#pragma once

enum eQA // queue actions
{
  /*  0 */ QA_NONE=0,
  
  /*    */ QA_QUE_FIND_ORIG,
  /*    */ QA_QUE_FIND_PARTG,
  /*    */ QA_QUE_FOUND_ORIG,
  /*    */ QA_QUE_FOUND_PARTG,
  /*    */ QA_QUE_NOT_ORIG,
  /*    */ QA_QUE_NOT_PARTG,

  /*    */ QA_QUE_ORIGINATORS,
  /*    */ QA_QUE_PARTICPANTS,

  /*    */ QA_ENUMS,
};

static Stz* aQA[]
{
  /*  0 */ "NONE"_Stz,
  
  /*    */ "QA_QUE_FIND_ORIG"_Stz,
  /*    */ "QA_QUE_FIND_PARTG"_Stz,
  /*    */ "QA_QUE_FOUND_ORIG"_Stz,
  /*    */ "QA_QUE_FOUND_PARTG"_Stz,
  /*    */ "QA_QUE_NOT_ORIG"_Stz,
  /*    */ "QA_QUE_NOT_PARTG"_Stz,

  /*    */ "QA_QUE_ORIGINATORS"_Stz,
  /*    */ "QA_QUE_PARTICPANTS"_Stz,

  /*    */ "ENUMS"_Stz,
};

static Stz* aQA_short[]
{
  /*  0 */ "NONE"_Stz,
  
  /*    */ "Queue find originiator"_Stz,
  /*    */ "Queue find participant"_Stz,
  /*    */ "Queue found originiator"_Stz,
  /*    */ "Queue found participant"_Stz,
  /*    */ "Queue not found originiator"_Stz,
  /*    */ "Queue found found participant"_Stz,
  
  /*    */ "Queue originiators"_Stz,
  /*    */ "Queue participants"_Stz,

  /*    */ "ENUMS"_Stz,
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
