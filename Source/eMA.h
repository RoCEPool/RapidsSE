/*============================================================================*/
/* eMA.h -- Request Actions                                                   */
/*============================================================================*/
#pragma once

enum eMA // command instance actions
{
  /*  0 */ MA_REQ_NONE=0,
  
  /*    */ MA_REQ_CONSTRUCT,
  /*    */ MA_REQ_DESTRUCT,

  /*    */ MA_REQ_ENUMS,
};

static Stz* aMA[]
{
  /*  0 */ "NONE"_Stz,
  
  /*  0 */ "MA_REQ_BEGIN"_Stz,
  /*  0 */ "MA_REQ_END"_Stz,

  /*    */ "ENUMS"_Stz,
};

static Stz* aMA_short[]
{
  /*  0 */ "NONE"_Stz,
  
  /*    */ "Request begin"_Stz,
  /*    */ "Request end"_Stz,
  
  /*    */ "ENUMS"_Stz,
};


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
