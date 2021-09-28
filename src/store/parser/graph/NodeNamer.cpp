/*============================================================================*/
/* NodeNamer.hpp - Gives nodes unique names                                   */
/*============================================================================*/

#include "BASE.h"
#include "NodeNamer.hpp"
#include "QueryNode.hpp"

using namespace Parser;

/*----------------------------------------------------------------------------*/
/*-- Constants ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
// Static
Atomic* NodeNamer::ms_pObjId = new Atomic();


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Creates a new unique name for the given node                               */
/*----------------------------------------------------------------------------*/
void NodeNamer::vGenerateName(QueryNode* pNode, s256& sNameBuf)
{
  ASSERT(pNode);

  NodeNamer::vGenerateName(pNode->szType(), sNameBuf);
}

/*----------------------------------------------------------------------------*/
/* Creates a new unique name for the given node type                          */
/*----------------------------------------------------------------------------*/
void NodeNamer::vGenerateName(Stz* szNodeType, s256& sNameBuf)
{
  ASSERT(szNodeType);

  // Get the last leaf of the definition type. e.g. given /a/b/c this gets 'c'
  char* ps = StrLib::acLast(szNodeType->azChar());
  b1 bFound = StrLib::bBackward(&ps, szNodeType->azChar(), '/');

  Stz* szPrefix;

  if (bFound) {
    szPrefix = (Stz*) (ps + 1);

  } else {
    szPrefix = szNodeType;
  }

  sNameBuf.vSprint("%s%04lld"_Stz, szPrefix, ms_pObjId->iInc());
}



/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

