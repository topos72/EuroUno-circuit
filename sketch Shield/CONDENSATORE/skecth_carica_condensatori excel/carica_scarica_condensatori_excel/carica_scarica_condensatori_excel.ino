  /* 
*  carica  e scarica di un condensatore
*   paola riccobelli*/
const int analogPin = 3; // Definisice la porta A0 usata per la lettura
const int digitalPin =11  ; // Definisce la porta 7 usata per la carica
const int N_dati=250; //numero di misure effettuate
int i; // Definisice la variabile intera i (contatore)
int delays = 30;     //delay per la lettura dei valori utilizzata riferimento 
int factor = 100;     // intervallo di acquisizione (factor * delays) Microseconds;
long StartTime;       // Definisce il valore StartTime come intero long
int start = 0;    // Definisce il valore start (usato come flag)
int b=1; //variabile utilizzata per mettere in standby Arduino alla fine dell'acquisizione
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
  int V[N_dati]; // Definisce l’array intero V1
  long t[N_dati]; // Definisice l’array t come intero long
  if (Serial.available() > 0) // Controlla se il buffer seriale ha qualcosa
  {
    delays = (Serial.read() - '0') * 100;
    Serial.flush();           // Svuota la seriale
    start = 1;                 // Pone il flag start a uno
  }
  delay(2000);                      // Aspetta 2000 ms per permettere di partire con condensatore scarico
  // Fa un ciclo di due letture a vuoto per "scaricare" l’analogPin
  for (i = 0; i < 2; i++)
  {
    V[i] = analogRead(analogPin);
  }
  digitalWrite(digitalPin, HIGH);   // Pone digitalPin_uno a livello alto per la carica
  StartTime = micros();             // Misura il tempo iniziale con l’orologio interno (lettura in us)
  // Loop per la carica
  for (i = 0; i < N_dati; i++)
  {
    t[i] = (micros() - StartTime)/1000.0;  // Legge il timestamp
    V[i] = analogRead(analogPin); // Legge analogPin e lo mette in array V mette il risultato i
    delayMicroseconds(factor * delays);  // Aspetta tot us
  }
    // Serial.println("\tCARICA");
  // Loop per la scrittura su porta seriale dei N_dati dati della carica
  Serial.println("CLEARDATA"); //RIPULISCE IL FOGLIO EXCEL
  Serial.println("CELL,SET,C1,CARICA DEL CONDENSATORE"); // SCRIVE "SCARICA DEL CONDENSATORE" NELLA CASELLA C1 
   Serial.println("LABEL,t [Microseconds],tensione[V]"); // SCRIVE LE ETICHETTE DELLE CASELLE A1 (t [s]) e B1 (V [V])
  for (i = 0; i < N_dati; i++)
  {
    Serial.print("DATA,");//scrittura dei dati su EXCEL
    Serial.print(t[i]);
    Serial.print(",");
    Serial.print(V[i] * 5 / 1023.0, 3);      
    Serial.println(",CARICA"); // Nella colonna C viene scritto CARICA
    delay(10);
  }
  delay(500);// Aspetta 500 ms per completare la carica
  // Fa un ciclo di due letture a vuoto per "scaricare" l’analogPin
  for (i = 0; i < 2; i++)
  {
    V[i] = analogRead(analogPin);
  }
  StartTime = micros();           // Misura il tempo iniziale con l’orologio interno (lettura in us)
  digitalWrite(digitalPin, LOW);  // Pone digitalPin a livello basso per la scarica

  // Loop per la carica
  for (i = 0; i < N_dati; i++)
  {
    t[i] = (micros() - StartTime)/1000.0;  // Legge il timestamp in us, sottrae lo StartTime e mette il risultato i
    V[i] = analogRead(analogPin); // Legge analogPin e lo mette in array V
    delayMicroseconds(factor * delays);  // Aspetta tot us
    //delay(delays);
  }
Serial.println("Carica scarica SCARICA");
  // Loop per la scrittura su porta seriale dei N_dati dati della carica
  for (i = 0; i < N_dati; i++)
  {
    Serial.print("DATA,");//scrittura dei dati su EXCEL di seguito a quelli della carica
    Serial.print(t[i]);
    Serial.print(",");
    Serial.print(V[i] * 5 / 1023.0, 3);      
    Serial.println(",SCARICA"); // Nella colonna C viene scritto SCARICA
    delay(100);
  }

   Serial.println("CELL,SET,C1,TERMINE ACQUISIZIONE DATI"); 
   // COMUNICA LA FINE DELL'ACQUISIZIONE DATI, NELLA CASELLA C1
  while(b=1) {} //Mette l’Arduino in standby, 
  
}
