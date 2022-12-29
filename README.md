# Koskut
_Koskut_ on pienikokoinen ja siirrettävä C-kirjasto suomen kielen sanojen
(toistaiseksi vain nominien, mutta jossain vaiheessa myös verbien)
taivuttamiseen. Koskut koostuu useasta osasta, joista jokainen on yhdessä
.c-tiedostossa. Tämän lisäksi on kaksia otsaketiedostoa, joista ensimmäinen
sisältää esittelyt kaikille Koskuen aliohjelmille (`koskut.h`) ja toinen
Koskuen kaikkien kirjastojen käyttämiä määrittelyjä (`koskutio.h`).

## Merkistökoodaus
Koskut on sisäisesti suunniteltu tapauksiin, joissa kaikki suomen kielen
kirjaimet A-Z, Ä, Ö, heittomerkki (') sekä kaksoispiste (:) voidaan esittää
yhdellä tavulla (`char`). Merkkien koodit määritellään
otsaketiedostossa `koskutio.h`.

Jos halutaan käyttää muuta merkistökoodausta, `koskutio.h`:ssa pitää
ottaa käyttöön `KOSKUT_CONVERT` ja käyttää yhtä merkistömuunto-osista
(ks. alla olevaa osaluetteloa).

## Huomautukset
* Koskut olettaa tekstin käyttävän sekä pieniä että isoja kirjaimia
  (koskut.h:n `KOSKUT_CASE_SENSITIVE`).

## Osat
Tarkempi osien dokumentaatio löytyy `ohje.txt`-tiedostosta.

* `k_aste.c`: asteet/astevaihtelut.
  * riippuvuudet: `koskut.h`, `koskutio.h`
* `k_nvart.c`: luo nominivartaloita.
  * riippuvuudet: `k_aste.c`
* `k_ntaiv.c`: luo nominimuotoja eli taivuttaa nomineita.
  * riippuvuudet: `k_nvart.c`
* `k_nveik.c`: nominiveikkain, joka veikkaa nominien taivutuksia.
  * riippuvuudet: `k_ntaiv.c`

### Merkistön muuntaminen
Jos `KOSKUT_CONVERT` otetaan käyttöön `koskutio.h`-tiedostossa,
kirjasto olettaa, että sen mukaan linkataan tasan yksi seuraavista kirjastoista.
* `k_cu8.c`: UTF-8.
