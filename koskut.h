/*******************************************************************************
            Koskut -- pieni suomen kielen taivutuskirjasto
            tehnyt ziplantil 2022 -- lisenssi: MIT
            versio: 2022-12-30
            <https://github.com/ziplantil/koskut>
*******************************************************************************/
/* koskut.h - kirjaston otsake                                                */

#ifndef KOSKUT_H
#define KOSKUT_H

#include "koskutio.h"

#ifndef KOSKUT_PLURALS
/* tarvitsetko monikkomuotojen tukea? */
#define KOSKUT_PLURALS 1
#endif

#ifndef KOSKUT_GRADE_VOICED_PLOSIVES
/* tarvitsetko bb:b, gg:g, dd:d asteet? */
#define KOSKUT_GRADE_VOICED_PLOSIVES 1
#endif

#ifndef KOSKUT_VERIFY
/* tarvitaanko parempi k_ntaiv_ok? */
#define KOSKUT_VERIFY 1
#endif

#if KOSKUT_C99
#include <stdint.h>
typedef uint_least16_t kt_nomtaiv;
typedef uint_least16_t kt_verbtaiv;
#define K_INL static inline
#else
typedef unsigned short kt_nomtaiv;
typedef unsigned short kt_verbtaiv;
#define K_INL static
#endif

/* kt_nomtaiv bittirakenne (16 bittiä):
    vHLLLLLL AAAAAVVV

    kaikki bitit 0 = virheellinen taivutus.

    v:  vokaalisointu (1 = etinen/äöy, 0 = takainen/aou)
    H:  heittomerkki (1 = on (vain luokille 1, 7, 9, 10), 0 = ei)
                     (1 = luokalla 38 käyttää monikkomuotoja, esim. Ikaalinen.
                          toimii vain taivutuksessa, ei vartaloissa, ja
                          vaatii lisäksi monikkotuen kääntämisen.)
    LLLLLL: taivutusluokka Kielitoimiston sanakirjan numeroinnin mukaisesti
        (ks. nomtaiv.txt)
    AAAAA: astevaihtelu (huom. ks. nomtaiv.txt)
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
        28:    gg : g
        29:    bb : b
        30:    dd : d
        31: ei astevaihtelua tai loppuvokaalia, tai partitiivi aina takainen
    VVV: loppuvokaali, käytetään taivutustyypistä riippuen (ks. nomtaiv.txt).
        0: I
        1: A
        2: O
        3: U
        4: E
        5: Ä
        6: Ö
        7: Y

*/

typedef struct k_nomini {
    kt_koko leikkaa;       /* montako tavua leikataan perusmuodon lopusta */
    kt_nomtaiv taiv;       /* sanan taivutus */
} kt_nomini;

typedef struct k_nomveik {
    const char *sana;           /* veikattava sana */
    kt_koko sana_pituus;        /* sen pituus */
    unsigned char tila;         /* tilakoneen sisäinen tila */
    unsigned char monikot;      /* tarvitaanko monikkoja? */
    unsigned char muoto;        /* veikkausvaiheen muoto (esim. yks. gen.) */
    unsigned char valintoja;    /* montako valintaa */
    kt_nomini edellinen;        /* edellinen arvaus */
    kt_nomini vaihdot[9];       /* vaihtoehtojen taivutukset */
    unsigned char tilat[9];     /* vaihtoehtojen kohdetilat */
    unsigned char luokka;       /* väliaikainen taivutusluokka */
    unsigned char vokaali;      /* väliaikainen loppuvokaali ja sointu
                                    s----VVV
                                        s: 0=taka, 1=etu
                                        VVV: vokaali (kuin yllä) */
} kt_nomveik;

/* aste.c: Siirtää vahvan tai heikon asteen puskuriin. Palauttaa
   tavujen määrän, vaikka niin monelle ei olisi ollut tilaa */
extern kt_koko k_aste_aste(kt_nomtaiv taiv, unsigned vahva,
                           char *puskuri, kt_koko koko);
extern kt_koko k_aste_aste2(kt_nomtaiv taiv, unsigned vahva,
                            char **puskuri, char *loppu);

#define K_NVART_YV 1         /* vahva yksikkövartalo */
#define K_NVART_YH 2         /* heikko yksikkövartalo */
#define K_NVART_YN 3         /* yksikön nominatiivin vartalo */
#define K_NVART_YP 4         /* yksikön partitiivin vartalo */
#define K_NVART_YI 5         /* yksikön illatiivin vartalo */
#if KOSKUT_PLURALS
#define K_NVART_MV 6         /* vahva monikkovartalo */
#define K_NVART_MH 7         /* heikko monikkovartalo */
#define K_NVART_MG 8         /* monikon genetiivin vartalo */
#define K_NVART_MP 9         /* monikon partitiivin vartalo */
#define K_NVART_MI 10        /* monikon illatiivin vartalo */
#endif

/* k_nvart.c: Kirjoittaa nominipäätteen vartalon annettuun paikkaan.
   Palauttaa tavujen määrän, vaikka niin monelle ei olisi ollut tilaa */
extern kt_koko k_nvart_luo(kt_nomtaiv taiv, unsigned vart,
                           char *puskuri, kt_koko koko);

#define K_NTAIV_YNOM 0x00    /* yksikön nominatiivi */
#define K_NTAIV_YGEN 0x01    /* yksikön genetiivi */
#define K_NTAIV_YPAR 0x02    /* yksikön partitiivi */
#define K_NTAIV_YINE 0x03    /* yksikön inessiivi */
#define K_NTAIV_YELA 0x04    /* yksikön elatiivi */
#define K_NTAIV_YILL 0x05    /* yksikön illatiivi */
#define K_NTAIV_YADE 0x06    /* yksikön adessiivi */
#define K_NTAIV_YABL 0x07    /* yksikön ablatiivi */
#define K_NTAIV_YALL 0x08    /* yksikön allatiivi */
#define K_NTAIV_YESS 0x09    /* yksikön essiivi */
#define K_NTAIV_YTRA 0x0A    /* yksikön translatiivi */
#define K_NTAIV_YABE 0x0C    /* yksikön abessiivi */

#if KOSKUT_PLURALS
#define K_NTAIV_MNOM 0x10    /* monikon nominatiivi */
#define K_NTAIV_MGEN 0x11    /* monikon genetiivi */
#define K_NTAIV_MPAR 0x12    /* monikon partitiivi */
#define K_NTAIV_MINE 0x13    /* monikon inessiivi */
#define K_NTAIV_MELA 0x14    /* monikon elatiivi */
#define K_NTAIV_MILL 0x15    /* monikon illatiivi */
#define K_NTAIV_MADE 0x16    /* monikon adessiivi */
#define K_NTAIV_MABL 0x17    /* monikon ablatiivi */
#define K_NTAIV_MALL 0x18    /* monikon allatiivi */
#define K_NTAIV_MESS 0x19    /* monikon essiivi */
#define K_NTAIV_MTRA 0x1A    /* monikon translatiivi */
#define K_NTAIV_MIST 0x1B    /* monikon instruktiivi */
#define K_NTAIV_MABE 0x1C    /* monikon abessiivi */
#define K_NTAIV_MCOM 0x1D    /* monikon komitatiivi (ei persoonapäätettä) */
#endif

/* k_ntaiv.c: Kirjoittaa taivutuspäätteen vartaloineen annettuun paikkaan.
   Palauttaa tavujen määrän, vaikka niin monelle ei olisi ollut tilaa */
extern kt_koko k_ntaiv_paate(kt_nomtaiv taiv, unsigned muoto,
                             char *puskuri, kt_koko koko);

/* k_ntaiv.c: tarkistaa onko kt_nomini kunnossa, <>0 = OK, =0 = ei OK */
extern int k_ntaiv_ok(kt_nomini vart);
extern int k_ntaiv_ok2(kt_nomtaiv taiv);

/* k_ntaiv.c: taivuttaa nominia taivutusvartalon mukaan.
   Palauttaa tavujen määrän, vaikka niin monelle ei olisi ollut tilaa.
   sanan pituus luetaan tavuina. */
extern kt_koko k_ntaiv_taivuta(kt_nomini vart, unsigned muoto,
                               const char *sana, kt_koko sana_pituus,
                               char *puskuri, kt_koko puskuri_koko);
extern kt_koko k_ntaiv_taivuta2(kt_nomtaiv taiv, unsigned muoto,
                                const char *sana, kt_koko sana_pituus,
                                char *puskuri, kt_koko puskuri_koko);

/* k_nveik.c: nominiveikkain */
extern void k_nveik_alusta(kt_nomveik *veikkaus, unsigned monikot,
                           const char *sana, kt_koko sana_pituus);
extern unsigned k_nveik_vaihtoehtoja(const kt_nomveik *veikkaus);
extern unsigned k_nveik_arvausmuoto(const kt_nomveik *veikkaus);
extern kt_koko k_nveik_vaihtoehto(const kt_nomveik *veikkaus, unsigned numero,
                                  char *puskuri, kt_koko puskuri_koko);
extern void k_nveik_valitse(kt_nomveik *veikkaus, unsigned numero);
extern kt_nomini k_nveik_tulos(const kt_nomveik *veikkaus);

/* kt_verbtaiv bittirakenne (16 bittiä):
    vHHLLLLL AAAAAVVV

    kaikki bitit 0 = virheellinen taivutus.

    v:  vokaalisointu (1 = etinen/äöy, 0 = takainen/aou)
    HH; luokille 52, 61: 01 = heittomerkki, 00 = ei heittomerkkiä
            luokalle 67: 00 = L, 01 = N, 10 = R
    LLLLL: taivutusluokka jotakuinkin Kielitoimiston sanakirjan
            numeroinnin mukaisesti; numerosta vähennetään 51
            (ks. verbtaiv.txt)
    AAAAA: astevaihtelu (huom. ks. verbtaiv.txt)
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
        28:    gg : g
        29:    bb : b
        30:    dd : d
        31: ei astevaihtelua tai loppuvokaalia, tai partitiivi aina takainen
    VVV: loppuvokaali, käytetään taivutustyypistä riippuen (ks. verbtaiv.txt).
        0: I
        1: A
        2: O
        3: U
        4: E
        5: Ä
        6: Ö
        7: Y

*/

typedef struct k_verbi {
    kt_koko leikkaa;       /* montako tavua leikataan perusmuodon lopusta */
    kt_verbtaiv taiv;      /* sanan taivutus */
} kt_verbi;

#define K_VTAIV_IPY1 0x00  /* yksikön 1. persoonan indikatiivin preesens */
#define K_VTAIV_IPY2 0x01  /* yksikön 2. persoonan indikatiivin preesens */
#define K_VTAIV_IPY3 0x02  /* yksikön 3. persoonan indikatiivin preesens */
#define K_VTAIV_IPYK 0x03  /* yksikön indikatiivin preesensin kieltomuoto */
#define K_VTAIV_IPM1 0x04  /* monikon 1. persoonan indikatiivin preesens */
#define K_VTAIV_IPM2 0x05  /* monikon 2. persoonan indikatiivin preesens */
#define K_VTAIV_IPM3 0x06  /* monikon 3. persoonan indikatiivin preesens */
#define K_VTAIV_IPMK 0x07  /* monikon indikatiivin preesensin kieltomuoto */
#define K_VTAIV_IIY1 0x08  /* yksikön 1. persoonan indikatiivin imperfekti */
#define K_VTAIV_IIY2 0x09  /* yksikön 2. persoonan indikatiivin imperfekti */
#define K_VTAIV_IIY3 0x0A  /* yksikön 3. persoonan indikatiivin imperfekti */
#define K_VTAIV_IIYK 0x0B  /* yksikön indikatiivin imperfektin kieltomuoto */
#define K_VTAIV_IIM1 0x0C  /* monikon 1. persoonan indikatiivin imperfekti */
#define K_VTAIV_IIM2 0x0D  /* monikon 2. persoonan indikatiivin imperfekti */
#define K_VTAIV_IIM3 0x0E  /* monikon 3. persoonan indikatiivin imperfekti */
#define K_VTAIV_IIMK 0x0F  /* monikon indikatiivin imperfektin kieltomuoto */
#define K_VTAIV_TKY1 0x10  /* yksikön 1. persoonan konditionaalin preesens */
#define K_VTAIV_TKY2 0x11  /* yksikön 2. persoonan konditionaalin preesens */
#define K_VTAIV_TKY3 0x12  /* yksikön 3. persoonan konditionaalin preesens */
#define K_VTAIV_TKYK 0x13  /* yksikön konditionaalin preesensin kieltomuoto */
#define K_VTAIV_TKM1 0x14  /* monikon 1. persoonan konditionaalin preesens */
#define K_VTAIV_TKM2 0x15  /* monikon 2. persoonan konditionaalin preesens */
#define K_VTAIV_TKM3 0x16  /* monikon 3. persoonan konditionaalin preesens */
#define K_VTAIV_TKMK 0x17  /* monikon konditionaalin preesensin kieltomuoto */
#define K_VTAIV_TPY1 0x18  /* yksikön 1. persoonan potentiaalin preesens */
#define K_VTAIV_TPY2 0x19  /* yksikön 2. persoonan potentiaalin preesens */
#define K_VTAIV_TPY3 0x1A  /* yksikön 3. persoonan potentiaalin preesens */
#define K_VTAIV_TPYK 0x1B  /* yksikön potentiaalin preesensin kieltomuoto */
#define K_VTAIV_TPM1 0x1C  /* monikon 1. persoonan potentiaalin preesens */
#define K_VTAIV_TPM2 0x1D  /* monikon 2. persoonan potentiaalin preesens */
#define K_VTAIV_TPM3 0x1E  /* monikon 3. persoonan potentiaalin preesens */
#define K_VTAIV_TPMK 0x1F  /* monikon potentiaalin preesensin kieltomuoto */
#define K_VTAIV_TIY2 0x21  /* yksikön 2. persoonan imperatiivin preesens */
#define K_VTAIV_TIY3 0x22  /* yksikön 3. persoonan imperatiivin preesens */
#define K_VTAIV_TIYK 0x23  /* y. 2. p. imperatiivin preesensin kieltomuoto */
#define K_VTAIV_TIM1 0x24  /* monikon 1. persoonan imperatiivin preesens */
#define K_VTAIV_TIM2 0x25  /* monikon 2. persoonan imperatiivin preesens */
#define K_VTAIV_TIM3 0x26  /* monikon 3. persoonan imperatiivin preesens */
#define K_VTAIV_TIMK 0x27  /* toinen imperatiivin preesensin kieltomuoto */
#define K_VTAIV_IPPP 0x30  /* passiivin indikatiivin preesens */
#define K_VTAIV_IPPK 0x31  /* passiivin indikatiivin preesensin kieltomuoto */
#define K_VTAIV_IPIP 0x32  /* passiivin indikatiivin imperfekti */
#define K_VTAIV_IPIK 0x33  /* passiivin indikatiivin imperfektin kieltomuoto */
#define K_VTAIV_ITKP 0x34  /* passiivin konditionaalin preesens */
#define K_VTAIV_ITKK 0x35  /* passiivin konditionaalin preesensin kieltomuoto */
#define K_VTAIV_ITPP 0x36  /* passiivin potentiaalin preesens */
#define K_VTAIV_ITPK 0x37  /* passiivin potentiaalin preesensin kieltomuoto */
#define K_VTAIV_ITIP 0x38  /* passiivin imperatiivin preesens */
#define K_VTAIV_ITIK 0x39  /* passiivin imperatiivin preesensin kieltomuoto */
#define K_VTAIV_FN1A 0x40  /* 1. infinitiivi (A-infinitiivi) */
#define K_VTAIV_FN1P 0x41  /* 1. infinitiivi (A-infinitiivi) + -kse- */
#define K_VTAIV_FN2A 0x42  /* 2. infinitiivi (E-infinitiivi), akt. iness. */
#define K_VTAIV_FN2P 0x43  /* 2. infinitiivi (E-infinitiivi), pass. iness. */
#define K_VTAIV_FN2I 0x44  /* 2. infinitiivi (E-infinitiivi), akt. instr. */
#define K_VTAIV_FN4V 0x46  /* 4. infinitiivi (MINEN-infinitiivi), vartalo */
#define K_VTAIV_FN5V 0x47  /* 5. infinitiivi (MAISILLA-infinitiivi), vartalo */
#define K_VTAIV_FN3I 0x48  /* 3. infinitiivi (MA-infinitiivi), akt. iness. */
#define K_VTAIV_FN3E 0x49  /* 3. infinitiivi (MA-infinitiivi), akt. elat. */
#define K_VTAIV_FN3L 0x4A  /* 3. infinitiivi (MA-infinitiivi), akt. illat. */
#define K_VTAIV_FN3A 0x4B  /* 3. infinitiivi (MA-infinitiivi), akt. adess. */
#define K_VTAIV_FN3B 0x4C  /* 3. infinitiivi (MA-infinitiivi), akt. abess. */
#define K_VTAIV_FN3N 0x4E  /* 3. infinitiivi (MA-infinitiivi), akt. instr. */
#define K_VTAIV_FN3P 0x4F  /* 3. infinitiivi (MA-infinitiivi), pass. instr. */
#define K_VTAIV_PA1P 0x50  /* aktiivin 1. partisiippi (VA) */
#define K_VTAIV_PA2P 0x51  /* aktiivin 2. partisiippi (NUT) */
#define K_VTAIV_PP1P 0x52  /* passiivin 1. partisiippi (TAVA) */
#define K_VTAIV_PP2P 0x53  /* passiivin 2. partisiippi (TU) */
#define K_VTAIV_PA2M 0x55  /* aktiivin 2. partisiipin monikko */
#define K_VTAIV_PPAG 0x58  /* agenttipartisiippi (MA) */
#define K_VTAIV_PPKI 0x59  /* kieltopartisiippi (MATON) */

#define K_VVART_PV 1     /* vahva preesensvartalo */
#define K_VVART_PH 2     /* heikko preesensvartalo */
#define K_VVART_IV 3     /* vahva imperfektivartalo */
#define K_VVART_IH 4     /* heikko preesensvartalo */
#define K_VVART_PP 5     /* passiivin preesensvartalo */
#define K_VVART_IP 6     /* passiivin imperfektivartalo */
#define K_VVART_I1 7     /* 1. infinitiivin (A-infinitivin) vartalo */
#define K_VVART_TK 8     /* konditionaalivartalo */
#define K_VVART_TI 9     /* imperatiivivartalo */
#define K_VVART_TP 10    /* potentiaalivartalo */
#define K_VVART_I2 11    /* 2. infinitiivin (E-infinitivin) vartalo */

/* k_vvart.c */
extern kt_koko k_vvart_luo(kt_verbtaiv taiv, unsigned vart,
                           char *puskuri, kt_koko koko);

/* k_vtaiv.c */
extern int k_vtaiv_ok(kt_verbi vart);
extern int k_vtaiv_ok2(kt_verbtaiv taiv);
                           
/* k_vtaiv.c */
extern kt_koko k_vtaiv_taivuta(kt_verbi vart, unsigned muoto,
                               const char *sana, kt_koko sana_pituus,
                               char *puskuri, kt_koko puskuri_koko);
extern kt_koko k_vtaiv_taivuta2(kt_verbtaiv taiv, unsigned muoto,
                                const char *sana, kt_koko sana_pituus,
                                char *puskuri, kt_koko puskuri_koko);
extern kt_koko k_vtaiv_paate(kt_verbtaiv taiv, unsigned muoto,
                             char *puskuri, kt_koko koko);

#endif /* KOSKUT_H */
