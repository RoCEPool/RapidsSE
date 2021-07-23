/*============================================================================*/
/* Grid.hpp -- Grid of Nodes                                                  */
/*============================================================================*/
#pragma once

#include <list>
#include <iterator>
#include "Parcel.hpp"

#include "Node.hpp" //?


namespace STORE
{
  class Grid : public Operand
  {
  private:
  
    s32    m_sGridNm;          // name of this grid
    i8     m_iConnectCode;     // connect code for this grid (not yet used)
    
    i8     m_iThisNode;        // this node number
    Node*  m_pThisNode;        // this node pointer
  
    bool   m_bHasStarted;      // has the cluster started ?
    i8     m_iStartNodeNo;     // node that started the cluster

    i8     m_nBootNodes;       // number of nodes supplied via SE -node
    i8     m_nDefinedNodes;    // number of nodes in the nodes array
    i8     m_nConnectedNodes;  // number of connected nodes in the grid

    Parcel* m_pParcel;

#ifdef WITH_NODE_SKIPLIST

    std::list<Node*> * m_pNodes_stdList;

#else

    std::list<Node*> * m_pNodes_stdList;

#endif

  
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    Grid(Stz* szGridNm, i8 iConnectCode);
   ~Grid(void) noexcept(false);
    
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);

    void vAddNode(Node* pNode);
  
    void vAddNode(i8 iNodeNo, Stz* pszName,
                  Stz* pszAddr, i8 iPort, Stz* szRegionNm);

    i8   nBootNodes(void);
    i8   nDefinedNodes(void);
    i8   nConnectedNodes(void);
    void vDsv(void);
    s16KB szDumpInfo(void);
    Stz* szGridNm(void);
    void vHasStarted(bool bHasStarted);
    bool bHasStarted(void);
    Node* pFindNode(i8 iNodeNo);
    Node* pFindNode(Stz* pName);
    Node* pGetNode(i8 i_NodeNo);
    i8   iGetNodeNo(i8 i_Node);
    void vIncBootNodes(void);
    void vIncDefinedNodes(void);
    void vIncConnectedNodes(void);
    void vDecConnectedNodes(void);
    void vDecDefinedNodes(void);
    void vDecBootNodes(void);
    i8   nNodes(void);
    void vRemoveNode(Node* pNode);
    void vSeeNodes(void);
    void vStartNodeNo(i8 iStartNodeNo);
    i8   iStartNodeNo(void);
    i8   iThisNode(void);
    Node* pThisNode(void);
    void vThisNodeNo(i8 iNode);
    void vSetThisNode (Node* pNode); // newcode
    bool bFirstNode(i8 iNodeNo);
    i8   iPrepareParcel(void);
    Parcel* pParcel(void);
    i8 iReassembleFromParcel(Parcel* pParcel);

#ifndef WITH_NODE_SKIPLIST

    // newcode
    std::list<Node*>::const_iterator itBeginNodes(void);
    std::list<Node*>::const_iterator itEndNodes(void);

#endif
  
    Footer* aFooter(void);
  };
    
  Grid* _pGrid(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
