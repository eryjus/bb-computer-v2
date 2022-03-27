


.register           p       16      ; The Program Counter
.register           f       8       ; The Flags Register
.register           s       16      ; The Stack Pointer Register
.register           a       16      ; The A Register
.register           b       16      ; The B Register
.register           x       16      ; The X Registers


.memory             0x0000  0xbfff  ; the RAM memory
.memory             0xc000  0xffff  ; the ROM memory


.opcode             reset               .db 00
.opcode             nop                 .db 01
.opcode             hwi                 .db 02
.opcode             swi                 .db 03

.opcode             stc                 .db 04
.opcode             clc                 .db 05
.opcode             sti                 .db 06
.opcode             cli                 .db 07
.opcode             clv                 .db 08

.opcode             hlt                 .db 09

.opcode             rti                 .db 0a

.opcode             psh a               .db 0b
.opcode             psh b               .db 0c
.opcode             psh x               .db 0d
.opcode             psh s               .db 0e
.opcode             psh f               .db 0f

.opcode             pop a               .db 10
.opcode             pop b               .db 11
.opcode             pop x               .db 12
.opcode             pop s               .db 13
.opcode             pop f               .db 14

.opcode             out a               .db 15
.opcode             out b               .db 16
.opcode             out x               .db 17

.opcode             not a               .db 18
.opcode             not b               .db 19
.opcode             not x               .db 1a

.opcode             and a,$(16)         .db 1b $(16)
.opcode             and a,b             .db 1c
.opcode             and a,x             .db 1d
.opcode             and b,a             .db 1e
.opcode             and b,$(16)         .db 1f $(16)
.opcode             and b,x             .db 20
.opcode             and x,a             .db 21
.opcode             and x,b             .db 22
.opcode             and x,$(16)         .db 23 $(16)

.opcode             nand a,$(16)        .db 24 $(16)
.opcode             nand a,b            .db 25
.opcode             nand a,x            .db 26
.opcode             nand b,a            .db 27
.opcode             nand b,$(16)        .db 28 $(16)
.opcode             nand b,x            .db 29
.opcode             nand x,a            .db 2a
.opcode             nand x,b            .db 2b
.opcode             nand x,$(16)        .db 2c $(16)

.opcode             or a,$(16)          .db 2d $(16)
.opcode             or a,b              .db 2e
.opcode             or a,x              .db 2f
.opcode             or b,a              .db 30
.opcode             or b,$(16)          .db 31 $(16)
.opcode             or b,x              .db 32
.opcode             or x,a              .db 33
.opcode             or x,b              .db 34
.opcode             or x,$(16)          .db 35 $(16)

.opcode             nor a,$(16)         .db 36 $(16)
.opcode             nor a,b             .db 37
.opcode             nor a,x             .db 38
.opcode             nor b,a             .db 39
.opcode             nor b,$(16)         .db 3a $(16)
.opcode             nor b,x             .db 3b
.opcode             nor x,a             .db 3c
.opcode             nor x,b             .db 3d
.opcode             nor x,$(16)         .db 3e $(16)

.opcode             xor a,$(16)         .db 3f $(16)
.opcode             xor a,b             .db 40
.opcode             xor a,x             .db 41
.opcode             xor b,a             .db 42
.opcode             xor b,$(16)         .db 43 $(16)
.opcode             xor b,x             .db 44
.opcode             xor x,a             .db 45
.opcode             xor x,b             .db 46
.opcode             xor x,$(16)         .db 47 $(16)

.opcode             xnor a,$(16)        .db 48 $(16)
.opcode             xnor a,b            .db 49
.opcode             xnor a,x            .db 4a
.opcode             xnor b,a            .db 4b
.opcode             xnor b,$(16)        .db 4c $(16)
.opcode             xnor b,x            .db 4d
.opcode             xnor x,a            .db 4e
.opcode             xnor x,b            .db 4f
.opcode             xnor x,$(16)        .db 50 $(16)

.opcode             false a             .db 51
.opcode             false b             .db 52
.opcode             false x             .db 53

.opcode             true a              .db 54
.opcode             true b              .db 55
.opcode             true x              .db 56

.opcode             add a,$(16)         .db 57 $(16)
.opcode             add a,b             .db 58
.opcode             add a,x             .db 59
.opcode             add b,a             .db 5a
.opcode             add b,$(16)         .db 5b $(16)
.opcode             add b,x             .db 5c
.opcode             add x,a             .db 5d
.opcode             add x,b             .db 5e
.opcode             add x,$(16)         .db 5f $(16)
.opcode             add s,$(8)          .db 60 $(8)
.opcode             add s,$(16)         .db 61 $(16)

.opcode             sub a,$(16)         .db 62 $(16)
.opcode             sub a,b             .db 63
.opcode             sub a,x             .db 64
.opcode             sub b,a             .db 65
.opcode             sub b,$(16)         .db 66 $(16)
.opcode             sub b,x             .db 67
.opcode             sub x,a             .db 68
.opcode             sub x,b             .db 69
.opcode             sub x,$(16)         .db 6a $(16)
.opcode             sub s,$(8)          .db 6b $(8)
.opcode             sub s,$(16)         .db 6c $(16)

.opcode             shl a               .db 6d
.opcode             shl b               .db 6e
.opcode             shl x               .db 6f

.opcode             inc a               .db 70
.opcode             inc b               .db 71
.opcode             inc x               .db 72

.opcode             dec a               .db 73
.opcode             dec b               .db 74
.opcode             dec x               .db 75

.opcode             adc a,$(16)         .db 76 $(16)
.opcode             adc a,b             .db 77
.opcode             adc a,x             .db 78
.opcode             adc b,a             .db 79
.opcode             adc b,$(16)         .db 7a $(16)
.opcode             adc b,x             .db 7b
.opcode             adc x,a             .db 7c
.opcode             adc x,b             .db 7d
.opcode             adc x,$(16)         .db 7e $(16)

.opcode             sbb a,$(16)         .db 7f $(16)
.opcode             sbb a,b             .db 80
.opcode             sbb a,x             .db 81
.opcode             sbb b,a             .db 82
.opcode             sbb b,$(16)         .db 83 $(16)
.opcode             sbb b,x             .db 84
.opcode             sbb x,a             .db 85
.opcode             sbb x,b             .db 86
.opcode             sbb x,$(16)         .db 87 $(16)

.opcode             cmp a,$(16)         .db 88 $(16)
.opcode             cmp a,b             .db 89
.opcode             cmp a,x             .db 8a
.opcode             cmp b,a             .db 8b
.opcode             cmp b,$(16)         .db 8c $(16)
.opcode             cmp b,x             .db 8d
.opcode             cmp x,a             .db 8e
.opcode             cmp x,b             .db 8f
.opcode             cmp x,$(16)         .db 90 $(16)

.opcode             tst a,$(16)         .db 91 $(16)
.opcode             tst a,b             .db 92
.opcode             tst a,x             .db 93
.opcode             tst b,a             .db 94
.opcode             tst b,$(16)         .db 95 $(16)
.opcode             tst b,x             .db 96
.opcode             tst x,a             .db 97
.opcode             tst x,b             .db 98
.opcode             tst x,$(16)         .db 99 $(16)

.opcode             neg a               .db 9a
.opcode             neg b               .db 9b
.opcode             neg x               .db 9c

.opcode             lod a,$(16)         .db 9d $(16)
.opcode             lod a,($(16))       .db 9e $(16)
.opcode             lod a,(x)           .db 9f
.opcode             lod a,(x+$(8))      .db a0 $(8)
.opcode             lod a,(x+$(16))     .db a1 $(16)

.opcode             lod b,$(16)         .db a2 $(16)
.opcode             lod b,($(16))       .db a3 $(16)
.opcode             lod b,(x)           .db a4
.opcode             lod b,(x+$(8))      .db a5 $(8)
.opcode             lod b,(x+$(16))     .db a6 $(16)

.opcode             lod x,$(16)         .db a7 $(16)
.opcode             lod x,($(16))       .db a8 $(16)
.opcode             lod x,(x)           .db a9
.opcode             lod x,(x+$(8))      .db aa $(8)
.opcode             lod x,(x+$(16))     .db ab $(16)

.opcode             lod s,$(16)         .db ac $(16)
.opcode             lod s,($(16))       .db ad $(16)
.opcode             lod s,(x)           .db ae
.opcode             lod s,(x+$(8))      .db af $(8)
.opcode             lod s,(x+$(16))     .db b0 $(16)

.opcode             sto a,$(16)         .db b1 $(16)
.opcode             sto a,($(16))       .db b2 $(16)
.opcode             sto a,(x)           .db b3
.opcode             sto a,(x+$(8))      .db b4 $(8)
.opcode             sto a,(x+$(16))     .db b5 $(16)

.opcode             sto b,$(16)         .db b6 $(16)
.opcode             sto b,($(16))       .db b7 $(16)
.opcode             sto b,(x)           .db b8
.opcode             sto b,(x+$(8))      .db b9 $(8)
.opcode             sto b,(x+$(16))     .db ba $(16)

.opcode             sto x,$(16)         .db bb $(16)
.opcode             sto x,($(16))       .db bc $(16)
.opcode             sto x,(x)           .db bd
.opcode             sto x,(x+$(8))      .db be $(8)
.opcode             sto x,(x+$(16))     .db bf $(16)

.opcode             sto s,$(16)         .db c0 $(16)
.opcode             sto s,($(16))       .db c1 $(16)
.opcode             sto s,(x)           .db c2
.opcode             sto s,(x+$(8))      .db c3 $(8)
.opcode             sto s,(x+$(16))     .db c4 $(16)

.opcode             mov a,b             .db c5
.opcode             mov a,x             .db c6
.opcode             mov a,p             .db c7
.opcode             mov a,s             .db c8

.opcode             mov b,a             .db c9
.opcode             mov b,x             .db ca
.opcode             mov b,p             .db cb
.opcode             mov b,s             .db cc

.opcode             mov x,a             .db cd
.opcode             mov x,b             .db ce
.opcode             mov x,p             .db cf
.opcode             mov x,s             .db d0

.opcode             mov s,a             .db d1
.opcode             mov s,b             .db d2
.opcode             mov s,p             .db d3
.opcode             mov s,s             .db d4

.opcode             jmp $(16)           .db d5 $(16)
.opcode             jmp ($(16))         .db d6 $(16)
.opcode             jmp (x)             .db d7
.opcode             jmp (x+$(8))        .db d8 $(8)
.opcode             jmp (x+$(16))       .db d9 $(16)

.opcode             jc $(16)            .db da $(16)
.opcode             jc ($(16))          .db db $(16)
.opcode             jc (x)              .db dc
.opcode             jc (x+$(8))         .db dd $(8)
.opcode             jc (x+$(16))        .db de $(16)

.opcode             jz $(16)            .db df $(16)
.opcode             jz ($(16))          .db e0 $(16)
.opcode             jz (x)              .db e1
.opcode             jz (x+$(8))         .db e2 $(8)
.opcode             jz (x+$(16))        .db e3 $(16)

.opcode             jv $(16)            .db e4 $(16)
.opcode             jv ($(16))          .db e5 $(16)
.opcode             jv (x)              .db e6
.opcode             jv (x+$(8))         .db e7 $(8)
.opcode             jv (x+$(16))        .db e8 $(16)

.opcode             js $(16)            .db e9 $(16)
.opcode             js ($(16))          .db ea $(16)
.opcode             js (x)              .db eb
.opcode             js (x+$(8))         .db ec $(8)
.opcode             js (x+$(16))        .db ed $(16)

.opcode             jp $(16)            .db ee $(16)
.opcode             jp ($(16))          .db ef $(16)
.opcode             jp (x)              .db f0
.opcode             jp (x+$(8))         .db f1 $(8)
.opcode             jp (x+$(16))        .db f2 $(16)

.opcode             bsr $(16)           .db f3 $(16)
.opcode             bsr ($(16))         .db f4 $(16)
.opcode             bsr (x)             .db f5
.opcode             bsr (x+$(8))        .db f6 $(8)
.opcode             bsr (x+$(16))       .db f7 $(16)

.opcode             rts                 .db f8


