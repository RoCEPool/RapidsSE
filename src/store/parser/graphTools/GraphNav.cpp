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
#include "store/parser/shadowNodes/planObjects/planMarkers/PlanMarker.hpp"
#include "ShadowRoot.hpp"

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
  UNUSED(bFound); // in release

  return (Stz*) (ps + 1);
}

/*----------------------------------------------------------------------------*/
/* Returns the parent of the type definition path given.                      */
/* e.g. given a defn path of /a/b/c this will write '/a/b' into the buffer.   */
/* This path must not contain a property separator (':').                     */
/*----------------------------------------------------------------------------*/
void GraphNav::vParentType(Stz* szTypePath, s2K& buf)
{
  char* ps = szTypePath->azChar() + szTypePath->nSize() - 1;

  // This is a type definition path. It should not contain the property separator.
  ASSERT(!StrLib::bBackward(&ps, szTypePath->azChar(), ':'));

  b1 bFound = StrLib::bBackward(&ps, szTypePath->azChar(), '/');

  if (bFound && ps != szTypePath->azChar()) {
    i8 nLen = ps - szTypePath->azChar();
    buf.vCopy(szTypePath, nLen);
  }
}

/*----------------------------------------------------------------------------*/
/* Prints the plan graph                                                      */
/*----------------------------------------------------------------------------*/
void GraphNav::vPrintPlan(PlanNode* pRootNode)
{
  if (!ShadowRoot::bIsOfType(pRootNode)) {
    vTrace("GraphNav::vPrintPlan() needs to be passed the root node of the plan"_Stz);
    return;
  }

  QueryNode* pPlan = ShadowRoot::pPlanRoot(pRootNode);

  PlanNodeIter* pIter = pPlan->pNewChildrenIter();
  while (pIter->bHasNext()) {
    vPrintNode(pIter->pNext());
  }

  delete pIter;
}

/*----------------------------------------------------------------------------*/
/* Walks the graph from a given node and prints information to the console    */
/*----------------------------------------------------------------------------*/
void GraphNav::vPrintNode(PlanNode* pNode)
{
  vPrintNode(pNode, true);
}

/*----------------------------------------------------------------------------*/
/* Walks the graph from a given node and prints information to the console    */
/*----------------------------------------------------------------------------*/
void GraphNav::vPrintNode(PlanNode* pNode, b1 bPrintProperties)
{
  ASSERT(pNode);

  vTrace("\n\nPlan Graph:\n-----------\n\n"_Stz);

  auto fVisitor = [](PlanNode *pNode, s2K &sPath, i8 iDepth, void *pvArg) -> GraphNav::eWalkCtrl {

    UNUSED(iDepth);
    UNUSED(pvArg);

    s2K sBuf;
    pNode->xValue().vDescribe(sBuf);
    vTrace("%p %s\n    %s\n    %s\n\n"_Stz, pNode, sPath.szStz(), pNode->szType(), sBuf.szStz());

    return WALK_CONTINUE;
  };

  bGlobalWalk(pNode, bPrintProperties, true, fVisitor, nullptr);
}

/*----------------------------------------------------------------------------*/
/* This is a TOP-DOWN, depth-first graph traversal with VISITOR-callback semantics.
 * Walks a node graph (including all properties and children) and calls a
 * function for each PlanNode visited (i.e., it's a visitor pattern).
 * This function returns a boolean where true indicates that it walked every
 * node and false indicates that it stopped walking nodes early (on request
 * of the function).
 * A global walk traverses all nodes in the graph.
 *
 * Params:
 *   pNode    - The node of the graph to start with (may not be the root).
 *   fVisitor - A function that is called for each node visited. This function
 *              must have the following definition:
 *                 eWalkCtrl func(PlanNode* pNode, s2K& sPath, i8 iDepth, void* pvArg)
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
b1 GraphNav::bGlobalWalk(PlanNode* pNode,
                         std::function<eWalkCtrl(PlanNode*, s2K&, i8, void*)> fVisitor,
                         void* pvArg)
{
  return bGlobalWalk(pNode, true, true, fVisitor, pvArg);
}

/*----------------------------------------------------------------------------*/
/* This is a TOP-DOWN, depth-first graph traversal with EVENT-callback semantics.
 * Walks a node graph (including properties and children) and calls
 * the fEnterEvent function when a node is first visited (BEFORE its properties
 * and children are visited), and then calls the fExitEvent function AFTER its
 * properties and children are visited.
 * This function returns a boolean where true indicates that it walked every
 * node and false indicates that it stopped walking nodes early (on request
 * of the function).
 * A global walk traverses all nodes in the graph.
 *
 * Params:
 *   pNode    - The node of the graph to start with (may not be the root).
 *   fEnterEvent - A function that is called when the node is visited and before
 *                 any properties or children of the node are visited. This function
 *                 must have the following definition:
 *                   eWalkCtrl func(PlanNode* pNode, s2K& sPath, i8 iDepth, eWalkEvent eEventType, void* pvArg)
 *                 The function should return the appropriate enum value to control
 *                 the continuing traversal of the graph.
 *   pvArg - An optional argument to be passed to the callback functions.
 * Returns: false if the function aborted the walk prematurely otherwise true.
 *
 * NOTES:
 * Functions or C++11 lambdas can be passed to this function. Using lambdas
 * can often be easier than creating a new class to specifically overload
 * the operator(). The same lambda can be used for both events, if desired.   */
/*----------------------------------------------------------------------------*/
b1 GraphNav::bGlobalWalk(PlanNode* pNode,
                         std::function<eWalkCtrl(PlanNode*, s2K&, i8, eWalkEvent, void*)> fEnterEvent,
                         std::function<eWalkCtrl(PlanNode*, s2K&, i8, eWalkEvent, void*)> fExitEvent,
                         void* pvArg)
{
  return bGlobalWalk(pNode, true, true, fEnterEvent, fExitEvent, pvArg);
}

/*----------------------------------------------------------------------------*/
/* This is a TOP-DOWN, depth-first graph traversal with VISITOR-callback semantics.
 * Walks a node graph (including all properties and children) and calls a
 * function for each PlanNode visited (i.e., it's a visitor pattern).
 * This function returns a boolean where true indicates that it walked every
 * node and false indicates that it stopped walking nodes early (on request
 * of the function).
 * A bounded walk traverses all nodes until a PlanMarker node is encountered.
 * If the starting node is a PlanMarker then it is ignored.
 *
 * Params:
 *   pNode    - The node of the graph to start with (may not be the root).
 *   fVisitor - A function that is called for each node visited. This function
 *              must have the following definition:
 *                 eWalkCtrl func(PlanNode* pNode, s2K& sPath, i8 iDepth, void* pvArg)
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
b1 GraphNav::bBoundedWalk(PlanNode* pNode,
                         std::function<eWalkCtrl(PlanNode*, s2K&, i8, void*)> fVisitor,
                         void* pvArg)
{
  return bBoundedWalk(pNode, true, true, fVisitor, pvArg);
}


/*----------------------------------------------------------------------------*/
/* This is a TOP-DOWN, depth-first graph traversal with EVENT-callback semantics.
 * Walks a node graph (including properties and children) and calls
 * the fEnterEvent function when a node is first visited (BEFORE its properties
 * and children are visited), and then calls the fExitEvent function AFTER its
 * properties and children are visited.
 * This function returns a boolean where true indicates that it walked every
 * node and false indicates that it stopped walking nodes early (on request
 * of the function).
 * A bounded walk traverses all nodes until a PlanMarker node is encountered.
 * If the starting node is a PlanMarker then it is ignored.
 *
 * Params:
 *   pNode    - The node of the graph to start with (may not be the root).
 *   fEnterEvent - A function that is called when the node is visited and before
 *                 any properties or children of the node are visited. This function
 *                 must have the following definition:
 *                   eWalkCtrl func(PlanNode* pNode, s2K& sPath, i8 iDepth, eWalkEvent eEventType, void* pvArg)
 *                 The function should return the appropriate enum value to control
 *                 the continuing traversal of the graph.
 *   pvArg - An optional argument to be passed to the callback functions.
 * Returns: false if the function aborted the walk prematurely otherwise true.
 *
 * NOTES:
 * Functions or C++11 lambdas can be passed to this function. Using lambdas
 * can often be easier than creating a new class to specifically overload
 * the operator(). The same lambda can be used for both events, if desired.   */
/*----------------------------------------------------------------------------*/
b1 GraphNav::bBoundedWalk(PlanNode* pNode,
                          std::function<eWalkCtrl(PlanNode*, s2K&, i8, eWalkEvent, void*)> fEnterEvent,
                          std::function<eWalkCtrl(PlanNode*, s2K&, i8, eWalkEvent, void*)> fExitEvent,
                          void* pvArg)
{
  return bBoundedWalk(pNode, true, true, fEnterEvent, fExitEvent, pvArg);
}

/*----------------------------------------------------------------------------*/
/* This is a BOTTOM-UP, depth-first graph traversal with VISITOR-callback semantics.
 * Walks a node graph (including all properties and children) and calls a
 * function for each PlanNode visited (i.e., it's a visitor pattern).
 * This function returns a boolean where true indicates that it walked every
 * node and false indicates that it stopped walking nodes early (on request
 * of the function).
 * A global walk traverses all nodes in the graph.
 *
 * Params:
 *   pNode    - The node of the graph to start with (may not be the root).
 *   fVisitor - A function that is called for each node visited. This function
 *              must have the following definition:
 *                 b1 func(PlanNode* pNode, s2K& sPath, i8 iDepth, void* pvArg)
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
b1 GraphNav::bGlobalWalkBottomUp(PlanNode* pNode,
                                 std::function<eWalkCtrl(PlanNode*, s2K&, i8, void*)> fVisitor,
                                 void* pvArg)
{
  return bGlobalWalkBottomUp(pNode, true, true, fVisitor, pvArg);
}

/*----------------------------------------------------------------------------*/
/* This is a BOTTOM-UP, depth-first graph traversal with VISITOR-callback semantics.
 * Walks a node graph (including all properties and children) and calls a
 * function for each PlanNode visited (i.e., it's a visitor pattern).
 * This function returns a boolean where true indicates that it walked every
 * node and false indicates that it stopped walking nodes early (on request
 * of the function).
 * A bounded walk traverses all nodes until a PlanMarker node is encountered.
 * If the starting node is a PlanMarker then it is ignored.
 *
 * Params:
 *   pNode    - The node of the graph to start with (may not be the root).
 *   fVisitor - A function that is called for each node visited. This function
 *              must have the following definition:
 *                 b1 func(PlanNode* pNode, s2K& sPath, i8 iDepth, void* pvArg)
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
b1 GraphNav::bBoundedWalkBottomUp(PlanNode* pNode,
                                 std::function<eWalkCtrl(PlanNode*, s2K&, i8, void*)> fVisitor,
                                 void* pvArg)
{
  return bBoundedWalkBottomUp(pNode, true, true, fVisitor, pvArg);
}

/*----------------------------------------------------------------------------*/
/* This is a TOP-DOWN, depth-first graph traversal with VISITOR-callback semantics.
 * Walks a node graph (including properties and children if desired) and calls a
 * function for each PlanNode visited (i.e., it's a visitor pattern).
 * This function returns a boolean where true indicates that it walked every
 * node and false indicates that it stopped walking nodes early (on request
 * of the function).
 * A global walk traverses all nodes in the graph.
 *
 * Params:
 *   pNode    - The node of the graph to start with (may not be the root).
 *   bWalkProperties - If true then any property nodes will be walked as well.
 *                     If false then any property nodes will be skipped.
 *   bWalkChildren - If true then any child nodes will be walked as well.
 *                   If false then any child nodes will be skipped.
 *   fVisitor - A function that is called for each node visited. This function
 *              must have the following definition:
 *                 eWalkCtrl func(PlanNode* pNode, s2K& sPath, i8 iDepth, void* pvArg)
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
b1 GraphNav::bGlobalWalk(PlanNode* pNode, b1 bWalkProperties, b1 bWalkChildren,
                         std::function<eWalkCtrl(PlanNode*, s2K&, i8, void*)> fVisitor, void* pvArg)
{
  if (!pNode) {
    return true;
  }

  s2K sPath;

  // We may not be starting with the root node. If we are then set the path to "/"
  // so that each node can append its name appropriately.
  if (pNode->szName()->bEqual(const_cast<Stz*>(C_ROOT_NAME))) {
    sPath.vCopy("/"_Stz);

  } else {
    // We don't know where we're starting, so it will be a relative path
  }

  return bWalk(pNode, bWalkProperties, bWalkChildren, fVisitor, pvArg, sPath, 0, false);
}

/*----------------------------------------------------------------------------*/
/* This is a TOP-DOWN, depth-first graph traversal with EVENT-callback semantics.
 * Walks a node graph (including properties and children if desired) and calls
 * the fEnterEvent function when a node is first visited (BEFORE its properties
 * and children are visited), and then calls the fExitEvent function AFTER its
 * properties and children are visited.
 * This function returns a boolean where true indicates that it walked every
 * node and false indicates that it stopped walking nodes early (on request
 * of the function).
 * A global walk traverses all nodes in the graph.
 *
 * Params:
 *   pNode    - The node of the graph to start with (may not be the root).
 *   bWalkProperties - If true then any property nodes will be walked as well.
 *                     If false then any property nodes will be skipped.
 *   bWalkChildren - If true then any child nodes will be walked as well.
 *                   If false then any child nodes will be skipped.
 *   fEnterEvent - A function that is called when the node is visited and before
 *                 any properties or children of the node are visited. This function
 *                 must have the following definition:
 *                   eWalkCtrl func(PlanNode* pNode, s2K& sPath, i8 iDepth, eWalkEvent eEventType, void* pvArg)
 *                 The function should return the appropriate enum value to control
 *                 the continuing traversal of the graph.
 *   pvArg - An optional argument to be passed to the callback functions.
 * Returns: false if the function aborted the walk prematurely otherwise true.
 *
 * NOTES:
 * Functions or C++11 lambdas can be passed to this function. Using lambdas
 * can often be easier than creating a new class to specifically overload
 * the operator(). The same lambda can be used for both events, if desired.   */
/*----------------------------------------------------------------------------*/
b1 GraphNav::bGlobalWalk(PlanNode* pNode,
                         b1 bWalkProperties,
                         b1 bWalkChildren,
                         std::function<eWalkCtrl(PlanNode*, s2K&, i8, eWalkEvent, void*)> fEnterEvent,
                         std::function<eWalkCtrl(PlanNode*, s2K&, i8, eWalkEvent, void*)> fExitEvent,
                         void* pvArg)
{
  if (!pNode) {
    return true;
  }

  s2K sPath;

  // We may not be starting with the root node. If we are then set the path to "/"
  // so that each node can append its name appropriately.
  if (pNode->szName()->bEqual(const_cast<Stz*>(C_ROOT_NAME))) {
    sPath.vCopy("/"_Stz);

  } else {
    // We don't know where we're starting, so it will be a relative path
  }

  return bWalk(pNode, bWalkProperties, bWalkChildren, fEnterEvent, fExitEvent, pvArg, sPath, 0, false);
}

/*----------------------------------------------------------------------------*/
/* This is a TOP-DOWN, depth-first graph-traversal with VISITOR-callback semantics.
 * Walks a node graph (including properties and children if desired) and calls a
 * function for each PlanNode visited (i.e., it's a visitor pattern).
 * This function returns a boolean where true indicates that it walked every
 * node and false indicates that it stopped walking nodes early (on request
 * of the function).
 * A bounded walk traverses all nodes until a PlanMarker node is encountered.
 * If the starting node is a PlanMarker then it is ignored.
 *
 * Params:
 *   pNode    - The node of the graph to start with (may not be the root).
 *   bWalkProperties - If true then any property nodes will be walked as well.
 *                     If false then any property nodes will be skipped.
 *   bWalkChildren - If true then any child nodes will be walked as well.
 *                   If false then any child nodes will be skipped.
 *   fVisitor - A function that is called for each node visited. This function
 *              must have the following definition:
 *                 eWalkCtrl func(PlanNode* pNode, s2K& sPath, i8 iDepth, void* pvArg)
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
b1 GraphNav::bBoundedWalk(PlanNode* pNode, b1 bWalkProperties, b1 bWalkChildren,
                          std::function<eWalkCtrl(PlanNode*, s2K&, i8, void*)> fVisitor, void* pvArg)
{
  if (!pNode) {
    return true;
  }

  s2K sPath;

  // We may not be starting with the root node. If we are then set the path to "/"
  // so that each node can append its name appropriately.
  if (pNode->szName()->bEqual(const_cast<Stz*>(C_ROOT_NAME))) {
    sPath.vCopy("/"_Stz);

  } else {
    // We don't know where we're starting, so it will be a relative path
  }

  return bWalk(pNode, bWalkProperties, bWalkChildren, fVisitor, pvArg, sPath, 0, true);
}

/*----------------------------------------------------------------------------*/
/* This is a TOP-DOWN, depth-first graph traversal with EVENT-callback semantics.
 * Walks a node graph (including properties and children if desired) and calls
 * the fEnterEvent function when a node is first visited (BEFORE its properties
 * and children are visited), and then calls the fExitEvent function AFTER its
 * properties and children are visited.
 * This function returns a boolean where true indicates that it walked every
 * node and false indicates that it stopped walking nodes early (on request
 * of the function).
 * A bounded walk traverses all nodes until a PlanMarker node is encountered.
 * If the starting node is a PlanMarker then it is ignored.
 *
 * Params:
 *   pNode    - The node of the graph to start with (may not be the root).
 *   bWalkProperties - If true then any property nodes will be walked as well.
 *                     If false then any property nodes will be skipped.
 *   bWalkChildren - If true then any child nodes will be walked as well.
 *                   If false then any child nodes will be skipped.
 *   fEnterEvent - A function that is called when the node is visited and before
 *                 any properties or children of the node are visited. This function
 *                 must have the following definition:
 *                   eWalkCtrl func(PlanNode* pNode, s2K& sPath, i8 iDepth, eWalkEvent eEventType, void* pvArg)
 *                 The function should return the appropriate enum value to control
 *                 the continuing traversal of the graph.
 *   pvArg - An optional argument to be passed to the callback functions.
 * Returns: false if the function aborted the walk prematurely otherwise true.
 *
 * NOTES:
 * Functions or C++11 lambdas can be passed to this function. Using lambdas
 * can often be easier than creating a new class to specifically overload
 * the operator(). The same lambda can be used for both events, if desired.   */
/*----------------------------------------------------------------------------*/
b1 GraphNav::bBoundedWalk(PlanNode* pNode,
                         b1 bWalkProperties,
                         b1 bWalkChildren,
                         std::function<eWalkCtrl(PlanNode*, s2K&, i8, eWalkEvent, void*)> fEnterEvent,
                         std::function<eWalkCtrl(PlanNode*, s2K&, i8, eWalkEvent, void*)> fExitEvent,
                         void* pvArg)
{
  if (!pNode) {
    return true;
  }

  s2K sPath;

  // We may not be starting with the root node. If we are then set the path to "/"
  // so that each node can append its name appropriately.
  if (pNode->szName()->bEqual(const_cast<Stz*>(C_ROOT_NAME))) {
    sPath.vCopy("/"_Stz);

  } else {
    // We don't know where we're starting, so it will be a relative path
  }

  return bWalk(pNode, bWalkProperties, bWalkChildren, fEnterEvent, fExitEvent, pvArg, sPath, 0, true);
}

/*----------------------------------------------------------------------------*/
/* This is a BOTTOM-UP, depth-first graph-traversal with VISITOR-callback semantics.
 * Walks a node graph (including properties and children if desired) and calls a
 * function for each PlanNode visited (i.e., it's a visitor pattern).
 * This function returns a boolean where true indicates that it walked every
 * node and false indicates that it stopped walking nodes early (on request
 * of the function).
 * A global walk traverses all nodes in the graph.
 *
 * Params:
 *   pNode    - The node of the graph to start with (may not be the root).
 *   bWalkProperties - If true then any property nodes will be walked as well.
 *                     If false then any property nodes will be skipped.
 *   bWalkChildren - If true then any child nodes will be walked as well.
 *                   If false then any child nodes will be skipped.
 *   fVisitor - A function that is called for each node visited. This function
 *              must have the following definition:
 *                 eWalkCtrl func(PlanNode* pNode, s2K& sPath, i8 iDepth, void* pvArg)
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
b1 GraphNav::bGlobalWalkBottomUp(PlanNode* pNode, b1 bWalkProperties, b1 bWalkChildren,
                                 std::function<eWalkCtrl(PlanNode*, s2K&, i8, void*)> fVisitor, void* pvArg)
{
  if (!pNode) {
    return true;
  }

  s2K sPath;

  // We may not be starting with the root node. If we are then set the path to "/"
  // so that each node can append its name appropriately.
  if (pNode->szName()->bEqual(const_cast<Stz*>(C_ROOT_NAME))) {
    sPath.vCopy("/"_Stz);

  } else {
    // We don't know where we're starting, so it will be a relative path
  }

  return bWalkBottomUp(pNode, bWalkProperties, bWalkChildren, fVisitor, pvArg, sPath, 0, false);
}

/*----------------------------------------------------------------------------*/
/* This is a BOTTOM-UP, depth-first graph traversal with VISITOR-callback semantics.
 * Walks a node graph (including properties and children if desired) and calls a
 * function for each PlanNode visited (i.e., it's a visitor pattern).
 * This function returns a boolean where true indicates that it walked every
 * node and false indicates that it stopped walking nodes early (on request
 * of the function).
 * A bounded walk traverses all nodes until a PlanMarker node is encountered.
 * If the starting node is a PlanMarker then it is ignored.
 *
 * Params:
 *   pNode    - The node of the graph to start with (may not be the root).
 *   bWalkProperties - If true then any property nodes will be walked as well.
 *                     If false then any property nodes will be skipped.
 *   bWalkChildren - If true then any child nodes will be walked as well.
 *                   If false then any child nodes will be skipped.
 *   fVisitor - A function that is called for each node visited. This function
 *              must have the following definition:
 *                 eWalkCtrl func(PlanNode* pNode, s2K& sPath, i8 iDepth, void* pvArg)
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
b1 GraphNav::bBoundedWalkBottomUp(PlanNode* pNode, b1 bWalkProperties, b1 bWalkChildren,
                                  std::function<eWalkCtrl(PlanNode*, s2K&, i8, void*)> fVisitor, void* pvArg)
{
  if (!pNode) {
    return true;
  }

  s2K sPath;

  // We may not be starting with the root node. If we are then set the path to "/"
  // so that each node can append its name appropriately.
  if (pNode->szName()->bEqual(const_cast<Stz*>(C_ROOT_NAME))) {
    sPath.vCopy("/"_Stz);

  } else {
    // We don't know where we're starting, so it will be a relative path
  }

  return bWalkBottomUp(pNode, bWalkProperties, bWalkChildren, fVisitor, pvArg, sPath, 0, true);
}

/*----------------------------------------------------------------------------*/
/* This is a TOP-DOWN, depth-first graph traversal with VISITOR-callback semantics.
 * Walks a node graph (including properties and children if desired) and calls a
 * function for each PlanNode visited (i.e., it's a visitor pattern).
 * This function returns a boolean where true indicates that it walked every
 * node and false indicates that it stopped walking nodes early (on request
 * of the function).
 * A global walk traverses all nodes in the graph. A bounded walk will stop
 * when a PlanMarker node is encountered (see bBoundedWalk parameter)
 *
 * Params:
 *   pNode    - The node of the graph to start with (may not be the root).
 *   bWalkProperties - If true then any property nodes will be walked as well.
 *                     If false then any property nodes will be skipped.
 *   bWalkChildren - If true then any child nodes will be walked as well.
 *                   If false then any child nodes will be skipped.
 *   fVisitor - A function that is called for each node visited. This function
 *              must have the following definition:
 *                 eWalkCtrl func(PlanNode* pNode, s2K& sPath, i8 iDepth, void* pvArg)
 *              The function should return the appropriate enum value to control
 *              the continuing traversal of the graph.
 *   pvArg - An optional argument to be passed to the visitor function.
 *   sPath - the path (absolute or relative) of the parent node. The current
 *           node should be appended to this and cleared before returning.
 *   iDepth - the depth of the current node, starting at 0 for the first node.
 *   bBoundedWalk - if true then the walk stops when it hits the first
 *                  PlanMarker node (ignoring the initial pNode if it happens
 *                  to be a PlanMarker node). If false then all nodes of
 *                  the graph are walked.
 * Returns: false if the function aborted the walk prematurely otherwise true.
 *
 * NOTES:
 * Functions or C++11 lambdas can be passed to this function. Using lambdas
 * can often be easier than creating a new class to specifically overload
 * the operator().                                                            */
/*----------------------------------------------------------------------------*/
b1 GraphNav::bWalk(PlanNode* pNode, b1 bWalkProperties, b1 bWalkChildren,
                   std::function<eWalkCtrl(PlanNode*, s2K&, i8, void*)> fVisitor,
                   void* pvArg, s2K& sPath, i8 iDepth, b1 bBoundedWalk)
{
  if (!pNode) {
    return true;
  }

  // Have we hit a PlanMarker node on a bounded walk?
  if (bBoundedWalk && PlanMarker::bIsOfType(pNode) && iDepth > 0) {
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

        bRetVal = bWalk(pProp, bWalkProperties, bWalkChildren, fVisitor, pvArg, sPath, iDepth + 1, bBoundedWalk);

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

        bRetVal = bWalk(pChild, bWalkProperties, bWalkChildren, fVisitor, pvArg, sPath, iDepth + 1, bBoundedWalk);

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
/* This is a BOTTOM-UP, depth-first graph traversal with VISITOR-callback semantics.
 * Walks a node graph (including properties and children if desired) and calls a
 * function for each PlanNode visited (i.e., it's a visitor pattern).
 * This function returns a boolean where true indicates that it walked every
 * node and false indicates that it stopped walking nodes early (on request
 * of the function).
 * A global walk traverses all nodes in the graph. A bounded walk will stop
 * when a PlanMarker node is encountered (see bBoundedWalk parameter)
 *
 * Params:
 *   pNode    - The node of the graph to start with (may not be the root).
 *   bWalkProperties - If true then any property nodes will be walked as well.
 *                     If false then any property nodes will be skipped.
 *   bWalkChildren - If true then any child nodes will be walked as well.
 *                   If false then any child nodes will be skipped.
 *   fVisitor - A function that is called for each node visited. This function
 *              must have the following definition:
 *                 eWalkCtrl func(PlanNode* pNode, s2K& sPath, i8 iDepth, void* pvArg)
 *              The function should return the appropriate enum value to control
 *              the continuing traversal of the graph.
 *   pvArg - An optional argument to be passed to the visitor function.
 *   sPath - the path (absolute or relative) of the parent node. The current
 *           node should be appended to this and cleared before returning.
 *   iDepth - the depth of the current node, starting at 0 for the first node.
 *   bBoundedWalk - if true then the walk stops when it hits the first
 *                  PlanMarker node (ignoring the initial pNode if it happens
 *                  to be a PlanMarker node). If false then all nodes of
 *                  the graph are walked.
 * Returns: false if the function aborted the walk prematurely otherwise true.
 *
 * NOTES:
 * Functions or C++11 lambdas can be passed to this function. Using lambdas
 * can often be easier than creating a new class to specifically overload
 * the operator().                                                            */
/*----------------------------------------------------------------------------*/
b1 GraphNav::bWalkBottomUp(PlanNode* pNode, b1 bWalkProperties, b1 bWalkChildren,
                           std::function<eWalkCtrl(PlanNode*, s2K&, i8, void*)> fVisitor,
                           void* pvArg, s2K& sPath, i8 iDepth, b1 bBoundedWalk)
{
  if (!pNode) {
    return true;
  }

  // Have we hit a PlanMarker node on a bounded walk?
  if (bBoundedWalk && PlanMarker::bIsOfType(pNode) && iDepth > 0) {
    return true;
  }

  sPath.vConcat(pNode->szName());
  i8 nLen = sPath.nLength();

  b1 bRetVal;

  // In a bottom-up walk, walk the children before walking any properties.
  // It makes more sense to process any children before processing properties
  // of the current node.

  // Any children to walk down?
  if (bWalkChildren && pNode->bHasChildren()) {

    PlanNodeIter* pIter = pNode->pNewChildrenIter();
    try {
      while (pIter->bHasNext()) {
        PlanNode *pChild = pIter->pNext();

        // Don't forget to clip the path and add the appropriate separator before each node
        sPath.vClip(nLen);
        sPath.vConcat(StrLib::szCharToStr(C_CHILD_SEPARATOR));

        bRetVal = bWalkBottomUp(pChild, bWalkProperties, bWalkChildren, fVisitor, pvArg, sPath, iDepth + 1, bBoundedWalk);

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

  // Any properties to walk down?
  if (bWalkProperties && pNode->bHasProperties()) {

    PlanNodeIter* pIter = pNode->pNewPropertyIter();
    try {
      while (pIter->bHasNext()) {
        PlanNode *pProp = pIter->pNext();

        // Don't forget to clip the path and add the appropriate separator before each node
        sPath.vClip(nLen);
        sPath.vConcat(StrLib::szCharToStr(C_PROPERTY_SEPARATOR));

        bRetVal = bWalkBottomUp(pProp, bWalkProperties, bWalkChildren, fVisitor, pvArg, sPath, iDepth + 1, bBoundedWalk);

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
/* This is a TOP-DOWN, depth-first graph traversal with EVENT-callback semantics.
 * Walks a node graph (including properties and children if desired) and calls a
 * function for each PlanNode visited (i.e., it's a visitor pattern).
 * This function returns a boolean where true indicates that it walked every
 * node and false indicates that it stopped walking nodes early (on request
 * of the function).
 * A global walk traverses all nodes in the graph. A bounded walk will stop
 * when a PlanMarker node is encountered (see bBoundedWalk parameter)
 *
 * Params:
 *   pNode    - The node of the graph to start with (may not be the root).
 *   bWalkProperties - If true then any property nodes will be walked as well.
 *                     If false then any property nodes will be skipped.
 *   bWalkChildren - If true then any child nodes will be walked as well.
 *                   If false then any child nodes will be skipped.
 *   fVisitor - A function that is called for each node visited. This function
 *              must have the following definition:
 *                 eWalkCtrl func(PlanNode* pNode, s2K& sPath, i8 iDepth, void* pvArg)
 *              The function should return the appropriate enum value to control
 *              the continuing traversal of the graph.
 *   pvArg - An optional argument to be passed to the visitor function.
 *   sPath - the path (absolute or relative) of the parent node. The current
 *           node should be appended to this and cleared before returning.
 *   iDepth - the depth of the current node, starting at 0 for the first node.
 *   bBoundedWalk - if true then the walk stops when it hits the first
 *                  PlanMarker node (ignoring the initial pNode if it happens
 *                  to be a PlanMarker node). If false then all nodes of
 *                  the graph are walked.
 * Returns: false if the function aborted the walk prematurely otherwise true.
 *
 * NOTES:
 * Functions or C++11 lambdas can be passed to this function. Using lambdas
 * can often be easier than creating a new class to specifically overload
 * the operator().                                                            */
/*----------------------------------------------------------------------------*/
b1 GraphNav::bWalk(PlanNode* pNode, b1 bWalkProperties, b1 bWalkChildren,
                   std::function<eWalkCtrl(PlanNode*, s2K&, i8, eWalkEvent, void*)> fEnterEvent,
                   std::function<eWalkCtrl(PlanNode*, s2K&, i8, eWalkEvent, void*)> fExitEvent,
                   void* pvArg, s2K& sPath, i8 iDepth, b1 bBoundedWalk)
{
  if (!pNode) {
    return true;
  }

  // Have we hit a PlanMarker node on a bounded walk?
  if (bBoundedWalk && PlanMarker::bIsOfType(pNode) && iDepth > 0) {
    return true;
  }

  sPath.vConcat(pNode->szName());
  i8 nLen = sPath.nLength();

  // Visit the current node for the ENTER event
  eWalkCtrl eCtrl = WALK_CONTINUE;
  if (fEnterEvent) {
    eCtrl = fEnterEvent(pNode, sPath, iDepth, EVENT_ENTER, pvArg);

    if (eCtrl == WALK_ABORT) {
      return false;
    }

    if (eCtrl == WALK_SKIP_SUBTREE) {
      // Still need to invoke the exit event before we return back up.
      // Visit the current node for the EXIT event
      if (fExitEvent) {
        eCtrl = fExitEvent(pNode, sPath, iDepth, EVENT_EXIT, pvArg);
      }

      if (eCtrl == WALK_ABORT) {
        return false;
      }

      // SKIP_SUBTREE, SKIP_PROPERTIES, SKIP_CHILDREN are meaningless when returned
      // on an exit event as we have already processed the properties and children.
      return true;
    }
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

        bRetVal = bWalk(pProp, bWalkProperties, bWalkChildren, fEnterEvent, fExitEvent,
                        pvArg, sPath, iDepth + 1, bBoundedWalk);

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

        bRetVal = bWalk(pChild, bWalkProperties, bWalkChildren, fEnterEvent, fExitEvent,
                        pvArg, sPath, iDepth + 1, bBoundedWalk);

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

  // Visit the current node for the EXIT event
  sPath.vClip(nLen);
  if (fExitEvent) {
    eCtrl = fExitEvent(pNode, sPath, iDepth, EVENT_EXIT, pvArg);
  }

  if (eCtrl == WALK_ABORT) {
    return false;
  }

  // SKIP_SUBTREE, SKIP_PROPERTIES, SKIP_CHILDREN are meaningless when returned
  // on an exit event as we have already processed the properties and children.
  return true;
}

/*----------------------------------------------------------------------------*/
/* Starting at pNode, walk the graph upwards until the walk is either aborted */
/* or we hit the root node. Call the callback function for each node visited. */
/* Params:                                                                    */
/*  - pNode:     The node to begin walking upwards from.                      */
/*  - pRootNode: The root node of the plan.                                   */
/*  - fVisitor:  The callback function to invoke for each node visited.       */
/*  - pvArg:     An argument to be passed to fVisitor when called.            */
/* Returns: false if the function aborted the walk prematurely otherwise true.*/
/* Notes:   This can be an expensive operation if called repeatedly because   */
/*          we first need to find the path of pNode in the entire graph.      */
/*----------------------------------------------------------------------------*/
b1 GraphNav::bWalkUp(QueryNode* pNode, PlanNode* pRootNode,
                     std::function<eWalkCtrl(PlanNode*, s2K&, i8, eWalkEvent, void*)> fVisitor, void* pvArg)
{
  ASSERT(pNode);
  ASSERT(pRootNode);

  // First we need to find the path of pNode in the plan.
  s2K sStartingPath;

  b1 bResult = bGetPath(pNode, pRootNode, sStartingPath);
  ASSERT(bResult);
  ASSERT(sStartingPath.bBeginsWith("/"_Stz));

  // Let's now do a bottom-up walk from this path back to the root node.
  return bWalkPath(pRootNode, sStartingPath, nullptr, fVisitor, pvArg);
}


/*----------------------------------------------------------------------------*/
/* From a given starting node (typically the root node), walk a path given by */
/* a string and call callback functions for each node visited, both on descent*/
/* of the path and ascent.                                                    */
/* Params:                                                                    */
/*  - pStartNode: The node to begin walking the path from (usually root node).*/
/*  - sPathToWalk: The path to follow                                         */
/*  - fEnterVisitor: the callback to invoke on the path descent               */
/*  - fExitVisitor:  the callback to invoke on the path ascent                */
/*  - pvArg:     An argument to be passed to fVisitor when called.            */
/* Returns: false if the function aborted the walk prematurely otherwise true.*/
/*----------------------------------------------------------------------------*/
b1 GraphNav::bWalkPath(PlanNode* pStartNode, s2K& sPathToWalk,
                       std::function<eWalkCtrl(PlanNode*, s2K&, i8, eWalkEvent, void*)> fEnterVisitor,
                       std::function<eWalkCtrl(PlanNode*, s2K&, i8, eWalkEvent, void*)> fExitVisitor,
                       void* pvArg)
{

  s2K sCurrentPath;
  s2K sPathToBeWalked(sPathToWalk.szStz());

  // special handling for absolute paths?
  if (sPathToWalk.bBeginsWith(const_cast<Stz*>(C_ROOT_PATH))) {
    sCurrentPath.vCopy(const_cast<Stz*>(C_ROOT_PATH));
    sPathToBeWalked.vCopy(sPathToWalk.szStz() + const_cast<Stz*>(C_ROOT_PATH)->nLength());

  } else {
    // If the path is not absolute then the relative path might start with the name
    // of the first node. This commonly occurs from walking the graph and being given
    // the current path - it always starts with the name of the first node. If this is
    // the case let's remove that first element before we call the base walk function
    // so that it is clear about the remaining path **to be walked**.
    s256 sBufChild;
    s256 sBufProp;
    sBufChild.vSprint("%s%s"_Stz, pStartNode->szName(), C_CHILD_SEPARATOR_STZ);
    sBufProp.vSprint( "%s%s"_Stz, pStartNode->szName(), C_PROPERTY_SEPARATOR_STZ);

    if (sPathToWalk.bBeginsWith(sBufChild.szStz()) ||
        sPathToWalk.bBeginsWith(sBufProp.szStz()) ) {
      sCurrentPath.vCopy(pStartNode->szName());
      sPathToBeWalked.vCopy(sPathToWalk.szStz() + pStartNode->szName()->nLength());
    }
  }

  return bWalkPath(pStartNode, sPathToBeWalked, sCurrentPath,0,
                   fEnterVisitor, fExitVisitor, pvArg);
}


/*----------------------------------------------------------------------------*/
/* Internal function for walking a specific path downwards and back up.       */
/* Params:                                                                    */
/*  - pCurrentNode: The current node being visited.                           */
/*  - sPathToWalk: The path to follow relative to the current node            */
/*  - sCurrentPath: The current path relative to the starting node            */
/*  - iDepth: The depth of the current node relative to the starting node     */
/*  - fEnterVisitor: the callback to invoke on the path descent               */
/*  - fExitVisitor:  the callback to invoke on the path ascent                */
/*  - pvArg:     An argument to be passed to fVisitor when called.            */
/* Returns: false if the function aborted the walk prematurely otherwise true.*/
/*----------------------------------------------------------------------------*/
b1 GraphNav::bWalkPath(PlanNode* pCurrentNode, s2K& sPathToBeWalked, s2K& sCurrentPath, i8 iDepth,
                       std::function<eWalkCtrl(PlanNode*, s2K&, i8, eWalkEvent, void*)> fEnterVisitor,
                       std::function<eWalkCtrl(PlanNode*, s2K&, i8, eWalkEvent, void*)> fExitVisitor,
                       void* pvArg)
{
  // Visit the current node for the ENTER event
  eWalkCtrl eCtrl = WALK_CONTINUE;
  if (fEnterVisitor) {
    eCtrl = fEnterVisitor(pCurrentNode, sCurrentPath, iDepth, EVENT_ENTER, pvArg);

    if (eCtrl == WALK_ABORT) {
      return false;
    }

    if (eCtrl == WALK_SKIP_SUBTREE) {
      // Still need to invoke the exit event before we return back up.
      // Visit the current node for the EXIT event
      if (fExitVisitor) {
        eCtrl = fExitVisitor(pCurrentNode, sCurrentPath, iDepth, EVENT_EXIT, pvArg);
      }

      if (eCtrl == WALK_ABORT) {
        return false;
      }

      // SKIP_SUBTREE, SKIP_PROPERTIES, SKIP_CHILDREN are meaningless when returned
      // on an exit event as we have already processed the properties and children.
      return true;
    }
  }

  b1 bRetVal;

  // Determine if the next element of the path to be walked is a child, a property, or
  // if we have actually reached the end of the path.

  if (sPathToBeWalked.bBeginsWith(C_CHILD_SEPARATOR_STZ)) {
    // We've got a child to visit

    s2K sNextPathToBeWalked;
    s256 sChildName;
    i8 iEnd = iNextSeparator(sPathToBeWalked.szStz() + 1);

    if (iEnd == -1) {
      // This will be the last node to be visited
      sChildName.vCopy(sPathToBeWalked.szStz() + 1);
      sNextPathToBeWalked.vReset();

    } else {
      sChildName.vCopy(sPathToBeWalked.szStz() + 1, iEnd);
      sNextPathToBeWalked.vCopy(sPathToBeWalked.szStz() + 1 + iEnd);
    }

    s2K sNextCurrentPath;
    sNextCurrentPath.vCatSprint("%s/%s"_Stz, sCurrentPath.szStz(), sChildName.szStz());

    // Find the child node
    PlanNode* pNextChild = GraphNav::pFindChild(sChildName.szStz(), pCurrentNode);
    ASSERT(pNextChild);

    // Recurse!
    bRetVal = bWalkPath(pNextChild, sNextPathToBeWalked, sNextCurrentPath, iDepth+1, fEnterVisitor, fExitVisitor, pvArg);

    if (!bRetVal) {
      return false; // We are aborting
    }

  } else if (sPathToBeWalked.bBeginsWith(C_PROPERTY_SEPARATOR_STZ)) {
    // We've got a property to visit

    s2K sNextPathToBeWalked;
    s256 sPropertyName;
    i8 iEnd = iNextSeparator(sPathToBeWalked.szStz() + 1);

    if (iEnd == -1) {
      // This will be the last node to be visited
      sPropertyName.vCopy(sPathToBeWalked.szStz() + 1);
      sNextPathToBeWalked.vReset();

    } else {
      sPropertyName.vCopy(sPathToBeWalked.szStz() + 1, iEnd);
      sNextPathToBeWalked.vCopy(sPathToBeWalked.szStz() + 1 + iEnd);
    }

    s2K sNextCurrentPath;
    sNextCurrentPath.vCatSprint("%s/%s"_Stz, sCurrentPath.szStz(), sPropertyName.szStz());

    // Find the property node
    PlanNode* pNextProperty = GraphNav::pFindProperty(sPropertyName.szStz(), pCurrentNode);
    ASSERT(pNextProperty);

    // Recurse!
    bRetVal = bWalkPath(pNextProperty, sNextPathToBeWalked, sNextCurrentPath, iDepth + 1, fEnterVisitor, fExitVisitor, pvArg);

    if (!bRetVal) {
      return false; // We are aborting
    }

  } else if (sPathToBeWalked.bIsEmpty()) {
    // Reached the end of the walk. Don't return because we may have an exit visitor.
    bRetVal = true;

  } else {
    NEVER_REACH
  }


  // Visit the current node for the EXIT event
  if (fExitVisitor) {
    eCtrl = fExitVisitor(pCurrentNode, sCurrentPath, iDepth, EVENT_EXIT, pvArg);
  }

  if (eCtrl == WALK_ABORT) {
    return false;
  }

  // SKIP_SUBTREE, SKIP_PROPERTIES, SKIP_CHILDREN are meaningless when returned
  // on an exit event as we have already processed the properties and children.
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
/* Looks for a child which matches a given condition. Returns the first       */
/* matching child, if found, otherwise nullptr.                               */
/*----------------------------------------------------------------------------*/
PlanNode* GraphNav::pFindChild(std::function<b1(PlanNode*)> fCondition, PlanNode* pNode)
{
  ASSERT(pNode);
  ASSERT(fCondition);

  if (!pNode->bHasChildren()) {
    return nullptr;
  }

  PlanNodeIter* pIter = pNode->pNewChildrenIter();
  while (pIter->bHasNext()) {
    PlanNode* pItem = pIter->pNext();

    if (fCondition(pItem)) {
      delete pIter;
      return pItem;
    }
  }

  delete pIter;
  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* Search for a node that matches a given condition. If found then the node^  */
/* is returned. Searches the entire graph from the starting node.             */
/* The condition function is a lambda/function with the following definition: */
/*   b1 bCondition(PlanNode* pNode, void* pvArg);                             */
/* Returning true out of this function will return pNode and abort the search.*/
/* Returning false out of this function will continue the search.             */
/* If no node matching the condition is found then a nullptr is returned.     */
/*----------------------------------------------------------------------------*/
PlanNode* GraphNav::pGlobalSearch(PlanNode* pStartNode, b1 bSearchProperties, b1 bSearchChildren,
                                  std::function<b1(PlanNode*, void*)> fCondition, void* pvArg)
{
  ASSERT(pStartNode);

  PlanNode* pResult = nullptr;

  auto fVisitor = [&pResult, fCondition](PlanNode* pNode, s2K& sPath, i8 iDepth, void* pvArg) -> GraphNav::eWalkCtrl {

    UNUSED(sPath);
    UNUSED(iDepth);

    // test the condition
    if (fCondition(pNode, pvArg)) {
      pResult = pNode;
      return WALK_ABORT;
    }

    return WALK_CONTINUE;
  };

  // Walk the graph then return the result, if any
  bGlobalWalk(pStartNode, bSearchProperties, bSearchChildren, fVisitor, pvArg);

  return pResult;
}

/*----------------------------------------------------------------------------*/
/* Search for a node that matches a given condition in a bottom-up fashion.   */
/* If found then the node^ is returned. Searches the entire graph from the    */
/* starting node.                                                             */
/* The condition function is a lambda/function with the following definition: */
/*   b1 bCondition(PlanNode* pNode, void* pvArg);                             */
/* Returning true out of this function will return pNode and abort the search.*/
/* Returning false out of this function will continue the search.             */
/* If no node matching the condition is found then a nullptr is returned.     */
/*----------------------------------------------------------------------------*/
PlanNode* GraphNav::pGlobalSearchBottomUp(PlanNode* pStartNode, b1 bSearchProperties, b1 bSearchChildren,
                                          std::function<b1(PlanNode*, void*)> fCondition, void* pvArg)
{
  ASSERT(pStartNode);

  PlanNode* pResult = nullptr;

  auto fVisitor = [&pResult, fCondition](PlanNode* pNode, s2K& sPath, i8 iDepth, void* pvArg) -> GraphNav::eWalkCtrl {

    UNUSED(sPath);
    UNUSED(iDepth);

    // test the condition
    if (fCondition(pNode, pvArg)) {
      pResult = pNode;
      return WALK_ABORT;
    }

    return WALK_CONTINUE;
  };

  // Walk the graph then return the result, if any
  bGlobalWalkBottomUp(pStartNode, bSearchProperties, bSearchChildren, fVisitor, pvArg);

  return pResult;
}

/*----------------------------------------------------------------------------*/
/* Search for a node that matches a given condition. If found then the node^  */
/* is returned. Searches this plan segment only (until a PlanMarker           */
/* is hit).                                                                   */
/* The condition function is a lambda/function with the following definition: */
/*   b1 bCondition(PlanNode* pNode, void* pvArg);                             */
/* Returning true out of this function will return pNode and abort the search.*/
/* Returning false out of this function will continue the search.             */
/* If no node matching the condition is found then a nullptr is returned.     */
/*----------------------------------------------------------------------------*/
PlanNode* GraphNav::pBoundedSearch(PlanNode* pStartNode, b1 bSearchProperties, b1 bSearchChildren,
                                  std::function<b1(PlanNode*, void*)> fCondition, void* pvArg)
{
  ASSERT(pStartNode);

  PlanNode* pResult = nullptr;

  auto fVisitor = [&pResult, fCondition](PlanNode* pNode, s2K& sPath, i8 iDepth, void* pvArg) -> GraphNav::eWalkCtrl {

    UNUSED(sPath);
    UNUSED(iDepth);

    // test the condition
    if (fCondition(pNode, pvArg)) {
      pResult = pNode;
      return WALK_ABORT;
    }

    return WALK_CONTINUE;
  };

  // Walk the graph then return the result, if any
  bBoundedWalk(pStartNode, bSearchProperties, bSearchChildren, fVisitor, pvArg);

  return pResult;
}

/*----------------------------------------------------------------------------*/
/* Search for a node that matches a given condition in a bottom-up fashion.   */
/* If found then the node^ is returned. Searches this plan segment only       */
/* (until a PlanMarker is hit).                                               */
/* The condition function is a lambda/function with the following definition: */
/*   b1 bCondition(PlanNode* pNode, void* pvArg);                             */
/* Returning true out of this function will return pNode and abort the search.*/
/* Returning false out of this function will continue the search.             */
/* If no node matching the condition is found then a nullptr is returned.     */
/*----------------------------------------------------------------------------*/
PlanNode* GraphNav::pBoundedSearchBottomUp(PlanNode* pStartNode, b1 bSearchProperties, b1 bSearchChildren,
                                           std::function<b1(PlanNode*, void*)> fCondition, void* pvArg)
{
  ASSERT(pStartNode);

  PlanNode* pResult = nullptr;

  auto fVisitor = [&pResult, fCondition](PlanNode* pNode, s2K& sPath, i8 iDepth, void* pvArg) -> GraphNav::eWalkCtrl {

    UNUSED(sPath);
    UNUSED(iDepth);

    // test the condition
    if (fCondition(pNode, pvArg)) {
      pResult = pNode;
      return WALK_ABORT;
    }

    return WALK_CONTINUE;
  };

  // Walk the graph then return the result, if any
  bBoundedWalkBottomUp(pStartNode, bSearchProperties, bSearchChildren, fVisitor, pvArg);

  return pResult;
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
/* Given a relative/absolute path and an initial starting node, return        */
/* the node that represents the parent of this path, or nullptr if not found. */
/*----------------------------------------------------------------------------*/
PlanNode* GraphNav::pGetParentOfPath(Stz* szPath, PlanNode* pInitialNode)
{
  ASSERT(szPath);
  ASSERT(pInitialNode);

  // Get the parent path
  s2K sParentPath;
  if(!bParentPath(szPath, sParentPath)) {
    return nullptr;
  }

  PlanNode* pNode = pFind(sParentPath, pInitialNode);
  return pNode;
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
/* Given a path, write the parent path to sParentBuf. Return false if there   */
/* isn't a parent.                                                            */
/*----------------------------------------------------------------------------*/
b1 GraphNav::bParentPath(s2K &sParentBuf)
{
  char* pChar = StrLib::acLast(sParentBuf.azChar());

  while (pChar != sParentBuf.azChar()) {
    if (*pChar == C_CHILD_SEPARATOR || *pChar == C_PROPERTY_SEPARATOR) {
      *pChar = 0;
      return true;
    }

    --pChar;
  }

  return false;
}

/*----------------------------------------------------------------------------*/
/* Walks the plan graph to find the given query node. Saves the path and depth*/
/*----------------------------------------------------------------------------*/
b1 GraphNav::bGetPathAndDepth(QueryNode* pSearchNode, PlanNode* pRootNode, s2K& sPathBuf, i8& iDepth, b1 bPlanSubtree)
{
  UNUSED(pRootNode);

  ASSERT(pSearchNode);
  ASSERT(pRootNode);

  iDepth = -1;

  auto fVisitor = [pSearchNode, &sPathBuf, &iDepth, bPlanSubtree](Parser::PlanNode *pNode, s2K &sWalkPath, i8 iWalkDepth, void *pvArg) -> Parser::GraphNav::eWalkCtrl {

    UNUSED(pvArg);

    // Only interested in /__root__/plan subtree?
    if (bPlanSubtree && iWalkDepth == 1 && !sWalkPath.bEqual(const_cast<Stz*>(C_PLAN_PATH))) {
      return GraphNav::WALK_SKIP_SUBTREE;
    }

    // Did we get a match?
    if (pNode == pSearchNode) {
      iDepth = iWalkDepth;
      sPathBuf.vCopy(sWalkPath.szStz());
      return GraphNav::WALK_ABORT;
    }

    return GraphNav::WALK_CONTINUE;
  };

  GraphNav::bGlobalWalk(pRootNode, true, true, fVisitor, nullptr);

  return (iDepth != -1);
}

/*----------------------------------------------------------------------------*/
/* Walks the plan graph to find the given query node. Returns true if found   */
/*----------------------------------------------------------------------------*/
b1 GraphNav::bGetPath(QueryNode* pSearchNode, PlanNode* pStartingNode, s2K& sPathBuf)
{
  ASSERT(pSearchNode);
  ASSERT(pStartingNode);

  b1 bFound = false;

  auto fVisitor =
      [pSearchNode, &sPathBuf, &bFound](Parser::PlanNode *pNode, s2K &sWalkPath, i8 iWalkDepth, void *pvArg) -> Parser::GraphNav::eWalkCtrl {

    UNUSED(pvArg);

    // Did we get a match?
    if (pNode == pSearchNode) {
      sPathBuf.vCopy(sWalkPath.szStz());
      bFound = true;
      return GraphNav::WALK_ABORT;
    }

    return GraphNav::WALK_CONTINUE;
  };

  GraphNav::bGlobalWalk(pStartingNode, true, true, fVisitor, nullptr);

  return bFound;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
