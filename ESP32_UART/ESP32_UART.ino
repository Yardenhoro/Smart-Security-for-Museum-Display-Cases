#include <WiFi.h>
#include <ArduinoHttpClient.h>
//#include <HTTPClient.h>
#define RXp2 16
#define TXp2 17

// WiFi credentials
//const char* ssid = "BeSpotA928_2.4";
//const char* password = "0903A928";
const char* ssid = "Yarden";
const char* password = "yardenhoro";

// ThingsBoard server settings
const char* tbServer = "demo.thingsboard.io";
const int tbPort = 80;
const char* tbAccessToken = "Bearer eyJhbGciOiJIUzUxMiJ9.eyJzdWIiOiJ5YXJkZW4yMThAZ21haWwuY29tIiwidXNlcklkIjoiODhmODZlMzAtMGEwOS0xMWVlLTg2MGYtZTFjYjgyYjU5ZTU2Iiwic2NvcGVzIjpbIlRFTkFOVF9BRE1JTiJdLCJzZXNzaW9uSWQiOiI0MTM2MmYxYi1mZTYxLTQ0ODAtYjA1MS01ODcxZDRiNjRlYjgiLCJpc3MiOiJ0aGluZ3Nib2FyZC5pbyIsImlhdCI6MTY4NjY3NDY3NSwiZXhwIjoxNjg4NDc0Njc1LCJmaXJzdE5hbWUiOiLXmdeo15PXnyIsImxhc3ROYW1lIjoi15TXldeo15XXkdeZ16UiLCJlbmFibGVkIjp0cnVlLCJwcml2YWN5UG9saWN5QWNjZXB0ZWQiOnRydWUsImlzUHVibGljIjpmYWxzZSwidGVuYW50SWQiOiI4NmU5YjI3MC0wYTA5LTExZWUtODYwZi1lMWNiODJiNTllNTYiLCJjdXN0b21lcklkIjoiMTM4MTQwMDAtMWRkMi0xMWIyLTgwODAtODA4MDgwODA4MDgwIn0.rGKQco6DGVPWwnFaRZY4stUNPANstCPWSXcq8R-ZpppYfJWcyhXovZTVtjAAEQUqXCBCNs9hwOcaabI_8A38uQ";

// ThingsBoard telemetry endpoint
const char* telemetryEndpoint = "/api/v1/QouS0ctygxbTxJyM8uIR/telemetry";

// Data to be sent to ThingsBoard
String uartString;
const char* payload;
//const char* payload = "{\"temperature\": 25.5, \"humidity\": 50.2, \"Ultrasonic\": 14.2}";
//const char* payload = "{\"Ultrasonic\": 25.5, \"Magnet\": 50.2, \"Light\": 14.2}";


WiFiClient wifiClient;
HttpClient httpClient(wifiClient, tbServer, tbPort);

void setup() {
  Serial.begin(9600);
  Serial2.begin(9600, SERIAL_8N1, RXp2, TXp2);
  connectToWiFi();
}

void loop() {
    // Check if connected to Wi-Fi, retry connection if necessary
  if (WiFi.status() != WL_CONNECTED) {
    connectToWiFi();
  }
  Serial.println("Message Received From Arduino By UART: ");
  Serial.println(Serial2.readString());
  uartString = Serial2.readString();
  payload = uartString.c_str();  // Converting String to const char*

  // Send telemetry data to ThingsBoard
  sendTelemetry(payload);

  delay(5000); // Send data every 5 seconds
}

void connectToWiFi() {
  Serial.println("Connecting to WiFi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi!");
}

void sendTelemetry(const char* data) {
  httpClient.beginRequest();
  httpClient.post(telemetryEndpoint);
  httpClient.sendHeader("Content-Type", "application/json");
  httpClient.sendHeader("X-Authorization", tbAccessToken);
  httpClient.sendHeader("Content-Length", String(strlen(data)));
  httpClient.beginBody();
  httpClient.print(data);
  httpClient.endRequest();

  int httpResponseCode = httpClient.responseStatusCode();

  if (httpResponseCode > 0) {
    Serial.print("Telemetry data sent, response code: ");
    Serial.println(httpResponseCode);
  } else {
    Serial.print("Error sending telemetry data, response code: ");
    Serial.println(httpResponseCode);
  }
}



