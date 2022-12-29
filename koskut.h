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
#define K_INL static inline
#else
typedef unsigned short kt_nomtaiv;
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

/* k_ntaiv.c: taivuttaa nominia taivutusvartalon mukaan.
   Palauttaa tavujen määrän, vaikka niin monelle ei olisi ollut tilaa.
   sanan pituus luetaan tavuina. */
extern kt_koko k_ntaiv_taivuta(kt_nomini vart, unsigned muoto,
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

#endif /* KOSKUT_H */
