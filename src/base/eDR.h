/*============================================================================*/
/* eDR.h -- De-Reference Action                                               */
/*============================================================================*/
#pragma once

enum eDR // De-Reference Action
{
  /* 0 */ DR_NONE=0,
  
  /* 1 */ DR_ENQUEUE,
  /* 2 */ DR_DELETE,
  /* 3 */ DR_DEQUEUE,
  /* 4 */ DR_DEREF,
  /* 5 */ DR_NEW,
  
  /* 6 */ DR_ENUMS
};

static Stz* aDR[]
{
  /* 0 */ "DR_NONE"_Stz,
  
  /* 1 */ "DR_ENQUEUE"_Stz,
  /* 2 */ "DR_DELETE"_Stz,
  /* 3 */ "DR_DEQUEUE"_Stz,
  /* 4 */ "DR_DEREF"_Stz,
  /* 5 */ "DR_NEW"_Stz

  /* 6 */ "DR_ENUMS"_Stz
};

static Stz* aDR_short[]
{
  /* 0 */ "NONE"_Stz,
    
  /* 1 */ "enqueue"_Stz,
  /* 2 */ "delete"_Stz,
  /* 3 */ "dequeue"_Stz,
  /* 4 */ "deref"_Stz,
  /* 5 */ "new"_Stz,
  
  /* 6 */ "ENUMS"_Stz
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
