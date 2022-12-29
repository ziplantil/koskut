/*******************************************************************************
            Koskut -- pieni suomen kielen taivutuskirjasto
            tehnyt ziplantil 2022 -- lisenssi: MIT
            versio: 2022-12-30
            <https://github.com/ziplantil/koskut>
*******************************************************************************/
/* k_nvart.c - nominivartalot                                                 */

#include "koskut.h"
#include "koskutio.h"

#include <limits.h>

#define K_LUOKKA(taiv)      (((taiv) >> 8) & 63)
#define K_ASTE(taiv)        (((taiv) >> 3) & 31)
#define K_VOKAALI(taiv)     ((taiv) & 7)
#define K_HEITTO(taiv)      ((taiv) & 0x4000U)
#define K_ETINEN(taiv)      ((taiv) & 0x8000U)

#define K_0                 K_FI_NA
#define K_V                 K_FI_X1
#define K_SUO_V             K_FI_X2
#define K_A                 K_FI_X3
#define K_O                 K_FI_X4
#define K_SISAR_V           K_FI_X5
#define K_KV                K_FI_X6
#define K_KH                K_FI_X7
#define K_KYNSI_K           K_FI_X8
#define K_SISAR_K           K_FI_X9
#define K_HM                K_FI_XA

#define K_LA                K_FI_A
#define K_LB                K_FI_B
#define K_LC                K_FI_C
#define K_LD                K_FI_D
#define K_LE                K_FI_E
#define K_LF                K_FI_F
#define K_LG                K_FI_G
#define K_LH                K_FI_H
#define K_LI                K_FI_I
#define K_LJ                K_FI_J
#define K_LK                K_FI_K
#define K_LL                K_FI_L
#define K_LM                K_FI_M
#define K_LN                K_FI_N
#define K_LO                K_FI_O
#define K_LP                K_FI_P
#define K_LQ                K_FI_Q
#define K_LR                K_FI_R
#define K_LS                K_FI_S
#define K_LT                K_FI_T
#define K_LU                K_FI_U
#define K_LV                K_FI_V
#define K_LW                K_FI_W
#define K_LX                K_FI_X
#define K_LY                K_FI_Y
#define K_LZ                K_FI_Z
#define K_LAE               K_FI_AE
#define K_LOE               K_FI_OE

#define K_NVART_MVJ        11

typedef const kt_merkki *kt_merkkip;

static const kt_merkki paatteet_data[] = {
    K_0,

#define V_YV           1
#define V_0YV          2
    K_KV, K_V, K_0,

#define V_KOIRA_YV     4
#define V_MAA_YP_A     5
    K_KV, K_A, K_0,

#define V_SUO_YV       7
    K_SUO_V, K_V, K_0,

#define V_KYTKIN_YV   10
#define V_TOIMI_YV    12
    K_KV, K_V, K_LM, K_LE, K_0,

#define V_LAPSI_YV    15
    K_LP, K_LS, K_LE, K_0,

#define V_VEITSI_YV   19
    K_LT, K_LS, K_LE, K_0,

#define V_KAKSI_YV    23
    K_LH, K_LT, K_LE, K_0,

#define V_SISAR_YV    27
#define V_MG_E        30
    K_KV, K_SISAR_V, K_SISAR_K, K_LE, K_0,

#define V_LAMMIN_YV   32
    K_KV, K_V, K_LM, K_A, K_0,

#define V_VASTAUS_YV  37
#define V_NAINEN_YV   38
    K_LK, K_LS, K_LE, K_0,

#define V_VIERAS_YV   41
#define V_KEVAT_YV    42
    K_KV, K_V, K_V, K_0,

#define V_MIES_YV     45
    K_LH, K_LE, K_0,

#define V_OHUT_YV     48
    K_KV, K_V, K_LE, K_0,

#define V_HAME_YV     52
#define V_OLLUT_YV    53
    K_KV, K_LE, K_LE, K_0,

#define V_ASKEL_YV    56
    K_KV, K_SISAR_V, K_SISAR_K, K_LE, K_LE, K_0,

#define V_YH          62
    K_KH, K_V, K_0,

#define V_VALO_YH     65
    K_KH, K_HM, K_V, K_0,

#define V_KOIRA_YH    69
    K_KH, K_HM, K_A, K_0,

#define V_KAKSI_YH    73
    K_LH, K_LD, K_LE, K_0,

#define V_OVI_I       77
    K_KV, K_LI, K_0,

#define V_KYNSI_I     80
#define V_VESI_I      81
    K_KYNSI_K, K_LS, K_LI, K_0,

#define V_LAPSI_I     84
    K_LP, K_LS, K_LI, K_0,

#define V_VEITSI_I    88
    K_LT, K_LS, K_LI, K_0,

#define V_KAKSI_I     92
    K_LK, K_LS, K_LI, K_0,

#define V_TOIMI_I     96
    K_LM, K_LI, K_0,

#define V_SISAR_I     99
    K_KH, K_SISAR_V, K_SISAR_K, K_0,

#define V_KYTKIN_I   103
#define V_SISIN_I    105
    K_KH, K_V, K_LN, K_0,

#define V_NAINEN_I   107
    K_LN, K_LE, K_LN, K_0,

#define V_VIERAS_I   111
#define V_MIES_I     113
    K_KH, K_V, K_LS, K_0,

#define V_OHUT_I     115
#define V_KEVAT_I    116
#define V_TUHAT_I    117
    K_KH, K_V, K_LT, K_0,

#define V_HAME_I     119
    K_KH, K_LE, K_0,

#define V_HAME_YP    122
#define V_TUHAT_YP   124
#define V_MAA_YP     125
    K_KH, K_LE, K_LT, K_LT, K_0,

#define V_TOIMI_YP   127
    K_LN, K_LT, K_0,

#define V_KYNSI_YP   130
    K_KV, K_LT, K_0,

#define V_LAPSI_YP   133
    K_LS, K_LT, K_0,

#define V_KAKSI_YP   136
    K_LH, K_LT, K_0,

#define V_ASKEL_MP   139
    K_KV, K_SISAR_V, K_SISAR_K, K_LI, K_0,

#define V_ILL_SEEN   144
    K_LS, K_LE, K_LE, K_0,

#define V_ILL_H      148
#define V_ILL        149
    K_LH, K_V, K_0,

#define V_ILL_AAN    151
#define V_ILL_AN     152
    K_A, K_A, K_0,

#define V_ILL_EEN    154
    K_LE, K_0,

#define V_AP         156
    K_FI_AP, K_0,

#define V_CL         158
    K_FI_CL, K_0,

#if KOSKUT_PLURALS

#define V_OVI_MV     160
    K_KV, K_0,

#define V_KALA_MV    162
    K_KV, K_O, K_0,

#define V_SOLAKKA_MV 165
    K_KH, K_O, K_0,

#define V_RISTI_MV   168
    K_KV, K_LE, K_0,

#define V_SISAR_MV   171
    K_KV, K_SISAR_V, K_SISAR_K, K_0,

#define V_KYTKIN_MV  175
    K_KV, K_V, K_LM, K_0,

#define V_VASTAUS_MV 179
#define V_NAINEN_MV  180
    K_LK, K_LS, K_0,

#define V_TUHAT_MV   182
    K_LN, K_LS, K_0,

#define V_MIES_MV    185
    K_LH, K_0,

#define V_MH         187
    K_KH, K_0,

#define V_VALO_MHH   189
    K_KH, K_HM, K_V, K_0,

#define V_RISTI_MH   193
    K_KH, K_LE, K_0,

#define V_KALA_MH    196
    K_KH, K_O, K_0,

#define V_OVI_MH     199
    K_KH, K_HM, K_0,

#define V_MG_DE      202
    K_LD, K_LE, K_0,

#define V_MG_STE     205
#define V_MG_TE      206
    K_LS, K_LT, K_LE, K_0,

#define V_MILL_SIIN  209
    K_LS, K_LI, K_LI, K_0,

#define V_MILL_IH    213
#define V_MILL_H     214
#define V_MILL       215
    K_LI, K_LH, K_LI, K_0
#endif /* KOSKUT_PLURALS */
};

static const kt_merkki loppuvokaalit[8] = {
    K_FI_I, K_FI_A, K_FI_O, K_FI_U, K_FI_E, K_FI_AE, K_FI_OE, K_FI_Y
};

static const kt_merkki suo_vokaalit[8] = {
    K_FI_A, K_FI_A, K_FI_U, K_FI_U, K_FI_I, K_FI_AE, K_FI_Y, K_FI_Y
};

static const kt_merkki sisar_kons[3] = { K_FI_L, K_FI_N, K_FI_R };

static kt_koko k_vartalo(kt_nomtaiv taiv, char **p_puskuri, char *loppu,
                         unsigned char pvart) {
    kt_merkki m;
    kt_koko n = 0;
    const kt_merkki *vart;
    char *puskuri = *p_puskuri;

    if (!pvart) return 0;
    vart = &paatteet_data[pvart];

    while ((m = *vart++) != K_0) {
        switch (m) {
        case K_V:
            m = loppuvokaalit[K_VOKAALI(taiv)];
            break;
        case K_SUO_V:
            m = suo_vokaalit[K_VOKAALI(taiv)];
            break;
        case K_SISAR_V:
            if (K_VOKAALI(taiv) & 4) {
                m = K_FI_E;
                break;
            }
        case K_A:
            m = K_ETINEN(taiv) ? K_FI_AE : K_FI_A;
            break;
        case K_O:
            m = K_ETINEN(taiv) ? K_FI_OE : K_FI_O;
            break;

        case K_KV:
        case K_KH:
            n += k_aste_aste2(taiv, m == K_KV, &puskuri, loppu);
            break;
        case K_KYNSI_K:
            m = sisar_kons[K_ASTE(taiv) - 9];
            break;
        case K_SISAR_K:
            m = sisar_kons[K_VOKAALI(taiv) & 3];
            break;

        case K_HM:
            if (!K_HEITTO(taiv)) continue;
            m = K_FI_AP;
            break;
        }
        n += K_OUT(puskuri, loppu, m);
    }

    *p_puskuri = puskuri;
    return n;
}

static const unsigned char vartalot_yv[64] = {
    V_YV, V_YV, V_YV, V_YV, V_YV, V_YV, V_YV, V_YV, V_YV, /*  0-8   C*V$     */
    V_KOIRA_YV, V_KOIRA_YV,                               /*  9-10  C*A$     */
    V_YV, V_YV, V_YV, V_YV, V_YV, V_YV, V_YV, V_YV,       /* 11-18  C*V$     */
    V_SUO_YV,                                             /* 19     V2V$     */
    V_YV,                                                 /* 20     C*V$     */
    0,                                                    /* 21     $        */
    V_AP,                                                 /* 22     '$       */
    V_YV, V_YV,                                           /* 23-24  C*V$     */
    V_TOIMI_YV,                                           /* 25     me$      */
    V_YV, V_YV, V_YV,                                     /* 26-28  C*V$     */
    V_LAPSI_YV,                                           /* 29     pse$     */
    V_VEITSI_YV,                                          /* 30     tse$     */
    V_KAKSI_YV,                                           /* 31     hte$     */
    V_SISAR_YV,                                           /* 32     C*VRe$   */
    V_KYTKIN_YV,                                          /* 33     C*Vme$   */
    V_LAMMIN_YV, V_LAMMIN_YV,                             /* 34-35  C*VmA$   */
    V_YV, V_YV,                                           /* 36-37  C*V$     */
    V_NAINEN_YV,                                          /* 38     se$      */
    V_VASTAUS_YV,                                         /* 39     kse$     */
    V_YV,                                                 /* 40     C*V$     */
    V_VIERAS_YV,                                          /* 41     C*VV$    */
    V_MIES_YV,                                            /* 42     he$      */
    V_OHUT_YV,                                            /* 43     C*Ve$    */
    V_KEVAT_YV,                                           /* 44     VV$      */
    V_YV, V_YV,                                           /* 45-46  C*V$     */
    V_OLLUT_YV,                                           /* 47     ee$      */
    V_HAME_YV,                                            /* 48     CÄee$    */
    V_ASKEL_YV,                                           /* 49     C*VRee$  */
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,                   /* 50-61  --       */
    V_CL, V_CL                                            /* 62-63  :$       */
};

/* sisältää heikot yksikkövartalot, illatiivivartalot ja partitiivikannat. */
/* oktaalilukuna:
    yks.par.kanta        yks.ill.vartalo      yks.heik.vartalo
     0   --               0   V_ILL            0   =yks.vahv.
     1   yks.vahv.        1   V_ILL_H          1   V_YH
     2   yks.nom.         2   V_ILL_EEN        2   V_VALO_YH
     3   yks.vahv. + A    3   V_ILL_AN         3   V_KOIRA_YH
                          4   V_ILL_AAN        4   V_KAKSI_YH
                          5   V_ILL_SEEN
*/

static const unsigned char vartalot_ypih[64] = {
/*    --      1      2      3      4      5      6      7  */
    0101U, 0102U, 0100U, 0100U, 0101U, 0101U, 0100U, 0101U,
/*     8      9     10     11     12     13     14     15  */
    0102U, 0133U, 0133U, 0100U, 0100U, 0100U, 0101U, 0100U,
/*    16     17     18     19     20     21     22     23  */
    0101U, 0150U, 0110U, 0110U, 0150U, 0110U, 0110U, 0000U,
/*    24     25     26     27     28     29     30     31  */
    0000U, 0000U, 0000U, 0001U, 0001U, 0000U, 0000U, 0004U,
/*    32     33     34     35     36     37     38     39  */
    0220U, 0220U, 0230U, 0230U, 0233U, 0233U, 0000U, 0200U,
/*    40     41     42     43     44     45     46     47  */
    0001U, 0250U, 0200U, 0220U, 0250U, 0001U, 0001U, 0250U,
/*    48     49                                            */
    0230U, 0310U,     0,     0,     0,     0,     0,     0,
/*                                              62     63  */
        0,     0,     0,     0,     0,     0, 0340U, 0110U
};

static const unsigned char vartalot_yp_a[] = {
    0, K_NVART_YV, K_NVART_YN, K_NVART_YV
};
static const unsigned char vartalot_yh_a[] = {
    V_YH, V_VALO_YH, V_KOIRA_YH, V_KAKSI_YH
};
static const unsigned char vartalot_yi_a[] = {
    V_ILL, V_ILL_H, V_ILL_EEN, V_ILL_AN, V_ILL_AAN, V_ILL_SEEN
};

/* sisältää partitiivivartalot ja nominatiivivartalot. */
/* yhdistelmänä N(a, b):
   yks.nom.vartalo                                          yks.par.vartalo
    0   = yks.vahv.    8   V_SISAR_I    16   V_TUHAT_I       0   0
    1   V_OVI_I        9   V_KYTKIN_I   17   V_HAME_I        1   V_MAA_YP
    2   V_TOIMI_I     10   V_SISIN_I    18   0               2   V_TOIMI_YP
    3   V_VESI_I      11   V_NAINEN_I                        3   V_TUHAT_YP
    4   V_KYNSI_I     12   V_MIES_I                          4   V_KYNSI_YP
    5   V_LAPSI_I     13   V_VIERAS_I                        5   V_LAPSI_YP
    6   V_VEITSI_I    14   V_OHUT_I                          6   V_KAKSI_YP
    7   V_KAKSI_I     15   V_KEVAT_I                         7   V_HAME_YP
*/

#define N(a, b) (((a##U) << 3) | (b##U))
static const unsigned char vartalot_ynp[64] = {
/*    --        1        2        3        4        5        6        7     */
    N( 0,0), N( 0,0), N( 0,0), N( 0,1), N( 0,0), N( 0,0), N( 0,0), N( 1,0),
/*      8        9       10       11       12       13       14       15    */
    N( 0,0), N( 0,0), N( 0,0), N( 0,0), N( 0,0), N( 0,0), N( 0,0), N( 0,0),
/*     16       17       18       19       20       21       22       23    */
    N( 1,0), N( 0,1), N( 0,1), N( 0,1), N( 0,1), N(18,1), N(18,1), N( 1,1),
/*     24       25       26       27       28       29       30       31    */
    N( 1,1), N( 2,2), N( 1,1), N( 3,3), N( 4,4), N( 5,5), N( 6,5), N( 7,6),
/*     32       33       34       35       36       37       38       39    */
    N( 8,1), N( 9,1), N( 9,1), N( 9,1), N(10,1), N(10,1), N(11,5), N(12,1),
/*     40       41       42       43       44       45       46       47    */
    N(12,3), N(13,1), N(12,1), N(14,1), N(15,1), N(12,3), N(16,3), N(15,1),
/*     48       49                                                          */
    N(17,7), N( 8,1),       0,       0,       0,       0,       0,       0,
/*                                                          62       63     */
          0,       0,       0,       0,       0,       0, N(18,0), N(18,1)
};
#undef N

static const unsigned char vartalot_yp_b[] = {
    0, V_MAA_YP, V_TOIMI_YP, V_TUHAT_YP, V_KYNSI_YP,
    V_LAPSI_YP, V_KAKSI_YP, V_HAME_YP
};

static const unsigned char vartalot_yn_b[] = {
    V_OVI_I, V_TOIMI_I, V_VESI_I, V_KYNSI_I, V_LAPSI_I, V_VEITSI_I,
    V_KAKSI_I, V_SISAR_I, V_KYTKIN_I, V_SISIN_I, V_NAINEN_I, V_MIES_I,
    V_VIERAS_I, V_OHUT_I, V_KEVAT_I, V_TUHAT_I, V_HAME_I, 0
};

#if KOSKUT_PLURALS

static const unsigned char vartalot_mv[64] = {
    V_YV, V_YV, V_YV, V_YV, V_YV,                         /*  0-4   C*Vi$    */
    V_RISTI_MV, V_RISTI_MV,                               /*  5-6   C*ei$    */
    V_OVI_MV,                                             /*  7     C*i$     */
    V_YV,                                                 /*  8     C*Vi$    */
    V_KALA_MV,                                            /*  9     C*Oi$    */
    V_OVI_MV, V_OVI_MV,                                   /* 10-11  C*i$     */
    V_KALA_MV, V_KALA_MV,                                 /* 12-13  C*Oi$    */
    V_SOLAKKA_MV,                                         /* 14     C*i$     */
    V_OVI_MV, V_OVI_MV, V_OVI_MV, V_OVI_MV,               /* 15-18  C*i$     */
    V_YV,                                                 /* 19     C*V$     */
    V_OVI_MV, V_OVI_MV,                                   /* 20-21  C*i$     */
    V_AP,                                                 /* 22     '$       */
    V_OVI_MV, V_OVI_MV,                                   /* 23-24  C*i$     */
    0,                                                    /* 25     =yks.nom.*/
    V_OVI_MV,                                             /* 26     C*i$     */
    0, 0, 0, 0, 0,                                        /* 27-31  =yks.nom.*/
    V_SISAR_MV,                                           /* 32     C*VRe$   */
    V_KYTKIN_MV, V_KYTKIN_MV, V_KYTKIN_MV,                /* 33-35  C*Vmi$   */
    V_OVI_MV, V_OVI_MV,                                   /* 36-37  C*i$     */
    V_NAINEN_MV,                                          /* 38     si$      */
    V_VASTAUS_MV, V_VASTAUS_MV,                           /* 39-40  ksi$     */
    V_YV,                                                 /* 41     C*Vi$    */
    V_MIES_MV,                                            /* 42     hi$      */
    V_YV,                                                 /* 43     C*Vi$    */
    V_0YV,                                                /* 44     Vi$      */
    V_TUHAT_MV, V_TUHAT_MV,                               /* 45-46  nsi$     */
    V_MG_E,                                               /* 47     ei$      */
    V_YV,                                                 /* 48     C*Vi$    */
    V_SISAR_YV,                                           /* 49     C*VRei$  */
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,                   /* 50-61  --       */
    V_CL, V_CL                                            /* 62-63  :$       */
};

/* sisältää heikot monikkovartalot, illatiivivartalot ja partitiivikannat. */
/* oktaalilukuna:
    mon.par.kanta        mon.ill.vartalo      mon.heik.vartalo
     0   MVi +   -A       0   -in              0   =mon.vahv.
     1   MVj +   -A       1   -hin             1   =yks.nom.
     2   MH  + -itA       2   -ihin            2   V_YH
     3   0   +  -iA       3   -siin            3   V_MH
                                               4   V_RISTI_MH
                                               5   V_OVI_MH
                                               6   V_KALA_MH
*/

static const unsigned char vartalot_mpih[64] = {
/*    --      1      2      3      4      5      6      7  */
    0012U, 0112U, 0110U, 0210U, 0212U, 0114U, 0214U, 0005U,
/*     8      9     10     11     12     13     14     15  */
    0112U, 0116U, 0003U, 0003U, 0216U, 0216U, 0216U, 0233U,
/*    16     17     18     19     20     21     22     23  */
    0003U, 0233U, 0213U, 0210U, 0213U, 0210U, 0210U, 0003U,
/*    24     25     26     27     28     29     30     31  */
    0003U, 0001U, 0003U, 0001U, 0001U, 0001U, 0001U, 0001U,
/*    32     33     34     35     36     37     38     39  */
    0000U, 0000U, 0000U, 0000U, 0003U, 0003U, 0000U, 0000U,
/*    40     41     42     43     44     45     46     47  */
    0000U, 0230U, 0000U, 0230U, 0230U, 0020U, 0020U, 0230U,
/*    48     49                                            */
    0230U, 0310U,     0,     0,     0,     0,     0,     0,
/*                                              62     63  */
        0,     0,     0,     0,     0,     0, 0000U, 0210U
};

static const unsigned char vartalot_mh_a[] = {
    V_YH, V_MH, V_RISTI_MH, V_OVI_MH, V_KALA_MH
};

static const unsigned char vartalot_mi_a[] = {
    V_MILL, V_MILL_H, V_MILL_IH, V_MILL_SIIN
};

static const unsigned char vartalot_mp_a[] = {
    K_NVART_MV, K_NVART_MVJ, K_NVART_MH, 0
};

static const unsigned char vartalot_mp_b[] = {
    0, 0, V_MAA_YP, V_ASKEL_MP
};

/* sisältää monikon genetiivimuotojen tiedot. */
/* oktaalilukuna:
    mon.gen.kanta        mon.gen.pääte
     0   0                0   V_MG_E
     1   K_NVART_YV       1   V_MG_DE
     2   K_NVART_YN       2   V_MG_TE
     3   K_NVART_MH       3   V_MG_STE
     4   K_NVART_MV
     5   K_NVART_MVJ
*/

static const unsigned char vartalot_mg[64] = {
/*    --      1      2      3      4      5      6      7  */
    0040U, 0050U, 0050U, 0041U, 0031U, 0010U, 0041U, 0040U,
/*     8      9     10     11     12     13     14     15  */
    0050U, 0050U, 0040U, 0040U, 0041U, 0041U, 0031U, 0041U,
/*    16     17     18     19     20     21     22     23  */
    0040U, 0041U, 0041U, 0041U, 0041U, 0041U, 0041U, 0040U,
/*    24     25     26     27     28     29     30     31  */
    0040U, 0040U, 0002U, 0040U, 0040U, 0003U, 0040U, 0040U,
/*    32     33     34     35     36     37     38     39  */
    0040U, 0040U, 0040U, 0040U, 0040U, 0040U, 0003U, 0022U,
/*    40     41     42     43     44     45     46     47  */
    0040U, 0041U, 0022U, 0040U, 0041U, 0040U, 0040U, 0041U,
/*    48     49                                            */
    0230U, 0310U,     0,     0,     0,     0,     0,     0,
/*                                              62     63  */
        0,     0,     0,     0,     0,     0, 0040U, 0041U
};

static const unsigned char vartalot_mg_a[] = {
    0, K_NVART_YV, K_NVART_YN, K_NVART_MH, K_NVART_MV, K_NVART_MVJ
};
static const unsigned char vartalot_mg_b[] = {
    V_MG_E, V_MG_DE, V_MG_TE, V_MG_STE
};

#endif /* KOSKUT_PLURALS */

static kt_koko k_nvart_luo2(kt_nomtaiv taiv, unsigned vart,
                            char **puskuri, char *loppu);

static kt_koko k_nvart_yv(kt_nomtaiv taiv, char **puskuri, char *loppu) {
    return k_vartalo(taiv, puskuri, loppu, vartalot_yv[K_LUOKKA(taiv)]);
}

static kt_koko k_nvart_yh(kt_nomtaiv taiv, char **puskuri, char *loppu) {
    unsigned char varsi = vartalot_ypih[K_LUOKKA(taiv)] & 7;
    if (!varsi--) return k_nvart_yv(taiv, puskuri, loppu);
    return k_vartalo(taiv, puskuri, loppu, vartalot_yh_a[varsi]);
}

static kt_koko k_nvart_yi(kt_nomtaiv taiv, char **puskuri, char *loppu) {
    kt_koko n = 0;
    unsigned char varsi = (vartalot_ypih[K_LUOKKA(taiv)] >> 3) & 7;
    n = k_nvart_yv(taiv, puskuri, loppu);
    return n + k_vartalo(taiv, puskuri, loppu, vartalot_yi_a[varsi]);
}

static kt_koko k_nvart_yp(kt_nomtaiv taiv, char **puskuri, char *loppu) {
    kt_koko n;
    unsigned char varsi;
    unsigned kanta;

    kanta = vartalot_ypih[K_LUOKKA(taiv)] >> 6;
    if (kanta == 3) {
        kanta = K_NVART_YV;
        varsi = V_MAA_YP_A;
    } else {
        kanta = vartalot_yp_a[kanta];
        varsi = vartalot_yp_b[vartalot_ynp[K_LUOKKA(taiv)] & 7];
    }

    if (kanta) {
        n = k_nvart_luo2(taiv, kanta, puskuri, loppu);
    } else
        n = 0;
    return n + k_vartalo(taiv, puskuri, loppu, varsi);
}

static kt_koko k_nvart_yn(kt_nomtaiv taiv, char **puskuri, char *loppu) {
    unsigned char varsi;

    switch (K_LUOKKA(taiv)) {
        case 5:         /* risti */
        case 6:         /* paperi */
            if (K_ASTE(taiv) == 31) { /* ei vartalovokaalia */
                /* 0 */
                return 0;
            }
    }

    varsi = vartalot_ynp[K_LUOKKA(taiv)] >> 3;
    if (!varsi--) return k_nvart_yv(taiv, puskuri, loppu);
    return k_vartalo(taiv, puskuri, loppu, vartalot_yn_b[varsi]);
}

#if KOSKUT_PLURALS

static kt_koko k_nvart_mv(kt_nomtaiv taiv, char **puskuri, char *loppu) {
    kt_koko n;
    unsigned char varsi = vartalot_mv[K_LUOKKA(taiv)];
    if (!varsi) return k_nvart_yn(taiv, puskuri, loppu);
    n = k_vartalo(taiv, puskuri, loppu, varsi);
    n += K_OUT(*puskuri, loppu, K_FI_I);
    return n;
}

static kt_koko k_nvart_mvj(kt_nomtaiv taiv, char **puskuri, char *loppu) {
    kt_koko n;
    unsigned char varsi = vartalot_mv[K_LUOKKA(taiv)];
    if (!varsi) return k_nvart_yn(taiv, puskuri, loppu);
    n = k_vartalo(taiv, puskuri, loppu, varsi);
    n += K_OUT(*puskuri, loppu, K_FI_J);
    return n;
}

static kt_koko k_nvart_mh(kt_nomtaiv taiv, char **puskuri, char *loppu) {
    kt_koko n;
    unsigned char varsi;

    if (K_ASTE(taiv) == 4) {
        switch (K_LUOKKA(taiv)) {
        case 1:
            varsi = V_VALO_MHH;
            goto varsi_ok;
        case 10:
            if (!K_VOKAALI(taiv) && K_ETINEN(taiv)) {
                varsi = V_AP;
                goto varsi_ok;
            }
        }
    }

    varsi = vartalot_mpih[K_LUOKKA(taiv)] & 7;
    if (!varsi--) return k_nvart_mv(taiv, puskuri, loppu);
    if (!varsi--) return k_nvart_yn(taiv, puskuri, loppu);
    varsi = vartalot_mh_a[varsi];

varsi_ok:
    n = k_vartalo(taiv, puskuri, loppu, varsi);
    n += K_OUT(*puskuri, loppu, K_FI_I);
    return n;
}

static kt_koko k_nvart_mp(kt_nomtaiv taiv, char **puskuri, char *loppu) {
    kt_koko n;
    unsigned char varsi;
    unsigned char kanta;

    varsi = vartalot_mpih[K_LUOKKA(taiv)] >> 6;
    kanta = vartalot_mp_a[varsi];
    varsi = vartalot_mp_b[varsi];

    n = k_nvart_luo2(taiv, kanta, puskuri, loppu);
    return n + k_vartalo(taiv, puskuri, loppu, varsi);
}

static kt_koko k_nvart_mi(kt_nomtaiv taiv, char **puskuri, char *loppu) {
    kt_koko n;
    unsigned char varsi;

    if (K_LUOKKA(taiv) == 14) { /* solakka: heikko vartalo */
        n = k_nvart_mh(taiv, puskuri, loppu);
    } else {
        n = k_nvart_mv(taiv, puskuri, loppu);
    }

    varsi = vartalot_mi_a[(vartalot_mpih[K_LUOKKA(taiv)] >> 3) & 3];
    return n + k_vartalo(taiv, puskuri, loppu, varsi);
}

static kt_koko k_nvart_mg(kt_nomtaiv taiv, char **puskuri, char *loppu) {
    kt_koko n;
    unsigned char varsi = vartalot_mg[K_LUOKKA(taiv)];
    unsigned char kanta = vartalot_mg_a[varsi >> 3];
    varsi = vartalot_mg_b[varsi & 7];
    n = k_nvart_luo2(taiv, kanta, puskuri, loppu);
    return n + k_vartalo(taiv, puskuri, loppu, varsi);
}

#endif /* KOSKUT_PLURALS */

static kt_koko k_nvart_luo2(kt_nomtaiv taiv, unsigned vart,
                            char **puskuri, char *loppu) {
    switch (vart) {
        case 0:
            break;
        case K_NVART_YV:         /* vahva yksikkövartalo */
            return k_nvart_yv(taiv, puskuri, loppu);
        case K_NVART_YH:         /* heikko yksikkövartalo */
            return k_nvart_yh(taiv, puskuri, loppu);
        case K_NVART_YN:         /* yksikön nominatiivin vartalo */
            return k_nvart_yn(taiv, puskuri, loppu);
        case K_NVART_YP:         /* yksikön partitiivin vartalo */
            return k_nvart_yp(taiv, puskuri, loppu);
        case K_NVART_YI:         /* yksikön illatiivin vartalo */
            return k_nvart_yi(taiv, puskuri, loppu);
#if KOSKUT_PLURALS
        case K_NVART_MV:         /* vahva monikkovartalo (-i) */
            return k_nvart_mv(taiv, puskuri, loppu);
        case K_NVART_MH:         /* heikko monikkovartalo (-i) */
            return k_nvart_mh(taiv, puskuri, loppu);
        case K_NVART_MG:         /* monikon genetiivin vartalo */
            return k_nvart_mg(taiv, puskuri, loppu);
        case K_NVART_MP:         /* monikon partitiivin vartalo */
            return k_nvart_mp(taiv, puskuri, loppu);
        case K_NVART_MI:         /* monikon illatiivin vartalo */
            return k_nvart_mi(taiv, puskuri, loppu);

        case K_NVART_MVJ:        /* vahva monikkovartalo (-j) */
            return k_nvart_mvj(taiv, puskuri, loppu);
#endif
    }
    return 0;
}


kt_koko k_nvart_luo(kt_nomtaiv taiv, unsigned vart,
                     char *puskuri, kt_koko koko) {
    return k_nvart_luo2(taiv, vart, &puskuri, puskuri + koko);
}

