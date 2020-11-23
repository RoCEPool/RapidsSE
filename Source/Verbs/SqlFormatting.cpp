/*============================================================================*/
/* SqlFormatting.cpp -- Sql Reformat/Restate                                  */
/*============================================================================*/

#include "Verbs.h"

/*----------------------------------------------------------------------------*/
/* reformat token vector for sql statement                                    */
/*----------------------------------------------------------------------------*/
bool SqlFormatting::bReformat(TokenVec* pTknVec)
{
  bool bWasReformatted = false;
  
  if (Select::bIsReformatable(pTknVec)) {
    bWasReformatted = Select::bReformat(pTknVec);
    return bWasReformatted;
  }
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* restate token vector for sql statement                                     */
/*----------------------------------------------------------------------------*/
bool SqlFormatting::bRestate(TokenVec* pTknVec)
{
  bool bWasRestated = false;
  
  if (Select::bIsRestatable(pTknVec)) {
    bWasRestated = Select::bRestate(pTknVec);
    return bWasRestated;
  }
  
  return false;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
