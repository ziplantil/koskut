/* Koskuen esimerkkiohjelma */

#include "koskut.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char sana[256];
char puskuri[256];

kt_nomveik veikkaus;

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

static const char *k_muodon_nimi(unsigned muoto) {
    switch (muoto) {
        case K_NTAIV_YGEN: return "yks. genetiivi";
        case K_NTAIV_YPAR: return "yks. partitiivi";
        case K_NTAIV_YILL: return "yks. illatiivi";
        case K_NTAIV_MGEN: return "mon. genetiivi";
        case K_NTAIV_MPAR: return "mon. partitiivi";
        case K_NTAIV_MILL: return "mon. illatiivi";
    }
    return NULL;
}

static int on_pieni_kirjain(kt_merkki m) {
    switch (m) {
    case K_FI_A: case K_FI_B: case K_FI_C: case K_FI_D: case K_FI_E:
    case K_FI_F: case K_FI_G: case K_FI_H: case K_FI_I: case K_FI_J:
    case K_FI_K: case K_FI_L: case K_FI_M: case K_FI_N: case K_FI_O:
    case K_FI_P: case K_FI_Q: case K_FI_R: case K_FI_S: case K_FI_T:
    case K_FI_U: case K_FI_V: case K_FI_W: case K_FI_X: case K_FI_Y:
    case K_FI_Z: case K_FI_AE: case K_FI_OE:
        return 1;
    }
    return 0;
}

int main(int argc, char *argv[]) {
    unsigned n;
    char s[8];
    kt_nomini nom;
    unsigned monikot = 1;
    char *alku = sana, *loppu;
    kt_merkki merkki;

    printf("Sana? ");
    fflush(stdout);
    if (!fgets(sana, sizeof(sana), stdin))
        return 1;
    loppu = strchr(sana, '\n');
    if (loppu) *loppu = 0;

    /* jos sana alkaa isolla alkukirjaimella, älä anna monikkomuotoja */
    merkki = K_INF(alku, loppu);
    monikot = on_pieni_kirjain(merkki);

    k_nveik_alusta(&veikkaus, monikot, sana, strlen(sana));
    while ((n = k_nveik_vaihtoehtoja(&veikkaus)) > 1) {
        unsigned i;

        printf("Valitse oikea %s\n", k_muodon_nimi(
                                     k_nveik_arvausmuoto(&veikkaus)));
        for (i = 0; i < n; ++i) {
            kt_koko k;
            k = k_nveik_vaihtoehto(&veikkaus, i, puskuri, sizeof(puskuri) - 1);
            nolla_loppuun(puskuri, sizeof(puskuri), k);
#if DEBUG
            printf("%d. 0x%04x %s\n", i + 1, veikkaus.vaihdot[i].taiv, puskuri);
#else
            printf("%d. %s\n", i + 1, puskuri);
#endif
        }

        i = 0;
        while (!i && i < n) {
            printf("Vaihtoehto? ");
            fflush(stdout);
            if (!fgets(s, sizeof(s), stdin))
                return 1;
            i = atoi(s);
        }

        k_nveik_valitse(&veikkaus, i - 1);
        putchar('\n');
    }

    nom = k_nveik_tulos(&veikkaus);
    if (!k_ntaiv_ok(nom)) {
        puts("Jokin meni pieleen.");
        return 1;
    }

    printf("taiv=0x%04X\n", nom.taiv);

    taivuta_ja_tulosta(sana, nom, K_NTAIV_YNOM, "yks. nominatiivi");
    taivuta_ja_tulosta(sana, nom, K_NTAIV_YGEN, "yks. genetiivi");
    taivuta_ja_tulosta(sana, nom, K_NTAIV_YPAR, "yks. partitiivi");
    taivuta_ja_tulosta(sana, nom, K_NTAIV_YINE, "yks. inessiivi");
    taivuta_ja_tulosta(sana, nom, K_NTAIV_YELA, "yks. elatiivi");
    taivuta_ja_tulosta(sana, nom, K_NTAIV_YILL, "yks. illatiivi");
    taivuta_ja_tulosta(sana, nom, K_NTAIV_YADE, "yks. adessiivi");
    taivuta_ja_tulosta(sana, nom, K_NTAIV_YABL, "yks. ablatiivi");
    taivuta_ja_tulosta(sana, nom, K_NTAIV_YALL, "yks. allatiivi");
    taivuta_ja_tulosta(sana, nom, K_NTAIV_YESS, "yks. essiivi");
    taivuta_ja_tulosta(sana, nom, K_NTAIV_YTRA, "yks. translatiivi");
    taivuta_ja_tulosta(sana, nom, K_NTAIV_YABE, "yks. abessiivi");

#if KOSKUT_PLURALS
    if (monikot) {
        putchar('\n');

        taivuta_ja_tulosta(sana, nom, K_NTAIV_MNOM, "mon. nominatiivi");
        taivuta_ja_tulosta(sana, nom, K_NTAIV_MGEN, "mon. genetiivi");
        taivuta_ja_tulosta(sana, nom, K_NTAIV_MPAR, "mon. partitiivi");
        taivuta_ja_tulosta(sana, nom, K_NTAIV_MINE, "mon. inessiivi");
        taivuta_ja_tulosta(sana, nom, K_NTAIV_MELA, "mon. elatiivi");
        taivuta_ja_tulosta(sana, nom, K_NTAIV_MILL, "mon. illatiivi");
        taivuta_ja_tulosta(sana, nom, K_NTAIV_MADE, "mon. adessiivi");
        taivuta_ja_tulosta(sana, nom, K_NTAIV_MABL, "mon. ablatiivi");
        taivuta_ja_tulosta(sana, nom, K_NTAIV_MALL, "mon. allatiivi");
        taivuta_ja_tulosta(sana, nom, K_NTAIV_MESS, "mon. essiivi");
        taivuta_ja_tulosta(sana, nom, K_NTAIV_MTRA, "mon. translatiivi");
        taivuta_ja_tulosta(sana, nom, K_NTAIV_MABE, "mon. abessiivi");
        taivuta_ja_tulosta(sana, nom, K_NTAIV_MIST, "mon. instruktiivi");
        taivuta_ja_tulosta(sana, nom, K_NTAIV_MCOM, "mon. komitatiivi");
    }
#endif

    return 0;
}
