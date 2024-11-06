Time to overcomplicate |*_*|

            Problemstilling
Nåværende parametere er hardkodet i .cpp filer, noen er tilgjengelige som felt på nodene og i 
editoren. Ha et felles sted for parametere/konfigurasjon i editor og kun i cpp filer. _(Ikke låse 
oss i at alt konfigerbart må være endel av godot node hierarkiet)_
Kan ikke oppdatere parametere fra .tscn filene under spilling. Må restarte spillet, som kan ta tid, men
kan også være litt vanskelig å se forskjellene mellom to verdier når en playtester. 

            Ønsker
- Alle spillets parametere/konfigurasjoner ligger i en enkelt json fil. 
- Ha en autoload node, godot sin Singleton, som sjekker om det har skjedd forandringer på filen innen en tids periode. Forandringer skrives inn i ConfigHandler.
- All bruk av parametere i kode leser den fra en ConfigHandler.
    - Alle parametere få registreres hos ConfigHandler
- Skriving til json filen skjer via:
  Bruker -> json
  Bruker -> setter editor parameter -> ConfigHandler -> json
  Bruker skriver til json direkte eller verdier i editor som så ber ConfigHandler om å skrive over verdien i json filen. _(mer quality of life for å tillate å forandre verdier i editoren også)_
- CLI som tillater spillet å bruke en spesifik konfig fil ved startup. Også kunne konfigurere dette i et felt. 

Håpet er at ved å dytte dette til en enkelt fil som kan lagres i en config/ eller noe sånt, så blir det lettere å leke seg litt rundt med parametere og finne hva som føles best. 
