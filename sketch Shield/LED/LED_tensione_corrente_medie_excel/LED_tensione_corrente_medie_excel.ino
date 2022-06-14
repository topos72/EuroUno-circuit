/*caratteristiche circuito su Shield EURO1
 * -pin 5 uscita PWM per generare la rampa.
 * -switch 1 led giallo; switch 2 verde; switch 3 rosso
 * -resistenza RD = 100 Ohm
 media di N_media valori 
 utilizzo del file excel per visualizzare i dati
 Serial.println("CLEARDATA"); //RIPULISCE IL FOGLIO EXCEL
  Serial.println("CELL,SET,C1"); // SCRIVE "SCARICA DEL CONDENSATORE" NELLA CASELLA C1
  Serial.println("LABEL,tensione[],corrente[A]");
 */
 
const unsigned int RampPin = 5; // porta di uscita Segnale PWM
const unsigned int analogPin_uno = 1;// d.d.p. prima della Rd
const unsigned int analogPin_due = 2; // d.d.p. dopo la Rd
unsigned int i = 0;
float medio_1 = 0;
float medio_2 = 0;
int N_media = 5;
int start = 1; //flag per fermare il ciclo
int rd = 100;  //Resistenza Rd da modificare 
void setup()
{
  pinMode(RampPin, OUTPUT);
  Serial.begin(9600);
}
void loop()
{
  delay(1000); // attende 1s
   Serial.println("CLEARDATA"); //RIPULISCE IL FOGLIO EXCEL
  Serial.println("CELL,SET,C1"); // SCRIVE "SCARICA DEL CONDENSATORE" NELLA CASELLA C1
  Serial.println("LABEL,tensione[],corrente[A]");
  for (i = 0; i < 256; i = i + 4) {
    analogWrite(RampPin, i);
    delay(50);
    //media di 5 letture
    for (int j = 0; j < N_media; j++) {
      medio_2 = medio_2 + analogRead(analogPin_due); 
      medio_1 = medio_1 + analogRead(analogPin_uno);
    }
    medio_2 = medio_2 / N_media; //valor medio di 5 letture
    medio_1 = medio_1 / N_media;
  
    // stampa Tensione media sul LED
    Serial.print("DATA,");
    Serial.print(5.0 * medio_2  / 1023.0, 4);
    //Serial.print(5.0 * (analogRead(analogPin_due)) / 1023.0, 4);
    Serial.print(" , ");

    // stampa IntensitÃ  di corrente media sul LED
   Serial.println((5.0 / 1.023) * ((medio_1 - medio_2)) / rd, 4);
    //  Serial.println((5.0 / 1.023) * ((analogRead(analogPin_uno)) - (analogRead(analogPin_due))) / rd, 4);
    medio_1 = 0;
    medio_2 = 0;
  }
   Serial.println("CELL,SET,C1,TERMINE ACQUISIZIONE DATI");
  delay(100); // lâ€™intervallo di tempo campionamento 
  analogWrite(RampPin, 0);
  while (start = 1) {};
}
