/*******************************************************************************
            Koskut -- pieni suomen kielen taivutuskirjasto
            tehnyt ziplantil 2022 -- lisenssi: MIT
            versio: 2022-12-30
            <https://github.com/ziplantil/koskut>
*******************************************************************************/
/* k_aste.c - asteet/astevaihtelut                                            */

#include "koskut.h"
#include "koskutio.h"

/*
    AAAAA: astevaihtelu
         0: ei astevaihtelua
         1: A (kk : k )
         2: B (pp : p )
         3: C (tt : t )
         4: D (k  : 0 )
         5: E (p  : v )
         6: F (t  : d )
         7: G (nk : ng)
         8: H (mp : mm)
         9: Ì (lt : ll)
        10: J (nt : nn)
        11: K (rt : rr)
        12: L (k  : j)
        13: M (k  : v)
        28:    bb : b
        29:    gg : g
        30:    dd : d
        31: ei astevaihtelua tai loppuvokaalia
*/

static const kt_merkki a_konsonantit[] = {
    K_FI_NA, K_FI_K, K_FI_P, K_FI_T, K_FI_V, K_FI_D,
    K_FI_N, K_FI_G, K_FI_M, K_FI_L, K_FI_R, K_FI_J
};

#define A_KONS_K  1
#define A_KONS_P  2
#define A_KONS_T  3
#define A_KONS_V  4
#define A_KONS_D  5
#define A_KONS_N  6
#define A_KONS_G  7
#define A_KONS_M  8
#define A_KONS_L  9
#define A_KONS_R 10
#define A_KONS_J 11

#define A_ASTE(a, b) ((a) | ((b) << 4))

static const unsigned char a_vahvat[] = {
    0,
    A_ASTE(A_KONS_K, A_KONS_K), /* kk */
    A_ASTE(A_KONS_P, A_KONS_P), /* pp */
    A_ASTE(A_KONS_T, A_KONS_T), /* tt */
    A_ASTE(A_KONS_K, 0),        /* k  */
    A_ASTE(A_KONS_P, 0),        /* p  */
    A_ASTE(A_KONS_T, 0),        /* t  */
    A_ASTE(A_KONS_N, A_KONS_K), /* nk */
    A_ASTE(A_KONS_M, A_KONS_P), /* mp */
    A_ASTE(A_KONS_L, A_KONS_T), /* lt */
    A_ASTE(A_KONS_N, A_KONS_T), /* nt */
    A_ASTE(A_KONS_R, A_KONS_T), /* rt */
    A_ASTE(A_KONS_K, 0),        /* k  */
    A_ASTE(A_KONS_K, 0),        /* k  */
    0, 0
};

static const unsigned char a_heikot[] = {
    0,
    A_ASTE(A_KONS_K, 0),        /* k  */
    A_ASTE(A_KONS_P, 0),        /* p  */
    A_ASTE(A_KONS_T, 0),        /* t  */
    0,                          /*    */
    A_ASTE(A_KONS_V, 0),        /* v  */
    A_ASTE(A_KONS_D, 0),        /* d  */
    A_ASTE(A_KONS_N, A_KONS_G), /* ng */
    A_ASTE(A_KONS_M, A_KONS_M), /* mm */
    A_ASTE(A_KONS_L, A_KONS_L), /* ll */
    A_ASTE(A_KONS_N, A_KONS_N), /* nn */
    A_ASTE(A_KONS_R, A_KONS_R), /* rr */
    A_ASTE(A_KONS_J, 0),        /* j  */
    A_ASTE(A_KONS_V, 0),        /* v  */
    0, 0
};

kt_koko k_aste_aste2(kt_nomtaiv taiv, unsigned vahva,
                     char **p_puskuri, char *loppu) {
    char *puskuri = *p_puskuri;
    unsigned astevaihtelu = (taiv >> 3) & 31;
    kt_koko n;
    unsigned char aste;

    if (astevaihtelu & 16) {
        n = 0;
#if KOSKUT_GRADE_VOICED_PLOSIVES
        switch (astevaihtelu) {
        case 28: /* gg : g */
            if (vahva) n += K_OUT(puskuri, loppu, K_FI_G);
            n += K_OUT(puskuri, loppu, K_FI_G);
            break;
        case 29: /* bb : b */
            if (vahva) n += K_OUT(puskuri, loppu, K_FI_B);
            n += K_OUT(puskuri, loppu, K_FI_B);
            break;
        case 30: /* dd : d */
            if (vahva) n += K_OUT(puskuri, loppu, K_FI_D);
            n += K_OUT(puskuri, loppu, K_FI_D);
            break;
        }
#endif
    } else {
        aste = vahva ? a_vahvat[astevaihtelu] : a_heikot[astevaihtelu];

        if (!aste) return 0;
        if (aste & 240) {
            n = K_OUT(puskuri, loppu, a_konsonantit[aste & 15]);
            aste >>= 4;
        } else {
            n = 0;
        }
        n += K_OUT(puskuri, loppu, a_konsonantit[aste]);
    }

    *p_puskuri = puskuri;
    return n;
}

kt_koko k_aste_aste(kt_nomtaiv taiv, unsigned vahva,
                    char *puskuri, kt_koko koko) {
    return k_aste_aste2(taiv, vahva, &puskuri, puskuri + koko);
}

