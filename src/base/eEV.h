/*============================================================================*/
/* eEV.h -- Events                                                            */
/*============================================================================*/
#pragma once

enum eEV // resource actions
{
  /*  0 */ EV_NONE=0,

  /*    */ EV_CLOSED,
  /*    */ EV_CONSTRUCT,
  /*    */ EV_DELETE,
  /*    */ EV_DESTRUCT,
  /*    */ EV_INFO,
  /*    */ EV_NEW,
  /*    */ EV_OPENED,
  /*    */ EV_READ,
  /*    */ EV_RECV,
  /*    */ EV_SEEK,
  /*    */ EV_SEND,
  /*    */ EV_WRITE,

  /*    */ EV_ENUMS,
};

static Stz* aEV[]
{
  /*  0 */ "EV_NONE"_Stz,

  /*    */ "EV_CLOSED"_Stz,
  /*    */ "EV_CONSTRUCT"_Stz,
  /*    */ "EV_DELETE"_Stz,
  /*    */ "EV_DESTRUCT"_Stz,
  /*    */ "EV_INFO"_Stz,
  /*    */ "EV_NEW"_Stz,
  /*    */ "EV_OPENED"_Stz,
  /*    */ "EV_READ"_Stz,
  /*    */ "EV_RECV"_Stz,
  /*    */ "EV_SEEK"_Stz,
  /*    */ "EV_SEND"_Stz,
  /*    */ "EV_WRITE"_Stz,

  /*    */ "ENUMS"_Stz,
};

// to be removed
static Stz* aEV_short[]
{
  /*  0 */ "NONE"_Stz,
  
  /*    */ "closed"_Stz,
  /*    */ "opened"_Stz,
  
  /*    */ "ENUMS"_Stz,
};


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
