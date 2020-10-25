/*============================================================================*/
/* eIU.h -- Insert Upwards reason                                             */
/*============================================================================*/
#pragma once

enum eIU // Insert Upwards
{
  /* 0 */ IU_NONE=0,
  /* 1 */ IU_ALONG_LEFT_LEAD,
  /* 2 */ IU_ALONG_RIGHT_LEAD,
  /* 3 */ IU_DATA_ENTRY,
  /* 4 */ IU_HIGH_LEFT_LEAD,
  /* 5 */ IU_SPLIT_LEFT_LEAD,
  /* 6 */ IU_SPLIT_RIGHT_LEAD
};

static Stz* aIU[]
{
  /* 0 */ "IU_NONE"_Stz,
  /* 1 */ "IU_ALONG_LEFT_LEAD"_Stz,
  /* 2 */ "IU_ALONG_RIGHT_LEAD"_Stz,
  /* 3 */ "IU_DATA_ENTRY"_Stz,
  /* 5 */ "IU_HIGH_LEFT_LEAD"_Stz,
  /* 6 */ "IU_SPLIT_LEFT_LEAD"_Stz,
  /* 6 */ "IU_SPLIT_RIGHT_LEAD"_Stz
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
