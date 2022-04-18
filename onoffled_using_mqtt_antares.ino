  /* made by Kusnh13
  For refrence watch YouTube video:-https://youtu.be/Hgq2KX5w-_o   */

  #include <AntaresESP32MQTT.h>
  #include <WiFi.h>


#define ACCESSKEY "3930e5xxxxx:xxxxxxxx"
#define WIFISSID "Mi Phone"
#define PASSWORD "xxxxxxx"

#define projectName "LedInternalESP32"
#define deviceName "esp32"

  const char* ssid = "Mi Phone"; 
  const char* password = "xxxxxxx"; 

 


AntaresESP32MQTT antares(ACCESSKEY);
WiFiServer server(80);

  String header;
  String ledstatus = "off";
    
  const int ledid =2;
  
  
  void setup() {

  Serial.begin(115200);
  antares.setDebug(true);
  antares.wifiConnection(WIFISSID, PASSWORD);
  antares.setMqttServer();
  pinMode(ledid, OUTPUT);
  digitalWrite(ledid, LOW);
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
  delay(500);
  Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  
  server.begin();
  }
  
  void loop(){
   antares.checkMqttConnection();
  WiFiClient client = server.available(); 
  if (client) {      
  Serial.println("New Client.");
  String currentLine = "";       
  while (client.connected()) {    
  if (client.available()) {   
  char c = client.read();  
  Serial.write(c);          
  header += c;
  if (c == '\n') {     
  if (currentLine.length() == 0) {
  client.println("HTTP/1.1 200 OK");
  client.println("Content-type:text/html");
  client.println("Connection: close");
  client.println();
  
  // turns the GPIOs on and off
  if (header.indexOf("GET /led/on") >= 0) {
  Serial.println("LED on");
  ledstatus = "on";
  digitalWrite(ledid, HIGH);
  } else if (header.indexOf("GET /led/off") >= 0) {
  Serial.println("LED off");
  ledstatus = "off";
  digitalWrite(ledid, LOW);
  } 



  
   client.println("<!DOCTYPE html><html>");
  client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
  client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
  client.println(".button { background-color: #4CAF50; border: none; color: white; padding: 16px 40px;");
  client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
  client.println(".button2 {background-color: #555555;}</style></head>");
  
  client.println("<body><h1>ESP32 Web Server</h1>");
  client.println("<p>Led status " + ledstatus + "</p>");
  
  
  
  
 
  
  client.println();
  break;
  } else {   currentLine = "";  }
  } else if (c != '\r') {   currentLine += c;   }
  }
  
  }
  header = "";
  client.stop();
  Serial.println("Client disconnected.");
  Serial.println("");
    // Add variable to data storage buffer
  antares.add("ledstatus", ledstatus);
 
  
  antares.publish(projectName, deviceName);
  }
  }
