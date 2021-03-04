/*============================================================================*/
/* GraphNav.cpp - a utility for walking plan graphs                           */
/*============================================================================*/

#include "BASE.h"
#include "GraphNav.hpp"
#include "PlanNode.hpp"
#include "QueryNode.hpp"
#include "DefinitionNode.hpp"
#include "PrimitiveNode.hpp"
#include "PlanConstants.hpp"

using namespace Parser;


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
 * function for each PlanNode visited (i.e., it's a visitor pattern). This is a
 * TOP-DOWN, depth-first traversal.
 * This function returns a boolean where true indicates that it walked every
 * node and false indicates that it stopped walking nodes early (on request
 * of the function).
 *
 * Params:
 *   pNode    - The node of the graph to start with (may not be the root).
 *   fVisitor - A function that is called for each node visited. This function
 *              must have the following definition:
 *                 eWalkCtrl func(PlanNode* pNode, s256& sPath, i8 iDepth, void* pvArg)
 *              The function should return the appropriate enum value to control
 *              the continuing traversal of the graph.
 * Returns: false if the function aborted the walk prematurely otherwise true.
 *
 * NOTES:
 *
 *
 * Functions or C++11 lambdas can be passed to this function. Using lambdas
 * can often be easier than creating a new class to specifically overload
 * the operator().                                                            */
/*----------------------------------------------------------------------------*/
b1 GraphNav::bWalk(PlanNode* pNode,
                   std::function<eWalkCtrl(PlanNode*, s256&, i8, void*)> fVisitor,
                   void* pvArg)
{
  return bWalk(pNode, true, true, fVisitor, pvArg);
}

/*----------------------------------------------------------------------------*/
/* Walks a node graph (including all properties and children) and calls a
 * function for each PlanNode visited (i.e., it's a visitor pattern). This is a
 * BOTTOM-UP, depth-first traversal.
 * This function returns a boolean where true indicates that it walked every
 * node and false indicates that it stopped walking nodes early (on request
 * of the function).
 *
 * Params:
 *   pNode    - The node of the graph to start with (may not be the root).
 *   fVisitor - A function that is called for each node visited. This function
 *              must have the following definition:
 *                 b1 func(PlanNode* pNode, s256& sPath, i8 iDepth, void* pvArg)
 *              The function should return the appropriate enum value to control
 *              the continuing traversal of the graph.
 * Returns: false if the function aborted the walk prematurely otherwise true.
 *
 * NOTES:
 *
 *
 * Functions or C++11 lambdas can be passed to this function. Using lambdas
 * can often be easier than creating a new class to specifically overload
 * the operator().                                                            */
/*----------------------------------------------------------------------------*/
b1 GraphNav::bWalkBottomUp(PlanNode* pNode,
                           std::function<eWalkCtrl(PlanNode*, s256&, i8, void*)> fVisitor,
                           void* pvArg)
{
  return bWalkBottomUp(pNode, true, true, fVisitor, pvArg);
}

/*----------------------------------------------------------------------------*/
/* Walks a node graph (including properties and children if desired) and calls a
 * function for each PlanNode visited (i.e., it's a visitor pattern). This is a
 * TOP-DOWN, depth-first traversal.
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
 *                 eWalkCtrl func(PlanNode* pNode, s256& sPath, i8 iDepth, void* pvArg)
 *              The function should return the appropriate enum value to control
 *              the continuing traversal of the graph.
 *   pvArg - An optional argument to be passed to the visitor function.
 * Returns: false if the function aborted the walk prematurely otherwise true.
 *
 * NOTES:
 * Functions or C++11 lambdas can be passed to this function. Using lambdas
 * can often be easier than creating a new class to specifically overload
 * the operator().                                                            */
/*----------------------------------------------------------------------------*/
b1 GraphNav::bWalk(PlanNode* pNode, b1 bWalkProperties, b1 bWalkChildren,
                   std::function<eWalkCtrl(PlanNode*, s256&, i8, void*)> fVisitor, void* pvArg)
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

  return bWalk(pNode, bWalkProperties, bWalkChildren, fVisitor, pvArg, sPath, 0);
}

/*----------------------------------------------------------------------------*/
/* Walks a node graph (including properties and children if desired) and calls a
 * function for each PlanNode visited (i.e., it's a visitor pattern). This is a
 * BOTTOM-UP, depth-first traversal.
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
 *                 eWalkCtrl func(PlanNode* pNode, s256& sPath, i8 iDepth, void* pvArg)
 *              The function should return the appropriate enum value to control
 *              the continuing traversal of the graph.
 *   pvArg - An optional argument to be passed to the visitor function.
 * Returns: false if the function aborted the walk prematurely otherwise true.
 *
 * NOTES:
 * Functions or C++11 lambdas can be passed to this function. Using lambdas
 * can often be easier than creating a new class to specifically overload
 * the operator().                                                            */
/*----------------------------------------------------------------------------*/
b1 GraphNav::bWalkBottomUp(PlanNode* pNode, b1 bWalkProperties, b1 bWalkChildren,
                           std::function<eWalkCtrl(PlanNode*, s256&, i8, void*)> fVisitor, void* pvArg)
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

  return bWalkBottomUp(pNode, bWalkProperties, bWalkChildren, fVisitor, pvArg, sPath, 0);
}

/*----------------------------------------------------------------------------*/
/* Walks a node graph (including properties and children if desired) and calls a
 * function for each PlanNode visited (i.e., it's a visitor pattern). This is a
 * TOP-DOWN, depth-first traversal.
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
 *                 eWalkCtrl func(PlanNode* pNode, s256& sPath, i8 iDepth, void* pvArg)
 *              The function should return the appropriate enum value to control
 *              the continuing traversal of the graph.
 *   pvArg - An optional argument to be passed to the visitor function.
 *   sPath - the path (absolute or relative) of the parent node. The current
 *           node should be appended to this and cleared before returning.
 *   iDepth - the depth of the current node, starting at 0 for the first node.
 * Returns: false if the function aborted the walk prematurely otherwise true.
 *
 * NOTES:
 * Functions or C++11 lambdas can be passed to this function. Using lambdas
 * can often be easier than creating a new class to specifically overload
 * the operator().                                                            */
/*----------------------------------------------------------------------------*/
b1 GraphNav::bWalk(PlanNode* pNode, b1 bWalkProperties, b1 bWalkChildren,
                   std::function<eWalkCtrl(PlanNode*, s256&, i8, void*)> fVisitor,
                   void* pvArg, s256& sPath, i8 iDepth)
{
  if (!pNode) {
    return true;
  }

  sPath.vConcat(pNode->szName());
  i8 nLen = sPath.nLength();

  // Visit the current node
  eWalkCtrl eCtrl = fVisitor(pNode, sPath, iDepth, pvArg);

  if (eCtrl == WALK_ABORT) {
    return false;
  }

  if (eCtrl == WALK_SKIP_SUBTREE) {
    return true;
  }

  b1 bRetVal;

  // Any properties to walk down?
  if (eCtrl != WALK_SKIP_PROPERTIES && bWalkProperties && pNode->bHasProperties()) {

    PlanNodeIter* pIter = pNode->pNewPropertyIter();
    try {
      while (pIter->bHasNext()) {
        PlanNode *pProp = pIter->pNext();

        // Don't forget to clip the path and add the appropriate separator before each node
        sPath.vClip(nLen);
        sPath.vConcat(StrLib::szCharToStr(C_PROPERTY_SEPARATOR));

        bRetVal = bWalk(pProp, bWalkProperties, bWalkChildren, fVisitor, pvArg, sPath, iDepth + 1);

        if (!bRetVal) {
          delete pIter;
          return false;
        }
      }
      delete pIter;

    } catch (std::runtime_error& e) {
      delete pIter;
      throw;
    }
  }

  // Any children to walk down?
  if (eCtrl != WALK_SKIP_CHILDREN && bWalkChildren && pNode->bHasChildren()) {

    PlanNodeIter* pIter = pNode->pNewChildrenIter();
    try {
      while (pIter->bHasNext()) {
        PlanNode *pChild = pIter->pNext();

        // Don't forget to clip the path and add the appropriate separator before each node
        sPath.vClip(nLen);
        sPath.vConcat(StrLib::szCharToStr(C_CHILD_SEPARATOR));

        bRetVal = bWalk(pChild, bWalkProperties, bWalkChildren, fVisitor, pvArg, sPath, iDepth + 1);

        if (!bRetVal) {
          delete pIter;
          return false;
        }
      }
      delete pIter;

    } catch (std::runtime_error& e) {
      delete pIter;
      throw;
    }
  }

  return true;
}


/*----------------------------------------------------------------------------*/
/* Walks a node graph (including properties and children if desired) and calls a
 * function for each PlanNode visited (i.e., it's a visitor pattern). This is a
 * BOTTOM-UP, depth-first traversal.
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
 *                 eWalkCtrl func(PlanNode* pNode, s256& sPath, i8 iDepth, void* pvArg)
 *              The function should return the appropriate enum value to control
 *              the continuing traversal of the graph.
 *   pvArg - An optional argument to be passed to the visitor function.
 *   sPath - the path (absolute or relative) of the parent node. The current
 *           node should be appended to this and cleared before returning.
 *   iDepth - the depth of the current node, starting at 0 for the first node.
 * Returns: false if the function aborted the walk prematurely otherwise true.
 *
 * NOTES:
 * Functions or C++11 lambdas can be passed to this function. Using lambdas
 * can often be easier than creating a new class to specifically overload
 * the operator().                                                            */
/*----------------------------------------------------------------------------*/
b1 GraphNav::bWalkBottomUp(PlanNode* pNode, b1 bWalkProperties, b1 bWalkChildren,
                           std::function<eWalkCtrl(PlanNode*, s256&, i8, void*)> fVisitor,
                           void* pvArg, s256& sPath, i8 iDepth)
{
  if (!pNode) {
    return true;
  }

  sPath.vConcat(pNode->szName());
  i8 nLen = sPath.nLength();

  b1 bRetVal;

  // Any properties to walk down?
  if (bWalkProperties && pNode->bHasProperties()) {

    PlanNodeIter* pIter = pNode->pNewPropertyIter();
    try {
      while (pIter->bHasNext()) {
        PlanNode *pProp = pIter->pNext();

        // Don't forget to clip the path and add the appropriate separator before each node
        sPath.vClip(nLen);
        sPath.vConcat(StrLib::szCharToStr(C_PROPERTY_SEPARATOR));

        bRetVal = bWalkBottomUp(pProp, bWalkProperties, bWalkChildren, fVisitor, pvArg, sPath, iDepth + 1);

        if (!bRetVal) {
          delete pIter;
          return false;
        }
      }
      delete pIter;

    } catch (std::runtime_error& e) {
      delete pIter;
      throw;
    }
  }

  // Any children to walk down?
  if (bWalkChildren && pNode->bHasChildren()) {

    PlanNodeIter* pIter = pNode->pNewChildrenIter();
    try {
      while (pIter->bHasNext()) {
        PlanNode *pChild = pIter->pNext();

        // Don't forget to clip the path and add the appropriate separator before each node
        sPath.vClip(nLen);
        sPath.vConcat(StrLib::szCharToStr(C_CHILD_SEPARATOR));

        bRetVal = bWalkBottomUp(pChild, bWalkProperties, bWalkChildren, fVisitor, pvArg, sPath, iDepth + 1);

        if (!bRetVal) {
          delete pIter;
          return false;
        }
      }
      delete pIter;

    } catch (std::runtime_error& e) {
      delete pIter;
      throw;
    }
  }

  // Now visit the current node
  sPath.vClip(nLen);
  eWalkCtrl eCtrl = fVisitor(pNode, sPath, iDepth, pvArg);

  switch (eCtrl) {
    case WALK_ABORT:
      return false;

    case WALK_CONTINUE:
      return true;

    case WALK_SKIP_SUBTREE:     // meaningless for a bottom-up traversal
    case WALK_SKIP_CHILDREN:    // meaningless for a bottom-up traversal
    case WALK_SKIP_PROPERTIES:  // meaningless for a bottom-up traversal
      THROW("Invalid eWalkCtrl values for a bottom-up traversal of the plan graph"_Stz)

    default:
      NEVER_REACH
  }
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

  ASSERT(!szPath->bEqual(StrLib::szCharToStr(C_CHILD_SEPARATOR)));
  ASSERT(!szPath->bEqual(StrLib::szCharToStr(C_PROPERTY_SEPARATOR)));

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
  if (cSep == '/' && pNode->bHasChildren()) {

    PlanNodeIter* pIter = pNode->pNewChildrenIter();
    while (pIter->bHasNext()) {
      pItem = pIter->pNext();

      pResult = pFind(szNextPath, pItem);
      if (pResult) {
        delete pIter;
        return pResult;
      }
    }
    delete pIter;

    // Check against the properties
  } else if (cSep == ':' && pNode->bHasProperties()) {

    PlanNodeIter* pIter = pNode->pNewPropertyIter();
    while (pIter->bHasNext()) {
      pItem = pIter->pNext();

      pResult = pFind(szNextPath, pItem);
      if (pResult) {
        delete pIter;
        return pResult;
      }
    }
    delete pIter;

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

  if (!pNode->bHasProperties()) {
    return nullptr;
  }

  PlanNodeIter* pIter = pNode->pNewPropertyIter();
  while (pIter->bHasNext()) {
    PlanNode* pItem = pIter->pNext();

    if (pItem->szName()->bEqual(szPropName)) {
      delete pIter;
      return pItem;
    }
  }

  delete pIter;
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

  if (!pNode->bHasChildren()) {
    return nullptr;
  }

  PlanNodeIter* pIter = pNode->pNewChildrenIter();
  while (pIter->bHasNext()) {
    PlanNode* pItem = pIter->pNext();

    if (pItem->szName()->bEqual(szChildName)) {
      delete pIter;
      return pItem;
    }
  }

  delete pIter;
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


/*----------------------------------------------------------------------------*/
/* Returns the parent node of a given definition node                         */
/*----------------------------------------------------------------------------*/
PlanNode* GraphNav::pGetParent(DefinitionNode* pDefnNode, PlanNode* pRootNode)
{
  ASSERT(pDefnNode);
  ASSERT(pDefnNode->szPath());

  // Get the path of the parent object
  s2K sParentPathBuf;
  if (!bParentPath(pDefnNode->szPath(), sParentPathBuf)) {
    return nullptr;
  }

  PlanNode* pParentNode = GraphNav::pFind(sParentPathBuf, pRootNode);
  return pParentNode;
}

/*----------------------------------------------------------------------------*/
/* Returns the parent node of a given primitive node                          */
/*----------------------------------------------------------------------------*/
PlanNode* GraphNav::pGetParent(PrimitiveNode* pPrimNode, PlanNode* pRootNode)
{
  ASSERT(pPrimNode);
  ASSERT(pPrimNode->szPath());

  // Get the path of the parent object
  s2K sParentPathBuf;
  if (!bParentPath(pPrimNode->szPath(), sParentPathBuf)) {
    return nullptr;
  }

  PlanNode* pParentNode = GraphNav::pFind(sParentPathBuf, pRootNode);
  return pParentNode;
}


/*----------------------------------------------------------------------------*/
/* Given a path, write the parent path to sParentBuf. Return false if there   */
/* isn't a parent.                                                            */
/*----------------------------------------------------------------------------*/
b1 GraphNav::bParentPath(Stz *szPath, s2K &sParentBuf)
{
  ASSERT(szPath);
  char* pChar = StrLib::acLast(szPath->azChar());

  while (pChar != szPath->azChar()) {
    if (*pChar == C_CHILD_SEPARATOR || *pChar == C_PROPERTY_SEPARATOR) {

      i8 nLen = pChar - szPath->azChar(); // not inclusive of last char
      sParentBuf.vCopy(szPath, nLen);
      return true;
    }

    --pChar;
  }

  return false;
}

/*----------------------------------------------------------------------------*/
/* Walks the plan graph to find the given query node. Saves the path and depth*/
/*----------------------------------------------------------------------------*/
void GraphNav::vGetPathAndDepth(QueryNode* pPlanNode, PlanNode* pRootNode, s256& sPathBuf, i8& iDepth)
{
  ASSERT(pPlanNode);
  ASSERT(pRootNode);

  iDepth == -1;

  auto fVisitor = [pPlanNode, &sPathBuf, &iDepth](Parser::PlanNode *pNode, s256 &sWalkPath, i8 iWalkDepth, void *pvArg) -> Parser::GraphNav::eWalkCtrl {

    // Only interested in /__root__/plan subtree
    if (iWalkDepth == 1 && !sWalkPath.bEqual(const_cast<Stz*>(C_PLAN_PATH))) {
      return GraphNav::WALK_SKIP_SUBTREE;
    }

    // Did we get a match?
    if (pNode == pPlanNode) {
      iDepth = iWalkDepth;
      sPathBuf.vCopy(sWalkPath.szStz());
      return GraphNav::WALK_ABORT;
    }

    return GraphNav::WALK_CONTINUE;
  };

  if (iDepth != -1) {
    return;
  }

  // We couldn't find the node in the graph
  NEVER_REACH
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
