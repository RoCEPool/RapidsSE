/*============================================================================*/
/* eDR.h -- De-Reference Action                                               */
/*============================================================================*/
#pragma once

enum eDR // De-Reference Action
{
  /* 0 */ DR_NONE=0,
  
  /* 1 */ DR_DELETE,
  /* 2 */ DR_DEQUEUE,
  /* 3 */ DR_DEREF,
  /* 4 */ DR_MALLOC,
  
  /* 5 */ DR_ENUMS
};

static Stz* aDR[]
{
  /* 0 */ "DR_NONE"_Stz,
  
  /* 1 */ "DR_DELETE"_Stz,
  /* 2 */ "DR_DEQUEUE"_Stz,
  /* 3 */ "DR_DEREF"_Stz,
  /* 4 */ "DR_MALLOC"_Stz

  /* 5 */ "DR_ENUMS"_Stz
};

static Stz* aDR_short[]
{
  /* 0 */ "NONE"_Stz,
    
  /* 1 */ "delete"_Stz,
  /* 2 */ "dequeue"_Stz,
  /* 3 */ "deref"_Stz,
  /* 4 */ "malloc"_Stz,
  
  /* 5 */ "ENUMS"_Stz
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
