/*============================================================================*/
/* eQA.h -- Queue Actions                                                     */
/*============================================================================*/
#pragma once

enum eQA // operator actions
{
  /*  0 */ QA_NONE=0,
  
  /*    */ QA_QUE_FIND_ORIGINATOR,
  /*    */ QA_QUE_FIND_PARTICPANT,

  /*    */ QA_QUE_ORIGINATORS,
  /*    */ QA_QUE_PARTICPANTS,

  /*    */ QA_ENUMS,
};

static Stz* aQA[]
{
  /*  0 */ "NONE"_Stz,
  
  /*    */ "QA_QUE_FIND_ORIGINATOR"_Stz,
  /*    */ "QA_QUE_FIND_PARTICPANT"_Stz,
  
  /*    */ "QA_QUE_ORIGINATORS"_Stz,
  /*    */ "QA_QUE_PARTICPANTS"_Stz,

  /*    */ "ENUMS"_Stz,
};

static Stz* aQA_short[]
{
  /*  0 */ "NONE"_Stz,
  
  /*    */ "Queue find originiator"_Stz,
  /*    */ "Queue find participant"_Stz,
  /*    */ "Queue originiators"_Stz,
  /*    */ "Queue participants"_Stz,

  /*    */ "ENUMS"_Stz,
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
