/*============================================================================*/
/* Cue.cpp --  Cue (litle prompt)                                             */
/*============================================================================*/

#include "BASE.h"

#include "Cue.hpp"

/*----------------------------------------------------------------------------*/
/* get input                                                                 */
/*----------------------------------------------------------------------------*/
void Cue::vGetInput(Stz* szPrompt, String* pString)
{
  vTrace("%s"_Stz,(char*)szPrompt);
  
  //fgets(pString->azStr(),(i4)pString->nLength(),stdin);
  
  char *azLine = nullptr;
  size_t iLineCapacity = 0;

  i8 iCharsRead = getline(&azLine,&iLineCapacity,stdin);
  UNUSED(iCharsRead);

  //printf(">>`%s`<<\n",azLine);

  pString->vCopy((Stz*)azLine);
}

/*----------------------------------------------------------------------------*/
/* get return                                                                 */
/*----------------------------------------------------------------------------*/
void Cue::vGetReturn(void)
{
  s256 sIn;
  char* pcResult = fgets(sIn.azChar(),128,stdin);
  UNUSED(pcResult);
}

/*----------------------------------------------------------------------------*/
/* get return                                                                 */
/*----------------------------------------------------------------------------*/
void Cue::vGetReturn(Stz* szPrompt)
{
  vTrace("%s"_Stz,(char*)szPrompt);
  
  s256 sIn;
  char* pcResult = fgets(sIn.azChar(),128,stdin);
  UNUSED(pcResult);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
