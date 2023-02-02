# GAMEMAPD xBoard Chess Engine

## Descriere generală
GAMEMAPD este un Chess Engine pentru GNU xBoard. Acesta a fost construit pentru a ști să efectueze mutările tuturor pieselor, rocade și mutarea en-passant. De asemenea, se efectuează transformarea pionului in regină. Se generează toate possibilele mutări și se alege una la întâmplare. Dacă se află în șah, engine-ul va efectua o mutare care să îl scoată din această situație. Dacă nu există mutări disponibile, jocul se încheie. 
    
## How to use

### Build
    make build

### Run
    xboard -fcp "make run"


## Informații suplimentare despre implementare

Engine-ul comunică cu xboard-ul prin intermediul stdin/stdout. În aceasă etapă, botul are grijă să mute doar pionii de pe următoarele poziții: "a2" și "a7".
Am luat în considerare diferite forme de reprezentare internă a tablei de șah și ne-am oprit la una care s-a dovedit a fi foarte intuitivă și ușor de lucrat, manipulat și modificat.

Ne-am hotarât să reprezentăm, în logica noastră, tabla de șah printr-o matrice de unsigned char și am denumit o serie de macros conform pieselor reale de pe o tablă de șah așa cum se poate observa in board.h, rezultând matricea U8 chessBoard[8][8].

Este important de menționat faptul că piesele de culoare neagră vor avea valori impare, în timp ce piesele de culoare albă vor fi întotdeauna reprezentate cu valori pare. Acest lucru va fi util pentru a face operații cât mai eficient posibil.

## Conținutul fișierelor


### engine.h
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void cmd_interpreter();
    Asteapta comenzi de la xboard, iar cand le detecteaza apeleaza
    functiile corespunzatoare care executa comenzi si intorc un raspuns
    corespunzator. Daca este cazul, se va printa la stdout informatii
    relevante pentru xboard, apeland print_cmd();


void print_cmd(char *cmd);
    Transmite comenzi xboard-ului.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
---

### commands.h
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void feature_cmd();
    Transmite la xboard (cu print_cmd(.)) comanda:
    <    feature sigint=0 san=0 name=GAMEMAPD


void new_cmd(int *engine_color, int *engine_mode);
    Reinitializeaza pozitiile pe tabla de sah;
    Initializeaza partida;


void force_cmd(int *engine_mode);
    Intra in starea "force mode"


void forced_move_cmd(char *movement);
    Muta piesa pentru culoarea curenta.
    Schimba randul culorii ce va muta.


void go_cmd(Board *myBoard, int *engine_mode, int engine_color);
    Intra in starea "go mode".


void think_cmd(Board *myBoard, int engine_color);
    Analizeaza structurile de date folosind algoritmii. Gaseste o mutare valida
    si buna pentru bot. Sau se preda.


void white_cmd(int *engine_color);
    Set engine as white ($engine_color).


void black_cmd(int *engine_color);
    Set engine as black ($engine_color).
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

---

### board.h
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
getBlackLegalMoves();
    Returnează, în format string, o mutare validă pentru pionul negru
    de care are grijă botul sa-l mute.


getWhiteLegalMoves();
    Returnează, în format string, o mutare validă pentru pionul alb
    de care are grijă botul sa-l mute.


move(char *movement);
    În funcția move se primește o mișcare în format string și se
    realizează mutările specifice în matricea chessBoard.
    Tot în această funcție am tratat și cazul în care pionul poate
    ajunge pe prima linie a adversarului, situație în care se
    transformă în regină.

getWhiteLegalMoves(); && getBlackLegalMoves();
    Apelează funcția coords_to_string care, primește un set de coordonate,
    primește un i și j pentru piesa sursă pe care vrem să o mutăm și mai
    primește un i si j pentru locul destinație în care vrem să mutăm piesa
    respectivă și, pentru că noi primim aceste coordonate in oglindă
    față de cum este așezată tabla în mod normal, cu ajutorul
    case-urilor scrise, vom forma un string pe care îl vom pasa mai
    departe pentru ca Xboard să realizeze mutarea corespunzătoare.
    Va analiza mutarile legale pe care piesa noastră, în cazul acesta
    pionul, poate să le facă, și în această etapa va merge tot înainte
    până se transformă în regină.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
---

*NOTĂ:*
*În etapa a 2-a, 95% din detaliile despre implementare se găsesc sub forma comentariilor în cod. Majoritatea modificărilor au fost efectuate în fișierele board.cpp/.h, unde s-au adăugat mișcarile tuturor pieselor, și în fișierele commands.cpp/.h unde s-au adunat toate aceste mutari + rocadele. Deciziile cu privire la efectuarea unei mutări sunt luate in funcția think_cmd() din commands.cpp.*



### Bibliografie

* Pentru comunicarea prin intermediul stdin/stdout:
https://www.gnu.org/software/xboard/
https://www.gnu.org/software/xboard/engine-intf.html#9
* Reprezentare internă:
https://peterellisjones.tumblr.com/post/39938456375/chess-engine-part-i-choosing-internal-data
https://github.com/peterellisjones/Checkmate


## Contribuții
Acest proiect a fost realizat de:

**Eduard-Robert MIEILĂ**
* Research xboard
* Research reprezentare internă
* Implementare reprezentare internă
* Deplasare piese(pion, cal, regina, rege) - etapa 2
* Implementare rocade
* Implementare en passant

---

**Lucian-Dragos PĂUNE**
* Research xboard
* Research reprezentare internă
* Implementare reprezentare internă
* Deplasare piese(turn, nebun, regina) - etapa 2
* Implementare rocade
* Implementare en passant

---

**Ana-Maria MIHNEA**
* Research xboard
* Research reprezentare internă
* Implementare reprezentare internă
* Implementare rocade
* README & Comentarii

---

**Alexandru GROZA**
* Schelet;
* Interfatarea engine-ului cu xboard-ul prin intermediul stdin/stdout;
* Miscarea pionilor - etapa 1
* Adaptare comunicare/traducere reprezentare interna->comunicare xboard - etapa 2
