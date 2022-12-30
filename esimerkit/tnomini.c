/* Koskuen esimerkkiohjelma */

#include "koskut.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Ohjelma olettaa, että tämän tiedoston lähdekoodin merkistö vastaa
   kohdejärjestelmän merkistöä. */

/* KTSK:n taivutusluokka, ks. nomtaiv.txt */
static unsigned luokka;
/* astevaihtelutyyppi, ks. nomtaiv.txt */
static unsigned aste;
/* vokaalisointu. 0 = takainen, <>0 = etinen */
static unsigned sointu;
/* vokaalitiedot:
    uni (24), pieni (26):       1 jos meri => merta/meressä, 0 muuten.
    rosé (21), parfait (22):    loppuvokaali (FI_K_...). */
static unsigned vaihto;

char sana[256];
char puskuri[256];

K_INL int k_vokaali_onko(kt_merkki m) {
    switch (m) {
    case K_FI_I:
    case K_FI_A:
    case K_FI_O:
    case K_FI_U:
    case K_FI_E:
    case K_FI_AE:
    case K_FI_OE:
    case K_FI_Y:
        return 1;
    }
    return 0;
}

K_INL unsigned k_vokaali_pakkaa(kt_merkki m) {
    switch (m) {
    case K_FI_I:    return 0;
    case K_FI_A:    return 1;
    case K_FI_O:    return 2;
    case K_FI_U:    return 3;
    case K_FI_E:    return 4;
    case K_FI_AE:   return 5;
    case K_FI_OE:   return 6;
    case K_FI_Y:    return 7;
    }
    return 0;
}

K_INL void k_vokaali_sointu(unsigned *sointu, kt_merkki m) {
    switch (m) {
    case K_FI_A:
    case K_FI_O:
    case K_FI_U:
        *sointu = 0;
        return;
    case K_FI_AE:
    case K_FI_OE:
    case K_FI_Y:
        *sointu = 1;
        return;
    }
}

K_INL kt_merkki k_vokaali_lue(const char *c) {
    if (!strncmp(c, "Ä", sizeof("Ä") - 1)) return K_FI_AE;
    if (!strncmp(c, "ä", sizeof("ä") - 1)) return K_FI_AE;
    if (!strncmp(c, "Ö", sizeof("Ö") - 1)) return K_FI_OE;
    if (!strncmp(c, "ö", sizeof("ö") - 1)) return K_FI_OE;

    switch (*c) {
    case 'A': case 'a': return K_FI_A;
    case 'E': case 'e': return K_FI_E;
    case 'I': case 'i': return K_FI_I;
    case 'O': case 'o': return K_FI_O;
    case 'U': case 'u': return K_FI_U;
    case 'Y': case 'y': return K_FI_Y;
    }
    return K_FI_NA;
}

static int k_astearkv(const char *p0, const char **pp, unsigned aste) {
    const char *p = *pp;

    switch (aste) {
    case 0:
        return 1;
    case 1:
        if (K_INB(p, p0) != K_FI_K) return 0;
    case 4:
    case 12:
    case 13:
        if (K_INB(p, p0) != K_FI_K) return 0;
        break;
    case 2:
        if (K_INB(p, p0) != K_FI_P) return 0;
    case 5:
        if (K_INB(p, p0) != K_FI_P) return 0;
        break;
    case 3:
        if (K_INB(p, p0) != K_FI_T) return 0;
    case 6:
        if (K_INB(p, p0) != K_FI_T) return 0;
        break;
    case 7:
        if (K_INB(p, p0) != K_FI_K) return 0;
        if (K_INB(p, p0) != K_FI_N) return 0;
        break;
    case 8:
        if (K_INB(p, p0) != K_FI_P) return 0;
        if (K_INB(p, p0) != K_FI_M) return 0;
        break;
    case 9:
        if (K_INB(p, p0) != K_FI_T) return 0;
        if (K_INB(p, p0) != K_FI_L) return 0;
        break;
    case 10:
        if (K_INB(p, p0) != K_FI_T) return 0;
        if (K_INB(p, p0) != K_FI_N) return 0;
        break;
    case 11:
        if (K_INB(p, p0) != K_FI_T) return 0;
        if (K_INB(p, p0) != K_FI_R) return 0;
        break;
#if KOSKUT_GRADE_VOICED_PLOSIVES
    case 28:
        if (K_INB(p, p0) != K_FI_G) return 0;
        if (K_INB(p, p0) != K_FI_G) return 0;
        break;
    case 29:
        if (K_INB(p, p0) != K_FI_B) return 0;
        if (K_INB(p, p0) != K_FI_B) return 0;
        break;
    case 30:
        if (K_INB(p, p0) != K_FI_D) return 0;
        if (K_INB(p, p0) != K_FI_D) return 0;
        break;
#endif
    default:
        return 0;
    }

    *pp = p;
    return 1;
}

static int k_astearkh(const char *p0, const char **pp, unsigned aste) {
    const char *p = *pp;

    switch (aste) {
    case 0:
        return 1;
    case 4:
    {   /* ota heittomerkki jos sellainen on */
        const char *temp = p;
        if (K_INB(temp, p0) == K_FI_AP) p = temp;
    }
        return 0;
    case 1:
        if (K_INB(p, p0) != K_FI_K) return 0;
        break;
    case 2:
        if (K_INB(p, p0) != K_FI_P) return 0;
        break;
    case 3:
        if (K_INB(p, p0) != K_FI_T) return 0;
        break;
    case 5:
    case 13:
        if (K_INB(p, p0) != K_FI_V) return 0;
        break;
    case 6:
        if (K_INB(p, p0) != K_FI_D) return 0;
        break;
    case 7:
        if (K_INB(p, p0) != K_FI_G) return 0;
        if (K_INB(p, p0) != K_FI_N) return 0;
        break;
    case 8:
        if (K_INB(p, p0) != K_FI_M) return 0;
        if (K_INB(p, p0) != K_FI_M) return 0;
        break;
    case 9:
        if (K_INB(p, p0) != K_FI_L) return 0;
        if (K_INB(p, p0) != K_FI_L) return 0;
        break;
    case 10:
        if (K_INB(p, p0) != K_FI_N) return 0;
        if (K_INB(p, p0) != K_FI_N) return 0;
        break;
    case 11:
        if (K_INB(p, p0) != K_FI_R) return 0;
        if (K_INB(p, p0) != K_FI_R) return 0;
        break;
    case 12:
        if (K_INB(p, p0) != K_FI_J) return 0;
        break;
#if KOSKUT_GRADE_VOICED_PLOSIVES
    case 28:
        if (K_INB(p, p0) != K_FI_G) return 0;
        break;
    case 29:
        if (K_INB(p, p0) != K_FI_B) return 0;
        break;
    case 30:
        if (K_INB(p, p0) != K_FI_D) return 0;
        break;
#endif
    default:
        return 0;
    }

    *pp = p;
    return 1;
}

/* 0, jos kelvoton. */
kt_nomini k_nomini_luo(const char *sana, unsigned luokka,
                        unsigned aste, unsigned sointu, unsigned vaihto) {
    kt_nomini tulos = { 0, 0 };
    const char *loppu = strchr(sana, '\0');
    const char *poikki = loppu;
    kt_merkki merkki;
    unsigned vokaali;

    kt_nomtaiv taiv = 0;

    printf("luokka=%u aste=%u sointu=%u vaihto=%u\n",
            luokka, aste, sointu, vaihto);

    switch (luokka) {
    case 1: /* valo */
        merkki = K_INB(poikki, sana);
        if (merkki != K_FI_O && merkki != K_FI_U
         && merkki != K_FI_Y && merkki != K_FI_OE) goto virhe;
        k_vokaali_sointu(&sointu, merkki);

        if (!k_astearkv(sana, &poikki, aste)) goto virhe;
        vokaali = k_vokaali_pakkaa(merkki);
        if (aste == 4) {
            /* tarvitaanko heittomerkkiä? liuku > liu’un */
            const char *temp = poikki;
            kt_merkki merkki2 = K_INB(temp, sana);
            if (merkki == merkki2) taiv |= 0x4000U; /* heittomerkki */
        }
        break;

    case 2: /* palvelu */
        merkki = K_INB(poikki, sana);
        if (merkki != K_FI_O && merkki != K_FI_U
         && merkki != K_FI_Y && merkki != K_FI_OE) goto virhe;
        k_vokaali_sointu(&sointu, merkki);
        vokaali = k_vokaali_pakkaa(merkki);
        aste = 0;
        break;

    case 3: /* valtio */
        merkki = K_INB(poikki, sana);
        if (merkki != K_FI_O && merkki != K_FI_OE
         && merkki != K_FI_E) goto virhe;
        k_vokaali_sointu(&sointu, merkki);
        vokaali = k_vokaali_pakkaa(merkki);
        aste = 0;
        break;

    case 4: /* laatikko */
        merkki = K_INB(poikki, sana);
        if (merkki != K_FI_O && merkki != K_FI_U
         && merkki != K_FI_Y && merkki != K_FI_OE) goto virhe;
        if (aste != 1 && aste != 2 && aste != 3) goto virhe;
        k_vokaali_sointu(&sointu, merkki);

        if (!k_astearkv(sana, &poikki, aste)) goto virhe;
        vokaali = k_vokaali_pakkaa(merkki);
        break;

    case 5: /* risti */
        {
            const char *temp = poikki;
            merkki = K_INB(temp, sana);
            if (merkki == K_FI_I)
                poikki = temp;
            else if (aste != 0)     /* ei i:tä. */
                goto virhe;
        }

        if (!k_astearkv(sana, &poikki, aste)) goto virhe;
        vokaali = k_vokaali_pakkaa(K_FI_I);
        break;

    case 6: /* paperi */
        {
            const char *temp = poikki;
            merkki = K_INB(temp, sana);
            if (merkki == K_FI_I)
                poikki = temp;
        }

        vokaali = k_vokaali_pakkaa(K_FI_I);
        aste = 0;
        break;

    case 24: /* uni */
    case 26: /* pieni */
        aste = vaihto ? 31 : 0; /* meri, merta, meressä */
        if (K_INB(poikki, sana) != K_FI_I) goto virhe;
        vokaali = k_vokaali_pakkaa(K_FI_E);
        break;

    case 23: /* tiili */
        aste = 0;
    case 7: /* ovi */
        if (K_INB(poikki, sana) != K_FI_I) goto virhe;
        if (!k_astearkv(sana, &poikki, aste)) goto virhe;
        vokaali = k_vokaali_pakkaa(K_FI_E);
        break;

    case 8: /* nalle */
        if (K_INB(poikki, sana) != K_FI_E) goto virhe;
        if (!k_astearkv(sana, &poikki, aste)) goto virhe;
        vokaali = k_vokaali_pakkaa(K_FI_E);
        break;

    case 9: /* kala */
        merkki = K_INB(poikki, sana);
        if (merkki != K_FI_A && merkki != K_FI_AE) goto virhe;
        if (!k_astearkv(sana, &poikki, aste)) goto virhe;
        k_vokaali_sointu(&sointu, merkki);
        vokaali = k_vokaali_pakkaa(merkki);

        if (aste == 4) {
            /* tarvitaanko heittomerkkiä? vaaka > vaa’an */
            const char *temp = poikki;
            kt_merkki merkki2 = K_INB(temp, sana);
            if (merkki2 == K_FI_I)
                vokaali = k_vokaali_pakkaa(merkki2);
            else if (merkki2 == merkki) {
                merkki2 = K_INB(temp, sana);
                if (merkki2 == merkki) {
                    taiv |= 0x4000U; /* heittomerkki */
                }
            }
        }

        break;

    case 10: /* koira */
        merkki = K_INB(poikki, sana);
        if (merkki != K_FI_A && merkki != K_FI_AE) goto virhe;
        if (!k_astearkv(sana, &poikki, aste)) goto virhe;
        k_vokaali_sointu(&sointu, merkki);
        vokaali = k_vokaali_pakkaa(merkki);

        if (aste == 4) {
            /* tarvitaanko heittomerkkiä? vaaka > vaa’an */
            const char *temp = poikki;
            kt_merkki merkki2 = K_INB(temp, sana);
            if (merkki2 == K_FI_I)
                vokaali = k_vokaali_pakkaa(merkki2);
            else if (merkki2 == merkki) {
                merkki2 = K_INB(temp, sana);
                if (merkki2 == merkki) {
                    taiv |= 0x4000U; /* heittomerkki */
                }
            }
        }

        break;

    case 11: /* omena */
    case 12: /* kulkija */
    case 13: /* katiska */
    case 15: /* korkea */
        merkki = K_INB(poikki, sana);
        if (merkki != K_FI_A && merkki != K_FI_AE) goto virhe;
        k_vokaali_sointu(&sointu, merkki);
        vokaali = k_vokaali_pakkaa(merkki);
        aste = 0;
        break;

    case 14: /* solakka */
        merkki = K_INB(poikki, sana);
        if (merkki != K_FI_A && merkki != K_FI_AE) goto virhe;
        if (aste != 1 && aste != 2 && aste != 3) goto virhe;
        if (!k_astearkv(sana, &poikki, aste)) goto virhe;
        k_vokaali_sointu(&sointu, merkki);
        vokaali = k_vokaali_pakkaa(merkki);
        break;

    case 16: /* vanhempi */
        if (K_INB(poikki, sana) != K_FI_I) goto virhe;
        aste = 8; /* mp:mm */
        if (!k_astearkv(sana, &poikki, aste)) goto virhe;
        vokaali = k_vokaali_pakkaa(sointu ? K_FI_AE : K_FI_A);
        break;

    case 17: /* vapaa */
        merkki = K_INB(poikki, sana);
        if (merkki != K_FI_A && merkki != K_FI_O && merkki != K_FI_U
         && merkki != K_FI_E && merkki != K_FI_Y) goto virhe;
        k_vokaali_sointu(&sointu, merkki);
        vokaali = k_vokaali_pakkaa(merkki);
        aste = 0;
        break;

    case 18: /* maa */
    case 20: /* filee */
        merkki = K_INB(poikki, sana);
        if (!k_vokaali_onko(merkki)) goto virhe;
        k_vokaali_sointu(&sointu, merkki);
        vokaali = k_vokaali_pakkaa(merkki);
        aste = 0;
        break;

    case 19: /* suo */
        merkki = K_INB(poikki, sana);
        if (!k_vokaali_onko(merkki)) goto virhe;
        k_vokaali_sointu(&sointu, merkki);
        vokaali = k_vokaali_pakkaa(merkki);
        merkki = K_INB(poikki, sana);
        if (!k_vokaali_onko(merkki)) goto virhe;
        aste = 0;
        break;

    case 22: /* parfait */
        taiv |= 0x4000U; /* heittomerkki */

    case 21: /* rosé */
    case 62: /* :koira */
    case 63: /* :maa */
        vokaali = vaihto;
        aste = 0;
        break;

    case 25: /* toimi */
        if (K_INB(poikki, sana) != K_FI_I) goto virhe;
        if (K_INB(poikki, sana) != K_FI_M) goto virhe;
        vokaali = k_vokaali_pakkaa(K_FI_E);
        aste = 0;
        break;

    case 27: /* käsi */
        aste = 6; /* t:d */
        if (K_INB(poikki, sana) != K_FI_I) goto virhe;
        if (K_INB(poikki, sana) != K_FI_S) goto virhe;
        vokaali = k_vokaali_pakkaa(K_FI_E);
        break;

    case 28: /* kynsi */
        if (K_INB(poikki, sana) != K_FI_I) goto virhe;
        if (K_INB(poikki, sana) != K_FI_S) goto virhe;
        merkki = K_INB(poikki, sana);
        if (merkki == K_FI_L)
            aste = 9;  /* lt:ll */
        else if (merkki == K_FI_N)
            aste = 10; /* nt:nn */
        else if (merkki == K_FI_R)
            aste = 11; /* rt:rr */
        else
            goto virhe;
        vokaali = k_vokaali_pakkaa(K_FI_E);
        break;

    case 29: /* lapsi */
        if (K_INB(poikki, sana) != K_FI_I) goto virhe;
        if (K_INB(poikki, sana) != K_FI_S) goto virhe;
        if (K_INB(poikki, sana) != K_FI_P) goto virhe;
        vokaali = k_vokaali_pakkaa(K_FI_E);
        aste = 0;
        break;

    case 30: /* veitsi */
        if (K_INB(poikki, sana) != K_FI_I) goto virhe;
        if (K_INB(poikki, sana) != K_FI_S) goto virhe;
        if (K_INB(poikki, sana) != K_FI_T) goto virhe;
        vokaali = k_vokaali_pakkaa(K_FI_E);
        aste = 0;
        break;

    case 31: /* kaksi */
        if (K_INB(poikki, sana) != K_FI_I) goto virhe;
        if (K_INB(poikki, sana) != K_FI_S) goto virhe;
        if (K_INB(poikki, sana) != K_FI_K) goto virhe;
        vokaali = k_vokaali_pakkaa(K_FI_E);
        aste = 0;
        break;

    case 32: /* sisar */
        merkki = K_INB(poikki, sana);
        if (merkki == K_FI_L)
            vokaali = 0;
        else if (merkki == K_FI_N)
            vokaali = 1;
        else if (merkki == K_FI_R)
            vokaali = 2;
        else
            goto virhe;
        merkki = K_INB(poikki, sana);
        if (merkki != K_FI_A && merkki != K_FI_AE
                             && merkki != K_FI_E) goto virhe;
        if (!k_astearkh(sana, &poikki, aste)) goto virhe;
        k_vokaali_sointu(&sointu, merkki);
        vokaali |= merkki == K_FI_E ? 4 : 0;
        break;

    case 34: /* onneton */
        aste = 3; /* tt:t */
    case 33: /* kytkin */
    case 35: /* lämmin */
        if (K_INB(poikki, sana) != K_FI_N) goto virhe;
        merkki = K_INB(poikki, sana);
        if (!k_vokaali_onko(merkki)) goto virhe;
        if (!k_astearkh(sana, &poikki, aste)) goto virhe;
        k_vokaali_sointu(&sointu, merkki);
        vokaali = k_vokaali_pakkaa(merkki);
        break;

    case 36: /* sisin */
    case 37: /* vasen */
        aste = 8; /* mp:mm */
        if (K_INB(poikki, sana) != K_FI_N) goto virhe;
        vokaali = k_vokaali_pakkaa(K_FI_I);
        break;

    case 38: /* nainen */
        if (K_INB(poikki, sana) != K_FI_N) goto virhe;
        if (K_INB(poikki, sana) != K_FI_E) goto virhe;
        if (K_INB(poikki, sana) != K_FI_N) goto virhe;
        vokaali = k_vokaali_pakkaa(K_FI_E);
        aste = 0;
        break;

    case 39: /* vastaus */
        merkki = K_INB(poikki, sana);
        if (merkki != K_FI_S) goto virhe;
        vokaali = k_vokaali_pakkaa(K_FI_E);
        aste = 0;
        break;

    case 40: /* kalleus */
        merkki = K_INB(poikki, sana);
        if (merkki != K_FI_S) goto virhe;
        vokaali = k_vokaali_pakkaa(K_FI_E);
        aste = 6; /* t:d */
        break;

    case 41: /* vieras */
        if (K_INB(poikki, sana) != K_FI_S) goto virhe;
        merkki = K_INB(poikki, sana);
        if (!k_vokaali_onko(merkki)) goto virhe;
        if (!k_astearkh(sana, &poikki, aste)) goto virhe;
        k_vokaali_sointu(&sointu, merkki);
        vokaali = k_vokaali_pakkaa(merkki);
        break;

    case 42: /* mies */
        if (K_INB(poikki, sana) != K_FI_S) goto virhe;
        vokaali = k_vokaali_pakkaa(K_FI_E);
        aste = 0;
        break;

    case 43: /* ohut */
        if (K_INB(poikki, sana) != K_FI_T) goto virhe;
        merkki = K_INB(poikki, sana);
        if (merkki != K_FI_U && merkki != K_FI_Y) goto virhe;
        if (!k_astearkh(sana, &poikki, aste)) goto virhe;
        k_vokaali_sointu(&sointu, merkki);
        vokaali = k_vokaali_pakkaa(merkki);
        break;

    case 44: /* kevät */
        if (K_INB(poikki, sana) != K_FI_T) goto virhe;
        merkki = K_INB(poikki, sana);
        k_vokaali_sointu(&sointu, merkki);
        vokaali = k_vokaali_pakkaa(merkki);
        aste = 0;
        break;

    case 45: /* kahdeksas */
        if (K_INB(poikki, sana) != K_FI_S) goto virhe;
        vokaali = k_vokaali_pakkaa(K_FI_E);
        aste = 10; /* nt:nn */
        break;

    case 46: /* tuhat */
        if (K_INB(poikki, sana) != K_FI_T) goto virhe;
        vokaali = k_vokaali_pakkaa(sointu ? K_FI_AE : K_FI_A);
        aste = 10; /* nt:nn */
        break;

    case 47: /* kuollut */
        if (K_INB(poikki, sana) != K_FI_T) goto virhe;
        merkki = K_INB(poikki, sana);
        if (merkki != K_FI_U && merkki != K_FI_Y) goto virhe;
        k_vokaali_sointu(&sointu, merkki);
        vokaali = k_vokaali_pakkaa(merkki);
        aste = 0;
        break;

    case 48: /* hame */
        merkki = K_INB(poikki, sana);
        if (merkki != K_FI_E && merkki != K_FI_I) goto virhe;
        if (!k_astearkh(sana, &poikki, aste)) goto virhe;
        vokaali = k_vokaali_pakkaa(merkki);
        break;

    case 49: /* hame */
        merkki = K_INB(poikki, sana);
        if (merkki == K_FI_L)
            vokaali = 0;
        else if (merkki == K_FI_N)
            vokaali = 1;
        else if (merkki == K_FI_R)
            vokaali = 2;
        else
            goto virhe;
        merkki = K_INB(poikki, sana);
        if (merkki != K_FI_A && merkki != K_FI_AE
                             && merkki != K_FI_E) goto virhe;
        if (!k_astearkh(sana, &poikki, aste)) goto virhe;
        k_vokaali_sointu(&sointu, merkki);
        vokaali |= merkki == K_FI_E ? 4 : 0;
        break;

    default:
        goto virhe;
    }

    taiv |= (luokka << 8) | (aste << 3) | vokaali | (sointu ? 0x8000U : 0);

    tulos.leikkaa = loppu - poikki;
    tulos.taiv = taiv;

    printf("taiv=0x%04X\n", taiv);

virhe:
    return tulos;
}

static void nolla_loppuun(char *puskuri, size_t koko, size_t pituus) {
    if (pituus > --koko) pituus = koko;
    puskuri[pituus] = 0;
}

static void taivuta_ja_tulosta(const char *sana, kt_nomini vart,
                               unsigned muoto, const char *muoto_nimi) {
    unsigned long pituus = strlen(sana);
    unsigned long n;
    n = k_ntaiv_taivuta(vart, muoto, sana, pituus,
                        puskuri, sizeof(puskuri) - 1);
    nolla_loppuun(puskuri, sizeof(puskuri), n);

    printf("%-30s", muoto_nimi);
    puts(puskuri);
}

int main(int argc, char *argv[]) {
    kt_nomini nomvart;
    char c[16], *p;

    puts("Syötä sana perusmuodossa (yksikön nominatiivi).");
    printf("Sana? ");
    fflush(stdout);
    if (!fgets(sana, sizeof(sana), stdin))
        return 1;
    p = strchr(sana, '\n');
    if (p) *p = 0;

    puts("Taivutusluokat ja astevaihtelut noudattavat Kielitoimiston");
    puts("sanakirjan mukaista merkintätapaa.");

    printf("Taivutusluokka (1-49)? ");
    fflush(stdout);
    if (!fgets(c, sizeof(c), stdin))
        return 1;
    luokka = atoi(c);

    printf("Astevaihtelu (tyhjä, A-M)? ");
    fflush(stdout);
    if (!fgets(c, sizeof(c), stdin))
        return 1;
    switch (*c) {
    case 'A': case 'a': aste =  1; break;
    case 'B': case 'b': aste =  2; break;
    case 'C': case 'c': aste =  3; break;
    case 'D': case 'd': aste =  4; break;
    case 'E': case 'e': aste =  5; break;
    case 'F': case 'f': aste =  6; break;
    case 'G': case 'g': aste =  7; break;
    case 'H': case 'h': aste =  8; break;
    case 'I': case 'i': aste =  9; break;
    case 'J': case 'j': aste = 10; break;
    case 'K': case 'k': aste = 11; break;
    case 'L': case 'l': aste = 12; break;
    case 'M': case 'm': aste = 13; break;
    default:            aste =  0;
    }

    printf("Vokaalisointu ([AOU]/[ÄÖY])? ");
    fflush(stdout);
    if (!fgets(c, sizeof(c), stdin))
        return 1;
    {
        kt_merkki m = k_vokaali_lue(c);
        sointu = 1;
        if (k_vokaali_onko(m))
            k_vokaali_sointu(&sointu, m);
    }

    switch (luokka) {
    case 21:
    case 22:
    case 62:
    case 63:
        printf("Loppuvokaali ([AEIOUYÄÖ])? ");
        fflush(stdout);
        if (!fgets(c, sizeof(c), stdin))
            return 1;
        else {
            kt_merkki m = k_vokaali_lue(c);
            if (k_vokaali_onko(m))
                vaihto = k_vokaali_pakkaa(m);
            else
                vaihto = K_FI_E;
        }
        break;
    case 24:
    case 26:
        if (sointu) {
            printf("Loppuuko partitiivin yksikkö A:han (merta) [A/-]? ");
            fflush(stdout);
            if (!fgets(c, sizeof(c), stdin))
                return 1;
            else
                vaihto = *c == 'A';
        }
        break;
    }

    nomvart = k_nomini_luo(sana, luokka, aste, sointu, vaihto);
    if (!k_ntaiv_ok(nomvart)) {
        puts("Taivutusta ei tueta. Jokin tiedoissa on väärin.");
        return 1;
    }

    taivuta_ja_tulosta(sana, nomvart, K_NTAIV_YNOM, "yks. nominatiivi");
    taivuta_ja_tulosta(sana, nomvart, K_NTAIV_YGEN, "yks. genetiivi");
    taivuta_ja_tulosta(sana, nomvart, K_NTAIV_YPAR, "yks. partitiivi");
    taivuta_ja_tulosta(sana, nomvart, K_NTAIV_YINE, "yks. inessiivi");
    taivuta_ja_tulosta(sana, nomvart, K_NTAIV_YELA, "yks. elatiivi");
    taivuta_ja_tulosta(sana, nomvart, K_NTAIV_YILL, "yks. illatiivi");
    taivuta_ja_tulosta(sana, nomvart, K_NTAIV_YADE, "yks. adessiivi");
    taivuta_ja_tulosta(sana, nomvart, K_NTAIV_YABL, "yks. ablatiivi");
    taivuta_ja_tulosta(sana, nomvart, K_NTAIV_YALL, "yks. allatiivi");
    taivuta_ja_tulosta(sana, nomvart, K_NTAIV_YESS, "yks. essiivi");
    taivuta_ja_tulosta(sana, nomvart, K_NTAIV_YTRA, "yks. translatiivi");
    taivuta_ja_tulosta(sana, nomvart, K_NTAIV_YABE, "yks. abessiivi");

#if KOSKUT_PLURALS
    putchar('\n');

    taivuta_ja_tulosta(sana, nomvart, K_NTAIV_MNOM, "mon. nominatiivi");
    taivuta_ja_tulosta(sana, nomvart, K_NTAIV_MGEN, "mon. genetiivi");
    taivuta_ja_tulosta(sana, nomvart, K_NTAIV_MPAR, "mon. partitiivi");
    taivuta_ja_tulosta(sana, nomvart, K_NTAIV_MINE, "mon. inessiivi");
    taivuta_ja_tulosta(sana, nomvart, K_NTAIV_MELA, "mon. elatiivi");
    taivuta_ja_tulosta(sana, nomvart, K_NTAIV_MILL, "mon. illatiivi");
    taivuta_ja_tulosta(sana, nomvart, K_NTAIV_MADE, "mon. adessiivi");
    taivuta_ja_tulosta(sana, nomvart, K_NTAIV_MABL, "mon. ablatiivi");
    taivuta_ja_tulosta(sana, nomvart, K_NTAIV_MALL, "mon. allatiivi");
    taivuta_ja_tulosta(sana, nomvart, K_NTAIV_MESS, "mon. essiivi");
    taivuta_ja_tulosta(sana, nomvart, K_NTAIV_MTRA, "mon. translatiivi");
    taivuta_ja_tulosta(sana, nomvart, K_NTAIV_MIST, "mon. instruktiivi");
    taivuta_ja_tulosta(sana, nomvart, K_NTAIV_MABE, "mon. abessiivi");
    taivuta_ja_tulosta(sana, nomvart, K_NTAIV_MCOM, "mon. komitatiivi");
#endif

    return 0;
}
