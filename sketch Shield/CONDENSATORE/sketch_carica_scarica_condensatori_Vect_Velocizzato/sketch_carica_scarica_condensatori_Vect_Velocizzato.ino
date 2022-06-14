  /* 
   *  SHIELD EURO 1
   *  inserire lo shield condensatori [EURO 1] sulla scheda Arduino UNO;
   *  per effettuare le misure sul tempo caratteristico di 
   *  carica  e scarica di condensatori 
   *  sono necessari il PIN e il relativo SWITCH 
    * --> PIN 11  un condensatore ,selettore dello SWITCH  4
    * --> PIN 10 due configurazione in parallelo C+C ,selettore dello  SWITCH  3
    * --> PIN 9  due configurazione in serie 1/C+1/C, selettore dello SWITCH  2
    * --> PIN 6  un condensatore in serie con due in parallelo,selettore dello SWITCH  1
   *  la porta analogica di acquisizione del segnale è
   *  la porta ANALOGICA A3  
   * Output su seriale 
   * tempo[microsecondi] <tab> tensione[volt]
*/

// scommentare l'opzioni relativa alla misura
// const int digitalPin =11  ; // selezionare nello SWITCH la levetta numero 4
const int digitalPin =10  ;   // selezionare nello SWITCH la levetta numero 3
// const int digitalPin =9  ; // selezionare nello SWITCH la levetta numero 2
// const int digitalPin =6  ; // selezionare nello SWITCH la levetta numero 1

const int analogPin = 3;  //porta analogica di acquisizione del segnale
int i; // Definisice la variabile intera i (contatore)
int delays = 500;     //delay per la lettura dei valori utilizzata in Microsecondi riferimento 230
int factor = 20;
int M =250;            // numero di misure M=250
long StartTime;       // Definisce il valore StartTime come intero long
int start = 0;    // Definisce il valore start (usato come flag)
 
 // Istruzioni di inizializzazione
void setup()
{
  Serial.begin(9600); // Inizializza la porta seriale a 19200
  Serial.flush(); // Pulisce il buffer della porta seriale
  pinMode(digitalPin, OUTPUT); // Definisce digitalPin come output
  digitalWrite(digitalPin, LOW); // e lo pone a valore low
  bitClear(ADCSRA, ADPS0); // Istruzioni necessarie per velocizzare
  bitClear(ADCSRA, ADPS2); // il rate di acquisizione analogica
}

// Istruzioni del programma
void loop()
{
  int V[M]; // Definisce l’array intero V1
  long t[M]; // Definisice l’array t come  long
  if (Serial.available() > 0) // Controlla se il buffer seriale ha qualcosa
  {
    delays = (Serial.read() - '0') * 100;
    Serial.flush();    // Svuota la seriale
    start = 1;         // Pone il flag start a uno
  }
  delay(2000);         // Aspetta 2000 ms per permettere di partire con condensatore scarico
  // Fa un ciclo di due letture a vuoto per "scaricare" l’analogPin
  for (i = 0; i < 2; i++)
  {
    V[i] = analogRead(analogPin);
  }
  digitalWrite(digitalPin, HIGH);   // Pone digitalPin_uno a livello alto per la carica
  StartTime = micros();    // Misura il tempo iniziale in microsecondi)
  // Loop per la carica
  for (i = 0; i < M; i++)
  {
    t[i] = micros() - StartTime;  // Legge il timestamp
    V[i] = analogRead(analogPin); // Memorizza la misura dell'segnale analogico nel vettore V
    delayMicroseconds(factor * delays);  //intervalli di campionamento
    //  attende tot=factor * delays microsecondi per ripetere l'acquisizione
  }
   Serial.println("\tCARICA");
  // Loop per la scrittura su porta seriale 
  for (i = 0; i < M; i++)
  {
    Serial.print(t[i]);        // commentare per visualizzare sul PLOTTER SERIALE
    Serial.print ( "\t\t" );      // commentare per visualizzare sul PLOTTER SERIALE
    Serial.println(V[i] * 5 / 1023.0, 3); // Scrive V[i] 
    delay(50);
  }
  delay(500);// Aspetta 500 ms per completare la carica
  // Fa un ciclo di due letture a vuoto per "scaricare" l’analogPin
  for (i = 0; i < 2; i++)
  {
    V[i] = analogRead(analogPin);
  }
  StartTime = micros();           // Misura il tempo iniziale (lettura in us)
  digitalWrite(digitalPin, LOW);  // Pone digitalPin a livello basso per scaricare il condensatori
  // Loop per la Scarica
  for (i = 0; i < M; i++)
  {
    t[i] = micros() - StartTime;  // Legge il timestamp in us, sottrae lo StartTime e mette il risultato i
    V[i] = analogRead(analogPin); // Memorizza la misura dell'segnale analogico nel vettore V
    delayMicroseconds(factor * delays);  //  intervalli di campionamento
    //delay(delays);
  }
  Serial.println("scarica SCARICA");
  for (i = 0; i < M; i++)
  { // commentare le successive due righe per visualizzare sul PLOTTER SERIALE
    Serial.print(t[i]);       // commentare per visualizzare sul PLOTTER SERIALE
    Serial.print ( "\t\t" );     // commentare per visualizzare sul PLOTTER SERIALE
    Serial.println(V[i] * 5 / 1023.0, 3); // Scrive V[i] e va a capo
    delay(50);
  }
  // commentare le successive due righe per mandare in loop i cicli di carica e scarica
  while (1); {  // si arresta dopo un ciclo di carica e scarica
  }
}
