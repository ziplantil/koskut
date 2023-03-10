/*******************************************************************************
            Koskut -- pieni suomen kielen taivutuskirjasto
            tehnyt ziplantil 2022 -- lisenssi: MIT
            versio: 2022-12-31
            <https://github.com/ziplantil/koskut>
*******************************************************************************/
/* k_nveik.c - nominiveikkain                                                 */

#include "koskut.h"
#include "koskutio.h"

static unsigned onko_vokaali(kt_merkki merkki) {
    switch (merkki) {
    case K_FI_A:
    case K_FI_O:
    case K_FI_U:
    case K_FI_AE:
    case K_FI_OE:
    case K_FI_Y:
    case K_FI_E:
    case K_FI_I:
        return 1;
    }
    return 0;
}

K_INL int edessa_vokaali(const char *sana, const char *haku) {
    return onko_vokaali(K_INB(haku, sana));
}

K_INL int edessa_vokaali_tai_h(const char *sana, const char *haku) {
    kt_merkki m = K_INB(haku, sana);
    return onko_vokaali(m) || m == K_FI_H;
}

K_INL int edessa_vokaali_likvida_nasaali(const char *sana, const char *haku,
                                         int p) {
    kt_merkki m = K_INB(haku, sana);
    return onko_vokaali(m) || m == K_FI_L || m == K_FI_R ||
                              m == (p ? K_FI_M : K_FI_N);
}

static unsigned lue_vahva_aste(const char *sana, const char **p_haku,
                               kt_merkki loppuvokaali) {
    const char *haku2 = *p_haku;
    const char *haku1;
    kt_merkki merkki1, merkki2;
    merkki2 = K_INB(haku2, sana);
    haku1 = haku2;

    switch (merkki2) {
    case K_FI_K:
        merkki1 = K_INB(haku1, sana);
        switch (merkki1) {
        case K_FI_K:
            if (!edessa_vokaali_likvida_nasaali(sana, haku1, 0)) break;
            *p_haku = haku1;
            return 1; /* kk:k */
        case K_FI_N:
            if (!edessa_vokaali(sana, haku1)) break;
            *p_haku = haku1;
            return 7; /* nk:ng */
        case K_FI_L:
        case K_FI_R:
            if (!edessa_vokaali(sana, haku1)) break;
            *p_haku = haku2;
            if (loppuvokaali == K_FI_E) {
                return 12; /* k:j (tai k:') */
            }
            return 4; /* k:' */
        case K_FI_U:
        case K_FI_Y:
            *p_haku = haku2;
            if (loppuvokaali == merkki1) {
                return 13; /* k:v (tai k:') */
            }
            return 4; /* k:' */
        case K_FI_H:
            if (loppuvokaali == K_FI_E) {
                return 12; /* k:j (tai k:') */
            }
        case K_FI_I:
        case K_FI_A:
        case K_FI_E:
        case K_FI_O:
        case K_FI_AE:
        case K_FI_OE:
            *p_haku = haku2;
            return 4; /* k:' */
        }
        break;
    case K_FI_P:
        merkki1 = K_INB(haku1, sana);
        switch (merkki1) {
        case K_FI_P:
            if (!edessa_vokaali_likvida_nasaali(sana, haku1, 1)) break;
            *p_haku = haku1;
            return 2; /* pp:p */
        case K_FI_M:
            if (!edessa_vokaali(sana, haku1)) break;
            *p_haku = haku1;
            return 8; /* mp:mm */
        case K_FI_A:
        case K_FI_E:
        case K_FI_I:
        case K_FI_O:
        case K_FI_U:
        case K_FI_Y:
        case K_FI_AE:
        case K_FI_OE:
            *p_haku = haku2;
            return 5; /* p:v */
        }
        break;
    case K_FI_T:
        merkki1 = K_INB(haku1, sana);
        switch (merkki1) {
        case K_FI_T:
            if (!edessa_vokaali_likvida_nasaali(sana, haku1, 0)) break;
            *p_haku = haku1;
            return 3; /* tt:t */
        case K_FI_L:
            if (!edessa_vokaali(sana, haku1)) break;
            *p_haku = haku1;
            return 9; /* lt:ll */
        case K_FI_N:
            if (!edessa_vokaali(sana, haku1)) break;
            *p_haku = haku1;
            return 10; /* nt:nn */
        case K_FI_R:
            if (!edessa_vokaali(sana, haku1)) break;
            *p_haku = haku1;
            return 11; /* rt:rr */
        case K_FI_A:
        case K_FI_E:
        case K_FI_I:
        case K_FI_O:
        case K_FI_U:
        case K_FI_Y:
        case K_FI_H:
        case K_FI_AE:
        case K_FI_OE:
            *p_haku = haku2;
            return 6; /* t:d */
        }
    }

    return 0;
}

static unsigned lue_heikko_aste(const char *sana, const char **p_haku,
                                kt_merkki loppuvokaali) {
    const char *haku2 = *p_haku;
    const char *haku1;
    kt_merkki merkki1, merkki2;
    merkki2 = K_INB(haku2, sana);
    haku1 = haku2;

    switch (merkki2) {
    case K_FI_K:
        if (!edessa_vokaali_likvida_nasaali(sana, haku2, 0)) break;
        *p_haku = haku2;
        return 1; /* kk:k */
    case K_FI_P:
        if (!edessa_vokaali_likvida_nasaali(sana, haku2, 1)) break;
        *p_haku = haku2;
        return 2; /* pp:p */
    case K_FI_T:
        if (!edessa_vokaali_likvida_nasaali(sana, haku2, 0)) break;
        *p_haku = haku2;
        return 3; /* tt:t */
    case K_FI_AP:
        if (!edessa_vokaali(sana, haku2)) break;
        *p_haku = haku2;
        return 4; /* k:' */
    case K_FI_V:
        if (!edessa_vokaali(sana, haku2)) break;
        *p_haku = haku2;
        return 5; /* p:v */
    case K_FI_D:
        if (!edessa_vokaali_tai_h(sana, haku2)) break;
        *p_haku = haku2;
        return 6; /* t:d */
    case K_FI_J:
        if (loppuvokaali == K_FI_E) {
            merkki1 = K_INB(haku1, sana);
            if (merkki1 == K_FI_H || merkki1 == K_FI_L || merkki1 == K_FI_R) {
                *p_haku = haku2;
                return 12; /* k:j */
            }
        }
        break;

    case K_FI_G:
        merkki1 = K_INB(haku1, sana);
        if (merkki1 == K_FI_N) {
            if (!edessa_vokaali(sana, haku1)) break;
            *p_haku = haku1;
            return 7; /* nk:ng */
        }
        break;
    case K_FI_M:
        merkki1 = K_INB(haku1, sana);
        if (merkki1 == K_FI_M) {
            if (!edessa_vokaali(sana, haku1)) break;
            *p_haku = haku1;
            return 8; /* mp:mm */
        }
        break;
    case K_FI_L:
        merkki1 = K_INB(haku1, sana);
        if (merkki1 == K_FI_L) {
            if (!edessa_vokaali(sana, haku1)) break;
            *p_haku = haku1;
            return 9; /* lt:ll */
        }
        break;
    case K_FI_N:
        merkki1 = K_INB(haku1, sana);
        if (merkki1 == K_FI_N) {
            if (!edessa_vokaali(sana, haku1)) break;
            *p_haku = haku1;
            return 10; /* nt:nl */
        }
        break;
    case K_FI_R:
        merkki1 = K_INB(haku1, sana);
        if (merkki1 == K_FI_R) {
            if (!edessa_vokaali(sana, haku1)) break;
            *p_haku = haku1;
            return 11; /* rt:rr */
        }
        break;

    case K_FI_A:
    case K_FI_E:
    case K_FI_I:
    case K_FI_O:
    case K_FI_U:
    case K_FI_Y:
    case K_FI_AE:
    case K_FI_OE:
    case K_FI_H:
        return 4;
    }

    return 0;
}

static unsigned pakkaa_vokaali(kt_merkki m) {
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

static unsigned voiko_olla_takainen(kt_nomveik *veikkaus) {
    const char *sana = veikkaus->sana;
    const char *loppu = sana + veikkaus->sana_pituus;

    while (loppu > sana) {
        kt_merkki m = K_INB(loppu, sana);

        switch (m) {
        case K_FI_A:
        case K_FI_O:
        case K_FI_U:
            return 1;
        case K_FI_AE:
        case K_FI_OE:
        case K_FI_Y:
            return 0;
        }
    }

    return 0;
}

#if KOSKUT_PLURALS
static unsigned tavuja_korkeintaan(kt_nomveik *veikkaus) {
    const char *sana = veikkaus->sana;
    const char *loppu = sana + veikkaus->sana_pituus;
    kt_merkki v = K_FI_NA;
    unsigned tavuja = 0;
    unsigned alku = 1;

    while (sana < loppu) {
        kt_merkki m = K_INF(sana, loppu);

        if (alku) {
            unsigned diftongi = 0;
            switch (m) {
            case K_FI_U:
                diftongi = v == K_FI_A || v == K_FI_O || v == K_FI_E
                                                      || v == K_FI_I;
                break;
            case K_FI_Y:
                diftongi = v == K_FI_AE || v == K_FI_OE || v == K_FI_E
                                                        || v == K_FI_I;
                break;
            case K_FI_E:
                diftongi = v == K_FI_I;
                break;
            case K_FI_O:
                diftongi = v == K_FI_U;
                break;
            case K_FI_OE:
                diftongi = v == K_FI_Y;
                break;
            }

            if (diftongi) {
                v = K_FI_NA;
                alku = 0;
                continue;
            }
        }

        switch (m) {
        case K_FI_I:
            if (v != K_FI_NA) {
                v = K_FI_NA;
                continue;
            }
        case K_FI_A:
        case K_FI_E:
        case K_FI_O:
        case K_FI_U:
        case K_FI_Y:
        case K_FI_AE:
        case K_FI_OE:
            if (v == m) {
                v = K_FI_NA;
                continue;
            }
            ++tavuja;
            v = m;
            break;
        default:
            if (v != K_FI_NA) {
                alku = 0;
            }
            v = K_FI_NA;
        }
    }

    return tavuja;
}

static unsigned on_labiaalivokaali(kt_nomveik *veikkaus) {
    const char *sana = veikkaus->sana;
    const char *loppu = sana + veikkaus->sana_pituus;

    while (sana < loppu) {
        kt_merkki m = K_INF(sana, loppu);

        switch (m) {
        case K_FI_O:
        case K_FI_U:
        case K_FI_Y:
        case K_FI_OE:
            return 1;
        }
    }

    return 0;
}
#endif

#define M_U_VALTIO      1
#define M_I_TOIMI       1
#define M_I_LAPSI       2
#define M_I_VEITSI      3

#define K_LUOKKA(taiv)    (((taiv) >> 8) & 63)
#define K_ASTE(taiv)      (((taiv) >> 3) & 31)
#define K_VOKAALI(taiv)   ((taiv) & 7)
#define K_ETINEN(taiv)    ((taiv) & 0x8000U)

K_INL kt_nomtaiv taiv_takainen(kt_nomtaiv taiv) {
    return taiv & ~0x8000U;
}

K_INL kt_nomtaiv taiv_etinen(kt_nomtaiv taiv) {
    return taiv | 0x8000U;
}

K_INL kt_nomtaiv taiv_heitto(kt_nomtaiv taiv) {
    return taiv | 0x4000U;
}

K_INL kt_nomtaiv aseta_luokka(kt_nomtaiv taiv, unsigned luokka) {
    return taiv | ((luokka & 63) << 8);
}

K_INL kt_nomtaiv vaihda_luokka(kt_nomtaiv taiv, unsigned luokka) {
    return aseta_luokka(taiv & ~0x3F00U, luokka);
}

K_INL kt_nomtaiv aseta_aste(kt_nomtaiv taiv, unsigned aste) {
    return taiv | ((aste & 31) << 3);
}

K_INL kt_nomtaiv aseta_luokka_aste(kt_nomtaiv taiv,
                                   unsigned luokka, unsigned aste) {
    return aseta_aste(aseta_luokka(taiv, luokka), aste);
}

K_INL kt_nomtaiv poista_aste(kt_nomtaiv taiv) {
    return taiv & ~0xF8U;
}

K_INL kt_nomtaiv aseta_vokaali(kt_nomtaiv taiv, unsigned vokaali) {
    return taiv | (vokaali & 7);
}

K_INL kt_nomtaiv poista_vokaali(kt_nomtaiv taiv) {
    return taiv & ~0x7U;
}

K_INL kt_nomtaiv vaihda_vokaali(kt_nomtaiv taiv, unsigned vokaali) {
    return aseta_vokaali(poista_vokaali(taiv), vokaali);
}

K_INL kt_nomtaiv luo_nomtaiv(unsigned luokka, unsigned aste,
                             unsigned sointu, unsigned heitto,
                             unsigned vokaali) {
    return (sointu ? 0x8000 : 0) | (heitto ? 0x4000 : 0)
         | ((luokka & 63) << 8)  | ((aste & 31) << 3) | (vokaali & 7);
}

K_INL kt_nomini luokanvaihto(kt_nomini edellinen) {
    edellinen.taiv = vaihda_luokka(edellinen.taiv, 0);
    return edellinen;
}

K_INL kt_nomini luo_nomini(unsigned leik, kt_nomtaiv taiv) {
    kt_nomini nomini;
    nomini.leikkaa = leik;
    nomini.taiv = taiv;
    return nomini;
}

K_INL kt_nomini muuta_taivutusta(kt_nomini edellinen, kt_nomtaiv taiv) {
    kt_nomini nomini;
    nomini.leikkaa = edellinen.leikkaa;
    nomini.taiv = taiv;
    return nomini;
}

static unsigned automaattinen_sointu(kt_merkki merkki) {
    switch (merkki) {
    case K_FI_A:
    case K_FI_O:
    case K_FI_U:
        return 0;
    case K_FI_AE:
    case K_FI_OE:
    case K_FI_Y:
        return 1;
    }
    return ~0U;
}

K_INL void lisaa_vaihtoehto(kt_nomveik *veikkaus, unsigned i,
                            unsigned char tila, kt_nomini nomini) {
    veikkaus->tilat[i] = tila;
    veikkaus->vaihdot[i] = nomini;
}

#define T_LOPPU             0
#define T_MAA_ILL           1
#define T_SUO_MON_ILL       2
#define T_KALA_PAR          3
#define T_KALA_MON_PAR      4
#define T_PARFAIT_ILL       5
#define T_VALO_PAR          6
#define T_VALO_MON_PAR      7
#define T_VALTIO_ILL        8
#define T_VAPAA_MON_ILL     9
#define T_RISTI_PAR        10
#define T_OVI_PAR          11
#define T_RISTI_MON_PAR    12
#define T_UNI_MON_GEN      13
#define T_NALLE_PAR        14
#define T_PAR_SOINTU       15
#define T_PAPERI_PAR       16
#define T_PARFAIT_PAR      17

static void lopeta(kt_nomveik *veikkaus) {
    veikkaus->tila = T_LOPPU;
    veikkaus->vaihdot[0] = veikkaus->edellinen;
    veikkaus->valintoja = 1;
    veikkaus->muoto = 0;
}

static unsigned suo_mon_ill(kt_nomveik *veikkaus) {
#if KOSKUT_PLURALS
    if (veikkaus->monikot) {
        unsigned valintoja = 0;
        const char *loppu = veikkaus->sana + veikkaus->sana_pituus, *poikki;
        unsigned vokaali = veikkaus->vokaali & 7;
        unsigned sointu = vokaali & 4;
        kt_nomtaiv taiv = luo_nomtaiv(0, 0, sointu, 0, vokaali);

        poikki = loppu;
        K_INB(poikki, veikkaus->sana);
        K_INB(poikki, veikkaus->sana);

        veikkaus->muoto = K_NTAIV_MILL;

        /* suo */
        lisaa_vaihtoehto(veikkaus, valintoja++,
                vokaali & 3 ? T_LOPPU : T_PAR_SOINTU,
                        luo_nomini(loppu - poikki, aseta_luokka(taiv, 19)));

        /* ros?? */
        lisaa_vaihtoehto(veikkaus, valintoja++,
                vokaali & 3 ? T_LOPPU : T_PAR_SOINTU,
                        luo_nomini(0, aseta_luokka(taiv, 21)));

        return valintoja;
    }
#endif

    /* ei tarvitse monikoita, palauta 9 */
    lopeta(veikkaus);
    return 0;
}

static unsigned vapaa_mon_ill(kt_nomveik *veikkaus) {
#if KOSKUT_PLURALS
    if (veikkaus->monikot) {
        unsigned valintoja = 0;
        kt_nomini pohja = luokanvaihto(veikkaus->edellinen);

        veikkaus->muoto = K_NTAIV_MILL;

        /* vapaa */
        lisaa_vaihtoehto(veikkaus, valintoja++, T_LOPPU,
                muuta_taivutusta(pohja, aseta_luokka(pohja.taiv, 17)));

        /* filee */
        lisaa_vaihtoehto(veikkaus, valintoja++, T_LOPPU,
                muuta_taivutusta(pohja, aseta_luokka(pohja.taiv, 20)));

        return valintoja;
    }
#endif

    /* ei tarvitse monikoita, palauta 9 */
    lopeta(veikkaus);
    return 0;
}

static unsigned kala_mon_par(kt_nomveik *veikkaus) {
#if KOSKUT_PLURALS
    if (veikkaus->monikot) {
        unsigned valintoja = 0;
        unsigned tavuja = tavuja_korkeintaan(veikkaus);
        kt_nomini pohja = luokanvaihto(veikkaus->edellinen);
        unsigned aste = K_ASTE(pohja.taiv);
        unsigned koira_ensin = !!(K_ETINEN(pohja.taiv)
                                || on_labiaalivokaali(veikkaus));

        veikkaus->muoto = K_NTAIV_MPAR;

        /* kala */
        lisaa_vaihtoehto(veikkaus, valintoja++, T_LOPPU,
                muuta_taivutusta(pohja,
                        aseta_luokka(pohja.taiv, 9 + koira_ensin)));

        /* koira */
        lisaa_vaihtoehto(veikkaus, valintoja++, T_LOPPU,
                muuta_taivutusta(pohja,
                        aseta_luokka(pohja.taiv, 10 - koira_ensin)));

        if (tavuja <= 2)
            return valintoja;

        if (aste == 1 || aste == 3) {
            /* solakka */
            lisaa_vaihtoehto(veikkaus, valintoja++, T_LOPPU,
                    muuta_taivutusta(pohja, aseta_luokka(pohja.taiv, 14)));
        } else if (!aste) {
            const char *leikkaus = veikkaus->sana + veikkaus->sana_pituus
                                                  - pohja.leikkaa;

            /* kulkija */
            lisaa_vaihtoehto(veikkaus, valintoja++, T_LOPPU,
                    muuta_taivutusta(pohja, aseta_luokka(pohja.taiv, 12)));

            switch (K_INB(leikkaus, veikkaus->sana)) {
            case K_FI_E:
            case K_FI_I:
            case K_FI_O:
            case K_FI_U:
            case K_FI_Y:
            case K_FI_OE:
                /* korkea, mutta vain jos ennen leikkausta on vokaali */
                lisaa_vaihtoehto(veikkaus, valintoja++, T_LOPPU,
                        muuta_taivutusta(pohja, aseta_luokka(pohja.taiv, 15)));
            }
        }

        return valintoja;
    }
#endif

    /* ei tarvitse monikoita, palauta 9 */
    lopeta(veikkaus);
    return 0;
}

static unsigned valo_mon_par(kt_nomveik *veikkaus) {
#if KOSKUT_PLURALS
    if (veikkaus->monikot) {
        unsigned valintoja = 0;
        unsigned tavuja = tavuja_korkeintaan(veikkaus);
        kt_nomini pohja = luokanvaihto(veikkaus->edellinen);
        unsigned aste = K_ASTE(pohja.taiv);

        veikkaus->muoto = K_NTAIV_MPAR;

        /* valo */
        lisaa_vaihtoehto(veikkaus, valintoja++, T_LOPPU,
                muuta_taivutusta(pohja, aseta_luokka(pohja.taiv, 1)));

        if (tavuja > 2 && (aste == 1 || aste == 3)) {
            /* laatikko */
            lisaa_vaihtoehto(veikkaus, valintoja++, T_LOPPU,
                    muuta_taivutusta(pohja, aseta_luokka(pohja.taiv, 4)));
        }

        return valintoja;
    }
#endif

    /* ei tarvitse monikoita, palauta sellaisenaan */
    lopeta(veikkaus);
    return 0;
}

static unsigned risti_mon_par(kt_nomveik *veikkaus) {
#if KOSKUT_PLURALS
    if (veikkaus->monikot) {
        unsigned valintoja = 0;
        unsigned tavuja = tavuja_korkeintaan(veikkaus);
        kt_nomini pohja = luokanvaihto(veikkaus->edellinen);
        unsigned aste = K_ASTE(pohja.taiv);

        veikkaus->muoto = K_NTAIV_MPAR;

        /* risti */
        lisaa_vaihtoehto(veikkaus, valintoja++, T_LOPPU,
                muuta_taivutusta(pohja, aseta_luokka(pohja.taiv, 5)));

        if (tavuja > 2 && (!aste || aste == 31)) {
            /* paperi */
            lisaa_vaihtoehto(veikkaus, valintoja++, T_LOPPU,
                    muuta_taivutusta(pohja, aseta_luokka(pohja.taiv, 6)));
        }

        return valintoja;
    }
#endif

    /* ei tarvitse monikoita, palauta sellaisenaan */
    lopeta(veikkaus);
    return 0;
}

static unsigned uni_mon_gen(kt_nomveik *veikkaus) {
#if KOSKUT_PLURALS
    if (veikkaus->monikot) {
        unsigned valintoja = 0;
        kt_nomini pohja = luokanvaihto(veikkaus->edellinen);

        veikkaus->muoto = K_NTAIV_MGEN;

        /* tiili */
        lisaa_vaihtoehto(veikkaus, valintoja++, T_LOPPU,
                muuta_taivutusta(pohja, aseta_luokka(pohja.taiv, 23)));

        /* pieni */
        lisaa_vaihtoehto(veikkaus, valintoja++, T_LOPPU,
                muuta_taivutusta(pohja, aseta_luokka(pohja.taiv, 26)));

        return valintoja;
    }
#endif

    /* ei tarvitse monikoita, palauta sellaisenaan */
    lopeta(veikkaus);
    return 0;
}

static unsigned parfait_ill(kt_nomveik *veikkaus) {
    unsigned valintoja = 0;
    kt_nomini pohja = veikkaus->edellinen;
    unsigned etisyys = veikkaus->vokaali & 0x80 ? 4 : 0;
    /* 4 = etu, 0 = takavokaalit. */

    pohja.taiv = poista_vokaali(pohja.taiv);
    veikkaus->muoto = K_NTAIV_YILL;

    /* e */
    lisaa_vaihtoehto(veikkaus, valintoja++, T_LOPPU,
        muuta_taivutusta(pohja, aseta_vokaali(pohja.taiv, 4)));
    /* i */
    lisaa_vaihtoehto(veikkaus, valintoja++, T_LOPPU,
        muuta_taivutusta(pohja, aseta_vokaali(pohja.taiv, 0)));
    /* a/?? */
    lisaa_vaihtoehto(veikkaus, valintoja++, T_LOPPU,
        muuta_taivutusta(pohja, aseta_vokaali(pohja.taiv, etisyys | 1)));
    /* o/?? */
    lisaa_vaihtoehto(veikkaus, valintoja++, T_LOPPU,
        muuta_taivutusta(pohja, aseta_vokaali(pohja.taiv, etisyys | 2)));
    /* u/y */
    lisaa_vaihtoehto(veikkaus, valintoja++, T_LOPPU,
        muuta_taivutusta(pohja, aseta_vokaali(pohja.taiv, etisyys | 3)));

    return valintoja;
}

static unsigned maa_ill(kt_nomveik *veikkaus) {
    unsigned valintoja = 0;
    const char *loppu = veikkaus->sana + veikkaus->sana_pituus, *poikki;
    unsigned vokaali = veikkaus->vokaali & 7;
    unsigned sointu = vokaali & 4;
    kt_nomtaiv taiv = luo_nomtaiv(0, 0, sointu, 0, vokaali);

    poikki = loppu;
    K_INB(poikki, veikkaus->sana);

    veikkaus->muoto = K_NTAIV_YILL;

    /* maa */
    lisaa_vaihtoehto(veikkaus, valintoja++,
            vokaali & 3 ? T_LOPPU : T_PAR_SOINTU,
            luo_nomini(loppu - poikki, aseta_luokka(taiv, 18)));

    /* vapaa */
    lisaa_vaihtoehto(veikkaus, valintoja++,
            vokaali & 3 ? T_VAPAA_MON_ILL : T_PAR_SOINTU,
            luo_nomini(loppu - poikki, aseta_luokka(taiv, 17)));

    return valintoja;
}

static unsigned valtio_ill(kt_nomveik *veikkaus) {
    unsigned valintoja = 0;
    kt_nomini pohja = luokanvaihto(veikkaus->edellinen);
    unsigned vokaali = K_VOKAALI(pohja.taiv);
    const char *loppu = veikkaus->sana + veikkaus->sana_pituus;
    const char *poikki = loppu;

    veikkaus->muoto = K_NTAIV_YILL;

    /* valtio */
    if (K_INB(poikki, veikkaus->sana) != K_FI_NA &&
            (vokaali == 4 || vokaali == 2 || vokaali == 6))
        lisaa_vaihtoehto(veikkaus, valintoja++, T_LOPPU,
                luo_nomini(loppu - poikki, vaihda_luokka(pohja.taiv, 3)));

    /* ros?? */
    lisaa_vaihtoehto(veikkaus, valintoja++, T_LOPPU,
            luo_nomini(0, vaihda_luokka(pohja.taiv, 21)));

    return valintoja;
}

static unsigned kala_par(kt_nomveik *veikkaus) {
    unsigned valintoja = 0;
    kt_nomini pohja = veikkaus->edellinen;

    if (K_ASTE(pohja.taiv)) {
        return kala_mon_par(veikkaus);
    }

    veikkaus->muoto = K_NTAIV_YPAR;

    /* edellinen */
    lisaa_vaihtoehto(veikkaus, valintoja++, T_KALA_MON_PAR, pohja);

    /* ros??? */
    lisaa_vaihtoehto(veikkaus, valintoja++, T_LOPPU,
            luo_nomini(0, vaihda_vokaali(vaihda_luokka(pohja.taiv, 21),
                    veikkaus->vokaali & 7)));

    return valintoja;
}

static unsigned valo_par(kt_nomveik *veikkaus) {
    unsigned valintoja = 0;
    kt_nomini pohja = veikkaus->edellinen;

    if (K_ASTE(pohja.taiv)) {
        return valo_mon_par(veikkaus);
    }

    veikkaus->muoto = K_NTAIV_YPAR;

    /* edellinen */
    if (!veikkaus->luokka)
        lisaa_vaihtoehto(veikkaus, valintoja++, T_VALO_MON_PAR, pohja);

    /* valtio / ros??? */
    lisaa_vaihtoehto(veikkaus, valintoja++, T_VALTIO_ILL,
            luo_nomini(0, vaihda_vokaali(
                    vaihda_luokka(pohja.taiv, 21), veikkaus->vokaali & 7)));

    /* edellinen */
    if (veikkaus->luokka)
        lisaa_vaihtoehto(veikkaus, valintoja++, T_VALO_MON_PAR, pohja);

    return valintoja;
}

static unsigned risti_par(kt_nomveik *veikkaus) {
    unsigned valintoja = 0;
    unsigned takainen = voiko_olla_takainen(veikkaus);
    kt_nomini pohja = luokanvaihto(veikkaus->edellinen);

    pohja.taiv = taiv_etinen(pohja.taiv);
    veikkaus->muoto = K_NTAIV_YPAR;

    /* risti */
    if (takainen)
        lisaa_vaihtoehto(veikkaus, valintoja++, T_RISTI_MON_PAR,
                muuta_taivutusta(pohja, taiv_takainen(
                        aseta_luokka(pohja.taiv, 5))));
    lisaa_vaihtoehto(veikkaus, valintoja++, T_RISTI_MON_PAR,
            muuta_taivutusta(pohja, aseta_luokka(pohja.taiv, 5)));

    pohja.taiv = vaihda_vokaali(pohja.taiv, veikkaus->vokaali & 7);

    /* ros?? */
    if (!K_ASTE(pohja.taiv)) {
        lisaa_vaihtoehto(veikkaus, valintoja++, T_LOPPU,
                luo_nomini(0, taiv_takainen(aseta_luokka(pohja.taiv, 21))));
        lisaa_vaihtoehto(veikkaus, valintoja++, T_LOPPU,
                luo_nomini(0, aseta_luokka(pohja.taiv, 21)));
    }

    return valintoja;
}

static unsigned ovi_par(kt_nomveik *veikkaus) {
    unsigned valintoja = 0;
    kt_nomini pohja = luokanvaihto(veikkaus->edellinen);
    unsigned x_luokka = 0, x_tila = T_LOPPU, x_poikki;
    unsigned takainen = voiko_olla_takainen(veikkaus);
    const char *sana_loppu = veikkaus->sana + veikkaus->sana_pituus;
    const char *leikkaus = sana_loppu- pohja.leikkaa;
    kt_merkki merkki = K_INB(leikkaus, veikkaus->sana);

    pohja.taiv = taiv_etinen(pohja.taiv);
    veikkaus->muoto = K_NTAIV_YPAR;

    /* ovi */
    if (takainen)
        lisaa_vaihtoehto(veikkaus, valintoja++, T_LOPPU,
                muuta_taivutusta(pohja, taiv_takainen(
                        aseta_luokka(pohja.taiv, 7))));
    lisaa_vaihtoehto(veikkaus, valintoja++, T_LOPPU,
            muuta_taivutusta(pohja, aseta_luokka(pohja.taiv, 7)));

    switch (veikkaus->luokka) {
    case M_I_TOIMI:
        if (merkki != K_FI_M) break;
        /* toimi */
        x_luokka = 25;
        x_poikki = 1;
        break;
    case M_I_LAPSI:
        /* lapsi */
        x_luokka = 29;
        x_poikki = 2;
        break;
    case M_I_VEITSI:
        /* veitsi */
        x_luokka = 30;
        x_poikki = 2;
        break;
    default:
        switch (merkki) {
        case K_FI_H:
        case K_FI_L:
        case K_FI_N:
        case K_FI_R:
        case K_FI_S:
            /* uni */
            x_luokka = 24;
            x_tila = T_UNI_MON_GEN;
            x_poikki = 0;
            break;
        }
    }

    if (x_luokka) {
        const char *poikki = sana_loppu - pohja.leikkaa;
        pohja.taiv = aseta_luokka(pohja.taiv, x_luokka);

        while (x_poikki--) {
            K_INB(poikki, veikkaus->sana);
            pohja.leikkaa = sana_loppu - poikki;
        }

        if (takainen)
            lisaa_vaihtoehto(veikkaus, valintoja++, x_tila,
                    muuta_taivutusta(pohja, taiv_takainen(pohja.taiv)));
        lisaa_vaihtoehto(veikkaus, valintoja++, x_tila, pohja);
    }

    return valintoja;
}

static unsigned nalle_par(kt_nomveik *veikkaus) {
    unsigned valintoja = 0;
    unsigned takainen = voiko_olla_takainen(veikkaus);
    kt_nomini pohja = luokanvaihto(veikkaus->edellinen);

    pohja.taiv = taiv_etinen(pohja.taiv);
    veikkaus->muoto = K_NTAIV_YPAR;

    /* nalle */
    if (takainen)
        lisaa_vaihtoehto(veikkaus, valintoja++, T_LOPPU,
                muuta_taivutusta(pohja, taiv_takainen(
                        aseta_luokka(pohja.taiv, 8))));
    lisaa_vaihtoehto(veikkaus, valintoja++, T_LOPPU,
            muuta_taivutusta(pohja, aseta_luokka(pohja.taiv, 8)));

    pohja.taiv = vaihda_vokaali(pohja.taiv, veikkaus->vokaali & 7);
    /* ros?? */
    if (!K_ASTE(pohja.taiv)) {
        lisaa_vaihtoehto(veikkaus, valintoja++, T_LOPPU,
                luo_nomini(0, taiv_takainen(aseta_luokka(pohja.taiv, 21))));
        lisaa_vaihtoehto(veikkaus, valintoja++, T_LOPPU,
                luo_nomini(0, aseta_luokka(pohja.taiv, 21)));
    }

    return valintoja;
}

static unsigned par_sointu(kt_nomveik *veikkaus, unsigned seuraava) {
    unsigned valintoja = 0;
    kt_nomini pohja = veikkaus->edellinen;

    pohja.taiv = taiv_etinen(pohja.taiv);

    veikkaus->muoto = K_NTAIV_YPAR;

    if (voiko_olla_takainen(veikkaus))
        lisaa_vaihtoehto(veikkaus, valintoja++, seuraava,
                muuta_taivutusta(pohja, taiv_takainen(pohja.taiv)));
    lisaa_vaihtoehto(veikkaus, valintoja++, seuraava, pohja);

    return valintoja;
}

static unsigned aja(kt_nomveik *veikkaus) {
    switch (veikkaus->tila) {
    case T_LOPPU:            break;
    case T_MAA_ILL:          return maa_ill(veikkaus);
    case T_SUO_MON_ILL:      return suo_mon_ill(veikkaus);
    case T_KALA_PAR:         return kala_par(veikkaus);
    case T_KALA_MON_PAR:     return kala_mon_par(veikkaus);
    case T_PARFAIT_ILL:      return parfait_ill(veikkaus);
    case T_VALO_PAR:         return valo_par(veikkaus);
    case T_VALO_MON_PAR:     return valo_mon_par(veikkaus);
    case T_VALTIO_ILL:       return valtio_ill(veikkaus);
    case T_VAPAA_MON_ILL:    return vapaa_mon_ill(veikkaus);
    case T_RISTI_PAR:        return risti_par(veikkaus);
    case T_OVI_PAR:          return ovi_par(veikkaus);
    case T_RISTI_MON_PAR:    return risti_mon_par(veikkaus);
    case T_UNI_MON_GEN:      return uni_mon_gen(veikkaus);
    case T_NALLE_PAR:        return nalle_par(veikkaus);
    case T_PAR_SOINTU:       return par_sointu(veikkaus, T_LOPPU);
    case T_PAPERI_PAR:       return par_sointu(veikkaus, T_RISTI_MON_PAR);
    case T_PARFAIT_PAR:      return par_sointu(veikkaus, T_PARFAIT_ILL);
    }
    return 0;
}

static void jatka(kt_nomveik *veikkaus) {
    unsigned valintoja = aja(veikkaus);
    if (valintoja)
        veikkaus->valintoja = valintoja;
    else
        lopeta(veikkaus);
}

static void aloita_a(kt_nomveik *veikkaus, kt_merkki merkki, const char *sana,
                     const char *haku, const char *loppu) {
    const char *poikki = haku;
    kt_merkki vokaali;
    unsigned valintoja = 0;
    unsigned sointu = 0;

    vokaali = merkki;
    sointu = automaattinen_sointu(vokaali);
    veikkaus->vokaali = pakkaa_vokaali(vokaali) | (sointu ? 0x80 : 0x00);

    poikki = haku;
    merkki = K_INB(haku, sana);
    if (merkki == vokaali) {    /* pitk?? vokaali */
        veikkaus->tila = T_MAA_ILL;
    } else {                    /* lyhyt -A */
        /* tarkista astevaihtelu */
        unsigned aste;
        /* kala */
        kt_nomtaiv taiv;

        haku = poikki;
        aste = lue_vahva_aste(sana, &haku, vokaali);
        taiv = luo_nomtaiv(9, aste, sointu, 0, veikkaus->vokaali);

        if (aste == 4 && haku > sana) {
            const char *temp = haku;
            /* aseta I vartalovokaaliksi, jos se edelt???? k:ta */
            merkki = K_INB(temp, sana);
            if (merkki == K_FI_I) {
                taiv = vaihda_vokaali(taiv, 0);
            } else if (merkki == vokaali && temp > sana) {
                merkki = K_INB(temp, sana);
                if (merkki == vokaali) {
                    /* vaaka => vaa'an */
                    taiv = taiv_heitto(taiv);
                }
            }
        }

        lisaa_vaihtoehto(veikkaus, valintoja++, T_KALA_PAR,
            luo_nomini(loppu - haku, taiv));

        if (aste) {
            /* genetiivi tarvitsee kysy?? vain, jos astevaihtelua on. */
            lisaa_vaihtoehto(veikkaus, valintoja++, T_KALA_PAR,
                luo_nomini(loppu - poikki, poista_aste(taiv)));
        }
    }

    veikkaus->muoto = K_NTAIV_YGEN;
    veikkaus->valintoja = valintoja;
}

static void aloita_o(kt_nomveik *veikkaus, kt_merkki merkki, const char *sana,
                     const char *haku, const char *loppu) {
    const char *poikki = haku;
    kt_merkki vokaali;
    unsigned valintoja = 0;
    unsigned sointu = 0;

    vokaali = merkki;
    sointu = automaattinen_sointu(vokaali);
    veikkaus->vokaali = pakkaa_vokaali(vokaali) | (sointu ? 0x80 : 0x00);

    poikki = haku;
    merkki = K_INB(haku, sana);
    if (merkki == vokaali) {    /* pitk?? vokaali */
        veikkaus->tila = T_MAA_ILL;
    } else if ((merkki == K_FI_U && vokaali == K_FI_O)
            || (merkki == K_FI_Y && vokaali == K_FI_OE)) { /* diftongi */
        veikkaus->tila = T_SUO_MON_ILL;
        poikki = haku;
    } else {                    /* lyhyt -O */
        /* tarkista astevaihtelu */
        unsigned aste;
        /* valo */
        kt_nomtaiv taiv;

        haku = poikki;
        aste = lue_vahva_aste(sana, &haku, vokaali);
        taiv = luo_nomtaiv(1, aste, sointu, 0, veikkaus->vokaali);

        if (aste == 4 && haku > sana) {
            const char *temp = haku;
            /* ruo'on */
            merkki = K_INB(temp, sana);
            if (merkki == vokaali) {
                switch (merkki) {
                case K_FI_O:
                case K_FI_OE:
                    merkki = K_INB(temp, sana);
                    if (merkki != K_FI_U && merkki != K_FI_Y) break;
                case K_FI_U:
                case K_FI_Y:
                    taiv = taiv_heitto(taiv);
                }
            }
        } else if (!aste && haku > sana) {
            const char *temp = haku;
            /* valtio */
            merkki = K_INB(temp, sana);
            if (merkki == K_FI_A || merkki == K_FI_E || merkki == K_FI_I) {
                veikkaus->luokka = M_U_VALTIO;
            }
        }

        lisaa_vaihtoehto(veikkaus, valintoja++, T_VALO_PAR,
            luo_nomini(loppu - haku, taiv));

        if (aste) {
            /* genetiivi tarvitsee kysy?? vain, jos astevaihtelua on. */
            lisaa_vaihtoehto(veikkaus, valintoja++, T_VALO_PAR,
                luo_nomini(loppu - poikki, poista_aste(taiv)));
        }
    }

    veikkaus->muoto = K_NTAIV_YGEN;
    veikkaus->valintoja = valintoja;
}

static void aloita_i(kt_nomveik *veikkaus, kt_merkki merkki, const char *sana,
                     const char *haku, const char *loppu) {
    const char *poikki = haku;
    kt_merkki vokaali;
    unsigned valintoja = 0;

    vokaali = merkki;
    veikkaus->vokaali = pakkaa_vokaali(vokaali);

    poikki = haku;
    merkki = K_INB(haku, sana);
    if (merkki == vokaali) {    /* pitk?? vokaali */
        veikkaus->tila = T_MAA_ILL;
    } else {                    /* lyhyt -I */
        unsigned aste;
        /* tarkista astevaihtelu */
        const char *temp;
        kt_nomtaiv taiv;

        haku = poikki;
        aste = lue_vahva_aste(sana, &haku, vokaali);
        taiv = luo_nomtaiv(0, aste, 0, 0, 0); /* vokaali = i */

        /* risti */
        lisaa_vaihtoehto(veikkaus, valintoja++, T_RISTI_PAR,
            luo_nomini(loppu - haku, aseta_luokka(taiv, 5)));

        /* risti */
        if (aste) {
            lisaa_vaihtoehto(veikkaus, valintoja++, T_RISTI_PAR,
                luo_nomini(loppu - poikki, poista_aste(aseta_luokka(taiv, 5))));
        }

        taiv = aseta_vokaali(taiv, 4); /* vokaali = e */

        /* ovi */
        if (aste != 1 && aste != 3) {
            lisaa_vaihtoehto(veikkaus, valintoja++, T_OVI_PAR,
                luo_nomini(loppu - haku, aseta_luokka(taiv, 7)));
        }

        if (aste == 8) {
            /* vanhempi */
            unsigned takainen = voiko_olla_takainen(veikkaus);

            /* risti */
            if (takainen)
                lisaa_vaihtoehto(veikkaus, valintoja++, T_LOPPU,
                        luo_nomini(loppu - haku, taiv_takainen(vaihda_vokaali(
                                aseta_luokka(taiv, 16), 1))));
            lisaa_vaihtoehto(veikkaus, valintoja++, T_LOPPU,
                luo_nomini(loppu - haku, taiv_etinen(vaihda_vokaali(
                        aseta_luokka(taiv, 16), 5))));
        }

        merkki = K_INB(haku, sana);
        switch (merkki) {
        case K_FI_M:
            merkki = K_INB(haku, sana);
            if (onko_vokaali(merkki))
                veikkaus->luokka = M_I_TOIMI;
            break;
        case K_FI_S:
            poikki = haku;
            merkki = K_INB(haku, sana);
            temp = haku;
            taiv = poista_aste(taiv);
            if (onko_vokaali(merkki)) {
                /* k??si */
                lisaa_vaihtoehto(veikkaus, valintoja++, T_PAR_SOINTU,
                    luo_nomini(loppu - poikki, aseta_luokka_aste(taiv, 27, 6)));
            } else if (onko_vokaali(K_INB(temp, sana))) {
                switch (merkki) {
                case K_FI_K:
                    /* kaksi */
                    lisaa_vaihtoehto(veikkaus, valintoja++, T_PAR_SOINTU,
                        luo_nomini(loppu - haku, aseta_luokka(taiv, 31)));
                    break;
                case K_FI_L:
                    /* kynsi */
                    lisaa_vaihtoehto(veikkaus, valintoja++, T_PAR_SOINTU,
                        luo_nomini(loppu - haku,
                            aseta_luokka_aste(taiv, 28, 9)));
                    break;
                case K_FI_N:
                    /* kynsi */
                    lisaa_vaihtoehto(veikkaus, valintoja++, T_PAR_SOINTU,
                        luo_nomini(loppu - haku,
                            aseta_luokka_aste(taiv, 28, 10)));
                    break;
                case K_FI_R:
                    /* kynsi */
                    lisaa_vaihtoehto(veikkaus, valintoja++, T_PAR_SOINTU,
                        luo_nomini(loppu - haku,
                            aseta_luokka_aste(taiv, 28, 11)));
                    break;
                case K_FI_P:
                    veikkaus->luokka = M_I_LAPSI;
                    break;
                case K_FI_T:
                    veikkaus->luokka = M_I_VEITSI;
                    break;
                }
            }
            break;
        }
    }

    veikkaus->muoto = K_NTAIV_YGEN;
    veikkaus->valintoja = valintoja;
}

static void aloita_e(kt_nomveik *veikkaus, kt_merkki merkki, const char *sana,
                     const char *haku, const char *loppu) {
    const char *poikki = haku;
    kt_merkki vokaali;
    unsigned valintoja = 0;

    vokaali = merkki;
    veikkaus->vokaali = pakkaa_vokaali(vokaali);

    poikki = haku;
    merkki = K_INB(haku, sana);
    if (merkki == vokaali) {    /* pitk?? vokaali */
        veikkaus->tila = T_MAA_ILL;
    } else if (merkki == K_FI_I) { /* diftongi */
        veikkaus->tila = T_SUO_MON_ILL;
        poikki = haku;
    } else {                    /* lyhyt -e */
        unsigned aste;
        kt_nomtaiv taiv;

        /* tarkista astevaihtelu */
        haku = poikki;
        aste = lue_vahva_aste(sana, &haku, vokaali);
        taiv = luo_nomtaiv(0, aste, 0, 0, 4); /* vokaali = e */

        /* nalle */
        lisaa_vaihtoehto(veikkaus, valintoja++, T_NALLE_PAR,
            luo_nomini(loppu - haku, aseta_luokka(taiv, 8)));

        /* nalle */
        if (aste) {
            lisaa_vaihtoehto(veikkaus, valintoja++, T_NALLE_PAR,
                luo_nomini(loppu - poikki, poista_aste(aseta_luokka(taiv, 8))));
        }

        switch (aste) {
        case 0:
        case 4:
        case 5:
        case 6:
        case 12:
        case 13:
            haku = poikki;
            aste = lue_heikko_aste(sana, &haku, vokaali);
            taiv = aseta_luokka_aste(taiv, 48, aste);

            /* hame */
            lisaa_vaihtoehto(veikkaus, valintoja++, T_PAR_SOINTU,
                luo_nomini(loppu - haku, taiv));

            /* hame */
            if (aste) {
                lisaa_vaihtoehto(veikkaus, valintoja++, T_PAR_SOINTU,
                    luo_nomini(loppu - poikki, poista_aste(taiv)));
            }
        }
    }

    veikkaus->muoto = K_NTAIV_YGEN;
    veikkaus->valintoja = valintoja;
}

static void aloita_nc(kt_nomveik *veikkaus, kt_merkki merkki, const char *sana,
                      const char *haku, const char *loppu) {
    const char *poikki = haku;
    kt_merkki vokaali;
    unsigned valintoja = veikkaus->valintoja;
    unsigned sointu;

    vokaali = merkki;
    veikkaus->vokaali = pakkaa_vokaali(vokaali);
    veikkaus->muoto = K_NTAIV_YGEN;

    poikki = haku;
    merkki = K_INB(haku, sana);
    sointu = automaattinen_sointu(merkki);

    /* 38 */
    if (merkki == K_FI_E) {
        const char *temp = haku;
        kt_merkki m = K_INB(temp, sana);
        if (m == K_FI_N) { /* nainen */
            kt_nomtaiv taiv = luo_nomtaiv(38, 0, 0, 0, 4);
            lisaa_vaihtoehto(veikkaus, valintoja++, T_PAR_SOINTU,
                luo_nomini(loppu - temp, taiv));
#if KOSKUT_PLURALS
            lisaa_vaihtoehto(veikkaus, valintoja++, T_PAR_SOINTU,
                luo_nomini(loppu - temp, taiv_heitto(taiv)));
#endif
            veikkaus->valintoja = valintoja;
            return;
        }
    }

    /* 34 */
    if (merkki == K_FI_O || merkki == K_FI_OE) {
        const char *temp = haku;
        kt_merkki m = K_INB(temp, sana);
        if (m == K_FI_T) {
            lisaa_vaihtoehto(veikkaus, valintoja++, T_LOPPU,
                luo_nomini(loppu - temp, luo_nomtaiv(34, 3,
                    merkki == K_FI_OE, 0, merkki == K_FI_OE ? 6 : 2)));
        }
        veikkaus->valintoja = valintoja;
        return;
    }

    /* 32, 49 */
    if (merkki == K_FI_A || merkki == K_FI_AE || merkki == K_FI_E) {
        const char *temp = haku;
        unsigned aste = lue_heikko_aste(sana, &temp, vokaali);
        /* sisar */
        kt_nomtaiv taiv = luo_nomtaiv(32, aste, sointu, 0,
                                      merkki == K_FI_E ? 5 : 1);
        unsigned tila = merkki == K_FI_E ? T_PAR_SOINTU : T_LOPPU;

        lisaa_vaihtoehto(veikkaus, valintoja++, tila,
            luo_nomini(loppu - temp, taiv));

        if (aste)
            lisaa_vaihtoehto(veikkaus, valintoja++, tila,
                luo_nomini(loppu - haku, poista_aste(taiv)));

        /* askel */
        taiv = vaihda_luokka(taiv, 49);

        lisaa_vaihtoehto(veikkaus, valintoja++, tila,
            luo_nomini(loppu - temp, taiv));

        if (aste)
            lisaa_vaihtoehto(veikkaus, valintoja++, tila,
                luo_nomini(loppu - haku, poista_aste(taiv)));
    }

    /* 33 */
    if (merkki == K_FI_A || merkki == K_FI_AE
                         || merkki == K_FI_I || merkki == K_FI_U) {
        const char *temp = haku;
        int i = merkki == K_FI_I;
        unsigned aste = lue_heikko_aste(sana, &temp, vokaali);
        /* kytkin */
        kt_nomtaiv taiv = luo_nomtaiv(33, aste, sointu,
                                      0, pakkaa_vokaali(merkki));

        lisaa_vaihtoehto(veikkaus, valintoja++, i ? T_PAR_SOINTU : T_LOPPU,
            luo_nomini(loppu - temp, taiv));

        if (aste)
            lisaa_vaihtoehto(veikkaus, valintoja++, i ? T_PAR_SOINTU : T_LOPPU,
                luo_nomini(loppu - haku, poista_aste(taiv)));

        if (merkki != K_FI_U && (aste == 2 || aste == 8)) {
            /* l??mmin */
            lisaa_vaihtoehto(veikkaus, valintoja++, i ? T_PAR_SOINTU : T_LOPPU,
                luo_nomini(loppu - temp, vaihda_luokka(taiv, 35)));
        }
    }

    /* 36 */
    if (merkki == K_FI_I) {
        kt_nomtaiv taiv = luo_nomtaiv(36, 8, 0, 0, 1);
        /* sisin */
        lisaa_vaihtoehto(veikkaus, valintoja++, T_LOPPU,
            luo_nomini(loppu - poikki, taiv));
        lisaa_vaihtoehto(veikkaus, valintoja++, T_LOPPU,
            luo_nomini(loppu - poikki, aseta_vokaali(taiv_etinen(taiv), 5)));
    }

    /* 37 */
    if (merkki == K_FI_E && voiko_olla_takainen(veikkaus)) {
        kt_nomtaiv taiv = luo_nomtaiv(37, 8, 0, 0, 1);
        /* vasen */
        lisaa_vaihtoehto(veikkaus, valintoja++, T_LOPPU,
            luo_nomini(loppu - poikki, taiv));
    }

    veikkaus->valintoja = valintoja;
}

static void aloita_tc(kt_nomveik *veikkaus, kt_merkki merkki, const char *sana,
                      const char *haku, const char *loppu) {
    const char *poikki = haku;
    kt_merkki vokaali;
    unsigned valintoja = veikkaus->valintoja;
    unsigned sointu;

    vokaali = merkki;
    veikkaus->vokaali = pakkaa_vokaali(vokaali);

    poikki = haku;
    merkki = K_INB(haku, sana);
    sointu = automaattinen_sointu(merkki);

    /* 44, 46 */
    if (merkki == K_FI_A || merkki == K_FI_AE) {
        unsigned vok = pakkaa_vokaali(merkki);

        /* kev??t */
        lisaa_vaihtoehto(veikkaus, valintoja++, T_LOPPU,
            luo_nomini(loppu - haku, luo_nomtaiv(44, 0, sointu, 0, vok)));

        /* tuhat */
        lisaa_vaihtoehto(veikkaus, valintoja++, T_LOPPU,
            luo_nomini(loppu - poikki, luo_nomtaiv(46, 10, sointu, 0, 4)));
    }

    /* 43, 47 */
    if (merkki == K_FI_U || merkki == K_FI_Y) {
        const char *temp = haku;
        unsigned aste = lue_heikko_aste(sana, &temp, vokaali);

        /* ohut */
        kt_nomtaiv taiv = luo_nomtaiv(43, 0, sointu,
                                      0, pakkaa_vokaali(merkki));

        /* ohut */
        lisaa_vaihtoehto(veikkaus, valintoja++, T_LOPPU,
            luo_nomini(loppu - temp, aseta_aste(taiv, aste)));

        if (aste) {
            lisaa_vaihtoehto(veikkaus, valintoja++, T_LOPPU,
                luo_nomini(loppu - haku, taiv));
        }

        /* kuollut */
        lisaa_vaihtoehto(veikkaus, valintoja++, T_LOPPU,
            luo_nomini(loppu - haku, vaihda_luokka(taiv, 47)));
    }

    veikkaus->muoto = K_NTAIV_YGEN;
    veikkaus->valintoja = valintoja;
}

static void aloita_sc(kt_nomveik *veikkaus, kt_merkki merkki, const char *sana,
                      const char *haku, const char *loppu) {
    const char *poikki = haku;
    kt_merkki vokaali;
    unsigned valintoja = veikkaus->valintoja;
    unsigned sointu;
    unsigned tila;

    vokaali = merkki;

    poikki = haku;
    merkki = K_INB(haku, sana);
    sointu = automaattinen_sointu(merkki);

    veikkaus->muoto = K_NTAIV_YGEN;
    if (!onko_vokaali(merkki))
        return;

    /* kysy sointua, jos ei selv?? vokaalista. */
    tila = !~sointu ? T_PAR_SOINTU : T_LOPPU;

    /* 39, 40 */
    /* vastaus */
    lisaa_vaihtoehto(veikkaus, valintoja++, tila,
        luo_nomini(loppu - poikki, luo_nomtaiv(39, 0, sointu, 0, 4)));

    if (merkki == K_FI_U || merkki == K_FI_Y) {
        /* kalleus */
        lisaa_vaihtoehto(veikkaus, valintoja++, T_LOPPU,
            luo_nomini(loppu - poikki, luo_nomtaiv(40, 6, sointu, 0, 4)));
    }

    /* 41 */
    if (merkki == K_FI_A || merkki == K_FI_AE
                         || merkki == K_FI_E || merkki == K_FI_I) {
        const char *temp = haku;
        unsigned aste = lue_heikko_aste(sana, &temp, vokaali);
        /* vieras */
        kt_nomtaiv taiv = luo_nomtaiv(41, aste, sointu,
                                      0, pakkaa_vokaali(merkki));

        lisaa_vaihtoehto(veikkaus, valintoja++, tila,
            luo_nomini(loppu - temp, taiv));

        if (aste) {
            lisaa_vaihtoehto(veikkaus, valintoja++, tila,
                luo_nomini(loppu - haku, poista_aste(taiv)));
        }
    }

    /* 42 */
    if (merkki == K_FI_E) {
        /* mies */
        lisaa_vaihtoehto(veikkaus, valintoja++, T_PAR_SOINTU,
            luo_nomini(loppu - poikki, luo_nomtaiv(42, 0, 0, 0, 4)));
    }

    /* 45 */
    /* kahdeksas */
    lisaa_vaihtoehto(veikkaus, valintoja++, tila,
        luo_nomini(loppu - poikki, luo_nomtaiv(45, 10, sointu, 0, 4)));

    veikkaus->valintoja = valintoja;
}

static void aloita_lc(kt_nomveik *veikkaus, kt_merkki merkki, const char *sana,
                      const char *haku, const char *loppu, unsigned ekstra) {
    kt_merkki vokaali;
    unsigned valintoja = veikkaus->valintoja;
    unsigned sointu;

    vokaali = merkki;
    veikkaus->vokaali = pakkaa_vokaali(vokaali);

    merkki = K_INB(haku, sana);
    sointu = automaattinen_sointu(merkki);

    /* 32, 49 */
    if (merkki == K_FI_A || merkki == K_FI_AE || merkki == K_FI_E) {
        const char *temp = haku;
        unsigned aste = lue_heikko_aste(sana, &temp, vokaali);
        /* sisar */
        kt_nomtaiv taiv = luo_nomtaiv(32, aste, sointu, 0,
                                      (merkki == K_FI_E ? 4 : 0) | ekstra);
        unsigned tila = merkki == K_FI_E ? T_PAR_SOINTU : T_LOPPU;

        lisaa_vaihtoehto(veikkaus, valintoja++, tila,
            luo_nomini(loppu - temp, taiv));

        if (aste)
            lisaa_vaihtoehto(veikkaus, valintoja++, tila,
                luo_nomini(loppu - haku, poista_aste(taiv)));

        /* askel */
        taiv = vaihda_luokka(taiv, 49);

        lisaa_vaihtoehto(veikkaus, valintoja++, tila,
            luo_nomini(loppu - temp, taiv));

        if (aste)
            lisaa_vaihtoehto(veikkaus, valintoja++, tila,
                luo_nomini(loppu - haku, poista_aste(taiv)));
    }

    veikkaus->muoto = K_NTAIV_YGEN;
    veikkaus->valintoja = valintoja;
}

static void iso_kirjain(kt_nomveik *veikkaus, const char *loppu,
                        unsigned luokka, unsigned vokaali) {
    lisaa_vaihtoehto(veikkaus, 0, T_LOPPU,
        luo_nomini(0, luo_nomtaiv(luokka, 0,
            !vokaali || (vokaali & 4), 0, vokaali)));
    veikkaus->valintoja = 1;
    veikkaus->edellinen = veikkaus->vaihdot[0];
    veikkaus->tila = T_LOPPU;
}

static void risti_ilman_ita(kt_nomveik *veikkaus) {
    /* risti (~i) */
    lisaa_vaihtoehto(veikkaus, veikkaus->valintoja++, T_PAPERI_PAR,
        luo_nomini(0, luo_nomtaiv(5, 31, 0, 0, 0)));
}

static void aloita(kt_nomveik *veikkaus,
                   const char *sana, kt_koko sana_pituus) {
    /* valitse alkutila sanan lopun perusteella. */
    const char *loppu = sana + sana_pituus;
    const char *haku = loppu;
    kt_merkki merkki;
    int vokaaliloppu = 0;
    unsigned rist = 0;

    merkki = K_INB(haku, sana);
    veikkaus->luokka = 0;
    veikkaus->vokaali = 0;

    vokaaliloppu = onko_vokaali(merkki);

    switch (merkki) {
    case K_FI_A:
    case K_FI_AE:
        aloita_a(veikkaus, merkki, sana, haku, loppu);
        break;

    case K_FI_O:
    case K_FI_OE:
    case K_FI_U:
    case K_FI_Y:
        aloita_o(veikkaus, merkki, sana, haku, loppu);
        break;

    case K_FI_I:
        aloita_i(veikkaus, merkki, sana, haku, loppu);
        break;

    case K_FI_E:
        aloita_e(veikkaus, merkki, sana, haku, loppu);
        break;

    case K_FI_N:
        if (!veikkaus->monikot) rist = 1, risti_ilman_ita(veikkaus);
        aloita_nc(veikkaus, merkki, sana, haku, loppu);
        break;

    case K_FI_S:
        if (!veikkaus->monikot) rist = 1, risti_ilman_ita(veikkaus);
        aloita_sc(veikkaus, merkki, sana, haku, loppu);
        break;

    case K_FI_T:
        if (!veikkaus->monikot) rist = 1, risti_ilman_ita(veikkaus);
        aloita_tc(veikkaus, merkki, sana, haku, loppu);
        break;

    case K_FI_L:
        if (!veikkaus->monikot) rist = 1, risti_ilman_ita(veikkaus);
        aloita_lc(veikkaus, merkki, sana, haku, loppu, 0);
        break;

    case K_FI_R:
        if (!veikkaus->monikot) rist = 1, risti_ilman_ita(veikkaus);
        aloita_lc(veikkaus, merkki, sana, haku, loppu, 2);
        break;

#if KOSKUT_CASE_SENSITIVE
    /* isot kirjaimet */
    case K_FI_UF:
    case K_FI_UL:
    case K_FI_UM:
    case K_FI_UN:
    case K_FI_UR:
    case K_FI_US:
    case K_FI_UX:       /* ??f(f??) */
        iso_kirjain(veikkaus, loppu, 62, 5);
        return;

    case K_FI_UZ:       /* tseta */
        iso_kirjain(veikkaus, loppu, 62, 1);
        return;

    case K_FI_UB:
    case K_FI_UC:
    case K_FI_UD:
    case K_FI_UE:       /* ..ee */
    case K_FI_UG:
    case K_FI_UP:
    case K_FI_UT:
    case K_FI_UV:
    case K_FI_UW:
        iso_kirjain(veikkaus, loppu, 63, 4);
        return;

    case K_FI_UI:       /* ..ii */
    case K_FI_UJ:
        iso_kirjain(veikkaus, loppu, 63, 0);
        return;

    case K_FI_UH:
    case K_FI_UK:
    case K_FI_UO:       /* ...oo */
        iso_kirjain(veikkaus, loppu, 63, 2);
        return;

    case K_FI_UA:       /* ..aa */
        iso_kirjain(veikkaus, loppu, 63, 1);
        return;

    case K_FI_UQ:
    case K_FI_UU:       /* ..uu */
        iso_kirjain(veikkaus, loppu, 63, 3);
        return;

    case K_FI_UY:       /* ..yy */
        iso_kirjain(veikkaus, loppu, 63, 7);
        return;

    case K_FI_UAE:      /* ..???? */
        iso_kirjain(veikkaus, loppu, 63, 5);
        return;

    case K_FI_UOE:      /* ..???? */
        iso_kirjain(veikkaus, loppu, 63, 6);
        return;
#endif
    }

    if (veikkaus->tila) {
        jatka(veikkaus);
    }

    while (veikkaus->valintoja == 1) {
        veikkaus->tila = veikkaus->tilat[0];
        veikkaus->edellinen = veikkaus->vaihdot[0];
        if (!veikkaus->tila) break;
        jatka(veikkaus);
    }

    if (!vokaaliloppu) {
        veikkaus->muoto = K_NTAIV_YGEN;

        /* risti (~i) */
        if (veikkaus->monikot || !rist) risti_ilman_ita(veikkaus);

        /* parfait */
        lisaa_vaihtoehto(veikkaus, veikkaus->valintoja++, T_PARFAIT_PAR,
            luo_nomini(0, luo_nomtaiv(22, 0, 0, 0, 4)));
    }

    switch (veikkaus->valintoja) {
    case 0:
        /* varavaihtoehdot */
        lisaa_vaihtoehto(veikkaus, veikkaus->valintoja++, T_PAR_SOINTU,
            luo_nomini(0, luo_nomtaiv(5, 31, 0, 0, 0)));
        lisaa_vaihtoehto(veikkaus, veikkaus->valintoja++, T_PAR_SOINTU,
            luo_nomini(0, luo_nomtaiv(5, 31, 1, 0, 0)));

    case 1:
        /* vain yksi vaihtoehto */
        veikkaus->edellinen = veikkaus->vaihdot[0];
        veikkaus->tila = T_LOPPU;
    }
}

void k_nveik_alusta(kt_nomveik *veikkaus, unsigned monikot,
                    const char *sana, kt_koko sana_pituus) {
    veikkaus->sana = sana;
    veikkaus->sana_pituus = sana_pituus;
#if KOSKUT_PLURALS
    veikkaus->monikot = !!monikot;
#else
    veikkaus->monikot = 0;
#endif
    aloita(veikkaus, sana, sana_pituus);
}

unsigned k_nveik_vaihtoehtoja(const kt_nomveik *veikkaus) {
    return veikkaus->valintoja;
}

unsigned k_nveik_arvausmuoto(const kt_nomveik *veikkaus) {
    return veikkaus->muoto;
}

kt_koko k_nveik_vaihtoehto(const kt_nomveik *veikkaus, unsigned numero,
                           char *puskuri, kt_koko puskuri_koko) {
    if (numero >= veikkaus->valintoja) return 0;
    return k_ntaiv_taivuta(veikkaus->vaihdot[numero],
                           veikkaus->muoto,
                           veikkaus->sana,
                           veikkaus->sana_pituus,
                           puskuri, puskuri_koko);
}

void k_nveik_valitse(kt_nomveik *veikkaus, unsigned numero) {
    if (numero < veikkaus->valintoja) {
        veikkaus->tila = veikkaus->tilat[numero],
        veikkaus->edellinen = veikkaus->vaihdot[numero];
        jatka(veikkaus);
    }
}

kt_nomini k_nveik_tulos(const kt_nomveik *veikkaus) {
    if (veikkaus->valintoja == 1) {
        return veikkaus->vaihdot[0];
    } else {
        kt_nomini nomini = { 0, 0 };
        return nomini;
    }
}
