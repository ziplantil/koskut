/*******************************************************************************
            Koskut -- pieni suomen kielen taivutuskirjasto
            tehnyt ziplantil 2022-2023 -- lisenssi: MIT
            versio: 2023-11-04
            <https://github.com/ziplantil/koskut>
*******************************************************************************/
/* koskutio.h - yleiset määrittelyt                                           */

#ifndef KOSKUT_IO_H
#define KOSKUT_IO_H

#ifndef KOSKUT_CONVERT
#define KOSKUT_CONVERT 0
#endif

#ifndef KOSKUT_CASE_SENSITIVE
#define KOSKUT_CASE_SENSITIVE 1
#endif

#include <stddef.h>

#if __STDC_VERSION__ >= 199901L
#define KOSKUT_C99 1
#endif

typedef char kt_merkki;
typedef size_t kt_koko;
#if KOSKUT_C99
#include <stdint.h>
typedef _Bool kt_bool;
typedef uint_fast8_t kt_uint;
#else
typedef unsigned char kt_bool;
typedef unsigned char kt_uint;
#endif

#if KOSKUT_CONVERT

extern kt_merkki kt_inf_conv(const char **p, const char *p1);
extern kt_merkki kt_inb_conv(const char **p, const char *p0);
extern kt_koko kt_out_conv(char **p, char *p1, const kt_merkki c);

/* lue merkki osoittimesta p etuperin ja kasvata sitä (mutta älä ylitä p1) */
#define K_INF(p, p1) (p == p1 ? K_FI_NA : kt_inf_conv((const char **)&(p), p1))
/* lue merkki osoittimesta p takaperin ja laske sitä (mutta älä alita p0) */
#define K_INB(p, p0) (p == p0 ? K_FI_NA : kt_inb_conv((const char **)&(p), p0))
/* kirjoita merkki osoittimeen p, mutta katkaise jos menisi p1:n yli.
   palauttaa montako tavua kirjoitett(ais)iin */
#define K_OUT(p, p1, c) kt_out_conv(&(p), p1, c)

#else

/* lue merkki osoittimesta p etuperin ja kasvata sitä (mutta älä ylitä p1) */
#define K_INF(p, p1) *p++
/* lue merkki osoittimesta p takaperin ja laske sitä (mutta älä alita p0) */
#define K_INB(p, p0) *--p
/* kirjoita merkki osoittimeen p, mutta katkaise jos menisi p1:n yli.
   palauttaa montako tavua kirjoitett(ais)iin */
#define K_OUT(p, p1, c) ((void)((p) < (p1) && (*(p)++ = (c))), 1)

#endif

/* merkit (tyyppiä kt_merkki) */
/* pienet kirjaimet a-z */
#define K_FI_A  ((char)(0x61U))
#define K_FI_B  ((char)(0x62U))
#define K_FI_C  ((char)(0x63U))
#define K_FI_D  ((char)(0x64U))
#define K_FI_E  ((char)(0x65U))
#define K_FI_F  ((char)(0x66U))
#define K_FI_G  ((char)(0x67U))
#define K_FI_H  ((char)(0x68U))
#define K_FI_I  ((char)(0x69U))
#define K_FI_J  ((char)(0x6AU))
#define K_FI_K  ((char)(0x6BU))
#define K_FI_L  ((char)(0x6CU))
#define K_FI_M  ((char)(0x6DU))
#define K_FI_N  ((char)(0x6EU))
#define K_FI_O  ((char)(0x6FU))
#define K_FI_P  ((char)(0x70U))
#define K_FI_Q  ((char)(0x71U))
#define K_FI_R  ((char)(0x72U))
#define K_FI_S  ((char)(0x73U))
#define K_FI_T  ((char)(0x74U))
#define K_FI_U  ((char)(0x75U))
#define K_FI_V  ((char)(0x76U))
#define K_FI_W  ((char)(0x77U))
#define K_FI_X  ((char)(0x78U))
#define K_FI_Y  ((char)(0x79U))
#define K_FI_Z  ((char)(0x7AU))
/* pienet kirjaimet ä, ö */
#define K_FI_AE ((char)(0xE4U))
#define K_FI_OE ((char)(0xF6U))
/* heittomerkki */
#define K_FI_AP ((char)(0x27U))
/* kaksoispiste */
#define K_FI_CL ((char)(0x3AU))

#if KOSKUT_CASE_SENSITIVE
/* isot kirjaimet a-z */
#define K_FI_UA  ((char)(0x41U))
#define K_FI_UB  ((char)(0x42U))
#define K_FI_UC  ((char)(0x43U))
#define K_FI_UD  ((char)(0x44U))
#define K_FI_UE  ((char)(0x45U))
#define K_FI_UF  ((char)(0x46U))
#define K_FI_UG  ((char)(0x47U))
#define K_FI_UH  ((char)(0x48U))
#define K_FI_UI  ((char)(0x49U))
#define K_FI_UJ  ((char)(0x4AU))
#define K_FI_UK  ((char)(0x4BU))
#define K_FI_UL  ((char)(0x4CU))
#define K_FI_UM  ((char)(0x4DU))
#define K_FI_UN  ((char)(0x4EU))
#define K_FI_UO  ((char)(0x4FU))
#define K_FI_UP  ((char)(0x50U))
#define K_FI_UQ  ((char)(0x51U))
#define K_FI_UR  ((char)(0x52U))
#define K_FI_US  ((char)(0x53U))
#define K_FI_UT  ((char)(0x54U))
#define K_FI_UU  ((char)(0x55U))
#define K_FI_UV  ((char)(0x56U))
#define K_FI_UW  ((char)(0x57U))
#define K_FI_UX  ((char)(0x58U))
#define K_FI_UY  ((char)(0x59U))
#define K_FI_UZ  ((char)(0x5AU))
/* isot kirjaimet ä, ö */
#define K_FI_UAE ((char)(0xC4U))
#define K_FI_UOE ((char)(0xD6U))
#endif /* KOSKUT_CASE_SENSITIVE */

/* "kelvoton" merkki, jos ei mikään yllä olevista.
   tämä merkki voidaan lukea, mutta sitä ei koskaan kirjoiteta.
   kannattaa olla 0, jos mahdollista. */
#define K_FI_NA ((char)(0x00U))

/* 9 kelvotonta merkkiä, kannattaa olla peräkkäiset koodit.
   näiden pitää erota kaikista ylläolevista koodeista, eikä
   niitä koskaan lueta tai kirjoiteta (vain sisäiseen käyttöön). */
#define K_FI_X1 ((char)(0x01U))
#define K_FI_X2 ((char)(0x02U))
#define K_FI_X3 ((char)(0x03U))
#define K_FI_X4 ((char)(0x04U))
#define K_FI_X5 ((char)(0x05U))
#define K_FI_X6 ((char)(0x06U))
#define K_FI_X7 ((char)(0x07U))
#define K_FI_X8 ((char)(0x08U))
#define K_FI_X9 ((char)(0x09U))
#define K_FI_XA ((char)(0x0AU))
#define K_FI_XB ((char)(0x0BU))

#endif /* KOSKUT_IO_H */
