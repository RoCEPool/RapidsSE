/*============================================================================*/
/* GraphNav.cpp - a utility for walking plan graphs                           */
/*============================================================================*/

#include "BASE.h"
#include "STORE.h"



/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
__pCastFunction(Parser, GraphNav, CN_GRAPHNAV)
__aNewFunction(GraphNav, CN_GRAPHNAV)
__vDeleteFunction(GraphNav, CN_GRAPHNAV)
__aFooterFunction(GraphNav)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
GraphNav::GraphNav()
    : Object(CN_GRAPHNAV)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_GRAPHNAV, _FOOTER_);

  _checkInitialised_(CN_GRAPHNAV);

  _postcondition_(_ADDRESS_, _MAGIC_, CN_GRAPHNAV, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
GraphNav::~GraphNav(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_GRAPHNAV, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Returns the leaf element of a type definition path.                        */
/* e.g. given a definition path of /a/b/c this will return 'c'.               */
/* This path must not contain a property separator (':').                     */
/*----------------------------------------------------------------------------*/
Stz* GraphNav::szLeafType(Stz* szTypePath)
{
  char* ps;

  // This is a type definition path. It should not contain the property separator.
#ifdef WITH_ASSERT_MACRO
  ps = StrLib::acLast(szTypePath->azChar());
  ASSERT(!StrLib::bBackward(&ps, szTypePath->azChar(), ':'));
#endif

  ps = StrLib::acLast(szTypePath->azChar());
  b1 bFound = StrLib::bBackward(&ps, szTypePath->azChar(), '/');
  ASSERT(bFound);

  return (Stz*) (ps + 1);
}

/*----------------------------------------------------------------------------*/
/* Returns the parent of the type definition path given.                      */
/* e.g. given a defn path of /a/b/c this will write '/a/b' into the buffer.   */
/* This path must not contain a property separator (':').                     */
/*----------------------------------------------------------------------------*/
void GraphNav::vParentType(Stz* szTypePath, s256& buf)
{
  char* ps;

  // This is a type definition path. It should not contain the property separator.
  ASSERT(!StrLib::bBackward(&ps, szTypePath->azChar(), ':'));

  b1 bFound = StrLib::bBackward(&ps, szTypePath->azChar(), '/');

  if (bFound && ps != szTypePath->azChar()) {
    i8 nLen = ps - szTypePath->azChar();
    buf.vCopy(szTypePath, nLen);
  }
}


/*----------------------------------------------------------------------------*/
/* Walks a node graph (including all properties and children) and calls a
 * function for each PlanNode visited (i.e., it's a visitor pattern).
 * This function returns a boolean where true indicates that it walked every
 * node and false indicates that it stopped walking nodes early (on request
 * of the function).
 *
 * Params:
 *   pNode    - The node of the graph to start with (may not be the root).
 *   fVisitor - A function that is called for each node visited. This function
 *              must have the following definition:
 *                 b1 func(PlanNode* pNode, s256& sPath)
 *              The function should return false if it wishes the walk operation
 *              to abort early, or return true if the walk is to continue
 *              walking other nodes.
 * Returns: true if all desired nodes were walked or false if the function
 *          aborted the walk prematurely.
 *
 * NOTES:
 *
 *
 * Functions or C++11 lambdas can be passed to this function. Using lambdas
 * can often be easier than creating a new class to specifically overload
 * the operator().                                                            */
/*----------------------------------------------------------------------------*/
b1 GraphNav::bWalk(PlanNode* pNode, std::function<b1(PlanNode*, s256&)> fVisitor)
{
  return bWalk(pNode, true, true, fVisitor);
}

/*----------------------------------------------------------------------------*/
/* Walks a node graph (including properties and children if desired) and calls a
 * function for each PlanNode visited (i.e., it's a visitor pattern).
 * This function returns a boolean where true indicates that it walked every
 * node and false indicates that it stopped walking nodes early (on request
 * of the function).
 *
 * Params:
 *   pNode    - The node of the graph to start with (may not be the root).
 *   bWalkProperties - If true then any property nodes will be walked as well.
 *                     If false then any property nodes will be skipped.
 *   bWalkChildren - If true then any child nodes will be walked as well.
 *                   If false then any child nodes will be skipped.
 *   fVisitor - A function that is called for each node visited. This function
 *              must have the following definition:
 *                 b1 func(PlanNode* pNode, s256& sPath)
 *              The function should return false if it wishes the walk operation
 *              to abort early, or return true if the walk is to continue
 *              walking other nodes.
 * Returns: true if all desired nodes were walked or false if the function
 *          aborted the walk prematurely.
 *
 * NOTES:
 * Functions or C++11 lambdas can be passed to this function. Using lambdas
 * can often be easier than creating a new class to specifically overload
 * the operator().                                                            */
/*----------------------------------------------------------------------------*/
b1 GraphNav::bWalk(PlanNode* pNode, b1 bWalkProperties, b1 bWalkChildren, std::function<b1(PlanNode*, s256&)> fVisitor)
{
  if (!pNode) {
    return true;
  }

  s256 sPath;

  // We may not be starting with the root node. If we are then set the path to "/"
  // so that each node can append its name appropriately.
  if (pNode->szName()->bEqual(const_cast<Stz*>(C_ROOT_NAME))) {
    sPath.vCopy("/"_Stz);

  } else {
    // We don't know where we're starting, so it will be a relative path
  }

  return bWalk(pNode, bWalkProperties, bWalkChildren, fVisitor, sPath);
}

/*----------------------------------------------------------------------------*/
/* Walks a node graph (including properties and children if desired) and calls a
 * function for each PlanNode visited (i.e., it's a visitor pattern).
 * This function returns a boolean where true indicates that it walked every
 * node and false indicates that it stopped walking nodes early (on request
 * of the function).
 *
 * Params:
 *   pNode    - The node of the graph to start with (may not be the root).
 *   bWalkProperties - If true then any property nodes will be walked as well.
 *                     If false then any property nodes will be skipped.
 *   bWalkChildren - If true then any child nodes will be walked as well.
 *                   If false then any child nodes will be skipped.
 *   fVisitor - A function that is called for each node visited. This function
 *              must have the following definition:
 *                 b1 func(PlanNode* pNode, s256& sPath)
 *              The function should return false if it wishes the walk operation
 *              to abort early, or return true if the walk is to continue
 *              walking other nodes.
 *   sPath - the path (absolute or relative) of the parent node. The current
 *           node should be appended to this and cleared before returning.
 * Returns: true if all desired nodes were walked or false if the function
 *          aborted the walk prematurely.
 *
 * NOTES:
 * Functions or C++11 lambdas can be passed to this function. Using lambdas
 * can often be easier than creating a new class to specifically overload
 * the operator().                                                            */
/*----------------------------------------------------------------------------*/
b1 GraphNav::bWalk(PlanNode* pNode, b1 bWalkProperties, b1 bWalkChildren, std::function<b1(PlanNode*, s256&)> fVisitor, s256& sPath)
{
  if (!pNode) {
    return true;
  }

  sPath.vConcat(pNode->szName());
  i8 nLen = sPath.nLength();

  // Visit the current node
  b1 bRetVal = fVisitor(pNode, sPath);

  // If the function returned false then we don't need to look at any other nodes
  if (!bRetVal) {
    return false;
  }

  // Any properties to walk down?
  if (bWalkProperties) {
    i8 nProperties = pNode->nProperties();

    // TODO: replace with something more efficient
    for (i8 i=0; i<nProperties; i++) {
      PlanNode* pProp = pNode->pNthProperty(i);

      // Don't forget to clip the path and add the appropriate separator before each node
      sPath.vClip(nLen);
      sPath.vConcat(const_cast<Stz*>(C_PROPERTY_SEPARATOR));

      bRetVal = bWalk(pProp, bWalkProperties, bWalkChildren, fVisitor, sPath);

      if (!bRetVal) {
        return false;
      }
    }
  }

  // Any children to walk down?
  if (bWalkChildren) {
    i8 nChildren = pNode->nChildren();

    // TODO: replace with something more efficient
    for (i8 i=0; i<nChildren; i++) {
      PlanNode* pChild = pNode->pNthChild(i);

      // Don't forget to clip the path and add the appropriate separator before each node
      sPath.vClip(nLen);
      sPath.vConcat(const_cast<Stz*>(C_CHILD_SEPARATOR));

      bRetVal = bWalk(pChild, bWalkProperties, bWalkChildren, fVisitor, sPath);

      if (!bRetVal) {
        return false;
      }
    }
  }

  return true;
}


/*----------------------------------------------------------------------------*/
/* Looks up and returns a PlanNode* for a given path string. Returns a nullptr*/
/* if a path for the entry cannot be found.                                   */
/* Params:                                                                    */
/*  - szPath: The absolute or relative path to look up. Absolute paths begin  */
/*            with a '/' and the pNode must be the root node. Relative paths  */
/*            begin with the name of the pNode that was passed.               */
/*  - pNode:  The node to begin searching from.                               */
/* Returns:   A valid PlanNode* if the path can be found, or nullptr if the   */
/*            path does not match.                                            */
/* Notes:     The '/' character can be used to separate a parent from a child */
/*            while the ':' character can be used to separate a parent from   */
/*            a property.                                                     */
/*----------------------------------------------------------------------------*/
PlanNode* GraphNav::pFind(Stz* szPath, PlanNode* pNode)
{
  // Example: Find the node corresponding to the childCount constraint
  //          for a sum aggregate expression definition.
  //
  // Stz* pzPath = "/__root__/definitions/object/expr/operator/aggregation/sum:__constraints/childCount"_Stz;
  // PlanNode* pFound = GraphNav::pFind(szPath, pRootNode);

  ASSERT(pNode);
  ASSERT(szPath->nLength() > 0);

  // Corner case for handling an absolute path
  if (szPath->bBeginsWith("/"_Stz)) {
    // We've got an absolute path
    if(!pNode->szName()->bEqual(const_cast<Stz*>(C_ROOT_NAME))) {
      return nullptr;
    }

    Stz* szNextPath = (Stz*)(szPath->azChar() + 1);

    PlanNode* pResult = pFind(szNextPath, pNode);
    return pResult;
  }

  ASSERT(!szPath->bEqual(const_cast<Stz*>(C_CHILD_SEPARATOR)));
  ASSERT(!szPath->bEqual(const_cast<Stz*>(C_PROPERTY_SEPARATOR)));

  // Compare the leading edge of the path to the name of the current node.
  // To do so, find the next path separator (if any).
  i8 iEnd = iNextSeparator(szPath);

  if (iEnd == -1) {
    // There is no end separator. This szPath should compare to the name of the node.
    if (pNode->szName()->bEqual(szPath)) {
      return pNode;

    } else {
      return nullptr;
    }
  }

  // There is a subpath to be checked. But first confirm that the name
  // of pNode matches the leading subset of szPath.
  if (!(szPath->bBeginsWith(pNode->szName()) && pNode->szName()->nLength() == iEnd)) {
    // No match, so there's no point checking any further subpaths
    return nullptr;
  }

  // We've matched thus far. Get the type of separator so we know if we need to
  // look for a property or child.
  char cSep = *(szPath->azChar() + iEnd);

  // Form the next path to look up. It starts after the last separator.
  Stz* szNextPath = (Stz*)(szPath->azChar() + iEnd + 1);

  if (szNextPath->nLength() == 0)  {
    return nullptr;
  }

  PlanNode* pItem;
  PlanNode* pResult;

  // Check against the children
  if (cSep == '/') {
    i8 nChildren = pNode->nChildren();
    for (i8 i=0; i<nChildren; i++) {
      pItem = pNode->pNthChild(i);

      pResult = pFind(szNextPath, pItem);
      if (pResult) {
        return pResult;
      }
    }

    // Check against the properties
  } else if (cSep == ':') {
    i8 nProperties = pNode->nProperties();
    for (i8 i=0; i<nProperties; i++) {
      pItem = pNode->pNthProperty(i);

      pResult = pFind(szNextPath, pItem);
      if (pResult) {
        return pResult;
      }
    }

  } else {
    NEVER_REACH
  }

  // No matches found
  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* Looks for a property with a given name of the specified node and returns   */
/* it if found. Otherwise returns nullptr.                                    */
/*----------------------------------------------------------------------------*/
PlanNode* GraphNav::pFindProperty(Stz* szPropName, PlanNode* pNode)
{
  ASSERT(pNode);
  ASSERT(szPropName);
  ASSERT(szPropName->nLength() > 0);
  ASSERT(!szPropName->bContains("/"_Stz));
  ASSERT(!szPropName->bContains(":"_Stz));

  i8 nProperties = pNode->nProperties();

  // TODO: Use a more efficient mechanism here
  for (i8 i=0; i<nProperties; i++) {
    PlanNode* pItem = pNode->pNthProperty(i);

    if (pItem->szName()->bEqual(szPropName)) {
      return pItem;
    }
  }

  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* Looks for a child with a given name of the specified node and returns      */
/* it if found. Otherwise returns nullptr.                                    */
/*----------------------------------------------------------------------------*/
PlanNode* GraphNav::pFindChild(Stz* szChildName, PlanNode* pNode)
{
  ASSERT(pNode);
  ASSERT(szChildName);
  ASSERT(szChildName->nLength() > 0);
  ASSERT(!szChildName->bContains("/"_Stz));
  ASSERT(!szChildName->bContains(":"_Stz));

  i8 nChildren = pNode->nChildren();

  // TODO: Use a more efficient mechanism here
  for (i8 i=0; i<nChildren; i++) {
    PlanNode* pItem = pNode->pNthChild(i);

    if (pItem->szName()->bEqual(szChildName)) {
      return pItem;
    }
  }

  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* Returns the index of the next path separator character ('/' or ':') in a   */
/* string given a starting position (inclusive). Returns -1 if the next       */
/* separator cannot be found.                                                 */
/*----------------------------------------------------------------------------*/
i8 GraphNav::iNextSeparator(Stz* szPath)
{
  i8 iIdx = 0;
  const char* c = szPath->azChar();

  while (*c != '\0') {
    if (*c == '/' || *c == ':') {
      return iIdx;
    }
    ++iIdx;
    ++c;
  }

  return -1;
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
