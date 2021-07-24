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
  
    s32    m_sGridNm;       // name of this grid
    
    Node*  m_pThisNode;     // this node pointer
  
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
    
    Grid(Stz* szGridNm);
   ~Grid(void) noexcept(false);
    
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);

    void vAddDetectedNode(i8 iNodeNo, Stz* szNodeNm,
                          Stz* szIPAddr, i8 iPortNo, Stz* szRegionNm);
  
    void vAddNode(Node* pNode);
  

    void vAdjustPtnMap(void);
    void vDsv(void);
    s16KB szDumpInfo(void);
    Node* pFindNode(i8 iNodeNo);
    Node* pFindNode(Stz* pName);
    Node* pGetNthNode(i8 i_Node);
    Stz* szGridNm(void);
    i8   nNodes(void);
    void vRemoveNode(Node* pNode);
    void vSeeNodes(void);
    i8   iThisNodeNo(void);
    Node* pThisNode(void);
    void vThisNode(Node* pNode);

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
