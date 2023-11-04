/*******************************************************************************
            Koskut -- pieni suomen kielen taivutuskirjasto
            tehnyt ziplantil 2022-2023 -- lisenssi: MIT
            versio: 2023-11-04
            <https://github.com/ziplantil/koskut>
*******************************************************************************/
/* k_vtaiv.c - verbien taivutus                                               */

#include "koskut.h"
#include "koskutio.h"

static const unsigned char vtaiv_vartalot[] = {
    /* K_VTAIV_IPY1 */ K_VVART_PH,
    /* K_VTAIV_IPY2 */ K_VVART_PH,
    /* K_VTAIV_IPY3 */ K_VVART_PV,
    /* K_VTAIV_IPYK */ K_VVART_PH,
    /* K_VTAIV_IPM1 */ K_VVART_PH,
    /* K_VTAIV_IPM2 */ K_VVART_PH,
    /* K_VTAIV_IPM3 */ K_VVART_PV,
    /* K_VTAIV_IPMK */ K_VVART_PH,
    /* K_VTAIV_IIY1 */ K_VVART_IH,
    /* K_VTAIV_IIY2 */ K_VVART_IH,
    /* K_VTAIV_IIY3 */ K_VVART_IV,
    /* K_VTAIV_IIYK */ K_VVART_TP,
    /* K_VTAIV_IIM1 */ K_VVART_IH,
    /* K_VTAIV_IIM2 */ K_VVART_IH,
    /* K_VTAIV_IIM3 */ K_VVART_IV,
    /* K_VTAIV_IIMK */ K_VVART_TP,
    /* K_VTAIV_TKY1 */ K_VVART_TK,
    /* K_VTAIV_TKY2 */ K_VVART_TK,
    /* K_VTAIV_TKY3 */ K_VVART_TK,
    /* K_VTAIV_TKYK */ K_VVART_TK,
    /* K_VTAIV_TKM1 */ K_VVART_TK,
    /* K_VTAIV_TKM2 */ K_VVART_TK,
    /* K_VTAIV_TKM3 */ K_VVART_TK,
    /* K_VTAIV_TKMK */ K_VVART_TK,
    /* K_VTAIV_TPY1 */ K_VVART_TP,
    /* K_VTAIV_TPY2 */ K_VVART_TP,
    /* K_VTAIV_TPY3 */ K_VVART_TP,
    /* K_VTAIV_TPYK */ K_VVART_TP,
    /* K_VTAIV_TPM1 */ K_VVART_TP,
    /* K_VTAIV_TPM2 */ K_VVART_TP,
    /* K_VTAIV_TPM3 */ K_VVART_TP,
    /* K_VTAIV_TPMK */ K_VVART_TP,
    0,
    /* K_VTAIV_TIY2 */ K_VVART_PH,
    /* K_VTAIV_TIY3 */ K_VVART_TI,
    /* K_VTAIV_TIYK */ K_VVART_PH,
    /* K_VTAIV_TIM1 */ K_VVART_TI,
    /* K_VTAIV_TIM2 */ K_VVART_TI,
    /* K_VTAIV_TIM3 */ K_VVART_TI,
    /* K_VTAIV_TIMK */ K_VVART_TI,
    0, 0, 0, 0, 0, 0, 0, 0,
    /* K_VTAIV_IPVP */ K_VVART_PP,
    /* K_VTAIV_IPVK */ K_VVART_PP,
    /* K_VTAIV_IPIP */ K_VVART_IP,
    /* K_VTAIV_IPIK */ K_VVART_IP,
    /* K_VTAIV_ITKP */ K_VVART_IP,
    /* K_VTAIV_ITKK */ K_VVART_IP,
    /* K_VTAIV_ITPP */ K_VVART_IP,
    /* K_VTAIV_ITPK */ K_VVART_IP,
    /* K_VTAIV_ITIP */ K_VVART_IP,
    /* K_VTAIV_ITIK */ K_VVART_IP,
    0, 0, 0, 0, 0, 0,
    /* K_VTAIV_FN1A */ K_VVART_I1,
    /* K_VTAIV_FN1P */ K_VVART_I1,
    /* K_VTAIV_FN2A */ K_VVART_I2,
    /* K_VTAIV_FN2P */ K_VVART_IP,
    /* K_VTAIV_FN2I */ K_VVART_I2,
    0,
    /* K_VTAIV_FN4V */ K_VVART_PV,
    /* K_VTAIV_FN5V */ K_VVART_PV,
    /* K_VTAIV_FN3I */ K_VVART_PV,
    /* K_VTAIV_FN3E */ K_VVART_PV,
    /* K_VTAIV_FN3L */ K_VVART_PV,
    /* K_VTAIV_FN3A */ K_VVART_PV,
    /* K_VTAIV_FN3B */ K_VVART_PV,
    0,
    /* K_VTAIV_FN3N */ K_VVART_PV,
    /* K_VTAIV_FN3P */ K_VVART_IP,
    /* K_VTAIV_PA1P */ K_VVART_PV,
    /* K_VTAIV_PA2P */ K_VVART_TP,
    /* K_VTAIV_PP1P */ K_VVART_IP,
    /* K_VTAIV_PP2P */ K_VVART_IP,
    0,
    /* K_VTAIV_PA2M */ K_VVART_TP,
    0, 0,
    /* K_VTAIV_PPAG */ K_VVART_PV,
    /* K_VTAIV_PPKI */ K_VVART_PV
};

#define K_0 K_FI_NA
#define K_AA K_FI_X1
#define K_OO K_FI_X2
#define K_UU K_FI_X3

static const kt_merkki vpaate_data[] = {
    K_0,

#define VP_FI_PM2 1
#define VP_FI_M2 2
#define VP_FI_PK 4
    K_FI_E, K_FI_T, K_FI_T, K_FI_E, K_0,

#define VP_FI_PM3 6
#define VP_FI_M3 7
    K_FI_E, K_FI_V, K_AA, K_FI_T, K_0,

#define VP_FI_IY3 11
#define VP_FI_Y1 13
    K_OO, K_OO, K_FI_N, K_0,

#define VP_FI_IM1 15
#define VP_FI_M1 17
    K_AA, K_AA, K_FI_M, K_FI_M, K_FI_E, K_0,

#define VP_FI_IM2 21
#define VP_FI_FN1 22
    K_AA, K_AA, K_0,

#define VP_FI_IM3 24
#define VP_FI_Y2 26
    K_OO, K_OO, K_FI_T, K_0,

#define VP_FI_TKP 28
#define VP_FI_PIP 31
    K_AA, K_FI_I, K_FI_S, K_FI_I, K_FI_I, K_FI_N, K_0,

#define VP_FI_TKK 35
    K_AA, K_FI_I, K_FI_S, K_FI_I, K_0,

#define VP_FI_TPP 40
#define VP_FI_PY1 43
    K_AA, K_FI_N, K_FI_E, K_FI_E, K_FI_N, K_0,

#define VP_FI_TPK 46
    K_AA, K_FI_N, K_FI_E, K_0,

#define VP_FI_TIP 50
    K_AA, K_FI_K, K_OO, K_OO, K_FI_N, K_0,

#define VP_FI_TIK 56
#define VP_FI_IMK 58
    K_AA, K_FI_K, K_OO, K_0,

#define VP_FI_FN1P 60
    K_AA, K_FI_K, K_FI_S, K_FI_E, K_0,

#define VP_FI_FN2P 65
#define VP_FI_FN2I 66
    K_AA, K_FI_E, K_FI_S, K_FI_S, K_AA, K_0,

#define VP_FI_FN4 71
    K_FI_M, K_FI_I, K_0,

#define VP_FI_FN5 74
    K_FI_M, K_AA, K_FI_I, K_FI_S, K_FI_I, K_FI_L, K_FI_L, K_AA, K_0,

#define VP_FI_FN3I 83
    K_FI_M, K_AA, K_FI_S, K_FI_S, K_AA, K_0,

#define VP_FI_FN3E 89
    K_FI_M, K_AA, K_FI_S, K_FI_T, K_AA, K_0,

#define VP_FI_FN3L 95
#define VP_FI_PVP 96
    K_FI_M, K_AA, K_AA, K_FI_N, K_0,

#define VP_FI_FN3A 100
    K_FI_M, K_AA, K_FI_L, K_FI_L, K_AA, K_0,

#define VP_FI_FN3B 106
    K_FI_M, K_AA, K_FI_T, K_FI_T, K_AA, K_0,

#define VP_FI_FN3P 112
#define VP_FI_FN3N 113
    K_AA, K_FI_M, K_AA, K_FI_N, K_0,

#define VP_FI_NUT 117
    K_UU, K_FI_T, K_0,

#define VP_FI_NEET 120
#define VP_FI_PY2 121
    K_FI_E, K_FI_E, K_FI_T, K_0,

#define VP_FI_TU 124
    K_UU, K_0,

#define VP_FI_TAVA 126
#define VP_FI_VA 127
    K_AA, K_FI_V, K_AA, K_0,

#define VP_FI_MA 130
    K_FI_M, K_AA, K_0,

#define VP_FI_MATON 133
    K_FI_M, K_AA, K_FI_T, K_OO, K_FI_N, K_0,

#define VP_FI_PY3 139
    K_FI_E, K_FI_E, K_0,

#define VP_FI_PM1 142
    K_FI_E, K_FI_M, K_FI_M, K_FI_E, K_0
};

static const unsigned char vtp_paatteet[] = {
    /* K_VTAIV_IPY1 */ VP_FI_Y1,        /* -n */
    /* K_VTAIV_IPY2 */ VP_FI_Y2,        /* -t */
    /* K_VTAIV_IPY3 */ 0,               /* ... */
    /* K_VTAIV_IPYK */ 0,               /* - */
    /* K_VTAIV_IPM1 */ VP_FI_M1,        /* -mme */
    /* K_VTAIV_IPM2 */ VP_FI_M2,        /* -tte */
    /* K_VTAIV_IPM3 */ VP_FI_M3,        /* -vAt */
    /* K_VTAIV_IPMK */ 0,               /* - */
    /* K_VTAIV_IIY1 */ VP_FI_Y1,        /* -n */
    /* K_VTAIV_IIY2 */ VP_FI_Y2,        /* -t */
    /* K_VTAIV_IIY3 */ 0,               /* - */
    /* K_VTAIV_IIYK */ VP_FI_NUT,       /* -nUt */
    /* K_VTAIV_IIM1 */ VP_FI_M1,        /* -mme */
    /* K_VTAIV_IIM2 */ VP_FI_M2,        /* -tte */
    /* K_VTAIV_IIM3 */ VP_FI_M3,        /* -vAt */
    /* K_VTAIV_IIMK */ VP_FI_NEET,      /* -neet */
    /* K_VTAIV_TKY1 */ VP_FI_Y1,        /* -n */
    /* K_VTAIV_TKY2 */ VP_FI_Y2,        /* -t */
    /* K_VTAIV_TKY3 */ 0,               /* - */
    /* K_VTAIV_TKYK */ 0,               /* - */
    /* K_VTAIV_TKM1 */ VP_FI_M1,        /* -mme */
    /* K_VTAIV_TKM2 */ VP_FI_M2,        /* -tte */
    /* K_VTAIV_TKM3 */ VP_FI_M3,        /* -vAt */
    /* K_VTAIV_TKMK */ 0,               /* - */
    /* K_VTAIV_TPY1 */ VP_FI_PY1,       /* -en */
    /* K_VTAIV_TPY2 */ VP_FI_PY2,       /* -et */
    /* K_VTAIV_TPY3 */ VP_FI_PY3,       /* -ee */
    /* K_VTAIV_TPYK */ VP_FI_PK,        /* -e */
    /* K_VTAIV_TPM1 */ VP_FI_PM1,       /* -emme */
    /* K_VTAIV_TPM2 */ VP_FI_PM2,       /* -ette */
    /* K_VTAIV_TPM3 */ VP_FI_PM3,       /* -evAt */
    /* K_VTAIV_TPMK */ VP_FI_PK,        /* -e */
    0,
    /* K_VTAIV_TIY2 */ 0,               /* - */
    /* K_VTAIV_TIY3 */ VP_FI_IY3,       /* -OOn */
    /* K_VTAIV_TIYK */ 0,               /* - */
    /* K_VTAIV_TIM1 */ VP_FI_IM1,       /* -AAmme */
    /* K_VTAIV_TIM2 */ VP_FI_IM2,       /* -AA */
    /* K_VTAIV_TIM3 */ VP_FI_IM3,       /* -OOt */
    /* K_VTAIV_TIMK */ VP_FI_IMK,       /* -O */
    0, 0, 0, 0, 0, 0, 0, 0,
    /* K_VTAIV_IPPP */ VP_FI_PVP,       /* -AAn */
    /* K_VTAIV_IPPK */ VP_FI_FN1,       /* -A */
    /* K_VTAIV_IPIP */ VP_FI_PIP,       /* -iin */
    /* K_VTAIV_IPIK */ VP_FI_TU,        /* -U */
    /* K_VTAIV_ITKP */ VP_FI_TKP,       /* -Aisiin */
    /* K_VTAIV_ITKK */ VP_FI_TKK,       /* -Aisi */
    /* K_VTAIV_ITPP */ VP_FI_TPP,       /* -Aneen */
    /* K_VTAIV_ITPK */ VP_FI_TPK,       /* -Ane */
    /* K_VTAIV_ITIP */ VP_FI_TIP,       /* -AkOOn */
    /* K_VTAIV_ITIK */ VP_FI_TIK,       /* -AkO */
    0, 0, 0, 0, 0, 0,
    /* K_VTAIV_FN1A */ VP_FI_FN1,       /* -A */
    /* K_VTAIV_FN1P */ VP_FI_FN1P,      /* -Akse */
    /* K_VTAIV_FN2A */ VP_FI_FN2I,      /* -essA */
    /* K_VTAIV_FN2P */ VP_FI_FN2P,      /* -AessA */
    /* K_VTAIV_FN2I */ VP_FI_PY1,       /* -en */
    0,
    /* K_VTAIV_FN4V */ VP_FI_FN4,       /* -mi */
    /* K_VTAIV_FN5V */ VP_FI_FN5,       /* -mAisillA */
    /* K_VTAIV_FN3I */ VP_FI_FN3I,      /* -mAssA */
    /* K_VTAIV_FN3E */ VP_FI_FN3E,      /* -mAstA */
    /* K_VTAIV_FN3L */ VP_FI_FN3L,      /* -mAAn */
    /* K_VTAIV_FN3A */ VP_FI_FN3A,      /* -mAllA */
    /* K_VTAIV_FN3B */ VP_FI_FN3B,      /* -mAttA */
    0,
    /* K_VTAIV_FN3N */ VP_FI_FN3N,      /* -mAn */
    /* K_VTAIV_FN3P */ VP_FI_FN3P,      /* -AmAn */
    /* K_VTAIV_PA1P */ VP_FI_VA,        /* -vA */
    /* K_VTAIV_PA2P */ VP_FI_NUT,       /* -nUt */
    /* K_VTAIV_PP1P */ VP_FI_TAVA,      /* -AvA */
    /* K_VTAIV_PP2P */ VP_FI_TU,        /* -U */
    0,
    /* K_VTAIV_PA2M */ VP_FI_NEET,      /* -neet */
    0, 0,
    /* K_VTAIV_PPAG */ VP_FI_MA,        /* -mA */
    /* K_VTAIV_PPKI */ VP_FI_MATON      /* -mAtOn */
};

static kt_koko k_vpaata(char *puskuri, char *loppu, unsigned vpaate,
                        kt_bool etinen) {
    kt_koko n = 0;
    kt_merkki m;
    const kt_merkki *paate = &vpaate_data[vpaate];
    while ((m = *paate++) != K_FI_NA) {
        switch (m) {
        case K_AA:
            m = etinen ? K_FI_AE : K_FI_A;
            break;
        case K_OO:
            m = etinen ? K_FI_OE : K_FI_O;
            break;
        case K_UU:
            m = etinen ? K_FI_Y : K_FI_U;
            break;
        }
        n += K_OUT(puskuri, loppu, m);
    }
    return n;
}

/* piteeenkö yksikön 3. persoonan indikatiivin preesensissä loppuvokaali?
     0                          0   ei
     1         52 sanoa         1   pitenee
     2         53 muistaa       1   pitenee
     3         54 huutaa        1   pitenee
     4         55 soutaa        1   pitenee
     5         56 kaivaa        1   pitenee
     6         57 saartaa       1   pitenee
     7         58 laskea        1   pitenee
     8         59 tuntea        1   pitenee
     9         60 lähteä        1   pitenee
    10         61 sallia        1   pitenee
    11         62 voida         0   ei
    12         63 saada         0   ei
    13         64 juoda         0   ei
    14         65 käydä         0   ei
    15         66 rohkaista     1   pitenee
    16         67 tulla         1   pitenee
    17         68 tupakoida     0   ei
    18         69 valita        1   pitenee
    19         70 juosta        1   pitenee
    20         71 nähdä         1   pitenee
    21         72 vanheta       1   pitenee
    22         73 salata        0   ei
    23         74 katketa       1   pitenee
    24         75 selvitä       1   pitenee
    25         76 taitaa        1   pitenee
    26        *77 kumajaa       1   pitenee

      0000 0111 1011 1101 1000 0111 1111 1110
    =0x 0    7    B    D    8    7    F    E
*/
static const unsigned long tvpv_y3p = 0x07BD87FEUL;

#define K_ETINEN(taiv)    ((taiv) & 0x8000U)
#define K_LKONS(taiv)     (((taiv) >> 13) & 2)
#define K_LUOKKA(taiv)    (((taiv) >> 8) & 31)
#define K_ASTE(taiv)      (((taiv) >> 3) & 31)
#define K_VOKAALI(taiv)   ((taiv) & 7)

/* Kirjoittaa taivutuspäätteen vartaloineen annettuun paikkaan.
   Palauttaa tavujen määrän, vaikka niin monelle ei olisi ollut tilaa */
kt_koko k_vtaiv_paate(kt_verbtaiv taiv, kt_uint muoto,
                      char *puskuri, kt_koko koko) {
    unsigned vartalo = vtaiv_vartalot[muoto];
    const char *alku = puskuri;
    char *loppu = puskuri + koko;
    kt_bool etinen = !!K_ETINEN(taiv);
    kt_koko n;

    if (!vartalo) return 0;

    n = k_vvart_luo(taiv, vartalo, puskuri, koko);

    if (n >= koko) {
        puskuri = loppu;
    } else {
        puskuri += n;
        if (muoto == K_VTAIV_IPY3) {
            if ((tvpv_y3p >> K_LUOKKA(taiv)) & 1) {
                const char *poikki = puskuri;
                /* erikoistapaus! viimeinen vokaali pitenee */
                kt_merkki m = K_INB(poikki, alku);
                n += K_OUT(puskuri, loppu, m);
            }
            return n;
        }
    }

    return n + k_vpaata(puskuri, loppu, vtp_paatteet[muoto], etinen);
}

#if KOSKUT_VERIFY

static const unsigned long vaste_ok[] = {
    0x00000000UL, /* ei sovi */
    0x00000001UL, /* - */
    0x70001FFFUL, /* -, A-L */
    0x70003FFFUL, /* -, A-M */
    0x00000040UL, /* F */
    0x00000E00UL, /* I-K */
    0x00000E40UL, /* F, I-K */
    0x00000010UL  /* D */
};

static const unsigned char vaste_ok_k[] = {
    0,
    /* 52 : sanoa     */      3,
    /* 53 : muistaa   */      6,
    /* 54 : huutaa    */      6,
    /* 55 : soutaa    */      6,
    /* 56 : kaivaa    */      2,
    /* 57 : saartaa   */      6,
    /* 58 : laskea    */      2,
    /* 59 : tuntea    */      5,
    /* 60 : lähteä    */      4,
    /* 61 : sallia    */      2,
    /* 62 : voida     */      1,
    /* 63 : saada     */      1,
    /* 64 : juoda     */      1,
    /* 65 : käydä     */      1,
    /* 66 : rohkaista */      2,
    /* 67 : tulla     */      2,
    /* 68 : tupakoida */      1,
    /* 69 : valita    */      1,
    /* 70 : juosta    */      1,
    /* 71 : nähdä     */      7,
    /* 72 : vanheta   */      2,
    /* 73 : salata    */      2,
    /* 74 : katketa   */      2,
    /* 75 : selvitä   */      2,
    /* 76 : taitaa    */      4,
    /* 77 : kumajaa   */      1,
    /* 78 : kaikaa    */      7
};

#define B(a,e,i,o,u,y,ae,oe)                                                   \
    (((!!(y)) << 7) | ((!!(oe)) << 6) | ((!!(ae)) << 5) | ((!!(e)) << 4)       \
   | ((!!(u)) << 3) | ((!!( o)) << 2) | ((!!( a)) << 1) | ((!!(i))))

static const unsigned char vvokaali_ok[64] = {
    0,
    /* 52 : sanoa     */      B(0, 0, 0, 1, 1, 1, 0, 1),
    /* 53 : muistaa   */      B(1, 0, 0, 0, 0, 0, 1, 0),
    /* 54 : huutaa    */      B(1, 0, 0, 0, 0, 0, 1, 0),
    /* 55 : soutaa    */      B(1, 0, 0, 0, 0, 0, 1, 0),
    /* 56 : kaivaa    */      B(1, 0, 0, 0, 0, 0, 1, 0),
    /* 57 : saartaa   */      B(1, 0, 0, 0, 0, 0, 1, 0),
    /* 58 : laskea    */      B(0, 1, 0, 0, 0, 0, 0, 0),
    /* 59 : tuntea    */      B(0, 1, 0, 0, 0, 0, 0, 0),
    /* 60 : lähteä    */      B(0, 1, 0, 0, 0, 0, 0, 0),
    /* 61 : sallia    */      B(0, 0, 1, 0, 0, 0, 0, 0),
    /* 62 : voida     */      B(0, 0, 1, 0, 0, 0, 0, 0),
    /* 63 : saada     */      B(1, 1, 1, 1, 1, 1, 1, 1),
    /* 64 : juoda     */      B(0, 1, 0, 1, 0, 0, 0, 1),
    /* 65 : käydä     */      B(0, 0, 0, 0, 0, 1, 0, 0),
    /* 66 : rohkaista */      B(1, 1, 1, 1, 1, 1, 1, 1),
    /* 67 : tulla     */      B(1, 1, 1, 1, 1, 1, 1, 1),
    /* 68 : tupakoida */      B(0, 1, 0, 0, 0, 0, 0, 0),
    /* 69 : valita    */      B(0, 1, 0, 0, 0, 0, 0, 0),
    /* 70 : juosta    */      B(0, 1, 0, 0, 0, 0, 0, 0),
    /* 71 : nähdä     */      B(0, 1, 0, 0, 0, 0, 0, 0),
    /* 72 : vanheta   */      B(1, 1, 1, 1, 1, 1, 1, 1),
    /* 73 : salata    */      B(1, 0, 0, 0, 0, 0, 1, 0),
    /* 74 : katketa   */      B(1, 1, 1, 1, 1, 1, 1, 1),
    /* 75 : selvitä   */      B(1, 1, 1, 1, 1, 1, 1, 1),
    /* 76 : taitaa    */      B(1, 0, 0, 0, 0, 0, 1, 0),
    /* 77 : kumajaa   */      B(1, 0, 0, 0, 0, 0, 1, 0),
    /* 78 : kaikaa    */      B(1, 0, 0, 0, 0, 0, 1, 0),
};
#undef B

#endif /* KOSKUT_VERIFY */

kt_bool k_vtaiv_ok2(kt_verbtaiv taiv) {
    if (taiv == 0) return 0;
    if (K_LUOKKA(taiv) > 27) return 0;
#if KOSKUT_VERIFY
    if (!((vaste_ok[vaste_ok_k[K_LUOKKA(taiv)]] >> K_ASTE(taiv)) & 1))
        return 0;
    if (!((vvokaali_ok[K_LUOKKA(taiv)] >> K_VOKAALI(taiv)) & 1))
        return 0;
#endif
    return 1;
}

kt_bool k_vtaiv_ok(kt_verbi vart) {
    return k_vtaiv_ok2(vart.taiv);
}

K_INL void k_kopioi(char *puskuri, const char *sana, kt_koko koko) {
    while (koko--) *puskuri++ = *sana++;
}

/* taivuttaa verbiä taivutusvartalon mukaan.
   Palauttaa tavujen määrän, vaikka niin monelle ei olisi ollut tilaa */
kt_koko k_vtaiv_taivuta(kt_verbi vart, kt_uint muoto,
                        const char *sana, kt_koko sana_pituus,
                        char *puskuri, kt_koko puskuri_koko) {
    char *loppu = puskuri + puskuri_koko;
    kt_koko leikkaa;
    kt_verbtaiv taiv;

    if (!k_vtaiv_ok(vart))
        return 0;
    if (muoto >= 0x5A)
        return 0;

    leikkaa = vart.leikkaa;

    if (leikkaa > sana_pituus) leikkaa = sana_pituus;
    leikkaa = sana_pituus - leikkaa;

    if (leikkaa > puskuri_koko) {
        if (puskuri != sana) k_kopioi(puskuri, sana, puskuri_koko);
        puskuri = loppu;
    } else {
        if (puskuri != sana) k_kopioi(puskuri, sana, leikkaa);
        puskuri += leikkaa;
    }

    taiv = vart.taiv;
    return leikkaa + k_vtaiv_paate(taiv, muoto, puskuri, loppu - puskuri);
}

kt_koko k_vtaiv_taivuta2(kt_verbtaiv taiv, kt_uint muoto,
                         const char *sana, kt_koko sana_pituus,
                         char *puskuri, kt_koko puskuri_koko) {
    kt_verbi verb;
    verb.leikkaa = 0;
    verb.taiv = taiv;
    return k_vtaiv_taivuta(verb, muoto, sana, sana_pituus,
                           puskuri, puskuri_koko);
}
