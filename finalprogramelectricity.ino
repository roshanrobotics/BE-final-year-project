#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>

#include <MCP3008.h>

#define CS_PIN D8
#define CLOCK_PIN D5
#define MOSI_PIN D7
#define MISO_PIN D6

MCP3008 adc(CLOCK_PIN, MOSI_PIN, MISO_PIN, CS_PIN);

//WiFiServer server(80);


const char *host = "192.168.43.13";

const char* ssid = "COGNITIOR";
const char* password = "RKCOGNIDS19";

int Led_OnBoard = 2; 
const int Sensor_Pin = A0;
unsigned int Sensitivity = 66;   // 185mV/A for 5A, 100 mV/A for 20A and 66mV/A for 30A Module

float Vpp = 0; // peak-peak v
float Vrms = 0; // rms v
float current = 0; // rms irms

float Vcc = 5;         // ADC reference v // v at 5V pin
float power = 0;         // power in watt
float Wh = 0 ;            // Energy in kWh
unsigned long last_time = 0;
unsigned long current_time = 0;
unsigned long interval = 100;

unsigned int pF = 85;


int inval = adc.readADC(1); // read Chanel 0 from MCP3008 ADC
//int inval = analogRead(0);
float voltage = (inval * 0.2453); // converts analog value(x) into input ac supply value using this formula


int cloud = 0;
int httpResponseCode ;


void setup() {
  Serial.begin(115200);
  delay(1000);
   Serial.println();
  Serial.println();
  Serial.println();

  
  
  pinMode(Led_OnBoard, OUTPUT);       // Initialize the Led_OnBoard pin as an output
  Serial.begin(115200);
  WiFi.mode(WIFI_OFF);        //Prevents reconnection issue (taking too long to connect)
  delay(1000);
  WiFi.mode(WIFI_STA);        //This line hides the viewing of ESP as wifi hotspot
  
  WiFi.begin(ssid, password);     //Connect to your WiFi router
  WiFi.config(IPAddress(192, 168, 43, 13), IPAddress(192, 168, 1, 1), IPAddress(255, 255, 255, 0));
  Serial.println("");

  Serial.print("Connecting");
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    digitalWrite(Led_OnBoard, LOW);
    delay(250);
    Serial.print(".");
    digitalWrite(Led_OnBoard, HIGH);
    delay(250);
  }

  digitalWrite(Led_OnBoard, HIGH);
  //If connection successful show IP address in serial monitor
  Serial.println("");
  Serial.println("Connected to Network/SSID");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());  //IP address assigned to your ESP
 
}



float getVPP()
{
  float result;
  int readValue;
  int maxValue = 0;
  int minValue = 1024;
  uint32_t start_time = millis();
  while ((millis() - start_time) < 950) //read every 0.95 Sec
  {
    readValue = analogRead(Sensor_Pin);
    if (readValue > maxValue)
    {
      maxValue = readValue;
    }
    if (readValue < minValue)
    {
      minValue = readValue;
    }
  }
  result = ((maxValue - minValue) * Vcc) / 1024.0;
  return result;
}



void loop() {


  Vpp = getVPP();
  Vrms = ((Vpp / 2.0) * 0.707);
  current = ((Vrms * 1000) / Sensitivity) ;
  if ((current > -0.015) && (current < 0.300)) { // remove low end chatter
    current = 0.0;
  }
  //Serial.print(voltage);
  power = (voltage * current) * (pF / 100.0);
  last_time = current_time;
  current_time = millis();
  Wh = Wh +  power * (( current_time - last_time) / 3600000.0) ; // calculating energy in Watt-Hour
  Serial.print("current:  ");
  Serial.print(String(current, 3));
  Serial.println(" A");
  Serial.print("voltage: ");
  Serial.print(String(voltage, 3));
  Serial.println(" V");
  Serial.print("Power: ");
  Serial.print(String(power, 3));
  Serial.println(" W");
  delay(100);
  // Serial.println(String("P = ") + power + " Watts");

  // delay(1000);
 
        // Your Domain name with URL path or IP address with path
        /*http.begin("https://electricityiotmanagement.000webhostapp.com/add.php?""&irms="+ String(irms) + "&v="+String(v) + "&power="+ String(power)); //HTTP
          http.addHeader("Content-Type", "application/x-www-form-urlencoded");
          httpResponseCode = http.GET();
          if (httpResponseCode>0) {
          Serial.print("HTTP Response code: ");
          Serial.println(httpResponseCode);
          }
          else {
          Serial.print("Error code: ");
          Serial.println(httpResponseCode);
          }*/
        // Free resources   //Declare object of class HTTPClient
 HTTPClient http;
      
        String currentPost, voltagePost, powerPost, postData;

        currentPost = String(current);   //String to interger conversion
        voltagePost = String(voltage);   //String to interger conversion
        powerPost = String(power);   //String to interger conversion

        postData = "currentval=" + currentPost + "&voltageval=" + voltagePost + "&powerval=" + powerPost;

        http.begin("http://192.168.43.13/nodmecu_dbrecord_view/InsertDB.php");               //Specify request destination
        http.addHeader("Content-Type", "application/x-www-form-urlencoded");                  //Specify content-type header

        int httpCode = http.POST(postData);   //Send the request
      //  String payload = http.getString();    //Get the response payload
    
    if(httpCode > 0){
      String payload = http.getString();
      Serial.print("PAYLOAD: ");
      Serial.println(payload);
    }
        //Serial.println("LDR Value=" + ldrvalue);
       // Serial.print("httpCode");
     //   Serial.println(httpCode);   //Print HTTP return code
      //    Serial.print("payload"); 
      //  Serial.println(payload);    //Print request response payload
       // Serial.println("current= " + currentPost + "voltage= " + voltagePost + "power= " + powerPost);


        http.end();
         delay(4000); 
       digitalWrite(Led_OnBoard, LOW);
  delay(1000);
  digitalWrite(Led_OnBoard, HIGH);
    

    // Specify content-type header
  
 /* //Web page
  WiFiClient client = server.available();
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println("Refresh: 5");
  client.println("");
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");
  client.println("<body style=background-color:skyblue> </body>");
  client.println("<style> h1 {text-align: center}</style>");
  client.println("<style> h3 {text-align: center}</style>");
  client.println("<style> b {text-align: center}</style>");
  client.println("<style> p {text-align: center}</style>");
  client.println("<head><style>div.relative{position:relative; left:200px; height:20px; width:350px;border:2px solid #73AD21;}</style></head>");
  client.println("<h1>Priyadarshini College of Engineering Nagpur </h1>");
  client.println("<h1>IOT based smart electricity management system </h1>");
  client.println("<h3> Current consumption <h3>");
  client.print(current);
  client.print("<b> Ampere </b>");
  client.println("<h3> Voltage consumption <h3>");
  client.print(voltage);
  client.print("<b> Volts</b>");
  client.println("<h3> Power consumption <h3>");
  client.print(power);
  client.print("<b> Watt </b>");
  client.println("");
  /*f(x==1 )
    {
    client.println("<h3> irms increase </h3>");
    }
    if(irms > 1 )
    {
    client.println("<h3> irms high </h3>");
    }
    client.println("</html>");
  */
  //Web page end
  //Send an HTTP POST request every 30 seconds
  //delay(1000);

}
