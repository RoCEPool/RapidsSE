/*============================================================================*/
/* KeyTest.cpp -- key library / test                                          */
/*============================================================================*/

#include "DBobjects.h"
#include "StorageEngine.h"

/*----------------------------------------------------------------------------*/
/* key testing                                                                */
/*----------------------------------------------------------------------------*/
void KeyTest::vTest(void)
{
  vTrace("\n\nKeyTest:*************************************************\n"_Str);

  KeyWsp xKeyWsp;
  ValWsp xValWsp;
  KeyVal* pKeyVal;
  
  /*-----------------------------------------------------------------------*/
  vTrace("\n\n--------------- Single Element Key : INTEGER ---------------\n"_Str);
  /*-----------------------------------------------------------------------*/

  // create the column
  Column* pCol = new Column("Col0"_Str);
  pCol->vColumnType(CT_INTEGER);
  
  // create the column vector
  ColumnVec* pColVec = new ColumnVec;
  pColVec->vAdd(pCol);
  
  RelOpAry* pOpAryFind = new RelOpAry;
  
  KeyVec* pKeyVec = new KeyVec;
  
  // create a vector of key values (ascending order 0,1,2, ... 9)
  for (i8 i = 0; i < 10; i++) {
  
    xValWsp = i;
    
    xKeyWsp.vReset();
    xKeyWsp.vAddElement(xValWsp);
    //xKeyWsp.vShow();
    
    pKeyVal = new KeyVal(xKeyWsp);
    
    pKeyVec->vAdd(pKeyVal);
  }
  
  i8 v;
  i8 nBytes;
  char* aCursor;
  
  // dump the key vector
  for (i8 i = 0; i < 10; i++) {
    pKeyVal = pKeyVec->pGet(i);
    pKeyVal->vShow(pColVec);
  }

  // asserts that keyval(n) < keyval(n+1)
  KeyVal* pKeyValA = pKeyVec->pGet(0);
  for (i8 i = 1; i < 10; i++) {
    
    KeyVal* pKeyValB = pKeyVec->pGet(i);
    ASSERT(pKeyValA->bLessThan(pKeyValB,pColVec));
    
    ASSERT(pKeyValA->iCmp(pKeyValB,pColVec) < 0);
    
    pKeyValA = pKeyValB;
  }
  
  // scan the key vector for key value: >= 3
  xValWsp = 3;
  xKeyWsp.vReset();
  xKeyWsp.vAddElement(xValWsp);
  KeyVal* pKeyValFind = new KeyVal(xKeyWsp);
  vTrace("Find Key value >= 3\n"_Str);
  
  pOpAryFind->vClear();
  pOpAryFind->vAdd(RO_GREATER_EQUAL);
  
  pKeyVal = nullptr;
  for (i8 i = 0; i < 10; i++) {

    pKeyVal = pKeyVec->pGet(i);
    
    if (pKeyVal->bMatches(pKeyValFind,pColVec,pOpAryFind))
      break;
    
    pKeyVal = nullptr;
  }
  
  if (pKeyVal) {
    aCursor = pKeyVal->m_xKey.aWspArea();
    v = *(i8*)pKeyVal->aElement(pCol,nBytes,&aCursor);
    
    vTrace("( %lld >= 3)\n"_Str,v);
  }
  
  // scan the key vector for key value: >= 3
  xValWsp = 7;
  xKeyWsp.vReset();
  xKeyWsp.vAddElement(xValWsp);
  vTrace("Find Key value < 7\n"_Str);
  
  pOpAryFind->vClear();
  pOpAryFind->vAdd(RO_LESS_THAN);
  
  pKeyVal = nullptr;
  for (i8 i = 0; i < 10; i++) {
    
    pKeyVal = pKeyVec->pGet(i);
    
    if (pKeyVal->bMatches(pKeyValFind,pColVec,pOpAryFind))
      break;
    
    pKeyVal = nullptr;
  }
  
  if (pKeyVal) {
    aCursor = pKeyVal->m_xKey.aWspArea();
    v = *(i8*)pKeyVal->aElement(pCol,nBytes,&aCursor);
    
    vTrace("( %lld < 7 )\n"_Str,v);
  }

  delete pKeyVec;
  delete pColVec;
  delete pOpAryFind;
  
  /*-----------------------------------------------------------------------*/
  vTrace("\n\n--------------- Single Element Key : VARCHAR ---------------\n"_Str);
  /*-----------------------------------------------------------------------*/

  // create the string column
  pCol = new Column("Col0"_Str);
  pCol->vColumnType(CT_VARCHAR);
  pCol->vDatumType(DM_VCH);
  
  // create the column vector
  pColVec = new ColumnVec;
  pColVec->vAdd(pCol);
  
  // create the operator array
  pOpAryFind = new RelOpAry;
  
  // create the key vector
  pKeyVec = new KeyVec;
  
  s48K sKey;
  
  StrVal* pStrVal;
  
  // generate random keyVal's and add to keyVec
  for (i8 i = 0; i < 10; i++) {
    sKey = Random::sRandomString(7);
    
    Str* ptr = sKey.aStr();
    
    xValWsp = ptr;
    pStrVal = _pStrVal(&xValWsp);
    
    xKeyWsp.vReset();
    xKeyWsp.vAddElement(xValWsp);
    //xKeyWsp.vShow();
    
    pKeyVal = new KeyVal(xKeyWsp);
    pKeyVec->vAdd(pKeyVal);
  }
  
  // dump the key vector
  for (i8 i = 0; i < 10; i++) {
    pKeyVal = pKeyVec->pGet(i);
    pKeyVal->vShow(pColVec);
  }

  // generate a random key value to find
  sKey = Random::sRandomString(7);
  Str* ptr = sKey.aStr();
  vTrace("Find Key value < %s\n"_Str, ptr);
  
  xValWsp = ptr;
  pStrVal = _pStrVal(&xValWsp);
  
  xKeyWsp.vReset();
  xKeyWsp.vAddElement(xValWsp);
  pKeyValFind = new KeyVal(xKeyWsp);
  
  // scan the key vector for key value: >= pKeyValFind
  pOpAryFind->vClear();
  pOpAryFind->vAdd(RO_LESS_THAN);
  
  pKeyVal = nullptr;
  for (i8 i = 0; i < 10; i++) {
    
    pKeyVal = pKeyVec->pGet(i);
    
    if (pKeyVal->bMatches(pKeyValFind,pColVec,pOpAryFind))
      break;
    
    pKeyVal = nullptr;
  }
  
  if (pKeyVal) {
    aCursor = pKeyVal->m_xKey.aWspArea();
    char* p = pKeyVal->aElement(pCol,nBytes,&aCursor);
    
    vTrace("( %s < %s)\n"_Str,p,pKeyValFind->m_xKey.aWspArea());
  }
  else {
    vTrace("Found nothing < %s\n"_Str,pKeyValFind->m_xKey.aWspArea());
  }
  
  // generate a random key value to find
  sKey = Random::sRandomString(7);
  ptr = sKey.aStr();
  vTrace("Find Key value >= %s\n"_Str, ptr);
  
  xValWsp = ptr;
  pStrVal = _pStrVal(&xValWsp);
  
  xKeyWsp.vReset();
  xKeyWsp.vAddElement(xValWsp);
  pKeyValFind = new KeyVal(xKeyWsp);
  
  // scan the key vector for key value: < pKeyValFind
  pOpAryFind->vClear();
  pOpAryFind->vAdd(RO_GREATER_EQUAL);
  
  pKeyVal = nullptr;
  for (i8 i = 0; i < 10; i++) {
    
    pKeyVal = pKeyVec->pGet(i);
    
    if (pKeyVal->bMatches(pKeyValFind,pColVec,pOpAryFind))
      break;
    
    pKeyVal = nullptr;
  }
  
  if (pKeyVal) {
    aCursor = pKeyVal->m_xKey.aWspArea();
    char* p = pKeyVal->aElement(pCol,nBytes,&aCursor);
    
    vTrace("( %s >= %s)\n"_Str,p,pKeyValFind->m_xKey.aWspArea());
  }
  else {
    vTrace("Found nothing >= %s)\n"_Str,pKeyValFind->m_xKey.aWspArea());
  }

  delete pKeyVec;
  delete pColVec;
  delete pOpAryFind;
  
  /*-----------------------------------------------------------------------*/
  vTrace("\n\n----------- Two Element Key : INTEGER , INTEGER -------------\n"_Str);
  /*-----------------------------------------------------------------------*/
  
  // create the column
  Column* pCol0 = new Column("Col0"_Str);
  pCol0->vColumnType(CT_INTEGER);
  
  Column* pCol1 = new Column("Col1"_Str);
  pCol1->vColumnType(CT_INTEGER);
  
  // create the column vector
  pColVec = new ColumnVec;
  pColVec->vAdd(pCol0);
  pColVec->vAdd(pCol1);

  pOpAryFind = new RelOpAry;
  
  pKeyVec = new KeyVec;
  
  // create a vector of key values
  // (ascending order col0: 0,1,2, ... 9)
  //                  col1: 10, 20, 30
  for (i8 i = 0; i < 10; i++) {
    
    for (i8 j = 10; j <= 30; j += 10) {
      xValWsp = i;
      xKeyWsp.vReset();
      xKeyWsp.vAddElement(xValWsp);
      //xKeyWsp.vShow();
      
      xValWsp = j;
      xKeyWsp.vAddElement(xValWsp);
      //xKeyWsp.vShow();
      pKeyVal = new KeyVal(xKeyWsp);
      pKeyVec->vAdd(pKeyVal);
    }    
    
  }
  
  // dump the key vector
  for (i8 i = 0; i < 10; i++) {
    
    pKeyVal = pKeyVec->pGet(i);
    pKeyVal->vShow(pColVec);
    
    //vTrace("%lld: %lld %lld\n"_Str,i,v, v2);
  }
  
  delete pKeyVec;
  delete pColVec;
  delete pOpAryFind;

  vTrace("\n\nEnd KeyTest**********************************************\n"_Str);
}

/*----------------------------------------------------------------------------*/
/* key testing                                                                */
/*----------------------------------------------------------------------------*/
void KeyTest::vTestA(void)
{
  vTrace("\n\nKeyTest A:+++++++++++++++++++++++++++++++++++++++++++++++\n"_Str);
  
  bool b;
  
  // create the column vector
  ColumnVec* pColVec = new ColumnVec;
  
  Column* pCol = new Column("Col0"_Str);
  pCol->vColumnType(CT_INTEGER);
  pColVec->vAdd(pCol);
  pCol = new Column("Col1"_Str);
  pCol->vColumnType(CT_INTEGER);
  pColVec->vAdd(pCol);
  
  // create the op array
  RelOpAry* pOpAry = new RelOpAry;
  pOpAry->vAdd(RO_LESS_EQUAL);
  pOpAry->vAdd(RO_LESS_THAN);
  
  KeyWsp xKeyWsp;
  ValWsp xValWsp;
  
  xKeyWsp.vReset();
  xValWsp = (i8)1;
  xKeyWsp.vAddElement(xValWsp);
  xValWsp = (i8)10;
  xKeyWsp.vAddElement(xValWsp);
  KeyVal* pKeyValThis = new KeyVal(xKeyWsp);
  
  xKeyWsp.vReset();
  xValWsp = (i8)1;
  xKeyWsp.vAddElement(xValWsp);
  xValWsp = (i8)20;
  xKeyWsp.vAddElement(xValWsp);
  KeyVal* pKeyValThat = new KeyVal(xKeyWsp);
  
  // [ ColA <= 1 , ColB < 10 ] should be before (2,20)
  b = BndLib::bIsBefore(pKeyValThis->xKey(),
                        pKeyValThat->xKey(),
                        *pOpAry,
                        *pColVec);
  assert(b);
  
  // [ ColA <= 2 , ColB < 20 ] should not be before (1,10)
  b = BndLib::bIsBefore(pKeyValThat->xKey(),
                        pKeyValThis->xKey(),
                        *pOpAry,
                        *pColVec);
  assert(!b);
  
  delete pKeyValThis;
  delete pKeyValThat;
  
  vTrace("\n\nEnd KeyTest A++++++++++++++++++++++++++++++++++++++++++++\n"_Str);
}

/*----------------------------------------------------------------------------*/
/* key testing                                                                */
/*----------------------------------------------------------------------------*/
void KeyTest::vTestB(void)
{
  vTrace("\n\nKeyTest B:%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n"_Str);
  
  s32 sSeed = ""_Str;
  
  NNP_Task nnpNewTask = new Task;
  
  nnpNewTask->vSeedWord(sSeed);
  
  delete *nnpNewTask;

  vTrace("\n\nEnd KeyTest B%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n"_Str);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
