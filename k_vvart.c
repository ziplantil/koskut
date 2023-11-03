/*******************************************************************************
            Koskut -- pieni suomen kielen taivutuskirjasto
            tehnyt ziplantil 2022-2023 -- lisenssi: MIT
            versio: 2023-11-03
            <https://github.com/ziplantil/koskut>
*******************************************************************************/
/* k_vvart.c - verbivartalot                                                  */

#include "koskut.h"
#include "koskutio.h"

#include <limits.h>

#define K_LKONS(taiv)       (((taiv) >> 13) & 3)
#define K_LUOKKA(taiv)      (((taiv) >> 8) & 31)
#define K_ASTE(taiv)        (((taiv) >> 3) & 31)
#define K_VOKAALI(taiv)     ((taiv) & 7)
#define K_ETINEN(taiv)      ((taiv) & 0x8000U)

#define K_0                 K_FI_NA
#define K_V                 K_FI_X1
#define K_SUO_V             K_FI_X2
#define K_A                 K_FI_X3
#define K_O                 K_FI_X4
#define K_U                 K_FI_X5
#define K_KV                K_FI_X6
#define K_KH                K_FI_X7
#define K_KX                K_FI_X8
#define K_KHX               K_FI_X9
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

static const kt_merkki vpaatteet_data[] = {
    K_0,

#define V_PV 1
#define V_PV_S 2
    K_KV, K_V, K_0,

#define V_PV_SUO 4
    K_SUO_V, K_V, K_0,

#define V_PV_KAY 7
    K_U, K_0,

#define V_PV_ISE 9
    K_KV, K_V, K_LS, K_LE, K_0,

#define V_PV_LE 14
    K_KV, K_V, K_KX, K_LE, K_0,

#define V_PV_TSE 19
    K_LT, K_LS, K_LE, K_0,

#define V_PV_KSE 23
    K_LK, K_LS, K_LE, K_0,

#define V_PV_NE 27
    K_KV, K_V, K_LN, K_LE, K_0,

#define V_PV_EA 32
    K_KV, K_V, K_A, K_0,

#define V_PV_JA 36
    K_LJ, K_A, K_0,

#define V_PH 39
    K_KH, K_V, K_0,

#define V_PH_AP 42
    K_KH, K_HM, K_V, K_0,

#define V_IV 46
    K_KV, K_0,

#define V_IV_NS 48
    K_KHX, K_LS, K_0,

#define V_IV_O 51
    K_KV, K_O, K_0,

#define V_IV_V 54
    K_LV, K_0,

#define V_IV_VS 56
#define V_IV_S 58
    K_KV, K_V, K_LS, K_0,

#define V_IV_L 60
    K_KV, K_V, K_KX, K_0,

#define V_IV_TS 64
    K_LT, K_LS, K_0,

#define V_IV_KS 67
    K_LK, K_LS, K_0,

#define V_IV_N 70
    K_KV, K_V, K_LN, K_0,

#define V_IV_J 74
    K_LJ, K_0,

#define V_IH 76
    K_KH, K_HM, K_0,

#define V_IH_O 79
    K_KH, K_O, K_0,

#define V_PP_AP 82
    K_KH, K_HM, K_V, K_LT, K_0,

#define V_PP 87
    K_KH, K_LE, K_LT, K_0,

#define V_PP_HD 91
#define V_PP_D 92
    K_LH, K_LD, K_0,

#define V_PP_VST 94
#define V_PP_ST 96
#define V_PP_T 97
    K_KH, K_V, K_LS, K_LT, K_0,

#define V_PP_VLL 99
    K_KH, K_V, K_KX, K_KX, K_0,

#define V_PP_HVT 104
    K_KH, K_V, K_LT, K_0,

#define V_IP_LT 108
    K_KH, K_V, K_KX, K_LT, K_0,

#define V_IP_HT 113
    K_LH, K_LT, K_0,

#define V_TK_22 116
    K_KV, K_A, K_LI, K_0,

#define V_TK_23 120
    K_KV, K_V, K_A, K_LI, K_0,

#define V_TI_15 125
#define V_TI_19 127
    K_KH, K_V, K_LS, K_LK, K_0,

#define V_TI_16 130
    K_KH, K_V, K_KX, K_LK, K_0,

#define V_TI_21 135
#define V_TI_18 137
    K_KH, K_V, K_LT, K_LK, K_0,

#define V_TI_20 140
    K_LH, K_LK, K_0,

#define V_TP_15 143
#define V_TP_19 145
    K_KH, K_V, K_LS, K_LS, K_0,

#define V_TP_16 148
    K_KH, K_V, K_KX, K_KX, K_0,

#define V_TP_21 153
#define V_TP_18 155
    K_KH, K_V, K_LN, K_LN, K_0,

#define V_TP_20 158
    K_LH, K_LN, K_0,

#define V_TP_25 161
    K_LN, K_LN, K_0,

#define V_TK_ISI 164
#define V_TK_SI 165
    K_LI, K_LS, K_LI, K_0,
};

static const unsigned char v_pv_v[] = {
    V_PV, V_PV_S, V_PV_SUO, V_PV_KAY, V_PV_ISE,
    V_PV_LE, V_PV_TSE, V_PV_KSE, V_PV_NE, V_PV_EA, V_PV_JA
};

static const unsigned char v_iv_v[] = {
    V_IV, V_PV, V_IV_NS, V_IV_O, V_IV_V, V_IV_VS,
    V_IV_L, V_IV_TS, V_IV_KS, V_IV_N, V_IV_J, V_PV_S, V_IV_S
};

/* PV + IV vartalot.
    PV:0 = 0.           IV:0 = 0.
    muut, ks. yllä. (vähennä 1, 1 indekseistä) */
#define N(a,b) (unsigned char)((b) << 4 | (a))
static const unsigned char v_pv_iv[] = {
/*      0         1         2         3    */
    N( 0, 0), N( 1, 2), N( 1, 1), N( 1, 3),
/*      4         5         6         7    */
    N( 1, 1), N( 1, 4), N( 1, 3), N( 1, 1),
/*      8         9        10        11    */
    N( 1, 3), N( 1, 1), N( 1, 1), N( 0, 0),
/*     12        13        14        15    */
    N( 2, 1), N( 3,12), N( 4, 5), N( 5, 6),
/*     16        17        18        19    */
    N( 6, 7), N( 0, 0), N( 7, 8), N( 8, 9),
/*     20        21        22        23    */
    N( 1, 1), N( 9,10), N(10, 6), N(10, 6),
/*     24        25        26              */
    N(10, 6), N( 1,13), N(11,11),
};
#undef N

static const unsigned char v_ph_v[] = { V_PH, V_PH_AP };
static const unsigned char v_ih_v[] = { V_IH, V_IH_O };

/* PH + IH vartalot.
    PH:0 = PV.     IH:0 = PH.    IH:1 = IV.
    muut, ks. yllä. (vähennä 1, 2 indekseistä) */
#define N(a,b) ((b##U) << 2 | (a##U))
static const unsigned char v_ph_ih[] = {
/*     0        1        2        3        4        5        6        7      */
    N(0, 0), N(2, 1), N(1, 2), N(1, 0), N(1, 2), N(1, 3), N(1, 0), N(1, 2),
/*     8        9       10       11       12       13       14       15      */
    N(1, 0), N(1, 2), N(2, 2), N(0, 0), N(0, 0), N(0, 1), N(0, 0), N(0, 1),
/*    16       17       18       19       20       21       22       23      */
    N(0, 1), N(0, 1), N(0, 1), N(0, 1), N(1, 2), N(0, 1), N(0, 1), N(0, 1),
/*    24       25       26     */
    N(0, 1), N(1, 1), N(0, 1)
};
#undef N

static const unsigned char v_pp_v[] = {
    V_PP, V_PP_AP, V_PP_VST, V_PP_VLL, V_PP_T, V_PP_ST, V_PP_HD, V_PP_HVT
};

static const unsigned char v_ip_v[] = { V_IP_LT, V_IP_HT };

/* PP + IP vartalot.
    PP:0 = PV + d.     IP:0 = PP.    IP:1 = PP + t.   IP:2 = PV + t.
    muut, ks. yllä. (vähennä 1, 3 indekseistä) */
#define N(a,b) ((b##U) << 4 | (a##U))
static const unsigned char v_pp_ip[] = {
/*     0        1        2        3        4        5        6        7      */
    N(0, 1), N(2, 1), N(1, 1), N(1, 1), N(1, 1), N(1, 1), N(1, 1), N(1, 1),
/*     8        9       10       11       12       13       14       15      */
    N(1, 1), N(1, 1), N(2, 1), N(0, 2), N(0, 2), N(0, 2), N(0, 2), N(3, 0),
/*    16       17       18       19       20       21       22       23      */
    N(4, 3), N(0, 2), N(5, 1), N(6, 0), N(7, 4), N(8, 1), N(8, 1), N(8, 1),
/*    24       25       26     */
    N(8, 1), N(1, 1), N(5, 1)
};
#undef N

static const unsigned char v_tk_v[] = { V_TK_22, V_TK_23 };

static const unsigned char v_ti_v[] = {
    V_TI_15, V_TI_16, V_TI_18, V_TI_19, V_TI_20, V_TI_21
};

static const unsigned char v_tp_v[] = {
    V_TP_15, V_TP_16, V_TP_18, V_TP_19, V_TP_20, V_TP_21, V_TP_25
};

/* TK + TI + TP vartalot.
    TK:0 = PV(-ei)+i.      TK:1 = IV       TI:0 = PV+k.      TP:0 = PV+n.
    muut, ks. yllä. (vähennä 2, 1, 1 indekseistä) */
#define N(a,b,c) ((c##U) << 5 | (b##U) << 2 | (a##U))
static const unsigned char v_tpik[] = {
/*      0         1         2         3    */
    N(0,0,0), N(0,0,0), N(0,0,0), N(0,0,0),
/*      4         5         6         7    */
    N(0,0,0), N(0,0,0), N(0,0,0), N(0,0,0),
/*      8         9        10        11    */
    N(0,0,0), N(0,0,0), N(0,0,0), N(1,0,0),
/*     12        13        14        15    */
    N(1,0,0), N(1,0,0), N(1,0,0), N(0,1,1),
/*     16        17        18        19    */
    N(0,2,2), N(1,0,0), N(0,3,3), N(0,4,4),
/*     20        21        22        23    */
    N(0,5,5), N(0,6,6), N(2,6,6), N(3,6,6),
/*     24        25        26              */
    N(0,6,6), N(0,0,7), N(0,3,3),
};
#undef N

static const kt_merkki loppuvokaalit[8] = {
    K_FI_I, K_FI_A, K_FI_O, K_FI_U, K_FI_E, K_FI_AE, K_FI_OE, K_FI_Y
};

static const kt_merkki suo_vokaalit[8] = {
    K_FI_A, K_FI_A, K_FI_U, K_FI_U, K_FI_I, K_FI_AE, K_FI_Y, K_FI_Y
};

static const kt_merkki v_xkons[3] = { K_FI_L, K_FI_N, K_FI_R };

static kt_koko k_vvartalo(kt_verbtaiv taiv, char **p_puskuri, char *loppu,
                          kt_uint pvart) {
    kt_merkki m;
    kt_koko n = 0;
    char *puskuri = *p_puskuri;
    const kt_merkki *vart = &vpaatteet_data[pvart];

    while ((m = *vart++) != K_0) {
        switch (m) {
        case K_V:
            m = loppuvokaalit[K_VOKAALI(taiv)];
            break;
        case K_SUO_V:
            m = suo_vokaalit[K_VOKAALI(taiv)];
            break;
        case K_A:
            m = K_ETINEN(taiv) ? K_FI_AE : K_FI_A;
            break;
        case K_O:
            m = K_ETINEN(taiv) ? K_FI_OE : K_FI_O;
            break;
        case K_U:
            m = K_ETINEN(taiv) ? K_FI_Y : K_FI_U;
            break;

        case K_KV:
        case K_KH:
            n += k_aste_aste2(taiv, m == K_KV, &puskuri, loppu);
            break;
        case K_KHX:
            if (K_ASTE(taiv) < 9) continue;
            m = v_xkons[K_ASTE(taiv) - 9];
            break;
        case K_KX:
            m = v_xkons[K_LKONS(taiv)];
            break;

        case K_HM:
            if (!K_LKONS(taiv)) continue;
            m = K_FI_AP;
            break;
        }
        n += K_OUT(puskuri, loppu, m);
    }

    *p_puskuri = puskuri;
    return n;
}

static kt_koko k_vvart_luo2(kt_verbtaiv taiv, kt_uint vart,
                            char **puskuri, char *loppu);

static kt_koko k_vvart_pv(kt_verbtaiv taiv, char **puskuri, char *loppu) {
    kt_uint vars = v_pv_iv[K_LUOKKA(taiv)] & 15;
    if (!vars--) return 0;
    return k_vvartalo(taiv, puskuri, loppu, v_pv_v[vars]);
}

static kt_koko k_vvart_ph(kt_verbtaiv taiv, char **puskuri, char *loppu) {
    kt_uint vars = v_ph_ih[K_LUOKKA(taiv)] & 3;
    if (!vars--) return k_vvart_pv(taiv, puskuri, loppu);
    return k_vvartalo(taiv, puskuri, loppu, v_ph_v[vars]);
}

static kt_koko k_vvart_iv(kt_verbtaiv taiv, char **puskuri, char *loppu) {
    kt_uint vars = (v_pv_iv[K_LUOKKA(taiv)] >> 4) & 15;
    kt_koko n;
    if (!vars--) return 0;
    n = k_vvartalo(taiv, puskuri, loppu, v_iv_v[vars]);
    n += K_OUT(*puskuri, loppu, K_FI_I);
    return n;
}

static kt_koko k_vvart_ih(kt_verbtaiv taiv, char **puskuri, char *loppu) {
    kt_uint vars = (v_ph_ih[K_LUOKKA(taiv)] >> 2) & 3;
    kt_koko n;
    if (!vars--) return k_vvart_ph(taiv, puskuri, loppu);
    if (!vars--) return k_vvart_iv(taiv, puskuri, loppu);
    n = k_vvartalo(taiv, puskuri, loppu, v_ih_v[vars]);
    n += K_OUT(*puskuri, loppu, K_FI_I);
    return n;
}

static kt_koko k_vvart_pp(kt_verbtaiv taiv, char **puskuri, char *loppu) {
    kt_uint vars = v_pp_ip[K_LUOKKA(taiv)] & 15;
    if (!vars--) {
        /* vars = 0: PV + d */
        kt_koko n = k_vvart_pv(taiv, puskuri, loppu);
        n += K_OUT(*puskuri, loppu, K_FI_D);
        return n;
    }
    return k_vvartalo(taiv, puskuri, loppu, v_pp_v[vars]);
}

static kt_koko k_vvart_ip(kt_verbtaiv taiv, char **puskuri, char *loppu) {
    kt_uint vars = (v_pp_ip[K_LUOKKA(taiv)] >> 4) & 7;
    if (!vars--) {
        /* vars = 0: PP */
        return k_vvart_pp(taiv, puskuri, loppu);
    }
    if (!vars--) {
        /* vars = 1: PP + t */
        kt_koko n = k_vvart_pp(taiv, puskuri, loppu);
        n += K_OUT(*puskuri, loppu, K_FI_T);
        return n;
    }
    if (!vars--) {
        /* vars = 2: PV + t */
        kt_koko n = k_vvart_pv(taiv, puskuri, loppu);
        n += K_OUT(*puskuri, loppu, K_FI_T);
        return n;
    }
    return k_vvartalo(taiv, puskuri, loppu, v_ip_v[vars]);
}

static kt_koko k_vvart_tk(kt_verbtaiv taiv, char **puskuri, char *loppu) {
    kt_uint vars = v_tpik[K_LUOKKA(taiv)] & 3;
    kt_koko n;
    if (!vars--) {
        /* vars = 0: PV(-ei) + i */
        char *alku = *puskuri;
        kt_koko s = loppu - alku;
        n = k_vvart_pv(taiv, puskuri, loppu);
        if (n && n <= s) { /* poista viimeinen E/I */
            char *poikki = *puskuri;
            char *haku = poikki;
            kt_merkki m = K_INB(haku, alku);
            if (m == K_FI_E || m == K_FI_I) {
                n -= poikki - haku;
                *puskuri = haku;
            }
        }
        return n + k_vvartalo(taiv, puskuri, loppu, V_TK_ISI);
    }
    if (!vars--) {
        /* vars = 1: IV */
        n = k_vvart_iv(taiv, puskuri, loppu);
    } else {
        n = k_vvartalo(taiv, puskuri, loppu, v_tk_v[vars]);
    }
    return n + k_vvartalo(taiv, puskuri, loppu, V_TK_SI);
}

static kt_koko k_vvart_ti(kt_verbtaiv taiv, char **puskuri, char *loppu) {
    kt_uint vars = (v_tpik[K_LUOKKA(taiv)] >> 2) & 7;
    if (!vars--) {
        /* vars = 0: PV+k */
        kt_koko n = k_vvart_pv(taiv, puskuri, loppu);
        n += K_OUT(*puskuri, loppu, K_FI_K);
        return n;
    }
    return k_vvartalo(taiv, puskuri, loppu, v_ti_v[vars]);
}

static kt_koko k_vvart_tp(kt_verbtaiv taiv, char **puskuri, char *loppu) {
    kt_uint vars = (v_tpik[K_LUOKKA(taiv)] >> 5) & 7;
    if (!vars--) {
        /* vars = 0: PV+n */
        kt_koko n = k_vvart_pv(taiv, puskuri, loppu);
        n += K_OUT(*puskuri, loppu, K_FI_N);
        return n;
    }
    return k_vvartalo(taiv, puskuri, loppu, v_tp_v[vars]);
}

static kt_koko k_vvart_i1(kt_verbtaiv taiv, char **puskuri, char *loppu) {
    unsigned luokka = K_LUOKKA(taiv);
    if (11 <= luokka && luokka <= 24)
        return k_vvart_pp(taiv, puskuri, loppu);
    else
        return k_vvart_pv(taiv, puskuri, loppu);
}

static kt_koko k_vvart_i2(kt_verbtaiv taiv, char **puskuri, char *loppu) {
    unsigned luokka = K_LUOKKA(taiv);
    char *alku = *puskuri;
    kt_koko s = loppu - alku;
    kt_koko n = k_vvart_i1(taiv, puskuri, loppu);
    if (n && n <= s && K_VOKAALI(taiv) == 4 && luokka <= 10) {
        /* korvaa loppu-E I:llä */
        char *poikki = *puskuri;
        char *haku = poikki;
        K_INB(haku, alku);
        n -= poikki - haku;
        n += K_OUT(haku, loppu, K_FI_I);
        *puskuri = haku;
    }
    return n;
}

static kt_koko k_vvart_luo2(kt_verbtaiv taiv, kt_uint vart,
                            char **puskuri, char *loppu) {
    switch (vart) {
        case 0:
            break;
        case K_VVART_PV:         /* vahva preesensvartalo */
            return k_vvart_pv(taiv, puskuri, loppu);
        case K_VVART_PH:         /* heikko preesensvartalo */
            return k_vvart_ph(taiv, puskuri, loppu);
        case K_VVART_IV:         /* vahva imperfektivartalo */
            return k_vvart_iv(taiv, puskuri, loppu);
        case K_VVART_IH:         /* heikko preesensvartalo */
            return k_vvart_ih(taiv, puskuri, loppu);
        case K_VVART_PP:         /* passiivin preesensvartalo */
            return k_vvart_pp(taiv, puskuri, loppu);
        case K_VVART_IP:         /* passiivin imperfektivartalo */
            return k_vvart_ip(taiv, puskuri, loppu);
        case K_VVART_I1:         /* 1. infinitiivin (A-infinitivin) vartalo */
            return k_vvart_i1(taiv, puskuri, loppu);
        case K_VVART_TK:         /* konditionaalivartalo */
            return k_vvart_tk(taiv, puskuri, loppu);
        case K_VVART_TI:         /* imperatiivivartalo */
            return k_vvart_ti(taiv, puskuri, loppu);
        case K_VVART_TP:         /* potentiaalivartalo */
            return k_vvart_tp(taiv, puskuri, loppu);
        case K_VVART_I2:         /* 2. infinitiivin (E-infinitivin) vartalo */
            return k_vvart_i2(taiv, puskuri, loppu);
    }
    return 0;
}

kt_koko k_vvart_luo(kt_verbtaiv taiv, kt_uint vart,
                    char *puskuri, kt_koko koko) {
    return k_vvart_luo2(taiv, vart, &puskuri, puskuri + koko);
}
