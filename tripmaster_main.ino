#include <Adafruit_GFX.h>
#include <MCUFRIEND_kbv.h>
MCUFRIEND_kbv tft;
#include <QMC5883L.h>
#include <Wire.h>
#include <NMEAGPS.h>
#include <NeoSWSerial.h>


QMC5883L compass;


NMEAGPS               gps;
float                 odometer;
NeoGPS::Location_t    lastLoc;
bool                  lastLocOK = false;
NeoSWSerial           gpsPort(52, 53); //tx, rx gps
const int SHOW_INTERVAL = 1; // 12;
const int INITIAL_SHOW  = (2 * SHOW_INTERVAL) - 1;
      int show          = INITIAL_SHOW;
const int   BUZZER_PIN  = 5;

gps_fix fix = gps.read();



#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF



int refresh = 0;
int refresco = 0 ;
int contador = 0;
int velocidad_actual;
int velocidad_actual_ent;
int line = 0;
int contador2 =0;

const int pulsadorPin = 43;
const int pulsadorPin2 = 45;
int valorPulsador = 0;
int valorPulsador2 = 0;
int pantalla = 0;
int doble_pulsacion = 0;
int boton1 = 0;
int boton2 = 0;

void setup()
{
  //pinMode(pulsadorPin, INPUT);
  
  gpsPort.begin(9600);
  Wire.begin();
  compass.init();
  compass.setSamplingRate(50);
  Serial.begin(9600);
  Serial.println("YEPA!! QUE ESTO EMPIEZA!!");
  Serial.println("GIRAR EN TODAS DIRECCIONES PARA CALIBRAR BRÚJULA");
  tft.reset();
  uint16_t id = tft.readID();
  tft.begin(id);
  tft.setRotation(1);     //APAISADO
  tft.fillScreen(BLACK);
  tft.setTextColor(WHITE, BLACK);
         // FALTA CAMBIAR LA FUENTE
  tft.setCursor(100, 0);

}

void loop()
{

//SUBIR O BAJAR EL TRIP
valorPulsador = digitalRead(pulsadorPin);
valorPulsador2 = digitalRead(pulsadorPin2);




    if (valorPulsador == LOW) {
          boton1 = 1;
  }


  if (boton1 == 1){
          odometer = odometer + 0.1;
          delay(250);
          boton1 = 0;

  }



    if (valorPulsador2 == LOW) {
          boton2 = 1;
  }


  if (boton2 == 1){
          odometer = odometer - 0.1;
          delay(250);
          boton2 = 0;

  }

    




if (valorPulsador2 == LOW && valorPulsador == LOW) {
  doble_pulsacion = 1;
}



//CAMBIAR ENTRE PANTALLAS    
    if (doble_pulsacion == 1 && pantalla == 0) {
    
          pantalla ++;;
          Serial.println(pantalla);
          doble_pulsacion = 0;
          delay(300);
          tft.fillScreen(BLACK);
      } 
       
    if (doble_pulsacion == 1 && pantalla == 1) {
    
          pantalla++;
          Serial.println(pantalla); 
          doble_pulsacion = 0;        
          delay(300);
          tft.fillScreen(BLACK);
      } 

    if (doble_pulsacion == 1 && pantalla == 2) {
    
          pantalla =0;
          Serial.println(pantalla);
          doble_pulsacion = 0;
          delay(300);
          tft.fillScreen(BLACK);
      }

    

//  COMPROBAR SI HAY GPS. FALTA AÑADIR INFO CUANDO ESTÁ BUSCANDO SATÉLITES (O NÚMERO DE SATELITES ENCONTRADOS)
if (gps.available( gpsPort )) {

    
    //   CAMBIA UNA VEZ POR SEGUNDO
    gps_fix fix = gps.read();  // get all the GPS fields
    show = (show + 1) % SHOW_INTERVAL;


                  // ACTUALIZAR EL ODÓMETRO (TRIP)
                  
                  if (fix.valid.location) { // && fix.valid.speed && (fix.speed_kph() > MIN_SPEED)
                    if (lastLocOK) {
                      odometer += fix.location.DistanceKm( lastLoc );
                    }
              
                    // GUARDAR LA POSICIÓN PARA LA PRÓXIMA VEZ
                    
                    lastLoc   = fix.location;
                    lastLocOK = true;
                  }
 

    

    // MONITORIZACIÓN POR SERIAL. QUITAR CUANDO FUNCIONE TODO
    
    Serial.print( F("LAT=") );
    if (fix.valid.location)
      Serial.print( fix.latitude(), 6);
    Serial.print( F(" LON=") );
    if (fix.valid.location)
      Serial.print( fix.longitude(), 6);
    Serial.print( F(" SAT=") );
    if (fix.valid.satellites)
      Serial.print( fix.satellites );
    Serial.print( F(" Velocidad=") );
    if (fix.valid.speed)
      Serial.print( fix.speed_kph() );
      Serial.print( F(" Trip=") );
      Serial.print( odometer );
      Serial.print( F(" km") );

    velocidad_actual = (fix.speed_kph());
    Serial.println();
    
    contador ++;  //ESTO HABRÁ QUE QUITARLO. SOLO ES PARA LA CONDICIÓN DE ENTRADA DE ACTUALIZACIÓN DE VALORES EN PANTALLA
  }






  




 if (contador == 1 && pantalla == 0){  
                                
                                
                              
                              
                              tft.drawLine(10, 90, 470, 90, 222);
                              tft.drawLine(240, 90, 240, 310, 222);
                              tft.setTextSize(7);
                              tft.setTextColor(WHITE, BLACK);
                              tft.setCursor(10, 20);
                              
                              
                              
                              
                              velocidad_actual_ent = int (fix.speed_kph());
                              
                              
                              // DESPUÉS DE CONVERTIR A ENTERO LA VELOCIDAD, LA MOSTRAMOS SOLO SI EL VALOR ES DIFERENTE AL ANTERIOR
                               
                                            if (velocidad_actual != velocidad_actual_ent){
                                              tft.setCursor(10, 20);
                                              tft.fillRect(10, 20, 120, 50, BLACK);
                                              tft.print( velocidad_actual );
                                            
                                             } 
                              
                              tft.setTextSize(3);
                              tft.setCursor(140, 45);
                              tft.println ("km/h");
                              
                              
                              
                              int heading = compass.readHeading();
                              
                              
                              
                              
                              //DIBUJAMOS LA BRÚJULA
                              
                              tft.setTextSize(4);
                              tft.setTextColor(RED, WHITE);
                              tft.setCursor(10, 110); 
                              tft.println(" Rumbo: ");
                              tft.setTextSize(10);
                              tft.setTextColor(WHITE, BLACK);
                              tft.setCursor(20, 200);
                              
                              
                              
                              
                              // PARA QUE LA BRÚJULA SOLO SE REFRESQUE AL CAMBIAR EL NÚMERO DE CIFRAS A MOSTRAR
                              
                                            if (heading < 100 && refresh == 0) {
                                              refresh = 1;
                                              tft.fillRect(20, 200, 170, 80, BLACK); // PARA QUE SE ACTUALICE SOLO UNA PARTE DE LA PANTALLA HAY QUE PONER UN RECTÁNGULO DEL MISMO COLOR QUE EL FONDO DETRÁS DEL TEXTO
                                            }
                                            if (heading >= 100 && refresh ==1) {
                                                refresh = 0;
                                              tft.fillRect(20, 200, 170, 80, BLACK); 
                                            }
                                            
                                            if (heading < 10 && refresco == 0) {
                                              refresco = 1;
                                              tft.fillRect(20, 200, 170, 80, BLACK); 
                                            }
                                            if (heading >= 10 && refresco == 1) {
                                              refresco = 0;
                                              tft.fillRect(20, 200, 170, 80, BLACK); 
                                            }
                              
                              
                              
                              // MOSTRAR VALOR BRÚJULA DESPUÉS DE ACTUALIZAR CUANDO SEA NECESARIO
                              
                              tft.println(heading);
                              Serial.println(heading);
                              
                              tft.setCursor(200, 200);
                              tft.setTextSize(3);
                              tft.println("o");
                              
                              
                              // MOSTRAR DISTANCIA RECORRIDA
                              
                              tft.setTextSize(4);
                              tft.setTextColor(BLUE, YELLOW);
                              tft.setCursor(265, 110);
                              tft.println("Trip: ");
                              tft.setTextSize(2);
                              tft.setTextSize(7);
                              tft.setCursor(270, 200);
                              tft.setTextColor(WHITE, BLACK);
                              tft.println(odometer);
                              
                                  contador = 0;
                                  line++;
                              }


 if (contador == 1 && pantalla == 1){  
  
                                tft.drawLine(10, 90, 470, 90, 222);
                                tft.drawLine(240, 90, 240, 310, 222);
                                tft.setTextSize(7);
                                tft.setTextColor(WHITE, BLACK);
                                tft.setCursor(10, 20);
                                
                                
                                
                                
                                velocidad_actual_ent = int (fix.speed_kph());
                                
                                
                                // DESPUÉS DE CONVERTIR A ENTERO LA VELOCIDAD, LA MOSTRAMOS SOLO SI EL VALOR ES DIFERENTE AL ANTERIOR
                                 
                                              if (velocidad_actual != velocidad_actual_ent){
                                                tft.setCursor(10, 20);
                                                tft.fillRect(10, 20, 120, 50, BLACK);
                                                tft.print( velocidad_actual );
                                              
                                               } 
                                
                                tft.setTextSize(3);
                                tft.setCursor(140, 45);
                                tft.println ("km/h");
                                
                                
                                
                                int heading = compass.readHeading();
                                
                                
                                
                                
                                //DIBUJAMOS LA BRÚJULA
                                
                                tft.setTextSize(4);
                                tft.setTextColor(BLACK, WHITE);
                                tft.setCursor(10, 110); 
                                tft.println(" Rumbo: ");
                                tft.setTextSize(10);
                                tft.setTextColor(WHITE, BLACK);
                                tft.setCursor(20, 200);
                                
                                
                                
                                
                                // PARA QUE LA BRÚJULA SOLO SE REFRESQUE AL CAMBIAR EL NÚMERO DE CIFRAS A MOSTRAR
                                
                                              if (heading < 100 && refresh == 0) {
                                                refresh = 1;
                                                tft.fillRect(20, 200, 170, 80, BLACK); 
                                              }
                                              if (heading >= 100 && refresh ==1) {
                                                  refresh = 0;
                                                tft.fillRect(20, 200, 170, 80, BLACK); 
                                              }
                                              
                                              if (heading < 10 && refresco == 0) {
                                                refresco = 1;
                                                tft.fillRect(20, 200, 170, 80, BLACK); 
                                              }
                                              if (heading >= 10 && refresco == 1) {
                                                refresco = 0;
                                                tft.fillRect(20, 200, 170, 80, BLACK); 
                                              }
                                
                                
                                
                                // MOSTRAR VALOR BRÚJULA DESPUÉS DE ACTUALIZAR CUANDO SEA NECESARIO
                                
                                tft.println(heading);
                                Serial.println(heading);
                                
                                tft.setCursor(200, 200);
                                tft.setTextSize(3);
                                tft.println("o");
                                
                                
                                // MOSTRAR DISTANCIA RECORRIDA
                                
                                tft.setTextSize(4);
                                tft.setTextColor(BLUE, YELLOW);
                                tft.setCursor(265, 110);
                                tft.println("Trip: ");
                                tft.setTextSize(2);
                                tft.setTextSize(7);
                                tft.setCursor(270, 200);
                                tft.setTextColor(WHITE, BLACK);
                                tft.println(odometer);
                                
                                    contador = 0;
                                    line++;
                                }




 if (contador == 1 && pantalla == 2){  
  
                                tft.drawLine(10, 90, 470, 90, 222);
                                tft.drawLine(240, 90, 240, 310, 222);
                                tft.setTextSize(7);
                                tft.setTextColor(WHITE, BLACK);
                                tft.setCursor(10, 20);
                                
                                
                                
                                
                                velocidad_actual_ent = int (fix.speed_kph());
                                
                                
                                // DESPUÉS DE CONVERTIR A ENTERO LA VELOCIDAD, LA MOSTRAMOS SOLO SI EL VALOR ES DIFERENTE AL ANTERIOR
                                 
                                              if (velocidad_actual != velocidad_actual_ent){
                                                tft.setCursor(10, 20);
                                                tft.fillRect(10, 20, 120, 50, BLACK);
                                                tft.print( velocidad_actual );
                                              
                                               } 
                                
                                tft.setTextSize(3);
                                tft.setCursor(140, 45);
                                tft.println ("km/h");
                                
                                
                                
                                int heading = compass.readHeading();
                                
                                
                                
                                
                                //DIBUJAMOS LA BRÚJULA
                                
                                tft.setTextSize(4);
                                tft.setTextColor(BLUE, WHITE);
                                tft.setCursor(10, 110); 
                                tft.println(" Rumbo: ");
                                tft.setTextSize(10);
                                tft.setTextColor(WHITE, BLACK);
                                tft.setCursor(20, 200);
                                
                                
                                
                                
                                // PARA QUE LA BRÚJULA SOLO SE REFRESQUE AL CAMBIAR EL NÚMERO DE CIFRAS A MOSTRAR
                                
                                              if (heading < 100 && refresh == 0) {
                                                refresh = 1;
                                                tft.fillRect(20, 200, 170, 80, BLACK); 
                                              }
                                              if (heading >= 100 && refresh ==1) {
                                                  refresh = 0;
                                                tft.fillRect(20, 200, 170, 80, BLACK); 
                                              }
                                              
                                              if (heading < 10 && refresco == 0) {
                                                refresco = 1;
                                                tft.fillRect(20, 200, 170, 80, BLACK); 
                                              }
                                              if (heading >= 10 && refresco == 1) {
                                                refresco = 0;
                                                tft.fillRect(20, 200, 170, 80, BLACK); 
                                              }
                                
                                
                                
                                // MOSTRAR VALOR BRÚJULA DESPUÉS DE ACTUALIZAR CUANDO SEA NECESARIO
                                
                                tft.println(heading);
                                Serial.println(heading);
                                
                                tft.setCursor(200, 200);
                                tft.setTextSize(3);
                                tft.println("o");
                                
                                
                                // MOSTRAR DISTANCIA RECORRIDA
                                
                                tft.setTextSize(4);
                                tft.setTextColor(BLUE, YELLOW);
                                tft.setCursor(265, 110);
                                tft.println("Trip: ");
                                tft.setTextSize(2);
                                tft.setTextSize(7);
                                tft.setCursor(270, 200);
                                tft.setTextColor(WHITE, BLACK);
                                tft.println(odometer);
                                
                                    contador = 0;
                                    line++;
                                }
}
