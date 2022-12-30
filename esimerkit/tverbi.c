/* Koskuen esimerkkiohjelma */

#include "koskut.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Ohjelma olettaa, että tämän tiedoston lähdekoodin merkistö vastaa
   kohdejärjestelmän merkistöä. */

/* KTSK:n taivutusluokka, ks. verbtaiv.txt */
static unsigned luokka;
/* astevaihtelutyyppi, ks. verbtaiv.txt */
static unsigned aste;

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
kt_verbi k_verbi_luo(const char *sana, unsigned luokka, unsigned aste) {
    kt_verbi tulos = { 0, 0 };
    const char *loppu = strchr(sana, '\0');
    const char *poikki = loppu, *haku;
    kt_merkki merkki;
    unsigned vokaali;
    unsigned sointu;
    unsigned konsonantti = 0;
    kt_verbtaiv taiv = 0;

    merkki = K_INB(poikki, sana);
    switch (merkki) {
    case K_FI_A:
        sointu = 0;
        break;
    case K_FI_AE:
        sointu = 1;
        break;
    default:
        goto virhe;
    }

    switch (luokka) {
    case 52: /* sanoa */
        merkki = K_INB(poikki, sana);
        if (merkki != K_FI_O && merkki != K_FI_U
         && merkki != K_FI_Y && merkki != K_FI_OE) goto virhe;

        if (!k_astearkv(sana, &poikki, aste)) goto virhe;
        vokaali = k_vokaali_pakkaa(merkki);
        if (aste == 4) {
            /* tarvitaanko heittomerkkiä? liukua > liu’un */
            const char *temp = poikki;
            kt_merkki merkki2 = K_INB(temp, sana);
            if (merkki == merkki2) konsonantti = 1; /* heittomerkki */
        }
        break;

    case 54: /* huutaa */
    case 55: /* soutaa */
    case 57: /* saartaa */
        if (aste != 6 && aste != 9 && aste != 10 && aste != 11) goto virhe;

    case 53: /* muistaa */
    case 56: /* kaivaa */
        if (K_INB(poikki, sana) != merkki) goto virhe;
        if (!k_astearkv(sana, &poikki, aste)) goto virhe;
        vokaali = k_vokaali_pakkaa(merkki);
        break;

    case 58: /* laskea */
        if (K_INB(poikki, sana) != K_FI_E) goto virhe;
        if (!k_astearkv(sana, &poikki, aste)) goto virhe;
        vokaali = k_vokaali_pakkaa(K_FI_E);
        break;

    case 59: /* tuntea */
        if (K_INB(poikki, sana) != K_FI_E) goto virhe;
        if (K_INB(poikki, sana) != K_FI_T) goto virhe;
        switch (K_INB(poikki, sana)) {
        case K_FI_L:
            aste = 9;
            break;
        case K_FI_N:
            aste = 10;
            break;
        case K_FI_R:
            aste = 11;
            break;
        default:
            goto virhe;
        }
        vokaali = k_vokaali_pakkaa(K_FI_E);
        break;

    case 60: /* lähteä */
        if (K_INB(poikki, sana) != K_FI_E) goto virhe;
        if (K_INB(poikki, sana) != K_FI_T) goto virhe;
        aste = 6;
        vokaali = k_vokaali_pakkaa(K_FI_E);
        break;

    case 61: /* sallia */
        if (K_INB(poikki, sana) != K_FI_I) goto virhe;
        if (!k_astearkv(sana, &poikki, aste)) goto virhe;
        vokaali = k_vokaali_pakkaa(K_FI_I);
        break;

    case 62: /* voida */
        aste = 0;
        if (K_INB(poikki, sana) != K_FI_D) goto virhe;
        haku = poikki;
        if (K_INB(haku, sana) != K_FI_I) goto virhe;
        if (!k_vokaali_onko(K_INB(haku, sana))) goto virhe;
        vokaali = k_vokaali_pakkaa(K_FI_I);
        break;

    case 63: /* saada */
        aste = 0;
        if (K_INB(poikki, sana) != K_FI_D) goto virhe;
        merkki = K_INB(poikki, sana);
        if (!k_vokaali_onko(merkki)) goto virhe;
        vokaali = k_vokaali_pakkaa(merkki);
        break;

    case 64: /* juoda */
        aste = 0;
        if (K_INB(poikki, sana) != K_FI_D) goto virhe;
        merkki = K_INB(poikki, sana);
        if (merkki != K_FI_E && merkki != K_FI_O && merkki != K_FI_OE)
            goto virhe;
        K_INB(poikki, sana);
        vokaali = k_vokaali_pakkaa(merkki);
        break;

    case 65: /* käydä */
        aste = 0;
        if (K_INB(poikki, sana) != K_FI_D) goto virhe;
        merkki = K_INB(poikki, sana);
        if (merkki != K_FI_Y) goto virhe;
        haku = poikki;
        if (K_INB(haku, sana) != K_FI_AE) goto virhe;
        vokaali = k_vokaali_pakkaa(merkki);
        break;

    case 66: /* rohkaista */
        if (K_INB(poikki, sana) != K_FI_T) goto virhe;
        if (K_INB(poikki, sana) != K_FI_S) goto virhe;
        merkki = K_INB(poikki, sana);
        if (!k_vokaali_onko(merkki)) goto virhe;
        if (!k_astearkh(sana, &poikki, aste)) goto virhe;
        vokaali = k_vokaali_pakkaa(merkki);
        break;

    case 67: /* tulla */
        merkki = K_INB(poikki, sana);
        switch (merkki) {
        case K_FI_L:
            konsonantti = 0;
            break;
        case K_FI_N:
            konsonantti = 1;
            break;
        case K_FI_R:
            konsonantti = 2;
            break;
        default:
            goto virhe;
        }
        if (K_INB(poikki, sana) != merkki) goto virhe;
        merkki = K_INB(poikki, sana);
        if (!k_vokaali_onko(merkki)) goto virhe;
        haku = poikki;
        if (!k_astearkh(sana, &poikki, aste)) goto virhe;
        vokaali = k_vokaali_pakkaa(merkki);
        break;

    case 68: /* tupakoida */
        aste = 0;
        if (K_INB(poikki, sana) != K_FI_D) goto virhe;
        vokaali = k_vokaali_pakkaa(K_FI_E);
        break;

    case 69: /* valita */
        aste = 0;
        if (K_INB(poikki, sana) != K_FI_T) goto virhe;
        vokaali = k_vokaali_pakkaa(K_FI_E);
        break;

    case 70: /* juosta */
        aste = 0;
        if (K_INB(poikki, sana) != K_FI_T) goto virhe;
        if (K_INB(poikki, sana) != K_FI_S) goto virhe;
        vokaali = k_vokaali_pakkaa(K_FI_E);
        break;

    case 71: /* nähdä */
        aste = 4;
        if (K_INB(poikki, sana) != K_FI_D) goto virhe;
        if (K_INB(poikki, sana) != K_FI_H) goto virhe;
        vokaali = k_vokaali_pakkaa(K_FI_E);
        break;

    case 72: /* vanheta */
        if (K_INB(poikki, sana) != K_FI_T) goto virhe;
        merkki = K_INB(poikki, sana);
        if (!k_vokaali_onko(merkki)) goto virhe;
        if (!k_astearkh(sana, &poikki, aste)) goto virhe;
        vokaali = k_vokaali_pakkaa(merkki);
        break;

    case 73: /* salata */
        if (K_INB(poikki, sana) != K_FI_T) goto virhe;
        if (K_INB(poikki, sana) != merkki) goto virhe;
        if (!k_astearkh(sana, &poikki, aste)) goto virhe;
        vokaali = k_vokaali_pakkaa(merkki);
        break;

    case 74: /* katketa */
        if (K_INB(poikki, sana) != K_FI_T) goto virhe;
        merkki = K_INB(poikki, sana);
        if (!k_vokaali_onko(merkki)) goto virhe;
        if (!k_astearkh(sana, &poikki, aste)) goto virhe;
        vokaali = k_vokaali_pakkaa(merkki);
        break;

    case 75: /* selvitä */
        if (K_INB(poikki, sana) != K_FI_T) goto virhe;
        merkki = K_INB(poikki, sana);
        if (!k_vokaali_onko(merkki)) goto virhe;
        if (!k_astearkh(sana, &poikki, aste)) goto virhe;
        vokaali = k_vokaali_pakkaa(merkki);
        break;

    case 76: /* taitaa */
        if (K_INB(poikki, sana) != merkki) goto virhe;
        aste = 6;
        if (!k_astearkv(sana, &poikki, aste)) goto virhe;
        vokaali = k_vokaali_pakkaa(merkki);
        break;

    case 77: /* kumata */
    {
        kt_merkki temp = K_INB(poikki, sana);
        if (temp == merkki) {
            if (K_INB(poikki, sana) != K_FI_J) goto virhe;
        } else if (temp != K_FI_T)
            goto virhe;
        vokaali = k_vokaali_pakkaa(merkki);
        break;
    }

    default:
        goto virhe;
    }

    luokka -= 51;
    taiv |= (konsonantti << 13) | (luokka << 8) | (aste << 3)
                                | vokaali | (sointu ? 0x8000U : 0);

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

static void taivuta_ja_tulosta(const char *sana, kt_verbi vart,
                               unsigned muoto, const char *yhteys) {
    unsigned long pituus = strlen(sana);
    unsigned long n;
    n = k_vtaiv_taivuta(vart, muoto, sana, pituus,
                        puskuri, sizeof(puskuri) - 1);
    nolla_loppuun(puskuri, sizeof(puskuri), n);

    printf("    ");
    printf(yhteys, puskuri);
    putchar('\n');
}

int main(int argc, char *argv[]) {
    kt_verbi verbvart;
    char c[16], *p;

    puts("Syötä sana perusmuodossa (1. infinitiivi).");
    printf("Sana? ");
    fflush(stdout);
    if (!fgets(sana, sizeof(sana), stdin))
        return 1;
    p = strchr(sana, '\n');
    if (p) *p = 0;

    puts("Taivutusluokat ja astevaihtelut noudattavat Kielitoimiston");
    puts("sanakirjan mukaista merkintätapaa.");

    printf("Taivutusluokka (52-78)? ");
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

    verbvart = k_verbi_luo(sana, luokka, aste);
    if (!k_vtaiv_ok(verbvart)) {
        puts("Taivutusta ei tueta. Jokin tiedoissa on väärin.");
        return 1;
    }

    puts("");
    puts("Indikatiivin preesens");
    /* yksikön 1. persoonan indikatiivin preesens */
    taivuta_ja_tulosta(sana, verbvart, K_VTAIV_IPY1, "minä %s");
    /* yksikön 2. persoonan indikatiivin preesens */
    taivuta_ja_tulosta(sana, verbvart, K_VTAIV_IPY2, "sinä %s");
    /* yksikön 3. persoonan indikatiivin preesens */
    taivuta_ja_tulosta(sana, verbvart, K_VTAIV_IPY3, "hän %s");
    /* monikon 1. persoonan indikatiivin preesens */
    taivuta_ja_tulosta(sana, verbvart, K_VTAIV_IPM1, "me %s");
    /* monikon 2. persoonan indikatiivin preesens */
    taivuta_ja_tulosta(sana, verbvart, K_VTAIV_IPM2, "te %s");
    /* monikon 3. persoonan indikatiivin preesens */
    taivuta_ja_tulosta(sana, verbvart, K_VTAIV_IPM3, "he %s");
    /* passiivin indikatiivin preesens */
    taivuta_ja_tulosta(sana, verbvart, K_VTAIV_IPPP, "se %s");
    puts("");

    /* yksikön indikatiivin preesensin kieltomuoto */
    taivuta_ja_tulosta(sana, verbvart, K_VTAIV_IPYK, "en %s");
    /* monikon indikatiivin preesensin kieltomuoto */
    taivuta_ja_tulosta(sana, verbvart, K_VTAIV_IPMK, "emme %s");
    /* passiivin indikatiivin preesensin kieltomuoto */
    taivuta_ja_tulosta(sana, verbvart, K_VTAIV_IPPK, "ei %s");

    puts("");
    puts("Indikatiivin imperfekti");
    /* yksikön 1. persoonan indikatiivin imperfekti */
    taivuta_ja_tulosta(sana, verbvart, K_VTAIV_IIY1, "minä %s");
    /* yksikön 2. persoonan indikatiivin imperfekti */
    taivuta_ja_tulosta(sana, verbvart, K_VTAIV_IIY2, "sinä %s");
    /* yksikön 3. persoonan indikatiivin imperfekti */
    taivuta_ja_tulosta(sana, verbvart, K_VTAIV_IIY3, "hän %s");
    /* monikon 1. persoonan indikatiivin imperfekti */
    taivuta_ja_tulosta(sana, verbvart, K_VTAIV_IIM1, "me %s");
    /* monikon 2. persoonan indikatiivin imperfekti */
    taivuta_ja_tulosta(sana, verbvart, K_VTAIV_IIM2, "te %s");
    /* monikon 3. persoonan indikatiivin imperfekti */
    taivuta_ja_tulosta(sana, verbvart, K_VTAIV_IIM3, "he %s");
    /* passiivin indikatiivin imperfekti */
    taivuta_ja_tulosta(sana, verbvart, K_VTAIV_IPIP, "se %s");
    puts("");

    /* yksikön indikatiivin imperfektin kieltomuoto */
    taivuta_ja_tulosta(sana, verbvart, K_VTAIV_IIYK, "en %s");
    /* monikon indikatiivin imperfektin kieltomuoto */
    taivuta_ja_tulosta(sana, verbvart, K_VTAIV_IIMK, "emme %s");
    /* passiivin indikatiivin imperfektin kieltomuoto */
    taivuta_ja_tulosta(sana, verbvart, K_VTAIV_IPIK, "ei %s");

    puts("");
    puts("Konditionaali");
    /* yksikön 1. persoonan konditionaalin preesens */
    taivuta_ja_tulosta(sana, verbvart, K_VTAIV_TKY1, "minä %s");
    /* yksikön 2. persoonan konditionaalin preesens */
    taivuta_ja_tulosta(sana, verbvart, K_VTAIV_TKY2, "sinä %s");
    /* yksikön 3. persoonan konditionaalin preesens */
    taivuta_ja_tulosta(sana, verbvart, K_VTAIV_TKY3, "hän %s");
    /* monikon 1. persoonan konditionaalin preesens */
    taivuta_ja_tulosta(sana, verbvart, K_VTAIV_TKM1, "me %s");
    /* monikon 2. persoonan konditionaalin preesens */
    taivuta_ja_tulosta(sana, verbvart, K_VTAIV_TKM2, "te %s");
    /* monikon 3. persoonan konditionaalin preesens */
    taivuta_ja_tulosta(sana, verbvart, K_VTAIV_TKM3, "he %s");
    /* passiivin konditionaalin preesens */
    taivuta_ja_tulosta(sana, verbvart, K_VTAIV_ITKP, "se %s");
    puts("");

    /* yksikön konditionaalin preesensin kieltomuoto */
    taivuta_ja_tulosta(sana, verbvart, K_VTAIV_TKYK, "en %s");
    /* monikon konditionaalin preesensin kieltomuoto */
    taivuta_ja_tulosta(sana, verbvart, K_VTAIV_TKMK, "emme %s");
    /* passiivin konditionaalin preesensin kieltomuoto */
    taivuta_ja_tulosta(sana, verbvart, K_VTAIV_ITKK, "ei %s");

    puts("");
    puts("Imperatiivi");
    /* yksikön 2. persoonan imperatiivin preesens */
    taivuta_ja_tulosta(sana, verbvart, K_VTAIV_TIY2, "sinä %s!");
    /* yksikön 3. persoonan imperatiivin preesens */
    taivuta_ja_tulosta(sana, verbvart, K_VTAIV_TIY3, "hän %s!");
    /* monikon 1. persoonan imperatiivin preesens */
    taivuta_ja_tulosta(sana, verbvart, K_VTAIV_TIM1, "me %s!");
    /* monikon 2. persoonan imperatiivin preesens */
    taivuta_ja_tulosta(sana, verbvart, K_VTAIV_TIM2, "te %s!");
    /* monikon 3. persoonan imperatiivin preesens */
    taivuta_ja_tulosta(sana, verbvart, K_VTAIV_TIM3, "he %s!");
    /* passiivin imperatiivin preesens */
    taivuta_ja_tulosta(sana, verbvart, K_VTAIV_ITIP, "%s se!");
    puts("");

    /* y. 2. p. imperatiivin preesensin kieltomuoto */
    taivuta_ja_tulosta(sana, verbvart, K_VTAIV_TIYK, "älä %s!");
    /* toinen imperatiivin preesensin kieltomuoto */
    taivuta_ja_tulosta(sana, verbvart, K_VTAIV_TIMK, "älkää %s!");
    /* passiivin imperatiivin preesensin kieltomuoto */
    taivuta_ja_tulosta(sana, verbvart, K_VTAIV_ITIK, "älköön %s!");

    puts("");
    puts("Potentiaali");
    /* yksikön 1. persoonan potentiaalin preesens */
    taivuta_ja_tulosta(sana, verbvart, K_VTAIV_TPY1, "minä %s");
    /* yksikön 2. persoonan potentiaalin preesens */
    taivuta_ja_tulosta(sana, verbvart, K_VTAIV_TPY2, "sinä %s");
    /* yksikön 3. persoonan potentiaalin preesens */
    taivuta_ja_tulosta(sana, verbvart, K_VTAIV_TPY3, "hän %s");
    /* monikon 1. persoonan potentiaalin preesens */
    taivuta_ja_tulosta(sana, verbvart, K_VTAIV_TPM1, "me %s");
    /* monikon 2. persoonan potentiaalin preesens */
    taivuta_ja_tulosta(sana, verbvart, K_VTAIV_TPM2, "te %s");
    /* monikon 3. persoonan potentiaalin preesens */
    taivuta_ja_tulosta(sana, verbvart, K_VTAIV_TPM3, "he %s");
    /* passiivin potentiaalin preesens */
    taivuta_ja_tulosta(sana, verbvart, K_VTAIV_ITPP, "se %s");
    puts("");

    /* yksikön potentiaalin preesensin kieltomuoto */
    taivuta_ja_tulosta(sana, verbvart, K_VTAIV_TPYK, "en %s");
    /* monikon potentiaalin preesensin kieltomuoto */
    taivuta_ja_tulosta(sana, verbvart, K_VTAIV_TPMK, "emme %s");
    /* passiivin potentiaalin preesensin kieltomuoto */
    taivuta_ja_tulosta(sana, verbvart, K_VTAIV_ITPK, "ei %s");

    puts("");
    puts("Infinitiivit");
    /* 1. infinitiivi (A-infinitiivi) */
    taivuta_ja_tulosta(sana, verbvart, K_VTAIV_FN1A, "miten %s");
    /* 1. infinitiivi (A-infinitiivi) + -kse- */
    taivuta_ja_tulosta(sana, verbvart, K_VTAIV_FN1P, "tein %sni");
    /* 2. infinitiivi (E-infinitiivi), akt. iness. */
    taivuta_ja_tulosta(sana, verbvart, K_VTAIV_FN2A, "minun %sni");
    /* 2. infinitiivi (E-infinitiivi), pass. iness. */
    taivuta_ja_tulosta(sana, verbvart, K_VTAIV_FN2P, "sitä %s");
    /* 2. infinitiivi (E-infinitiivi), akt. instr. */
    taivuta_ja_tulosta(sana, verbvart, K_VTAIV_FN2I, "sen %s");
    /* 3. infinitiivi (MA-infinitiivi), akt. iness. */
    taivuta_ja_tulosta(sana, verbvart, K_VTAIV_FN3I, "olin %s");
    /* 3. infinitiivi (MA-infinitiivi), akt. elat. */
    taivuta_ja_tulosta(sana, verbvart, K_VTAIV_FN3E, "tulin %s");
    /* 3. infinitiivi (MA-infinitiivi), akt. illat. */
    taivuta_ja_tulosta(sana, verbvart, K_VTAIV_FN3L, "menen %s");
    /* 3. infinitiivi (MA-infinitiivi), akt. adess. */
    taivuta_ja_tulosta(sana, verbvart, K_VTAIV_FN3A, "miten? %s!");
    /* 3. infinitiivi (MA-infinitiivi), akt. abess. */
    taivuta_ja_tulosta(sana, verbvart, K_VTAIV_FN3B, "%s");
    /* 3. infinitiivi (MA-infinitiivi), akt. instr. */
    taivuta_ja_tulosta(sana, verbvart, K_VTAIV_FN3N, "%s");
    /* 3. infinitiivi (MA-infinitiivi), pass. instr. */
    taivuta_ja_tulosta(sana, verbvart, K_VTAIV_FN3P, "%s");
    /* 4. infinitiivi (MINEN-infinitiivi), vartalo */
    taivuta_ja_tulosta(sana, verbvart, K_VTAIV_FN4V, "%snen");
    /* 5. infinitiivi (MAISILLA-infinitiivi), vartalo */
    taivuta_ja_tulosta(sana, verbvart, K_VTAIV_FN5V, "olin %sni");

    puts("");
    puts("Partisiipit");
    /* aktiivin 1. partisiippi (VA) */
    taivuta_ja_tulosta(sana, verbvart, K_VTAIV_PA1P, "%s");
    /* aktiivin 2. partisiippi (NUT) */
    taivuta_ja_tulosta(sana, verbvart, K_VTAIV_PA2P, "%s");
    /* passiivin 1. partisiippi (TAVA) */
    taivuta_ja_tulosta(sana, verbvart, K_VTAIV_PP1P, "%s");
    /* passiivin 2. partisiippi (TU) */
    taivuta_ja_tulosta(sana, verbvart, K_VTAIV_PP2P, "%s");
    /* agenttipartisiippi (MA) */
    taivuta_ja_tulosta(sana, verbvart, K_VTAIV_PPAG, "%s");
    /* kieltopartisiippi (MATON) */
    taivuta_ja_tulosta(sana, verbvart, K_VTAIV_PPKI, "%s");

    return 0;
}
