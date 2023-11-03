/*******************************************************************************
            Koskut -- pieni suomen kielen taivutuskirjasto
            tehnyt ziplantil 2022-2023 -- lisenssi: MIT
            versio: 2023-11-03
            <https://github.com/ziplantil/koskut>
*******************************************************************************/
/* k_cu8.c - UTF-8-tuki                                                       */

#include "koskutio.h"

/* koskutio.h KOSKUT_IO_CONVERSION=1 vaaditaan.
   olettaa UTF-8:n olevan kelvollista. */

static kt_merkki kt_unicode_merkiksi(unsigned long u) {
    switch (u) {
    case 0x61:      return K_FI_A;
    case 0x62:      return K_FI_B;
    case 0x63:      return K_FI_C;
    case 0x64:      return K_FI_D;
    case 0x65:      return K_FI_E;
    case 0x66:      return K_FI_F;
    case 0x67:      return K_FI_G;
    case 0x68:      return K_FI_H;
    case 0x69:      return K_FI_I;
    case 0x6A:      return K_FI_J;
    case 0x6B:      return K_FI_K;
    case 0x6C:      return K_FI_L;
    case 0x6D:      return K_FI_M;
    case 0x6E:      return K_FI_N;
    case 0x6F:      return K_FI_O;
    case 0x70:      return K_FI_P;
    case 0x71:      return K_FI_Q;
    case 0x72:      return K_FI_R;
    case 0x73:      return K_FI_S;
    case 0x74:      return K_FI_T;
    case 0x75:      return K_FI_U;
    case 0x76:      return K_FI_V;
    case 0x77:      return K_FI_W;
    case 0x78:      return K_FI_X;
    case 0x79:      return K_FI_Y;
    case 0x7A:      return K_FI_Z;
    case 0xE4:      return K_FI_AE;
    case 0xF6:      return K_FI_OE;
#if KOSKUT_CASE_SENSITIVE
    case 0x41:      return K_FI_UA;
    case 0x42:      return K_FI_UB;
    case 0x43:      return K_FI_UC;
    case 0x44:      return K_FI_UD;
    case 0x45:      return K_FI_UE;
    case 0x46:      return K_FI_UF;
    case 0x47:      return K_FI_UG;
    case 0x48:      return K_FI_UH;
    case 0x49:      return K_FI_UI;
    case 0x4A:      return K_FI_UJ;
    case 0x4B:      return K_FI_UK;
    case 0x4C:      return K_FI_UL;
    case 0x4D:      return K_FI_UM;
    case 0x4E:      return K_FI_UN;
    case 0x4F:      return K_FI_UO;
    case 0x50:      return K_FI_UP;
    case 0x51:      return K_FI_UQ;
    case 0x52:      return K_FI_UR;
    case 0x53:      return K_FI_US;
    case 0x54:      return K_FI_UT;
    case 0x55:      return K_FI_UU;
    case 0x56:      return K_FI_UV;
    case 0x57:      return K_FI_UW;
    case 0x58:      return K_FI_UX;
    case 0x59:      return K_FI_UY;
    case 0x5A:      return K_FI_UZ;
    case 0xC4:      return K_FI_UAE;
    case 0xD6:      return K_FI_UOE;
#endif
    case 0x3A:      return K_FI_CL;
    case 0x2019:    return K_FI_AP;
    }
    return K_FI_NA;
}

static unsigned long kt_merkki_unicodeksi(kt_merkki m) {
    switch (m) {
    case K_FI_A:    return 0x61;
    case K_FI_B:    return 0x62;
    case K_FI_C:    return 0x63;
    case K_FI_D:    return 0x64;
    case K_FI_E:    return 0x65;
    case K_FI_F:    return 0x66;
    case K_FI_G:    return 0x67;
    case K_FI_H:    return 0x68;
    case K_FI_I:    return 0x69;
    case K_FI_J:    return 0x6A;
    case K_FI_K:    return 0x6B;
    case K_FI_L:    return 0x6C;
    case K_FI_M:    return 0x6D;
    case K_FI_N:    return 0x6E;
    case K_FI_O:    return 0x6F;
    case K_FI_P:    return 0x70;
    case K_FI_Q:    return 0x71;
    case K_FI_R:    return 0x72;
    case K_FI_S:    return 0x73;
    case K_FI_T:    return 0x74;
    case K_FI_U:    return 0x75;
    case K_FI_V:    return 0x76;
    case K_FI_W:    return 0x77;
    case K_FI_X:    return 0x78;
    case K_FI_Y:    return 0x79;
    case K_FI_Z:    return 0x7A;
    case K_FI_AE:   return 0xE4;
    case K_FI_OE:   return 0xF6;
#if KOSKUT_CASE_SENSITIVE
    case K_FI_UA:   return 0x41;
    case K_FI_UB:   return 0x42;
    case K_FI_UC:   return 0x43;
    case K_FI_UD:   return 0x44;
    case K_FI_UE:   return 0x45;
    case K_FI_UF:   return 0x46;
    case K_FI_UG:   return 0x47;
    case K_FI_UH:   return 0x48;
    case K_FI_UI:   return 0x49;
    case K_FI_UJ:   return 0x4A;
    case K_FI_UK:   return 0x4B;
    case K_FI_UL:   return 0x4C;
    case K_FI_UM:   return 0x4D;
    case K_FI_UN:   return 0x4E;
    case K_FI_UO:   return 0x4F;
    case K_FI_UP:   return 0x50;
    case K_FI_UQ:   return 0x51;
    case K_FI_UR:   return 0x52;
    case K_FI_US:   return 0x53;
    case K_FI_UT:   return 0x54;
    case K_FI_UU:   return 0x55;
    case K_FI_UV:   return 0x56;
    case K_FI_UW:   return 0x57;
    case K_FI_UX:   return 0x58;
    case K_FI_UY:   return 0x59;
    case K_FI_UZ:   return 0x5A;
    case K_FI_UAE:  return 0xC4;
    case K_FI_UOE:  return 0xD6;
#endif
    case K_FI_CL:   return 0x3A;
    case K_FI_AP:   return 0x2019;
    case K_FI_NA:   return 0xFFFDU;
    }
    /* ? */
    return (unsigned long)m;
}

kt_merkki kt_inf_conv(const char **pp, const char *p1c) {
    const unsigned char *p = (const unsigned char *) *pp;
    const unsigned char *p1 = (const unsigned char *) p1c;
    unsigned char c;
    kt_merkki m;

    if (p == p1) return K_FI_NA;
    c = *p++;
    if (!(c & 0x80)) {
        m = kt_unicode_merkiksi(c);
    } else {
        unsigned n;
        unsigned long u;

        if ((c & 0xF8) == 0xF0)
            n = 3, u = c & 0x07U;
        else if ((c & 0xF0) == 0xE0)
            n = 2, u = c & 0x0FU;
        else if ((c & 0xE0) == 0xC0)
            n = 1, u = c & 0x1FU;
        else
            return K_FI_NA;

        while (n--) {
            if (p == p1) {
                u = 0;
                break;
            }
            c = *p++;
            if ((c & 0xC0) != 0x80) {
                u = 0;
                break;
            }
            u = (u << 6) | (c & 0x3F);
        }

        m = kt_unicode_merkiksi(u);
    }

    *pp = (const char *) p;
    return m;
}

kt_merkki kt_inb_conv(const char **pp, const char *p0c) {
    const unsigned char *p = (const unsigned char *) *pp, *p1 = p;
    const unsigned char *p0 = (const unsigned char *) p0c;
    const char *tmp;

    if (p == p0) return K_FI_NA;
    while (p > p0 && (*--p & 0xC0) == 0x80)
        ;

    *pp = tmp = (const char *) p;
    return kt_inf_conv(&tmp, (const char *) p1);
}

kt_koko kt_out_conv(char **pp, char *p1c, const kt_merkki c) {
    unsigned long u = kt_merkki_unicodeksi(c);
    unsigned char b[4];
    unsigned i, n;
    unsigned char *p = (unsigned char *) *pp;
    unsigned char *p1 = (unsigned char *) p1c;

    if (u >= 0x110000UL) u = 0xFFFDUL;

    if (u < 0x80)
        n = 1,
        b[0] = u & 0x7F;
    else if (u < 0x800)
        n = 2,
        b[0] = 0xC0U | ((u >>  6) & 0x1F),
        b[1] = 0x80U | ((u      ) & 0x3F);
    else if (u < 0x10000UL)
        n = 3,
        b[0] = 0xE0U | ((u >> 12) & 0x0F),
        b[1] = 0x80U | ((u >>  6) & 0x3F),
        b[2] = 0x80U | ((u      ) & 0x3F);
    else
        n = 4,
        b[0] = 0xF0U | ((u >> 18) & 0x07),
        b[1] = 0x80U | ((u >> 12) & 0x3F),
        b[2] = 0x80U | ((u >>  6) & 0x3F),
        b[3] = 0x80U | ((u      ) & 0x3F);

    for (i = 0; i < n && p < p1; ++i)
        *p++ = b[i];

    *pp = (char *) p;
    return n;
}
