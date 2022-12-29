/*******************************************************************************
            Koskut -- pieni suomen kielen taivutuskirjasto
            tehnyt ziplantil 2022 -- lisenssi: MIT
            versio: 2022-12-30
            <https://github.com/ziplantil/koskut>
*******************************************************************************/
/* k_ntaiv.c - nominin taivutus                                               */

#include "koskut.h"
#include "koskutio.h"

static const unsigned taiv_vartalot[] = {
    K_NVART_YN, K_NVART_YH, K_NVART_YP, K_NVART_YH,
    K_NVART_YH, K_NVART_YI, K_NVART_YH, K_NVART_YH,
    K_NVART_YH, K_NVART_YV, K_NVART_YH, 0,
    K_NVART_YH,
#if KOSKUT_PLURALS
    0, 0, 0,
    K_NVART_YH, K_NVART_MG, K_NVART_MP, K_NVART_MH,
    K_NVART_MH, K_NVART_MI, K_NVART_MH, K_NVART_MH,
    K_NVART_MH, K_NVART_MV, K_NVART_MH, K_NVART_MH,
    K_NVART_MH, K_NVART_MV
#endif
};

K_INL void k_kopioi(char *puskuri, const char *sana, kt_koko koko) {
    while (koko--) *puskuri++ = *sana++;
}

static const kt_merkki tp_mnom[] = { K_FI_T, K_FI_NA };
static const kt_merkki tp_ygen[] = { K_FI_N, K_FI_NA };
static const kt_merkki tp_yine[] = { K_FI_S, K_FI_S, K_FI_NA };
static const kt_merkki tp_yela[] = { K_FI_S, K_FI_T, K_FI_NA };
static const kt_merkki tp_yade[] = { K_FI_L, K_FI_L, K_FI_NA };
static const kt_merkki tp_yabl[] = { K_FI_L, K_FI_T, K_FI_NA };
static const kt_merkki tp_yall[] = { K_FI_L, K_FI_L, K_FI_E, K_FI_NA };
static const kt_merkki tp_ytra[] = { K_FI_K, K_FI_S, K_FI_I, K_FI_NA };
static const kt_merkki tp_yabe[] = { K_FI_T, K_FI_T, K_FI_NA };
static const kt_merkki tp_ycom[] = { K_FI_N, K_FI_E, K_FI_NA };

static kt_koko k_paata(char *puskuri, char *loppu, const kt_merkki *paate,
                       int etinen, int a) {
    kt_koko n = 0;
    kt_merkki m;
    if (paate)
        while ((m = *paate++) != K_FI_NA)
            n += K_OUT(puskuri, loppu, m);
    if (a)
        n += K_OUT(puskuri, loppu, etinen ? K_FI_AE : K_FI_A);
    return n;
}

static const kt_merkki *tp_paatteet[15] = {
    NULL,    tp_ygen, NULL,    tp_yine, tp_yela,
    tp_ygen, tp_yade, tp_yabl, tp_yall, tp_ygen,
    tp_ytra, tp_ygen, tp_yabe, tp_ycom, tp_mnom
};

/* onko taivutuspäätteen lopussa A:ta?
       14 13 12 11 10 _9 _8 _7 _6 _5 _4 _3 _2 _1 _0
        0  0  1  0  0  1  0  1  1  0  1  1  1  0  0

      0001 0010 1101 1100
    =0x 1    2    D    C
*/
static const unsigned int tp_a = 0x12DCU;

/* Kirjoittaa taivutuspäätteen vartaloineen annettuun paikkaan.
   Palauttaa tavujen määrän, vaikka niin monelle ei olisi ollut tilaa */
kt_koko k_ntaiv_paate(kt_nomtaiv taiv, unsigned muoto,
                      char *puskuri, kt_koko koko) {
    unsigned vartalo = taiv_vartalot[muoto];
    char *loppu = puskuri + koko;
    int etinen = taiv & 0x8000;
    kt_koko n = k_nvart_luo(taiv, vartalo, puskuri, koko);

    if (n > koko) {
        puskuri = loppu;
    } else {
        puskuri += n;
    }

    if (muoto > 16) muoto &= 15;
    if (muoto > 13) muoto -= 2;

    return n + k_paata(puskuri, loppu, tp_paatteet[muoto],
                       etinen, (tp_a >> muoto) & 1);
}

#if KOSKUT_VERIFY

static const unsigned long aste_ok[] = {
    0x00000000UL, /* ei sovi */
    0x00000001UL, /* - */
    0x80000001UL, /* -, 31 */
    0x70001FFFUL, /* -, A-L */
    0xF0001FFFUL, /* -, A-L, 31 */
    0x70003FFFUL, /* -, A-M */
    0x7000000EUL, /* A-C */
    0x00000100UL, /* H */
    0x00000104UL, /* B, H */
    0x00000E00UL, /* I-K */
    0x00000008UL, /* C */
    0x00000040UL, /* F */
    0x00000400UL, /* J */
};

static const unsigned char aste_ok_k[] = {
    0,
    /*  1 : valo      */     5,
    /*  2 : palvelu   */     1,
    /*  3 : valtio    */     1,
    /*  4 : laatikko  */     6,
    /*  5 : risti     */     4,
    /*  6 : paperi    */     2,
    /*  7 : ovi       */     3,
    /*  8 : nalle     */     3,
    /*  9 : kala      */     3,
    /* 10 : koira     */     3,
    /* 11 : omena     */     1,
    /* 12 : kulkija   */     1,
    /* 13 : katiska   */     1,
    /* 14 : solakka   */     6,
    /* 15 : korkea    */     1,
    /* 16 : vanhempi  */     7,
    /* 17 : vapaa     */     1,
    /* 18 : maa       */     1,
    /* 19 : suo       */     1,
    /* 20 : filee     */     1,
    /* 21 : rosé      */     1,
    /* 22 : parfait   */     1,
    /* 23 : tiili     */     1,
    /* 24 : uni       */     1,
    /* 25 : toimi     */     1,
    /* 26 : pieni     */     1,
    /* 27 : käsi      */     11,
    /* 28 : kynsi     */     9,
    /* 29 : lapsi     */     1,
    /* 30 : veitsi    */     1,
    /* 31 : kaksi     */     1,
    /* 32 : sisar     */     3,
    /* 33 : kytkin    */     3,
    /* 34 : onneton   */     10,
    /* 35 : lämmin    */     8,
    /* 36 : sisin     */     7,
    /* 37 : vasen     */     7,
    /* 38 : nainen    */     1,
    /* 39 : vastaus   */     1,
    /* 40 : kalleus   */     11,
    /* 41 : vieras    */     3,
    /* 42 : mies      */     1,
    /* 43 : ohut      */     3,
    /* 44 : kevät     */     1,
    /* 45 : kahdeksas */     12,
    /* 46 : tuhat     */     12,
    /* 47 : kuollut   */     1,
    /* 48 : hame      */     3,
    /* 49 : askel     */     3,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    /* 62 : :koira    */     1,
    /* 63 : :maa      */     1
};

#define B(a,e,i,o,u,y,ae,oe)                                                   \
    (((!!(y)) << 7) | ((!!(oe)) << 6) | ((!!(ae)) << 5) | ((!!(e)) << 4)       \
   | ((!!(u)) << 3) | ((!!( o)) << 2) | ((!!( a)) << 1) | ((!!(i))))

static const unsigned char vokaali_ok[64] = {
    0,
    /*  1 : valo     */      B(0, 0, 0, 1, 1, 1, 0, 1),
    /*  2 : palvelu  */      B(0, 0, 0, 1, 1, 1, 0, 1),
    /*  3 : valtio   */      B(0, 1, 0, 1, 0, 1, 0, 0),
    /*  4 : laatikko */      B(0, 0, 0, 1, 1, 1, 0, 1),
    /*  5 : risti    */      B(0, 0, 1, 0, 0, 0, 0, 0),
    /*  6 : paperi   */      B(0, 0, 1, 0, 0, 0, 0, 0),
    /*  7 : ovi      */      B(0, 1, 0, 0, 0, 0, 0, 0),
    /*  8 : nalle    */      B(0, 1, 0, 0, 0, 0, 0, 0),
    /*  9 : kala     */      B(1, 0, 1, 0, 0, 0, 1, 0),
    /* 10 : koira    */      B(1, 0, 1, 0, 0, 0, 1, 0),
    /* 11 : omena    */      B(1, 0, 0, 0, 0, 0, 1, 0),
    /* 12 : kulkija  */      B(1, 0, 0, 0, 0, 0, 1, 0),
    /* 13 : katiska  */      B(1, 0, 0, 0, 0, 0, 1, 0),
    /* 14 : solakka  */      B(1, 0, 0, 0, 0, 0, 1, 0),
    /* 15 : korkea   */      B(1, 0, 0, 0, 0, 0, 1, 0),
    /* 16 : vanhempi */      B(1, 0, 0, 0, 0, 0, 1, 0),
    /* 17 : vapaa    */      B(1, 1, 0, 1, 1, 1, 0, 0),
    /* 18 : maa      */      B(1, 1, 1, 1, 1, 1, 1, 1),
    /* 19 : suo      */      B(0, 1, 0, 1, 0, 0, 0, 1),
    /* 20 : filee    */      B(1, 1, 0, 1, 1, 1, 0, 1),
    /* 21 : rosé     */      B(1, 1, 1, 1, 1, 1, 1, 1),
    /* 22 : parfait  */      B(1, 1, 1, 1, 1, 1, 1, 1),
    /* 23 : tiili    */      B(0, 1, 0, 0, 0, 0, 0, 0),
    /* 24 : uni      */      B(0, 1, 0, 0, 0, 0, 0, 0),
    /* 25 : toimi    */      B(0, 1, 0, 0, 0, 0, 0, 0),
    /* 26 : pieni    */      B(0, 1, 0, 0, 0, 0, 0, 0),
    /* 27 : käsi     */      B(0, 1, 0, 0, 0, 0, 0, 0),
    /* 28 : kynsi    */      B(0, 1, 0, 0, 0, 0, 0, 0),
    /* 29 : lapsi    */      B(0, 1, 0, 0, 0, 0, 0, 0),
    /* 30 : veitsi   */      B(0, 1, 0, 0, 0, 0, 0, 0),
    /* 31 : kaksi    */      B(0, 1, 0, 0, 0, 0, 0, 0),
    /* 32 : sisar    */      0x77U,
    /* 33 : kytkin   */      B(1, 0, 1, 0, 1, 0, 1, 0),
    /* 34 : onneton  */      B(0, 0, 0, 1, 0, 0, 0, 1),
    /* 35 : lämmin   */      B(1, 0, 1, 0, 0, 0, 0, 0),
    /* 36 : sisin    */      B(1, 0, 0, 0, 0, 0, 1, 0),
    /* 37 : vasen    */      B(1, 0, 0, 0, 0, 0, 1, 0),
    /* 38 : nainen   */      B(0, 1, 0, 0, 0, 0, 0, 0),
    /* 39 : vastaus  */      B(0, 1, 0, 0, 0, 0, 0, 0),
    /* 40 : kalleus  */      B(0, 1, 0, 0, 0, 0, 0, 0),
    /* 41 : vieras   */      B(1, 1, 1, 1, 1, 0, 1, 0),
    /* 42 : mies     */      B(0, 1, 0, 0, 0, 0, 0, 0),
    /* 43 : ohut     */      B(0, 0, 0, 0, 1, 1, 0, 0),
    /* 44 : kevät    */      B(1, 0, 0, 0, 0, 0, 1, 0),
    /* 45 : kahdeksas*/      B(0, 1, 0, 0, 0, 0, 0, 0),
    /* 46 : tuhat    */      B(0, 1, 0, 0, 0, 0, 0, 0),
    /* 47 : kuollut  */      B(0, 0, 0, 0, 1, 1, 0, 0),
    /* 48 : hame     */      B(0, 1, 1, 1, 0, 0, 0, 0),
    /* 49 : askel    */      0x77U,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    /* 62 : :koira   */      B(1, 1, 1, 1, 1, 1, 1, 1),
    /* 63 : :maa     */      B(1, 1, 1, 1, 1, 1, 1, 1)
};
#undef B

#endif /* KOSKUT_VERIFY */

#define K_ETINEN(taiv)    ((taiv) & 0x8000)
#define K_HEITTO(taiv)    ((taiv) & 0x4000)
#define K_LUOKKA(taiv)    (((taiv) >> 8) & 63)
#define K_ASTE(taiv)      (((taiv) >> 3) & 31)
#define K_VOKAALI(taiv)   ((taiv) & 7)

int k_ntaiv_ok(kt_nomini vart) {
    kt_nomtaiv taiv = vart.taiv;
    if (taiv == 0) return 0;
    /*printf("(%u) <%u> %08lX, %02X (A=%u, V=%u)\n", K_LUOKKA(taiv), aste_ok_k[K_LUOKKA(taiv)], aste_ok[aste_ok_k[K_LUOKKA(taiv)]], vokaali_ok[K_LUOKKA(taiv)], K_ASTE(taiv), K_VOKAALI(taiv));
    */
#if KOSKUT_VERIFY
    if (!((aste_ok[aste_ok_k[K_LUOKKA(taiv)]] >> K_ASTE(taiv)) & 1))
        return 0;
    if (!((vokaali_ok[K_LUOKKA(taiv)] >> K_VOKAALI(taiv)) & 1))
        return 0;
#endif
    return 1;
}

/* yksikön heikot vartalot, monikossa sama mutta ^ 3
    15 14 13 12 11 10 _9 _8 _7 _6 _5 _4 _3 _2 _1 _0
     0  0  0  1  1  1  0  1  1  1  0  1  1  0  1  0

      0001 1101 1101 1010
    =0x 1    D    D    A
*/
static const unsigned int tvh_y = 0x1DDAU;

/* taivuttaa nominia taivutusvartalon mukaan.
   Palauttaa tavujen määrän, vaikka niin monelle ei olisi ollut tilaa */
kt_koko k_ntaiv_taivuta(kt_nomini vart, unsigned muoto,
                        const char *sana, kt_koko sana_pituus,
                        char *puskuri, kt_koko puskuri_koko) {
    char *loppu = puskuri + puskuri_koko;
    char *alku = puskuri;
    kt_koko leikkaa;
    kt_nomtaiv taiv;

    if (!k_ntaiv_ok(vart))
        return 0;
#if KOSKUT_PLURALS
    if (muoto >= 32 || (muoto & 15) >= 14)
#else
    if (muoto >= 14)
#endif
        return 0;

    leikkaa = vart.leikkaa;

    if (leikkaa > sana_pituus) leikkaa = sana_pituus;
    leikkaa = sana_pituus - leikkaa;

    if (leikkaa > puskuri_koko) {
        k_kopioi(puskuri, sana, puskuri_koko);
        puskuri = loppu;
    } else {
        k_kopioi(puskuri, sana, leikkaa);
        puskuri += leikkaa;
    }

    /* erikoistapaukset */
    taiv = vart.taiv;

    /* aika, poika -> aj|an, poj|an */
    /* taivutusluokka 9/10 (kala/koira), aste 4 (k:0), vokaali 0 (i),
       takavokaalit */
    if (K_VOKAALI(taiv) == 0 && K_ASTE(taiv) == 4 && !K_ETINEN(taiv)) {
        unsigned luokka = K_LUOKKA(taiv);
        if (luokka == 9 || luokka == 10) {
            char *temp = puskuri;
            unsigned int tvh = tvh_y;
            if (muoto & 16) tvh ^= 3;
            if ((tvh >> (muoto & 15)) & 1) {    /* heikot vartalot */
                /* korjaa viimeinen I J:llä */
                if (K_INB(temp, alku) == K_FI_I) {
                    leikkaa -= puskuri - temp;
                    leikkaa += K_OUT(temp, loppu, K_FI_J);
                    puskuri = temp;
                }
            }
        }
    } else if (K_ASTE(taiv) == 31 && muoto == K_NTAIV_YPAR) {
        switch (K_LUOKKA(taiv)) {
        case 24:
        case 26:
            if (K_ASTE(taiv) == 31) {   /* aina takainen partitiivi */
                taiv &= ~0x8000U;       /* aina takainen */
            }
        }
#if KOSKUT_PLURALS
    } else if (K_LUOKKA(taiv) == 38 && K_HEITTO(taiv) && muoto) {
        muoto |= 16; /* monikot */
#endif
    }

    return leikkaa + k_ntaiv_paate(taiv, muoto, puskuri, loppu - puskuri);
}
