  #include <avr/interrupt.h>
  #include <LiquidCrystal.h>;
    #include<Servo.h>;
  
  
  LiquidCrystal lcd(12,11,7,6,5,4);
  int test=0;
  int numeropassage=0;
  int num=0;
  int hlu=0;
  int h=0;
  int mlu=0;
  int m=0;
  int com=0;
  int hfi=0;
  unsigned i=0;
    int ledCarte=13;
    int ledBoite=3;
  String affichage;
    Servo fanion;
    int servoPin=9;
    String passage[50];
  int compteur;
  int numero=0;
  unsigned long cpt = 0;
  
  void setup(void)
  {
    Serial.begin(9600);
    for(compteur=0;compteur<50;compteur++){
     passage[compteur]="                "; 
    }
    lcd.begin(16, 2);
    lcd.print("Initialisation");
    lcd.setCursor(0,1);
    lcd.print("en cours");
    pinMode(13,OUTPUT);
    pinMode(2,INPUT);
    pinMode(8,INPUT);
    pinMode(10,INPUT);
     pinMode(servoPin,OUTPUT);
     pinMode(ledBoite,OUTPUT);
      fanion.attach(servoPin);
      fanion.write(90);
    attachInterrupt(0,remiseAZero,FALLING);
    Serial.print("Saisisez juste l'heure"); 
    while(h>=0 && h<=2){
      while ( Serial.available() ) {
        hlu = Serial.read();
        if ( estUnNombre(hlu) ) {
          h = consersionEtSomme(h, hlu);
        }
      }
    }
    Serial.print("\n");
    Serial.print("Saisisez juste les minutes"); 
    while(m>=0 && m<=6){
      while ( Serial.available() ) {
        mlu = Serial.read();
        if ( estUnNombre(mlu) ) {
          m = consersionEtSomme(m, mlu);
        }
      }
    }
  
    i=(h*3600)+(m*60);
    lcd.clear();
    initClock();
  
  }
  void loop(void)
  {
  
    if(cpt>=10000){
      digitalWrite(13,HIGH);
      cpt=0;
      i++;
      lcd.setCursor(0,0);
      lcd.print("        ");
      lcd.setCursor(0,0);
      lcd.print(conversionAffichage(i));
    } 
    else{
      digitalWrite(13,LOW);     
    }
    if(test==0){
    if(analogRead(A0)<700)
    {
      
      digitalWrite(ledBoite,HIGH);
          test=1;
          fanion.write(180);
      lcd.setCursor(0,1);
      //lcd.print("Passage:");
      passage[numeropassage]="Passage:"+conversionAffichage(i);
      numero=numeropassage;
      numeropassage++;
      afficher();
      Serial.print(passage[numero]);
    }
    }
  while(digitalRead(8)==LOW){
    if(com==0){
      numero++;
      afficher();
      com=1;
    }
    
   }
   com=0;
    while(digitalRead(10)==LOW){
    if(com==0){
      numero--;
      afficher();
      com=1;
    }
    
    }
    com=0;
    
  
  }
  void afficher(){
          lcd.setCursor(0,1);
    lcd.print("                ");
    lcd.setCursor(0,1);
     lcd.print(passage[numero]);
  }
  boolean estUnNombre(int nb)
  {
    return nb >= 48 && nb <= 57;
  }
  int consersionEtSomme(int h, int hlu)
  {
    return h*10+(hlu-48);
  }
  
  
  String conversionAffichage(unsigned i){
    int h;
    int m;
    int s;
    String retour;
    h=i/3600;
    m=(i%3600)/60;
    s=((i%3600)%60);
    retour.concat(h);
    retour.concat(":");
    retour.concat(m);
    retour.concat(":");
    retour.concat(s);
    return retour;
  
  }
  
  static void initClock() {
    TCCR2A = 0;
    // normal counting mode
    TCCR2B = _BV(CS21);
    // set prescaler of 8
    OCR2A = ( clockCyclesPerMicrosecond() * 100) / 8;
    TCNT2 = 0;
    // clear the timer count
    TIFR2 |= _BV(OCF2A);
    // clear any pending interrupts;
    TIMSK2 |= _BV(OCIE2A) ;
    // enable the output compare interrupt
  }
  
  // called every 0,1ms
  ISR(TIMER2_COMPA_vect)
  {
    TCNT2 = 0;
    OCR2A = ( clockCyclesPerMicrosecond() * 100) / 8;
    cpt++;
  
  }
  
    /*
    ---------------------------------------------
      Fonciton executer quand on appuie sur le 
              bouton de remise a 0
    --------------------------------------------
    */
    void remiseAZero(){
      digitalWrite(ledBoite,LOW);
      test=0;
      fanion.write(90);
      lcd.setCursor(0,1);
      lcd.print("                ");
    }

