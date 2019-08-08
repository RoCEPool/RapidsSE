/*============================================================================*/
/* Sql.cpp -- Sql Reformatter                                                 */
/*============================================================================*/

#include "Verbs.h"

/*----------------------------------------------------------------------------*/
/* reformat token vector for sql statement                                    */
/*----------------------------------------------------------------------------*/
bool Sql::bReformat(TokenVec* pTknVec)
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
bool Sql::bRestate(TokenVec* pTknVec)
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
