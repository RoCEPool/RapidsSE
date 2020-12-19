/*============================================================================*/
/* PlanWalker.cpp - a utility for walking plan graphs                         */
/*============================================================================*/

#include "Parser.h"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
__pCastFunction(Parser,PlanWalker,CN_PLANWALKER)
__aNewFunction(PlanWalker,CN_PLANWALKER)
__vDeleteFunction(PlanWalker,CN_PLANWALKER)
__aFooterFunction(PlanWalker)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
PlanWalker::PlanWalker()
    : Object(CN_PLANWALKER)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PLANWALKER,_FOOTER_);

  _checkInitialised_(CN_PLANWALKER);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_PLANWALKER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
PlanWalker::~PlanWalker(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PLANWALKER,_FOOTER_);
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
 * the operator().
/*----------------------------------------------------------------------------*/
b1 PlanWalker::bWalk(PlanNode* pNode, std::function<b1(PlanNode*, s256&)> fVisitor)
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
 * the operator().
/*----------------------------------------------------------------------------*/
b1 PlanWalker::bWalk(PlanNode* pNode, b1 bWalkProperties, b1 bWalkChildren, std::function<b1(PlanNode*, s256&)> fVisitor)
{
  if (!pNode) {
    return true;
  }

  s256 sPath;

  // We may not be starting with the root node. If we are then set the path to "/"
  // so that each node can append its name appropriately.
  if (pNode->getName()->bEqual(const_cast<Stz*>(PlanNode::C_ROOT_NAME))) {
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
 * the operator().
/*----------------------------------------------------------------------------*/
b1 PlanWalker::bWalk(PlanNode* pNode, b1 bWalkProperties, b1 bWalkChildren, std::function<b1(PlanNode*, s256&)> fVisitor, s256& sPath)
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
      sPath.vConcat(const_cast<Stz*>(PlanNode::C_PROPERTY_SEPARATOR));

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
      sPath.vConcat(const_cast<Stz*>(PlanNode::C_CHILD_SEPARATOR));

      bRetVal = bWalk(pChild, bWalkProperties, bWalkChildren, fVisitor, sPath);

      if (!bRetVal) {
        return false;
      }

      pChild = pChild->pNextItem();
    }
  }

  return true;
}



/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
