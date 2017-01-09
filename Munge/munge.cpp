/*======
/* 1m.se
/*======

\"1m.sen

: 1m 100

/*======
/* end  
/*======
/*======
/* actor
/*======

\"actors

\"lister
\"monito
//\"recl
\"reques

\"genera

/*------
/* calle
/*------
: startA

  \"star

  args.b

    snoo

    snoo
    snoo

    //sn
    snoo

  then

  //dic.

  reques
  ;

/*======
/* end  
/*======
/*======
/* Answe
/*======

\"Answer

/*------
/* defin
/*------
actor: a

/*------
/* decla
/*------
int: con

/*------
/* start
/*------
: answer

  0 \"an

  \"ans\
;

/*------
/* answe
/*------
: ans.lo

  answer

  args.v
    \"wa
    args
  then

  args.p

  locals

  0 conn
  begin

tp: wait

    list

tp: sock

    args
      \"
    then

  // sta
  //\"st

ddlLock.

    // s
    1 \"

    // c
    \"rr
    :act

    // s
    acto

ddlLock.

    conn
    1 + 
    dup 
    conn

    1  =
      ar
        
      th
    then
      
  again
;

/*======
/* end  
/*======
/*======
/* arith
/*======

\"arithm

: arithm

  \"  ar

  0 0 + 

  1 1 + 

  8 5 - 

;

/*======
/* end  
/*======
/*======
/* catal
/*======

\"catalo

: catalo

: :catal
    :dic
        
        
;

: within
    0 di
;

: checkI
    with
    EC_N
;

: checkI

    dup 
    dup 
    EC_N
;

: checkI
    dup 
    EC_C
;

/*------
/* check
/*------
: checkM

  checkI
  checkI

  true
;

/*------
/* check
/*------
: checkR

  dup ( 

  checkI
  checkI
  checkI
  checkN

  drop (

  true (
;

/*------
/* make 
/*------
: makeCa

  args.t
    dup 
  then

  find e
  \"cata
  swap  
  :folde
;

/*------
/* make 
/*------
: makeCa

tp: make

  args.t
    dup 
  then

  dup ( 

  :catal

  drop (

  makeSc

  true <
;

/*------
/* have 
/*------
/*
: ~makeC

tp: mkTe

  // set
  $$ swa

tp: theI

  $$>val

  >>allP

tp: mkCa

  checkR
;
*/

/*------
/* remov
/*------
: remove

tp: remo

  args.t
    dup 
  then

  :forge

  true <
;

/*------
/* remov
/*------
/*
: ~remov

tp: rmCa

  // set
  $$ swa

tp: theI

  $$>val

  >>allP

tp: rmCa

  checkR
;
*/
/*======
/* end  
/*======
/*======
/* colum
/*======

\"column

: column

//: make

: makeCo

  find e
  table.
;

: within
    0 di
;

: checkI
    with
    EC_N
;

: pushCo

    \"co

    not 

    exec

    true
;

: popCol
    dic.
;

: :colum

    :con
;

: checkA

  checkI

  true
;

/*------
/* add a
/*------
: addCol

  args.t
    dup 
  then

tp: addi
  pushTa

  pushCo

  :colum

  popCol

  popTab

  true <
;

/*------
/* have 
/*------
: ~addCo

tp: addC

  // set
  >r >r 

  tp: th

  $$>val

  >>allP

  tp: ad

  checkR
;

/*------
/* list 
/*------
: listCo

  pushTa
  pushCo

  \"\"  
  0 dic.
  dup   
  iterat

    obje

    \" \
    obje

    swap

    dup 
    if \

    str.

    obje

  next  

  //\"\n

  popCol
  popTab
;

/*======
/* end  
/*======
/*======
/* commo
/*======

\"common

\"dir.se
\"file.s
\"lock.s
\"timer.

\"trace.

/*======
/* end  
/*======
/*======
/* compi
/*======

\"compil

//
// step 
//


\"core.s
\"common
\"struct
\"tasks.
\"thread
\"transa
\"interf
\"system
\"gridMo
\"dbobje
\"actors
\"reques
\"munge.
\"testin
assert.s

//
// step 
//

//ut

assert.s

//
// step 
//

( spawn 
rootThre

( leave 
rootThre

assert.s

: __comp

/*======
/* end  
/*======
/*======
/* conne
/*======

\"connec

/*------
/* defin
/*------
actor: c

/*------
/* start
/*------
: connec

tp: conn

  0 \"co

  \"con\
;

/*------
/* conne
/*------
: con.lo

  connec

  args.n

    I ar

      nu
      nu

    else

      ar
      \"
      so

      ar
        
        
        
        
      th

      //
      //

ddlLock.

     // 
      1 

      //
      \"

      //
      du
      du

      //
      ac

ddlLock.

    then

  loop

  //1000

;

/*======
/* end  
/*======
/*======
/* coord
/*======

\"coordi

/*------
/* coord
/*------
: cp.loo

  \"cp.l

tp: coor

  locals

  begin

    dept

    // p
    task

    addS

    forw

    // h
    addP

    0   
    >snd

  again
;

/*======
/* end  
/*======
/*======
/* coord
/*======

\"coordi

/*------
/* coord
/*------
: c.loop

  \"c.lo

  // pus
  reg.pu

  begin
    dept

    // p
    task

    // e
    exeA

    // p
    a> (

    // p
    >cal

  again

  // pop
  reg.po
;

/*------
/* put i
/*------
: enqueu

  // get
  ppTask

  // pus
  >taskQ
;

/*------
/* coord
/*------
: sendPa

  args.n

tp: acks

  >valse

  >r

  args.n

    2 rp

    vals

    I

    enqu

  loop

  rdrop

  // thi
  ackWai

  // mov
  a>

  // not
  // tha
  // doe

//traceo
  true >

  begin
    dept
    vals
    not 
      rd
    then
  repeat

  r>
//traceo

tp: allR
;

//int: f

// - - -
: sendPa

  // a) 
  // b) 
  // c) 
  //    

tp: send

  // lay
  addSet

  // mov
  >r

tp: move

  // now
  args.n

    // s
    1 se

    // g
    2 rp

    // p
    I en

tp: ackW

    // t
    ackW

    // n

tp: runn

  loop /

  // - -

  // eac
  //   <

  // << 

  true l

  $$
tp: $$

  // now
  args.n

    // m
    a>

    tp: 

    // n
    vals

    tp: 

    // d
    drop

    // c
    dup 
      0 
    then

  loop

  args.n

  finalR

tp: alld

  $$>val

  >a

  // dro
  rdrop

  // ret
  //true

tp: allR
;

/*------
/* coord
/*------
: sendPa

  // a) 
  // b) 
  // c) 
  //    

tp: \"- 
tp: send

  locals

  // lay
  addSet

  // set
  1 setA

  // usi
  partic

tp: ackW

  // thi
  ackWai

  // now

tp: runn

  $$

  // mov
  a>

tp: have

  // non
  valset

tp: expa

  // dro
  drop

  // num
  1

  // all
  true


  $$>val

  >a

tp: resp
;

/*======
/* end  
/*======
/*======
/* copie
/*======

\"copier

: copy (

  swap
  >r >r
  // r0 
  // r1 

  \"__ro
    drop
  then
  execut

  >r

  // r0 
  // r1 
  // r2 

  r2 r0 

  beginT
  // r0 
  // r1 
  // r2 
  // r3 

  args.p
  do
    // r
    // r
    // r
    // r
    // r
    // r

    I r3
    r3 r

    begi
      wh
      r0
      r3
    repe

    r3 r
  loop

  commit

  // r0 
  // r1 
  // r2 
  // r3 

  rdrop 

  \"__ro

  true
;

: tableC

  find e

  swap  

  :table

  // swa

    copy
;

/*======
/* end  
/*======
/*======
/* core.
/*======

\"core.s

/*------
/* unnam
/*------
: tp imm

  \"tp\"

  encode

  ' trac
;

/*------
/* named
/*------
: tp: im

  token:

  encode

  ' trac
;

/*------
/* first
/*------
: snap0:

  ' bp.s

  ' >val

  ' >reg
;

/*------
/* secon
/*------
: snap1:

  ' bp.s

  ' >val

  ' >reg
;

/*------
/* add 1
/*------
: 1+ 1 +

/*------
/* incre
/*------
: ++ dup

/*------
/* decre
/*------
: 1- 1 -

/*------
/* decre
/*------
: -- dup

/*------
/* begin
/*------
: BEGIN 

  here
;

/*------
/* do a 
/*------
: DO imm

  POSTPO

  HERE t
;

/*------
/* ?do a
/*------
: ?DO im

  POSTPO

  POSTPO

  false 
;

/*------
/* unloo
/*------
: UNLOOP

  POSTPO
  POSTPO
;

/*------
/* itera
/*------
: ITERAT

  POSTPO

  POSTPO
;

/*------
/* defin
/*------
: ,_ALT

  HERE !
;

/*------
/* defin
/*------
: consta

  create
  ,
;

/*------
/* defin
/*------
: :const

  >creat
  ,
;

/*------
/* resol
/*------
: ELSE i

  POSTPO
  1 CS-R
  POSTPO
;

/*------
/* condi
/*------
: WHILE 

  POSTPO
  1 CS-R
;

/*------
/* resol
/*------
: REPEAT

  POSTPO
  POSTPO
;

/*------
/* initi
/*------
0 CONSTA
true ' C

/*------
/* OF bl
/*------
: OF imm

  1+   /
  >R   /

  POSTPO
  POSTPO
  POSTPO
  POSTPO

  R> // 
;

/*------
/* ENDOF
/*------
: ENDOF 

  >R // 

  POSTPO

  R> // 
;

/*------
/* ENDCA
/*------
: ENDCAS

  POSTPO

  0 ?DO 
    POST
  LOOP
;

/*------
/* case 
/*------
: caseTe

  10 0 d
    I ca
      0 
      1 
      2 
      3 
      4 
      5 
      6 
      7 
      8 
      9 
    endc
  loop
;

/*------
/* list 
/*------
: words
    0 di
    iter
      ob
      ob
    next
;

/*------
/* list 
/*------
: symbol
    0 di
    begi
      du
      wo
    repe
    drop
;

/*------
/* get a
/*------
: listCo

    \"\"
// \"(\"
    0 di
    iter
      ob
        
      \"
      ob
    next
//    \"

;

/*------
/* dump 
/*------
/*
: stk

  \"para

  depth 

  depth 
    I pi
  loop
;
*/

/*------
/* dump 
/*------
/*
: astk

  \"auxi

  adepth

  adepth
    I ap
  loop
;
*/

/*------
/* dump 
/*------
: rstk

  \"retu
  rdepth
  0 do
    I 2 
    dup 
    \" \
    loca
    dup 
    .\n
  loop
;

////////

/*------
/* inter
/*------
: >: inp

/*------
/* check
/*------
: checkI
    find
    dup 
    true
;

/*------
/* forge
/*------
: :forge

  find (

  dup no

  >forge
;

/*------
/* drop 
/*------
: nipAll

  // han
  depth 

  // sav
  >r

  // dro
  begin
    dept
      dr
  repeat

  // res
  r>
;

/*------
/* drop 
/*------
: dropN 

  // han
  depth 

  // dro
  0 do
    drop
  loop
;

/*------
/* test 
/*------
: isType

  // fal
  depth 

  // no 
  1 stac
;

/*------
/* snooz
/*------
: snooze

  C_1M t
;

/*------
/* an en
/*------
: $$
  //1234

  [ word
;

/*------
/* test 
/*------
: $$test

  $$ $$ 
  if
    \"eq
  else
    \"no
  then

  \"a\" 
  if
    \"eq
  else
    \"no
  then

  1 $$ e
  if
    \"eq
  else
    \"no
  then
;

/*------
/* get d
/*------
: $$dept

  // if 
  depth 

  depth 

    // m
    I st
    if
      I 
      un
      ex
    then

    // m
    I pi
    if
      I
      un
      ex
    then

  loop

  // mar
  depth
;

/*------
/* get d
/*------
: $$adep

  // if 
  adepth

  adepth

    // m
    I as
    if
      I 
      un
      ex
    then

    // m
    I ap
    if
      I
      un
      ex
    then

  loop

  // mar
  adepth
;

/*------
/* put $
/*------
: put$$ 

  $$ swa
;

/*------
/* conve
/*------
: $$>val

  $$dept
  >valse
  depth 
    1 pi
    if
      ni
    then
  then
;

/*------
/* conve
/*------
: $$>ava

  $$adep
  >avals
  adepth
    1 ap
    if
      an
    then
  then
;

/*------
/* test 
/*------
: $$vs

  $$
  1 2 3
tp: a
  $$>val
tp: b
  \"a\" 
tp: c
  $$>val
tp: d
  valset
tp: e
  1 - dr
  valset
tp: f
  drop
tp: g
;

//traceo
: tp:tes

\"dic.se
\"var.se
\"useful

/*======
/* end  
/*======
/*======
/* dbObj
/*======

\"dbObje

\"rowCur
\"table.
\"column
\"schema
\"catalo
\"tenant
\"user.s

\"row.se
\"rowBuf

/*======
/* end  
/*======
/*======
/* ddlLo
/*======

\"ddlLoc

/*------
/* lock 
/*------
args.nod

/*------
/* dump 
/*------
: ddlDum

  \"{ \"
  args.n
    I dd
  loop

  \"}\" 
;

/*------
/* dispa
/*------
: dispat

  addSet

  // det
  dup ( 
  args.n
  == ( .
  if ( .

    // a
    pTas

  else

    // a
    ppTa

  then


  // tra
  >taskQ
;

/*------
/* attem
/*------
: grabDD

  // tes

  dup ( 
  ddlInt
  if ( n

   // dd

   // dr
   drop 

   // re
   false
   exit

  then

  // ddl

  // put
  1 ( n 
  swap (
  ddlInt

  // ret
  true (
;

/*------
/* drop 
/*------
: dropDD

  // put
  0 ( n 
  swap (
  ddlInt

  // ret
  true (
;

////////


/*------
/* grab 
/*------
: grabDi

  // dis

  begin
    0   
    \"gr
    2   
    0   
    ( 0 

    // s
    disp
  until

  // dis

  args.n
    I   
    \"gr
    2   
    I   
    ( I 

    // s
    disp

    // j
    . ( 
  loop

  true (
;

/*------
/* drop 
/*------
: dropDi

  // dis

  args.n
    I   
    \"dr
    2   
    I   
    ( I 

    // s
    disp

    // j
    .
  loop

  // dis

  0     
  \"drop
  2     
  0     
  ( 0 \"

  // sen
  dispat

  // jus
  .

  true (
;

/*------
/* distr
/*------
: dl

  // put
  args.n
    0 I 
  loop

  // coo
  \"grab

  addSet
  >coord
  .

  ddlDum

  // coo
  \"drop
  addSet
  >coord
  .

  ddlDum

stk
;

/*======
/* end  
/*======
/*======
/* dic.s
/*======

\"dic.se

/*------
/* folde
/*------
: folder

: isFold

/*------
/* creat
/*------
: dic:
diclock.
    crea
    WT_O
    size
    word
    CN_D
    ' fo
    word
    true
diclock.
    does
;

/*------
/* creat
/*------
: :dic
diclock.
    >cre
    WT_O
    size
    word
    CN_D
    ' fo
    word
    true
    word
diclock.
    does
;

: checkI

    dup 
    dup 
    drop
    EC_N
;

: dicHas
    dup 
;

/*------
/* check
/*------
: checkN

  dicHas

  dicHas

  true
;

/*------
/* creat
/*------
: :folde
  {{ :di
;

/*------
/* get s
/*------
: seitf.

  // thi
  // ( .

  dic.co
    // c
    //di

    I
    // g
    dic.

    // p
    dic.
  loop

  // pus
  dic.co
;

/*------
/* set t
/*------
: dicSta

  // thi
  // ( .

tp: dicS
  dic.co
    dic.
  loop

  0 do

    dup 
      dr
      di
      di
    else

      du
        
        
        
      el

        
        
        
      th

    then

  loop
;

/*======
/* end  
/*======
/*======
/* dir.s
/*======

\"dir.se

/*------
/* dir i
/*------
: dir ;

: isDir 

/*------
/* creat
/*------
: dir:
    crea
    WT_O
    size
    word
    CN_D
    ' di
    word
    true
    does
;

/*------
/* creat
/*------
: :dir
    >cre
    WT_O
    size
    word
    CN_D
    ' di
    word
    true
    word
    does
;


: dirtes

//traceo

    \"__

    \"/t

    r0 d

    begi
      r0
      wh
        
    repe

    r0 d

    rdro

    \"__

;

/*======
/* end  
/*======
/*======
/* expor
/*======

\"export

: export

  swap f

  >r
  // r0 

  r0 fil
  if \"f

  \"__ro
    drop
  then
  execut

  >r
  // r0 
  // r1 

  r0 row

  args.p
  do
    // r
    // r

    I r2
    r2 r
    begi
      wh
      r2
//dup .
      r3
      \"
      r2
    repe

    r2 r
  loop

  r1 fil

  rdrop 

  \"__ro

  true
;

: export

//traceo

    \"/t

    expo

traceoff
;

: tableE

  find e

  export
  if
    \"ex
  else
    fals
  then
;


/*======
/* end  
/*======
/*======
/* file.
/*======

\"file.s

/*------
/* file 
/*------
: file ;

: isFile

/*------
/* creat
/*------
: file:
  create
  WT_OBJ
  sizeof
  word.t
  CN_FIL
  ' file
  word.t
  true w
  does>
;

/*------
/* creat
/*------
: :file
  >creat
  WT_OBJ
  sizeof
  word.t
  CN_FIL
  ' file
  word.t
  true w
  word.t
  does>
;

/*======
/* end  
/*======
/*======
/* fileG
/*======

\"fileGe

timer: f

: fileGe
//traceo

  reg.pu

  // cre
  \"__fi
    drop
    \"__
  then
  execut

  >reg0 
  >reg4 
  >reg3 
  >reg2 
  >reg1 

  0 >reg

  reg1 r
  open_w

  fileGe

  begin
    reg3
    dup 
    reg0

    reg5
  repeat

  fileGe

  \"file
  fileGe

  reg0 f

  // des
  \"__fi

  reg.po

//traceo
;

: gli

  \"/tmp
;

/*======
/* end  
/*======
/*======
/* flowT
/*======

\"flowTe

: flowTe

  \"  fl

  assert
  0
  assert
  begin
    asse
    1
    asse
    +
    asse
    dup
    asse
    10
    asse
    ==
    asse
  until
  assert
  drop
  assert

;

/*======
/* end  
/*======
/*======
/* gener
/*======

\"genera

/*------
/* compu
/*------
: noGene

  // the
  // siz

  // the
  // the

  C_1GB 

  noThre
    1
  else
    noTh
      CO
    then
  then

  noThre
;

/*------
/* defin
/*------
args.reg

N_GENERA

/*------
/* creat
/*------
: >genTa
: >genTh

: :genTa
: :genTh

: >genTa
: >genTh

: genera

  args.v

  N_GENE

    I :g
    I :g

    I >g
    I >g
    I >g

  loop
;

/*======
/* end  
/*======
/*======
/* grid.
/*======

\"grid.s

/*------
/* grid 
/*------
: grid ;

: isGrid

: <SUCCE

: <FAILU

/*------
/* check
/*------
: checkR

tp: chec

  // was
  true =

    // d
    dept

  else
    // l
    0 do

      fa

        
        

      el

        

      th

    loop

  then
;

/*------
/* print
/*------
: printR

tp: prin

  // was
  true =

    // p
    0 do

      as
      \"
    loop

  else

    // l
    0 do

      fa

        
        

      el

        

      th

    loop

  then

;


/*------
/* creat
/*------
: grid:
    crea
    WT_O
    size
    word
    CN_G
    word
    ' gr
    true
    does
;

/*------
/* creat
/*------
: :grid
    >cre
    WT_O
    size
    word
    CN_G
    word
    ' gr
    true
    word
    does
;

////////

grid: gr

0 node.n

/*======
/* end  
/*======
/*======
/* gridC
/*======

\"gridCa

/*------
/* creat
/*------
: create

  /*

    r->C
       |
       +

  */

  args.n

    //
    // p
    //

    0 \"

    \"p\
    dup 
    dup 
    acto

    //
    // c
    //

    I I 
    \"cp
    dup 
    dup 
    acto

    //
    // p
    //

    I ar
      I 
    else
      I 
    then

    \"pp
    dup 
    dup 
    acto

  loop

  //
  // coo
  //
  0 \"c.
  \"c\" 

  actor.

  args.v
    cpQu
  then
;

////////

createGr

////////

/*------
/* queue
/*------
: >coord

tp: >coo

/*
  // cou

  addSet
*/

  // tar
  c.task

tp: resu

  // we 
;

/*------
/* get c
/*------
: >>oneP

  // a) 
  // b) 

  tp: >>

  // che
  args.n
    grid
      \"
    then
  then

  // ass
  2 task

  // sav
  >r

  // add
  addSet

  // add
  r> add

  // add
  addSet

  // que
  >coord

  valset

  // dro
  drop
;

/*------
/* get c
/*------
: >>allP

  // a) 
  // b) 

tp: >>al

   // ch
   args.
     gri
       d
       E
     the
  then

  task.t

  // add
  addSet

  // add
  addSen

  // add
  addSet

  // que
  >coord

  valset

  // dro
  drop
;

/*======
/* end  
/*======
/*======
/* gridM
/*======

\"gridMo

\"node.s
\"grid.s
\"nodeCu

\"gridPr

\"coordi
\"partic
\"coordi
\"partic
\"receiv
\"sender

\"connec
\"answer

\"gridCa
\"ddlLoc

/*======
/* end  
/*======
/*======
/* gridP
/*======

\"gridPr

////////
////////

/*------
/* task 
/*------
args.nod
args.nod
args.nod
args.nod

args.nod
args.nod
args.nod
args.nod

/*------
: to_CAL
: to_QUE
: to_TAS
: to_THR

: _CALLE
: _QUEUE
: _TASKS
: _THREA

/*------
/* dump 
/*------
: ppQueu

  \"{ \"
  args.n
    I pp
  loop

  \"}\" 
;

/*------
/* dump 
/*------
: cpQueu

  \"{ \"
  args.n
    I cp
  loop

  \"}\" 
;

/*------
/* dump 
/*------
: pQueue

  \"{ \"
  args.n
    I pQ
  loop

  \"}\" 
;

/*------
/* dump 
/*------
: sender

  \"{ \"
  args.n
    I se
  loop

  \"}\" 
;

////////
////////

/*------
/* appen
/*------
: >taskQ

tp: >tas

  locals

  // gra
  target

  // app
  target

  // sig
  target
    targ
  then

  // dro
  target
;

/*------
/* appen
/*------
: >taskQ

  // set
  1 setA

  // app
  >taskQ

  tp: ac

  // thi
  ackWai

  tp: ru

  // mov
  a> ( <

tp: have
;

/*------
/* put i
/*------
: >sndQu

  // get
  sender

  tp: se

  // enq
  >taskQ
;

/*------
/* put i
/*------
: >sndQu

  // thi
  1 setA

  // get
  sender

tp: send

  // enq
  >taskQ

  // thi
  ackWai

tp: runn

  // mov
  a> ( <

tp: have
;

/*------
/* pop v
/*------
: taskQu

  tp: ta

  // gra
  task.t

  // att
  popQue
  not if
    // q
    sign
    // p
    popQ
  then

  // gra
  task.t

  tp: ha
;

/*------
/* push 
/*------
: >calle

tp: >cal

  // tra
  _CALLE

  tp: ca

  >auxSt

  // ack
  _CALLE
;

////////
////////

/*------
/* set t
/*------
: setCal

  to_CAL

  tp: re
;

/*------
/* set s
/*------
: setSen

  to_QUE

  tp: re
;

/*------
/* pass 
/*------
: passOn

  cpTask

  >taskQ
;

/*------
/* pass 
/*------
: passBa

  reg.pu

  ppTask

  >calle

  reg.po
;

////////
////////

/*------
/* execu
/*------
: !indir

tp: exec

  // fin
  find (
        

  // exe
  execut
;

/*------
/* conve
/*------
: >resul

tp: resu

  // put
  depth 

  // con
  >valse

tp: resu
;

/*------
/* expan
/*------
: exeOne

  valset

  drop (

  tp: ca

  !indir
;

/*------
/* execu
/*------
: exeAll

tp: exeA

  // we 
  CN_VAL

  // exe
  exeOne

  // exe
  begin

    // n
    CN_V
    if
      ex
    then

  // exe
  exeOne

  again
;

////////
////////

/*------
/* add c
/*------
: addSen

  \"send

tp: addi

  $$>val
;

/*------
/* add c
/*------
: addSen

  \"send

tp: addi

  $$>val
;

/*------
/* add c
/*------
: addSet

  task.t

tp: addi

  $$>val
;

/*------
/* add c
/*------
: addSet

  // add
  seitf.
  \"dicS

tp: addi

  $$>val
;

/*------
/* add c
/*------
: addSet

  args.n

tp: addi

  $$>val
;

/*------
/* add c
/*------
: addPas

  args.n

  tp: ad

  $$>val
;

/*------
/* add c
/*------
: addPas

  \"pass

tp: addi

  $$>val
;

/*------
/* inter
/*------
: interl

  // att
  _QUEUE
  _CALLE
  task.q

  // put
  _CALLE
  _TASKS
  taskse

  // att
  _TASKS
  _THREA
  thread

  // rec
  _TASKS
  _CALLE
  task.t
;

/*------
/* get t
/*------
: actor.

  \".tas
;

/*------
/* get q
/*------
: actor.

  \".que
;

/*------
/* get t
/*------
: actor.

  \".tas
;

/*------
/* get t
/*------
: actor.

  \".thr
;

/*------
/* spawn
/*------
: actor.

  locals

  // see
  actorN

  // spa
  actorN
;

/*------
/* creat
/*------
: actor:

  // get
  token:

  locals

  reg.pu

  actorN
  actorN
  actorN
  actorN

  interl

  reg.po
;

/*------
/* creat
/*------
: :actor

  locals

  reg.pu

  actorN
  actorN
  actorN
  actorN

  interl

  reg.po

  actorN
;

////////
////////

/*------
/* path 
/*------
: SE_PAT

/*------
/* form 
/*------
: se_com

  dup
  dup

  //  \"
  \"\"
  SE_PAT
  \" --s
  \" --s
  //  \"
  \" --c
  \" --r
  \" --n
  \" --n
  \" --p
  \" --p
  \" --s
  args.t
  args.v

  \" &\"
;

/*------
/* spawn
/*------
: spawnN

  // tes
  args.n

  // spa
  args.n

    // f
    I se

    // p
    dup 

    // e
    shel

    // c
    not 

  loop

  // set
  true g

;

/*======
/* end  
/*======
/*======
/* inter
/*======

\"interf

\"seitfL
\"seitfC

/*======
/* end  
/*======
/*======
/* ipc.s
/*======

\"ipc.se

: isRunn

: getSta
: setSta

: setAck
: ackWai
: isAckW
: ack   

: append
: popQue

: grabMu
: waitSi
: dropMu
: isSign
: signal

: signal

  TS_WAI

tp: nowW

  waitSi

TS_RUNNI

  tp: no
;

/*======
/* end  
/*======
/*======
/* jData
/*======

\"jData.

: >Digit

  >str \

  0 >r
  begin 
    swap
    dup 
    str.
    whil
      du
      0 
      sw
      1 
      -r
      >i
      >d

      r>
      if

      st
  repeat
  drop  
  rdrop
;

: jData1

  file.n

  \"../J

  OPEN_W

  1000 0
  do
    I >s
    \"|\
    I >D
    \"|\
    I 10
    \"|\
    I 10
    \"\n
  loop

  r0 fil

  rdrop
;

: jData1

  file.n

  \"../J

  OPEN_W

  100 0
  do
    I >s
    \"|\
    I >D
    \"|\
    I 10
    \"\n
  loop

  r0 fil

  rdrop
;

: jData1

  file.n

  \"../J

  OPEN_W

  10 0
  do
    I >s
    \"|\
    I >D
    \"\n
  loop

  r0 fil

  rdrop
;

: jData

  jData1
  jData1
  jData1
;

/*======
/* end  
/*======
/*======
/* liste
/*======

\"lister

/*------
/* creat
/*------

taskset:

thread: 

taskCurs

/*------
/* start
/*------
: lister

   0 \"l

   liste
   \"lis
        
;

/*------
/* liste
/*------
: iterat

    list
      \"
    then

  taskcr
        
  iterat

    list
      ob
    then

    curs

  next

  taskcr

;

/*------
/* liste
/*------
: lis.lo

    \"li

    list

    begi

      it

      3 

    agai
;

/*======
/* end  
/*======
/*======
/* loade
/*======

\"loader

/*------
/* creat
/*------
: :loade

  >creat
  WT_OBJ
  sizeof
  word.t
  CN_LOA
  word.t
  true w
  word.t
  does>
;

timer: l

/*------
/* paral
/*------
: tableL

  find e

  // che
  swap f
  dup r0
  OPEN_R
  if r0 

  loadTi

  // cre
  \"__ta
    drop
    \"__
  then
  execut

  // put
  >r
  // r0 

  // ini
  r0 loa
  r0 loa

  //- - 

  // beg
  beginT

  //- - 

  task.t

  N_GENE

  N_GENE

    $$ r

    seit

    1 \"

    dup 

    I >g
  loop

  //- - 
  begin 


  ackWai

  task.t

  //- - 

  // com
  commit

  // dis
  rdrop

  // des
  \"__ta

  loadTi

  \"Load
  loadTi

  true
;

/*------
/* loadi
/*------
: tableL

  // che
  swap f
  dup r0
  OPEN_R
  if r0 

stk

  reg.pu

  file.n
  >reg1 
  >reg2 
  rowBuf
  beginT
  0 >reg

  reg2 r
  OPEN_R

  loadTi

  begin
    reg0
    whil
      re
      no
        
        
        
        
      th

      re
      10
  repeat

  reg3 0

  loadTi

  \"Load
  loadTi

  reg0 f

  reg0 f

  reg.po

  true

stk
;

/*------
/* loadi
/*------
: tableL

  find e

  // che
  swap f
  dup r0
  OPEN_R
  if r0 

  stk

  reg.pu

  file.n
  >reg1 
  >reg2 
  rowBuf
  beginT
  0 >reg

  reg2 r
  OPEN_R

  loadTi

  begin
    reg0
    whil
      re
      no
        
        
        
        
      th

      re
      10
  repeat

  reg3 0

  loadTi

  \"Load
  loadTi

  reg0 f

  reg0 f

  reg.po

  true

  stk
;

/*======
/* end  
/*======
/*======
/* loade
/*======

\"loader

/*------
/* creat
/*------
: :loade

  >creat
  WT_OBJ
  sizeof
  word.t
  CN_LOA
  word.t
  true w
  word.t
  does>
;

/*------
/* form 
/*------
: >loadS

  \"sl\"
;

/*------
/* appen
/*------
: loadAp

tp: load

  // why
  //   l
  //   a

  // whe
  //   i

  // wha
  //   f
  //   t

  1 task

  args.t
    dup 
  then

  // get
  find e

  // app
  table.

  true <
;

/*------
/* forwa
/*------
: ~loadA

tp: ~loa

  // why
  //   l
  //   t

  // whe
  //   i
  //   p
  //   r

  // wha
  //   w
  //   t

  13 tas
  task.p

  //
  // a) 
  //

  $$
    load
    null
    load
    load
    \"lo

tp: theI

  $$>val

  //
  // b) 
  //    
  //

  // to 
  loader

  // for
  >>oneP

tp: theI

  // we 
  //asse

  checkR
;

/*------
/* paral
/*------
: loadSu

  // thi

  // fir
  // nee

  task.t
  task.t

  // exp
  valset

  // exp
  valset

  reg.pu

  // the
  >reg0

  // get
  //   o
  //   r
  loader

  // loa
  begin

    // a
    load

    // p
    whil

      //
      lo

      //
      lo

      //
      no

tp: buff

        
        

        
        
      th
  repeat


  // che
  args.p

    // s
    I lo

    // t
    load
    if
      //
      ~l
    then

  loop

  // clo
  loader

  // thi
  task.p

  reg.po

//traceO
;

/*======
/* end  
/*======
/*======
/* lock.
/*======

\"lock.s

/*------
/* lock 
/*------
: lock ;

: isLock

/*------
/* creat
/*------
: lock:
  create
  WT_OBJ
  sizeof
  word.t
  CN_LOC
  ' lock
  word.t
  true w
  does>
;

/*------
/* creat
/*------
: :lock
  >creat
  WT_OBJ
  sizeof
  word.t
  CN_LOC
  ' lock
  word.t
  true w
  word.t
  does>
;

/*======
/* end  
/*======
/*======
/* logic
/*======

\"logicT

: logicT

  \"  lo

  0 if \

  1 if e

  0 not 

  0 0 ==

  1 1 ==

  0 1 no

;

/*======
/* end  
/*======
/*======
/* miscT
/*======

\"miscTe

: iftest

: ifelse

: untilt

: whilet

: dotest
: ?dotes

: +loopt

: iterat
    word
      ob
      ob
    next
;

: string

traceon
    \"ab
    1000
      \"
      st
    loop
;

str: str

: strvar


traceon

  \"a\" 
  100000
    strv
  loop

;

: rottes

  2 1 0 
;

: invrot

  2 1 0 
;

/*------
/* globa
/*------
: global

  reg.pu

  2048 s

  >reg0

  reg0

  valset

  . .

  reg.po
;

: global

//traceo

  reg.pu

  // cre
  // cur
  1 neg 

  // sho
  //stk

  // con
  >valse

  // put
  >reg0

  // cop
  reg0 >

  // cop
  reg1 /

  // exp
  valset

  // sho
  stk

  reg.po
;

/*------
/* str.f
/*------
: strFor

  1 \"ab

  \"def\

  \"gh\"
  \"%v %
;

/*------
/* local
/*------
: lt

  789 45

  a . b 

  321 to
  654 to
  789 to

  a . b 
;

/*------
/* postp
/*------
: callTe

  test
;

: postpo

  postpo
;

/*------
/* get a
/*------
: snapsh

  reg.pu

  bp.sna

  >valse
  >reg0

  \"Firs
  reg0
  valset
  0 do
    I cl
    \"%v
  loop

  bp.sna

  >valse
  >reg1

  \"Seco
  reg1
  valset
  0 do
    I cl
   \"%v 
  loop

  reg1 r

  \"Diff
  0 do
    I cl
    \"%v
  loop

  reg.po
;

/*======
/* end  
/*======
/*======
/* monit
/*======

\"monito

/*------
/* creat
/*------

lock:   

str:    

taskset:

thread: 

/*------
/* start
/*------
: monito

   0 \"m

   monit
   \"mon
        
;

/*------
/* monit
/*------
: mon.lo

  \"moni

  \"\" m

  monito

  begin

    moni

      \"

      \"

      qu

      mo

    moni

    // m

    moni

  10 thr


  again
;

/*------
/* get m
/*------

: monito

    moni

      mo

    moni
;

/*======
/* end  
/*======
/*======
/* munge
/*======

\"munge.

/*------
/* munge
/*------
: mungeS

  // ...

  \"\" f

  senFil

  OPEN_R

  begin 

    dup 

    senF
      \"
      se
      .\

    senL

  repeat

  // clo
  senFil
  senFil
;

/*------
/* munge
/*------
: munge

  // cre
  file.n

  // set
  \".\" 

  // ope
  comman

  // ope
  \"../M
  OPEN_W

  // rea
  begin 

    // i
    comm

    // i
    4 co
    comm

    // c
    comm

  repeat

  // clo
  cppFil
  cppFil

  // clo
  comman
  comman
;

/*======
/* end  
/*======
/*======
/* node.
/*======

\"node.s

/*------
/* node 
/*------
: node ;

: isNode

/*------
/* creat
/*------
: node:
    crea
    WT_O
    size
    word
    CN_N
    word
    ' no
    true
    does
;

/*------
/* creat
/*------
: :node
    >cre
    WT_O
    size
    word
    CN_N
    word
    ' no
    true
    word
    does
;

/*======
/* end  
/*======
/*======
/* nodeC
/*======

\"nodeCu

/*------
/* creat
/*------
: nodeCu
    crea
    WT_O
    size
    word
    CN_N
    word
    true
    does
;

/*------
/* creat
/*------
: :nodeC
    >cre
    WT_O
    size
    word
    CN_N
    word
    true
    word
    does
;

////////

: ::node

    \"__
    not 
      dr
      \"
    then
    exec
;

: ~nodeC

    \"__
;

: listNo

    ::no

    r0 n
    r0 n
    begi
      wh
      r0
      no
      r0
    repe

    rdro

    ~nod
;

/*======
/* end  
/*======
/*======
/* parti
/*======

\"partic

/*------
/* parti
/*------
: pp.loo

  \"pp.l

tp: part

  // pus
  reg.pu

  locals

  //>r (
  //>r (
  // r0 
  // r1 

  task.t

  begin

    dept

    // p
    task

    // s
    exeO

    acto
    if
      //

      ad

      fo

    else
      //

      ad

      ac

      ac

    then

tp: resu

    >cal

  again

  // rdr
  // rdr

  // pop
  reg.po
;

/*======
/* end  
/*======
/*======
/* parti
/*======

\"partic

/*------
/* execu
/*------
: p.loop

  \"p.lo

  // pus
  reg.pu

  begin

    dept

    // p
    task

    exeA

    // c
    >res

    // p
    >cal

  again

  // pop
  reg.po
;

/*======
/* end  
/*======
/*======
/* queue
/*======

\"queue.

: queue 

: queue:
    crea
    WT_O
    size
    word
    CN_Q
    ' qu
    word
    true
    does
;

: :queue
    >cre
    WT_O
    size
    word
    CN_Q
    ' qu
    word
    true
    word
    does
;

/*======
/* end  
/*======
/*======
/* Recei
/*======

\"Receiv

/*------
/* recv 
/*------
: recv> 

  task.t

  tp: re

  socket

tp: rece
;

/*------
/* recei
/*------
: rcv.lo

tp: snd.

  task.t

  \"rcv.

  receiv

  begin
    dept

    recv

    exeO

  again
;

/*======
/* end  
/*======
/*======
/* recla
/*======

\"reclai

/*------
/* creat
/*------

lock:   

str:    

taskset:

thread: 

seitfCur

/*------
/* start
/*------
: reclai

   0 \"r

   recla
   \"rec
        
;

/*------
/* recla
/*------
: iterat

    seit

    iter

      re
        
      th

      ob
        
        

      re
        
      th

      pr
        
        
        
        
        
        
      el
        
        
        
        
      th

    next

    seit
;

/*------
/* recla
/*------
: rec.lo

    \"re

    \"\"

    recl

    begi

      re

      it

      re

      //

      3 

    agai
;

/*------
/* get r
/*------

: reclai

    recl

      re

    recl
;

/*======
/* end  
/*======
/*======
/* reque
/*======

\"reques

\"copier
\"export
\"fileGe
\"loader
\"loader

\"reques

\"stream

// creat
createDe

/*======
/* end  
/*======
/*======
/* reque
/*======

\"reques

/* * * *

Connecti

  con   
  dis   

Tenant R

  mkten 
  lsten 
  rmten 

Catalog 

  mkcat 
  lscat 
  rmcat 

Schema R

  mksch 
  lssch 
  rmsch 
  usesch

Table Re

  gettbl
  mktbl 
  lstbl 
  add   
  lscol 
  ld    
  tblInU
  sel   
  dump  
  cp    
  ex    
  sz    
  tr    
  rettbl
  rmtbl 

Dictiona

  cf    
        
        
  pwf   
  lf    

Misc Req

  seitf 
  mon   
  ping  
  x     

Grid Req
  lfG   
  pRB   

* * * * 

/*------
/*------
/*------

//: <ERR

: <NIP+E

: <SUCCE

: <OK> \

/*------
/* check
/*------
: checkE

  bp.fre
  if
    true
  else
    EC_L
  then
;

/*------
/*------
/*------

/*------
/* conne
/*------
: connec

  <SUCCE
;

/*------
/* disco
/*------
: discon

  <SUCCE
;

/*------
/*------
/*------

/*------
/* creat
/*------
: create

tp: crea

ddlLock.

  checkE
  checkM

//traceo
  makeTe
//traceo

tp: crea

  <OK>
;

/*------
/* get l
/*------
: list_t

ddlLock.

  checkI

  listCo

  <SUCCE
;

/*------
/* drop 
/*------
: drop_t

tp: drop

ddlLock.

  checkR

  remove

tp: remo

  <OK>
;

/*------
/*------
/*------

/*------
/* creat
/*------
: create

ddlLock.

  checkE
  checkM

  makeCa

  <OK>
;

/*------
/* get l
/*------
: list_c

ddlLock.

  checkI

  listCo

  <SUCCE
;

/*------
/* drop 
/*------
: drop_c

tp: drop

ddlLock.

  checkR

  remove

tp: remo

  <OK>
;

/*------
/*------
/*------

/*------
/* creat
/*------
: create

ddlLock.

  checkE
  checkM

  makeSc

  <OK>
;

/*------
/* get l
/*------
: list_s

ddlLock.

  checkI

  listCo

  <SUCCE
;

/*------
/* drop 
/*------
: drop_s

tp: drop

ddlLock.

  checkR

  remove

tp: drop

  <OK>
;

/*------
/* use a
/*------
: use_sc

  begin 

  // fin
  \"bora
  execut

  // fin
  \"cata
  execut

  // fin
  \"rapi
  execut

  // fin
  \"sche
  execut

  checkI
  checkI
  execut

  // fin
  \"tabl
  execut

  <OK>
;

/*------
/*------
/*------

/*------
/* creat
/*------
: create

ddlLock.

  checkE
  checkM

  makeTa

  <OK>
;

/*------
/* get l
/*------
: list_t

ddlLock.

  checkI

  listCo

  <SUCCE
;

/*------
/* load 
/*------
/*
: load_t

ddlLock.

  checkI
  checkI

  //bp.f

  reg.pu

snap0:

  tableL

  //bp.f

snap1:

  reg1 r

  0 do
    dup 
      I 
    else
      dr
    then
  loop
  reg.po
  <OK>
;
*/
: load_t

  ddlLoc

  checkI
  checkI

  //bp.f

  tableL

  //bp.f

  <OK>
;

/*------
/* load 
/*------
: load_t

  ddlLoc

  checkI
  checkI

  //bp.f

  tableL

  //bp.f

  <OK>
;

/*------
/* expor
/*------
: export

ddlLock.

  checkI
  checkI

  tableE

  <OK>
;

/*------
/* copy 
/*------
: copy_t

ddlLock.

  checkE
  checkI
  checkI

  //bp.f

  tableC

  //bp.f

  <OK>
;

/*------
/* dump 
/*------
: dump_t

ddlLock.

  checkI
  checkI

  dumpTa

  <OK>
;

/*------
/* appen
/*------
: table_

ddlLock.

  table.

  <OK>
;

/*------
/* get s
/*------
: size_t

ddlLock.

  checkI
  checkI

  sizeTa

  <SUCCE
;

/*------
/* get r
/*------
: select

ddlLock.

  getTab

  select

  <SUCCE
;

/*------
/* trunc
/*------
: trunca

ddlLock.

  checkI
  checkI

  trunca

  <OK>
;

/*------
/* drop 
/*------
/*
: drop_t

tp: drop

  reg.pu
snap0:

ddlLock.

  checkR

tp ( \"<

  remove

snap1:

  reg1 r
  0 do
    dup 
      I 
    else
      dr
    then
loop
  reg.po

tp: drop

  <OK>
;
*/
: drop_t

  tp: dr

  ddlLoc

  checkR

  tp ( \

  remove

  tp: dr

  <OK>
;

/*------
/* any o
/*------
: table_

  checkI
  checkI

  tableI
  if
    \"ta
  else
    \"ta
  then

   <OK>
;

/*------
/* get a
/*------
: get_ta

  getTab

  <OK>
;

/*------
/* retur
/*------
: return

  return

  <OK>
;

/*------
/*------
/*------

/*------
/* add a
/*------
: add_co

  checkA

  ~addCo

  <OK>
;

/*------
/* get l
/*------
: list_c

  checkI

  listCo

  <SUCCE
;


/*------
/*------
/*------

/*------
/* chang
/*------
: cf

  dup \"
    drop
    0 di
  else
    dup 
      dr
      be
    else
      du
        
        
        
      el
        
        
        
      th
    then
  then

  <OK>
;


/*------
/* print
/*------
: pwf

  \"/\"
  dic.co
    dic.
    dic.
    \"/\
  loop

  <SUCCE
;

/*------
/* list 
/*------
: lf

  \"\"
  0 dic.

    dup 

    obje

    \"(\

    obje
      \"
      ob
    then

    obje
      ob
    then

    \")\

    obje

  next

   <SUCC
;

/*------
/* list 
/*------
: ~lf ( 

tp: lfOn

  // set
  $$ \"l

  tp: th

  $$>val

  >>allP

tp: lfOn

  checkR
;

/*------
/* list 
/*------
: lfG

  ~lf

  <OK>
;

/*------
/* list 
/*------
: lf$ ( 

tp: lf

  \"LIST

  \"\"
  0 dic.

  dup st

  object

  \"(\" 

  object
  \"tabl
  object
  then

  object
  object
  then

  \")\" 

  object

  next

  .

  //drop

  true (
;

/*------
/* list 
/*------
: lf_ ( 

  depth 
    \"di
    if
  tp: se
    !ind
    then
  then

  lf$
;

/*------
/*------
/*------

/*------
/* get s
/*------
: space$

tp: gett

  args.t
    \"GE
  then

  \"Pool
  \" InU
  \" Fre

  str.ca

  <SUCCE
;

/*------
/* have 
/*------
: ~space

tp: spac

  // the
  \"spac

  tp: th

  $$>val

  // spa
  >>allP

  tp: sp

  printR
tp: dpne
;

/*------
/* get s
/*------
: space 

  ~space

  <OK>
;

/*------
/* get f
/*------
: getFre

  bp.fre

  >str

  <SUCCE
;

/*------
/* get i
/*------
: getInU

  bp.inU

  >str

  <SUCCE
;

/*------
/* get t
/*------
: getTot

  bp.tot

  >str

  <SUCCE
;

/*------
/* get s
/*------
: seitf_

  seitf.

  >str

  <SUCCE
;

/*------
/*------
/*------

/*------
/* get s
/*------
: status

tp: gett

  args.t
  \"GETT
  then

  node.s

  <SUCCE
;

/*------
/* have 
/*------
: ~statu

tp: stat

  // the
  \"stat

  tp: th

  $$>val

  // sta
  >>allP

  tp: st

  printR
;

/*------
/* get s
/*------
: status

  ~statu

  <OK>
;

/*------
/*------
/*------

/*------
/* get m
/*------
: get_mo

  monito

  <SUCCE
;

/*------
/* ping 
/*------
: ping (

  \"pong

  <SUCCE
;

/*------
/* docol
/*------
: docol_

  checkI
  execut

  begin
   depth
  repeat

 <OK>
;

/*------
/* get_v
/*------
: get_ve

  versio

  <SUCCE
;

/*------
/* print
/*------
: ~print

tp: prin

  // set
  $$ swa

tp: theI

  $$>val

  >>allP

tp: prin

  checkR
;

int: cou

/*------
/* print
/*------
: pRB ( 

tp: pRB

  reg.pu

  rowBuf
  beginT

/*
  0 coun
  begin
  while
    coun
    \"He
    coun
    1 +
    coun
  repeat
*/

  10 cou
  countR
    \"He
  loop

  // com
  commit

  drop

  //reg0

  reg0

tp: mark

$$>valse

tp: vals

  ~print

  reg.po

  <OK>
;

/*------
/* print
/*------
: printR

tp: prin

  \"PRIN

  rowbuf

  true (
;

/*------
/* print
/*------
: printR

  valset

  drop

  //dup 
  //if
//tp: se
  //  !i
  //  th

  printR
;

/*------
/* x tes
/*------
: x     

  words

  \"x\"

  <SUCCE
;

////////

/*------
/* creat
/*------
: create

  assert

  // pus
  \"tena
  execut

  // cre
  \"bora

  // pus
  \"bora
  execut

  // pus
  \"cata
  execut

  // cre
  \"rapi

  // go 
  dic.dr
  dic.dr
  dic.dr

  assert
;

/*======
/* end  
/*======
/*======
/* reque
/*======

\"reques

/*------
/* creat
/*------

queue:  

dic:    

taskset:

thread: 

/*------
/* start
/*------
: reques

  // set
  0 \"re

  // set
  0 \"re

  // set
  reques
  \"requ
        
;

/*------
/* pop v
/*------
: req> (

tp: wait

  begin

    // a
    task

    // t
    not 
      ta
      ar

  repeat

tp: popp
;

/*------
/* proce
/*------
: !req

  valset

  // che
  depth 
  0 stac
  1 stac
  2 stac

  drop /
  >:   /
  >:   /
;

/*------
/* send 
/*------
: >rsp (

  // che
  depth 

  0 stac
  1 stac
  2 stac

  >valse

  \"rspq

  0 task
;

/*------
/* reque
/*------
: req.lo

  reques

  \"rena

  \"req.

  \"req.

  reques

  reques

  begin
    dept

    req>

    !req

    >rsp

  again
;

/*======
/* end  
/*======
/*======
/* row.s
/*======

\"row.se

/*======
/* end  
/*======
/*======
/* rowBu
/*======

\"rowBuf

/*======
/* end  
/*======
/*======
/* rowCu
/*======

\"rowCur

/*------
/* creat
/*------
: rowCur
    crea
    WT_O
    size
    word
    CN_R
    word
    true
    does
;

/*------
/* creat
/*------
: :rowCu
    >cre
    WT_O
    size
    word
    CN_R
    word
    true
    word
    does
;

/*======
/* end  
/*======
/*======
/* schem
/*======

\"schema

: schema

//: make


: :schem
    :dic
        
        
;

: within
    0 di
;

: checkI
    with
    EC_N
;

: checkI

    dup 
    dup 
    EC_N
;

: checkI
  dup fi
  EC_SCH
;

/*------
/* check
/*------
: checkM

  checkI
  checkI

  true
;

/*------
/* make 
/*------
: makeSc

  args.t
    dup 
  then

  find e
  \"sche
  swap  
  :folde
;

/*------
/* make 
/*------
: makeSc

tp: maki

  args.t
    dup 
  then

    dup 

    :sch

    drop

    make

    true
;

/*------
/* have 
/*------
/*
: ~makeS

tp: mkSc

  // set
  $$ swa

tp: theI

  $$>val

  >>allP

tp: mkSc

  checkR
;
*/

/*------
/* check
/*------
: checkR

  dup ( 

  checkI
  checkI
  checkI
  checkN

  drop (

  true (
;

/*------
/* remov
/*------
: remove

tp: remo

  args.t
    dup 
  then

  :forge

  true <
;

/*------
/* remov
/*------
/*
: ~remov

tp: rmSc

  // set
  $$ swa

tp: theI

  $$>val

  >>allP

tp: rmSc

  checkR
;
*/
/*======
/* end  
/*======
/*======
/* seitf
/*======

\"seitfC

/*------
/* creat
/*------
: seitfC
    crea
    WT_O
    size
    word
    CN_S
    word
    true
    does
;

/*------
/* creat
/*------
: :seitf
    >cre
    WT_O
    size
    word
    CN_S
    word
    true
    word
    does
;

////////

: ::seit

    \"__
    not 
      dr
      \"
    then
    exec
;

: ~seitf

    \"__
;

: listSe

    ::se

    r0 s
    r0 s
    begi
      wh
      r0
      se
      r0
    repe

    rdro

    ~sei
;

/*======
/* end  
/*======
/*======
/* seitf
/*======

\"seitfL

: seitfL
    crea
    WT_O
    size
    word
    CN_S
    word
    true
    does
;

: seitfC
    crea
    WT_O
    size
    word
    CN_S
    word
    true
    does
;

/*======
/* end  
/*======
/*======
/* Sende
/*======

\"Sender

/*------
/* send 
/*------
: >send 

  task.t

  tp: se

  socket

  not if
    \"se
  then

  //\"No
;

/*------
/* sende
/*------
: snd.lo

tp: snd.

  task.t

  \"snd.

  sender

  begin
    dept

    // p
    task

tp: popp

    // s
    >sen

  again
;

/*======
/* end  
/*======
/*======
/* socke
/*======

\"socket

\"socket
\"socket

/*======
/* end  
/*======
\"client
: client
    \"cl
    4334
    Sock
    Sock
    \"he
    \"cl
;
\"server
: server
    \"se
    4334
    \"se
    Sock
    \"se
    Sock
    \"se
    .
; 
/*======
/* stree
/*======

\"stream

////////

/*------
/* initi
/*------
: sld

  reg.pu

  0 >reg

  file.n

  \"/tpc
  //\"/t

  OPEN_R

  loadTi

  begin
    reg0
    whil
      dr
      re
      10
  repeat

  loadTi

  \"Load
  loadTi

  reg7 l

  reg0 f

  reg0 f

  reg.po
;

/*------
/* loop 
/*------
: sld2

  100000
  do
    I 10
  loop
;

/*======
/* end  
/*======
/*======
/* struc
/*======

\"struct

\"queue.

/*======
/* end  
/*======
/*======
/* syste
/*======

\"system


( taskli

( thread
threadLi

( txMap 
txMap: t

( txColl
txCollec

( seitfL
seitfLis

( task s
taskSet:

//traceo
( root t
thread: 
//traceo

( give r
rootTask


/*======
/* end  
/*======
/*======
/* table
/*======

\"table.

int: que

/*------
/* table
/*------
: table 

//: make

/*------
/* make 
/*------
: makeTa

  args.t
    dup 
  then

  find e
  \"tabl
  swap  
  :folde
;

: isTabl

/*------
/* creat
/*------
: table:
    crea
    WT_O
    size
    word
    CN_T
    word
    true
    ' ta
    does
;

/*------
/* creat
/*------
: :table
    >cre
    WT_O
    size
    word
    CN_T
    word
    true
    ' ta
    word
    does
;

/*------
/* se> c
/*------

: within
    0 di
;

: checkI
    with

    EC_N
;

/*------
/* check
/*------
/*
: checkI

  find (

  dup no
  if
    drop
    exit
  then

  dup ( 
  word.k
  ' tabl
  eql ( 
  if
    true
    exit
  then

  drop (
  EC_NOT
;
*/
//------
: checkI

  dup ( 

  find (

  dup ( 
  not ( 
  if
    drop
    drop
    EC_N
  exit
  then

  dup ( 

  word.k
  ' tabl
  eql ( 
  if
    drop
    true
    exit
  then

  drop (
  drop (
  EC_NOT
;

: checkI
  dup fi
  EC_TAB
;

/*------
: pushTa

    chec

    find

    true
;

: popTab
    dic.
;

/*------
: getTab

    chec

    chec

    find

    task

    true
;

/*------
: return

    task

    true
;

/*------
/* any o
/*------
: tableI

  find e

  table.
  if tru
;

/*------
/* check
/*------
: checkM

  checkI
  checkI

  true
;

/*------
/* check
/*------
: checkR

  checkI
  checkI
  dup ta

  true (
;

/*------
/* make 
/*------
: makeTa

tp: maki

  args.t
    dup 
  then

  dup ( 

  :table

  drop (

  // for
  \"make

  true <
;

/*------
/* have 
/*------
/*
: ~makeT

tp: mkTb

  // set
  $$ swa

tp: theI

  $$>val

  >>allP

tp: mkTb

  checkR
;
*/
/*------
/* remov
/*------
: remove

tp: remo

  dup

  find e
  table.

  :forge

  true <
;

/*------
/* remov
/*------
/*
: ~remov

tp: rmTb

  // set
  //\"re
  $$ swa

tp: theI

  $$>val

  >>allP

tp: rmTb

  checkR

;
*/
/*------
/* get s
/*------
: sizeTa

  find e

  table.
;

////////

: dumpTa

  find e

  \"__ro
  if
  else
    drop
    \"__
  then
  execut

  >r
  // r0 

  r0 row
  args.p
  do
    // r
    I r2
    r2 r
    begi
      wh
      r2
      .
      r2
      re

      r2
  loop

  rdrop

  \"__ro
;

/*------
/* get r
/*------
: select

  queryC

  task.t

  rowcur

  >str
;

/*------
/* trunc
/*------
: trunca

  find e

  table.
;

/*======
/* end  
/*======
/*======
/* task.
/*======

\"task.s

/*------
/* creat
/*------

: task:

  create
  WT_OBJ
  sizeof
  word.t
  CN_TAS
  word.t
  true w
  does>
;

: :task

  >creat
  WT_OBJ
  sizeof
  word.t
  CN_TAS
  word.t
  true w
  word.t
  does>
;

/*======
/* end  
/*======
/*======
/* taskC
/*======

\"taskCu

/*------
/* creat
/*------
: taskCu
    crea
    WT_O
    size
    word
    CN_T
    word
    true
    does
;

/*------
/* creat
/*------
: :taskC
    >cre
    WT_O
    size
    word
    CN_T
    word
    true
    word
    does
;

////////

: ::task

    \"__
    not 
      dr
      \"
    then
    exec
;

: ~taskC

    \"__
;

: listTa

    ::ta

    r0 t
    r0 t
    begi
      wh
      r0
      ta
      r0
    repe

    rdro

    ~tas
;

/*======
/* end  
/*======
/*======
/* taskL
/*======

\"taskLi

: taskLi
    crea
    WT_O
    ( th
    size
    word
    CN_T
    word
    true
    does
;

: taskCu
    crea
    WT_O
    size
    word
    CN_T
    word
    true
    does
;

/*======
/* end  
/*======
/*======
/* tasks
/*======

\"tasks.

\"task.s
\"taskLi
\"taskCu
\"taskSe
\"ipc.se

/*======
/* end  
/*======
/*======
/* taskS
/*======

\"taskSe

/*------
/* creat
/*------

: taskSe
    crea
    WT_O
    size
    word
    CN_T
    word
    true
    does
;

: :taskS
    >cre
    WT_O
    size
    word
    CN_T
    word
    true
    word
    does
;

/*======
/* end  
/*======
/*======
/* tenan
/*======

\"tenant

: tenant

: :tenan
    :dic
        
        
;

: within
    0 di
;

: checkI
    with
    EC_N
;
/*
: checkI

    dup 
    dup 
    drop
    EC_N
;
*/

/*------
/* check
/*------
: checkI

  dup wo
  dup ex
  EC_NOT
;

/*------
/* check
/*------
: checkI
  dup fi
  EC_TEN
;

/*------
/* check
/*------
: checkM

  checkI
  checkI

  true
;

/*------
/* check
/*------
: checkR

  dup ( 

  checkI
  checkI
  checkI
  checkN

  drop (

  true (
;

/*------
/* make 
/*------
: makeTe

tp: maki

  args.t
    dup 
  then

    dup 

    :ten

    drop

    make

    true
;

/*------
/* have 
/*------
/*
: ~makeT

tp: mkTe

  // put
  1 put$

  // the
  \"make

tp: theI

  $$>val

  // mak
  >>allP

tp: mkTe

  checkR
;
*/

/*------
/* remov
/*------
: remove

tp: remo

  args.t
    dup 
  then

  :forge

  true <
;

/*------
/* remov
/*------
/*
: ~remov

tp: rmTe

  // set
  $$ swa

  tp: th

  $$>val

  >>allP

tp: rmTe

  checkR
;
*/
/*======
/* end  
/*======
/*======
/* testi
/*======

\"testin

\"jData.

\"miscTe

\"unitTe

\"1m.sen


/*======
/* end  
/*======
/*======
/* threa
/*======

\"thread

/*------
/* creat
/*------

: thread
    crea
    WT_O
    size
    word
    CN_T
    word
    true
    does
;

: :threa
    >cre
    WT_O
    size
    word
    CN_T
    word
    true
    word
    does
;

/*======
/* end  
/*======
/*======
/* threa
/*======

\"thread

/*------
/* creat
/*------
: thread
    crea
    WT_O
    size
    word
    CN_T
    word
    true
    does
;

/*------
/* creat
/*------
: :threa
    >cre
    WT_O
    size
    word
    CN_T
    word
    true
    word
    does
;

////////

: ::thre

    \"__
    not 
      dr
      \"
    then
    exec
;

: ~threa

    \"__
;

: listTh

    ::th

    r0 t
    r0 t
    begi
      wh
      r0
      th
      r0
    repe

    rdro

    ~thr
;

/*======
/* end  
/*======
/*======
/* threa
/*======

\"thread

: thread
    crea
    WT_O
    size
    word
    CN_T
    word
    true
    does
;

/*======
/* end  
/*======
/*======
/* threa
/*======

\"thread

\"thread
\"thread
\"thread

/*======
/* end  
/*======
/*======
/* timer
/*======

\"timer.

: timer 

: timer:
    crea
    WT_O
    size
    word
    CN_T
    ' ti
    word
    true
    does
;

: :timer
    >cre
    WT_O
    size
    word
    CN_T
    ' ti
    word
    true
    does
;

timer: t

: timerT

    test
    1 th
    test
    test
    test
    test
    test
;

/*======
/* end  
/*======
/*======
/* trace
/*======

\"trace.

/*------
/* defin
/*------

file: tr

/*------
/* attem
/*------
: open_t
  \"/tmp
  open_a
  if \"t
;

open_tra

/*======
/* end  
/*======
/*======
/* trans
/*======

\"transa

\"txColl
\"txCurs
\"txMap.
\"txRef.

/*======
/* end  
/*======
/*======
/* txCol
/*======

\"txColl

: txColl
    crea
    WT_O
    size
    word
    CN_T
    word
    true
    does
;

/*======
/* end  
/*======
/*======
/* txCur
/*======

\"txCurs

/*------
/* creat
/*------
: txCurs
    crea
    WT_O
    size
    word
    CN_T
    word
    true
    does
;

/*------
/* creat
/*------
: :txCur
    >cre
    WT_O
    size
    word
    CN_T
    word
    true
    word
    does
;

////////

: ::txCu

    \"__
    not 
      dr
      \"
    then
    exec
;

: ~txCur

    \"__
;

: listTh

    ::tx

    r0 t
    r0 t
    begi
      wh
      r0
      tx
      r0
    repe

    rdro

    ~txC
;

/*======
/* end  
/*======
/*======
/* txMap
/*======

\"txmap.

: txMap:
    crea
    WT_O
    size
    word
    CN_T
    word
    true
    does
;

/*======
/* end  
/*======
/*======
/* txRef
/*======

\"txref.

/*------
/* creat
/*------
: :txRef
    >cre
    WT_O
    size
    word
    CN_T
    word
    true
    word
    does
;

////////

: ::txRe

  \"txRe
  not if
    drop
    \"tx
  then
  execut
;

: beginT
  ::txRe
  txRef.
  ::txre
;

: commit
  ::txRe
  dup
  txRef.
  txRef.
  \"txRe
;

: rollba
  ::txRe
  dup
  txRef.
  txRef.
  \"txRe
;

/*======
/* end  
/*======
/*======
/* unitT
/*======

\"unitTe

\"arithm

\"flowTe

\"logicT

: ut

  \"unit

  logicT

  arithm

  flowTe

  \"unit
;

/*======
/* end  
/*======
/*======
/* usefu
/*======

\"useful

/*------
/* put t
/*------
: snooze
    begi
      \"
    agai
;

/*======
/* end  
/*======
/*======
/* user.
/*======

\"user.s

/*======
/* end  
/*======
/*======
/* var.s
/*======

\"var.se

: emptyD

  does>
;

/*------
/* creat
/*------
: int:

  create
  WT_OBJ
  sizeof
  word.t
  CN_INT
  true w
  does>
;

/*------
/* creat
/*------
: intArr

  create
  WT_OBJ
  dup >r
  sizeof
  word.t
  true w

  r> 0 d
    I si
    dup
    obje
    CN_I
  loop

  does> 
;

/*------
/* creat
/*------
: ptr:

  create
  WT_OBJ
  sizeof
  word.t
  CN_PTR
  true w
  does>
;

/*------
/* creat
/*------
: ptrArr

  create
  WT_OBJ
  dup >r
  sizeof
  word.t
  true w

  r> 0 d
    I si
    dup
    obje
    CN_P
  loop

  does> 
;

/*------
/* creat
/*------
: str:

  create
  WT_OBJ
  sizeof
  word.t
  CN_STR
  word.t
  true w
  does>
;

/*======
/* end  
/*======
