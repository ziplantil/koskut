# Koskut
_Koskut_ on pienikokoinen ja siirrettävä C-kirjasto suomen kielen sanojen
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
* `k_vvart.c`: luo verbivartaloita.
  * riippuvuudet: `k_aste.c`
* `k_vtaiv.c`: luo verbimuotoja eli taivuttaa verbejä.
  * riippuvuudet: `k_vvart.c`

### Merkistön muuntaminen
Jos `KOSKUT_CONVERT` otetaan käyttöön `koskutio.h`-tiedostossa,
kirjasto olettaa, että sen mukaan linkataan tasan yksi seuraavista kirjastoista.
* `k_cu8.c`: UTF-8.

## Huomautukset
* Koskut olettaa tekstin käyttävän sekä pieniä että isoja kirjaimia
  (koskut.h:n `KOSKUT_CASE_SENSITIVE`).
* Epäsäännöllisiä taivutuksia ei tueta (esim. nomini _auer_, verbit
  _olla_, _seistä_, _virkkaa_). Koskutta käyttävän ohjelman odotetaan
  toteuttavan näistä taivutuksista ne, joita se tarvitsee:
  * _auer_-sanassa on epäsäännöllinen astevaihtelu t : 0. Se tulee
    esittää muodossa _auder_ (49*F), mutta nominatiivin ja partitiivin yksikkö
    periytyykin _auer_-muodosta.
  * _olla_-verbi on yleensä luokkaa 67, mutta yksikön ja monikon
    kolmannen persoonan indikatiivin preesensmuodot ovatkin _on_ ja _ovat_,
    ei *_olee_ ja *_olevat_. Potentiaalin aktiivin muodot perustuvat vartaloon
    _liene_- (64).
  * _seistä_-verbi taipuu kuin luokassa 66, mutta sillä ei ole
    kaikkia muotoja (jotkin muodot ovat _seisoa_-verbiltä).
  * _virkkaa_-verbi on harvinainen, mutta se tulee taivuttaa eri muodoissa.
    Yleensä muodot ovat luokkaa 56*A, mutta vokaalisointu on
    passiivimuodoissa etinen, muuten takainen.
