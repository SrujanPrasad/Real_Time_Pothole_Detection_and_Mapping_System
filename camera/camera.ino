#include <WiFi.h>
#include <WebServer.h>

const char* ssid = "srujan";
const char* password = "12345678";

// Replace with your phone’s actual IP and port from IP Webcam app
const char* phoneCamURL = "http://10.88.250.163:8080/shot.jpg";

WebServer server(80);

void handleRoot() {
  String html = "<!DOCTYPE html><html><head><title>Phone Camera Snapshot</title></head><body>";
  html += "<h2>Live Snapshot from Phone</h2>";
  html += "<img src='" + String(phoneCamURL) + "' width='320' id='snap'>";
  html += "<p>This snapshot refreshes every 3 seconds.</p>";
  html += "<script>setInterval(() => { document.getElementById('snap').src = '" + String(phoneCamURL) + "?t=' + new Date().getTime(); }, 3000);</script>";
  html += "</body></html>";

  server.send(200, "text/html", html);
}

void setup() {
  Serial.begin(115200);
  Serial.println("Starting ESP32...");

  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi connected!");
  Serial.print("ESP32 IP address: ");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);
  server.begin();
  Serial.println("Web server started.");
  Serial.print("Open in browser: http://");
  Serial.println(WiFi.localIP());
}

void loop() {
  server.handleClient();
}
