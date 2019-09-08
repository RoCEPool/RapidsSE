/*============================================================================*/
/* eCA.h -- Request/Command Actions                                            */
/*============================================================================*/
#pragma once

enum eCA // request/command actions
{
  /*  0 */ CA_REQ_NONE=0,
  
  /*    */ CA_REQ_CONSTRUCT,
  /*    */ CA_REQ_DESTRUCT,

  /*    */ CA_REQ_ENUMS,
};

static Stz* aCA[]
{
  /*  0 */ "NONE"_Stz,
  
  /*  0 */ "CA_REQ_BEGIN"_Stz,
  /*  0 */ "CA_REQ_END"_Stz,

  /*    */ "ENUMS"_Stz,
};

static Stz* aCA_short[]
{
  /*  0 */ "NONE"_Stz,
  
  /*    */ "Request begin"_Stz,
  /*    */ "Request end"_Stz,
  
  /*    */ "ENUMS"_Stz,
};


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
