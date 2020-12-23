/*============================================================================*/
/* GraphNav.cpp - a utility for walking plan graphs                           */
/*============================================================================*/

#include "Parser.h"


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
  if (pNode->getName()->bEqual(const_cast<Stz*>(C_ROOT_NAME))) {
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

  sPath.vConcat(pNode->getName());
  i8 nLen = sPath.nLength();

  // Visit the current node
  b1 bRetVal = fVisitor(pNode, sPath);

  // If the function returned false then we don't need to look at any other nodes
  if (!bRetVal) {
    return false;
  }

  // Any properties to walk down?
  if (bWalkProperties && pNode->bHasProperties()) {
    PlanNode* pProp = pNode->pFirstProperty();

    while (pProp) {
      // Don't forget to clip the path and add the appropriate separator before each node
      sPath.vClip(nLen);
      sPath.vConcat(const_cast<Stz*>(C_PROPERTY_SEPARATOR));

      bRetVal = bWalk(pProp, bWalkProperties, bWalkChildren, fVisitor, sPath);

      if (!bRetVal) {
        return false;
      }

      pProp = pProp->pNextItem();
    }
  }

  // Any children to walk down?
  if (bWalkChildren && pNode->bHasChildren()) {
    PlanNode* pChild = pNode->pFirstChild();

    while (pChild) {
      // Don't forget to clip the path and add the appropriate separator before each node
      sPath.vClip(nLen);
      sPath.vConcat(const_cast<Stz*>(C_CHILD_SEPARATOR));

      bRetVal = bWalk(pChild, bWalkProperties, bWalkChildren, fVisitor, sPath);

      if (!bRetVal) {
        return false;
      }

      pChild = pChild->pNextItem();
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
    if(!pNode->getName()->bEqual(const_cast<Stz*>(C_ROOT_NAME))) {
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
    if (pNode->getName()->bEqual(szPath)) {
      return pNode;

    } else {
      return nullptr;
    }
  }

  // There is a subpath to be checked. But first confirm that the name
  // of pNode matches the leading subset of szPath.
  if (!(szPath->bBeginsWith(pNode->getName()) && pNode->getName()->nLength() == iEnd)) {
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

  // Check against the children
  if (cSep == '/') {
    pItem = pNode->pFirstChild();

    // Check against the properties
  } else if (cSep == ':') {
    pItem = pNode->pFirstProperty();

  } else {
    NEVER_REACH
  }

  // We need to check all properties or children before giving up
  PlanNode* pResult;

  while (pItem) {
    pResult = pFind(szNextPath, pItem);
    if (pResult) {
      return pResult;
    }

    pItem = pItem->pNextItem();
  }

  // No matches found
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
