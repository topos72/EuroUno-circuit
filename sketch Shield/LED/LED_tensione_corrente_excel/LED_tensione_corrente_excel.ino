/*caratteristiche  circuito su shield Euro 1
 * -pin 5 uscita PWM per generare la rampa.
 * -switch 1 led giallo; switch 2 verde; switch 3 rosso
 * -resistenza RD = 100 Ohm
 Utilizzo del file Excel per visualizzare i dati
 tramite le righe di comando
  Serial.println("CLEARDATA"); //RIPULISCE IL FOGLIO EXCEL
  Serial.println("CELL,SET,C1"); // SCRIVE "SCARICA DEL CONDENSATORE" NELLA CASELLA C1
  Serial.println("LABEL,tensione[],corrente[A]");
 */
 
const unsigned int RampPin = 5; //pin 5 uscita PWM per generare la rampa.
// Questo pin viene collegato allâ€™integratore
const unsigned int analogPin_uno = 1; //pin A0 per lettura V1
const unsigned int analogPin_due = 2; //pin A2 per lettura V2
unsigned int i = 0; //variabile che conta gli step durante la salita della rampa
int V1[256]; //array per memorizzare V1 (d.d.p, letta da analogPin_uno)
int V2[256]; //array per memorizzare V2 (d.d.p, letta da analogPin_due)
int start = 1; //flag per fermare il ciclo
int rd = 100; // Il valore della resistenza RD = 100 Ohm


//Inizializzazione
void setup()
{
  pinMode(RampPin, OUTPUT); //pin pwm RampPin configurato come uscita
  Serial.begin(9600); //inizializzazione della porta seriale
}
//Ciclo di istruzioni del programma
void loop()
{
  delay(1000); // attende 1s
  Serial.println("CLEARDATA"); //RIPULISCE IL FOGLIO EXCEL
  Serial.println("CELL,SET,C1"); // SCRIVE "SCARICA DEL CONDENSATORE" NELLA CASELLA C1
  Serial.println("LABEL,tensione[],corrente[A]");
  delay(100);
  //Serial.println("INIZIO LA MISURA");
  for (i = 0; i < 256; i++) //il valore che definisce il duty cycle dell'onda quadra Ã¨ scrivibile su 8 bit
    //cioÃ¨ assume valori da 0 a 256
  {
    analogWrite(RampPin, i); //incrementa il duty cycle di uno step
    V1[i] = analogRead(analogPin_uno); //legge il pin analogPin_uno e crea il vettore
    V2[i] = analogRead(analogPin_due); //legge il pin analogPin_due e crea
    delay(10); // tempo di attesa tra due misure successive in millisecondi= 0.01 s
  }
  //Serial.println("HO FINITO LA MISURA");
  delay(200);

  for (i = 0; i < 256; i++) //nuovo ciclo che scorre gli array di dati e li scrive sulla seriale
  {
    Serial.print("DATA,");
    Serial.print(5.0 * V2[i] / 1024, 4); // il 4 dopo la virgola sta a significare che stampa con 4 cifre decimali
    Serial.print(" , ");
    Serial.println((5.0 / 1.024) * (V1[i] - V2[i]) / rd, 4); // si divide per 1.024 invece che per 1024 per avere la corrente in milliampere invece che in ampere
  }
    Serial.println("CELL,SET,C1,TERMINE ACQUISIZIONE DATI"); 
   // COMUNICA LA FINE DELL'ACQUISIZIONE DATI, NELLA CASELLA C1
  analogWrite(RampPin, 0); // spegne il led
  while (start = 1) {}; // mette Arduino in standby
}
