#include <ESP8266WiFi.h>

#define LED0     D3        
#define LED1     D1        
#define LED2     D2        
#define LEDx     D0 

const int AnalogIn  = A0;
float suhu = 0;
int bpm = 0;
int bpmx = 0;
int x = 0;
float resp = 0;



const char* ssid = "EVERCOSS WIFI";
const char* password = "12345678";

//int ledPin = D0; 
WiFiServer server(80);

void setup() {

  pinMode(LED0 , OUTPUT);
  pinMode(LED1 , OUTPUT);
  pinMode(LED2 , OUTPUT);
  pinMode(LEDx , OUTPUT);
         
  Serial.begin(115200);
  delay(10);

//  pinMode(ledPin, OUTPUT);
//  digitalWrite(ledPin, LOW);

  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");

  // Start the server
  server.begin();
  Serial.println("Server started");

  // Print the IP address
  Serial.print("Use this URL to connect: ");
  Serial.print("http://api.thingspeak.com/update?api_key=LW27DQKN3VRJHK8K&field1=0");
  Serial.print(WiFi.localIP());
  Serial.println("/");

}

void loop() {
  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) {
    return;
  }

  // Wait until the client sends some data
  Serial.println("new client");
  while(!client.available()){
    delay(1);
  }

  // Read the first line of the request
  String request = client.readStringUntil('\r');
  Serial.println(request);
  client.flush();

  // Match the request

digitalWrite(LED0, LOW);                         
digitalWrite(LED1, LOW);
digitalWrite(LED2, LOW);
  
suhu = analogRead(AnalogIn);
suhu = suhu * (3.3 / 1023.0);
suhu = suhu * 100.0;

Serial.print("suhu = ");
Serial.println(suhu);
   
delay(10);                         

digitalWrite(LED0, HIGH);                         
digitalWrite(LED1, LOW);
digitalWrite(LED2, LOW);

for (int i=0; i <= 300; i++){

bpm = analogRead(AnalogIn);
Serial.print("bpm = ");
Serial.println(bpm);

if((bpm > 600)&&(x > 1)){
  digitalWrite(LEDx, LOW);
  x = 0;
  bpmx = bpmx + 1;
  }

else if((bpm < 600)&&(x < 1)){
  x = 2;
  digitalWrite(LEDx, HIGH);
  }

delay(200);   

}

delay(10);                         


digitalWrite(LED0, LOW);                         
digitalWrite(LED1, HIGH);
digitalWrite(LED2, LOW);
  
resp = analogRead(AnalogIn);
Serial.print("Respirasi = ");
Serial.println(resp);
   
delay(10);                         
               

// Set ledPin according to the request
//digitalWrite(ledPin, value);

  // Return the response
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println("Refresh: 1");
  client.println(""); //  do not forget this one
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");
    
  client.println("<center>");
  client.println("<h1>");
  client.println("Monitoring Kondisi Tubuh");
  client.println("</h1>");
    
  client.println("<h3>");
    
  client.println("Suhu = ");
  client.println(suhu);
  client.println(" C");
  client.println("<br>");
   
  client.println("Bpm = ");
  client.println(bpmx);
  client.println("<br>");
  
  client.println("Respirasi = ");
  client.println(resp);
  client.println("<br>");
  
  client.println("</h3>");   
  client.println("</center>"); 
  client.println("</html>");

  delay(1);
  Serial.println("Client disonnected");
  Serial.println("");

  bpmx = 0;

}

