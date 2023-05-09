#include <ESP8266WiFi.h>
#include <pitches.h>

#define BUTTON_PIN 13
#define TONE_PIN 12
#define LED_PIN 14

int port = 8080;  //Port number
WiFiClient client;
const char* server_address = "10.42.0.1";
const int server_port = 8080;

// Server connect to WiFi Network
const char *ssid = "tableWifi";  //Enter your wifi SSID
const char *password = "1234Blumenk0hl";  //Enter your wifi Password

int speed = 30;
int connect_melody[] = {NOTE_C5, 0, NOTE_A4, NOTE_F4, NOTE_E5, END};
int connect_duration[] = {4,4,4,4,4,};
int client_connect_melody[] = {NOTE_F4, NOTE_C5, END};
int client_connect_duration[] = {4,4,};
int client_disconnect_melody[] = {NOTE_C5, NOTE_F4, END};
int client_disconnect_duration[] = {4,4,};

//=======================================================================
//                    Power on setup
//=======================================================================

void playMelody(int* melody,int* durations)
{
  for (int thisNote = 0; melody[thisNote]!=-1; thisNote++) {
    int noteDuration = speed*durations[thisNote];
    tone(TONE_PIN, melody[thisNote],noteDuration*.95);
    delay(noteDuration);
    noTone(TONE_PIN);
  }
}

void setup()
{
  pinMode(LED_PIN, OUTPUT); // LEDs

  Serial.begin(115200);

  pinMode(BUTTON_PIN,INPUT_PULLUP);  //Btn to send data
  Serial.println();

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password); //Connect to wifi

  // Wait for connection
  Serial.println("Connecting to Wifi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(200);
    digitalWrite(LED_PIN, HIGH);
    Serial.print(".");
    delay(200);
    digitalWrite(LED_PIN, LOW);
  }

  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);

  //playMelody(connect_melody, connect_duration);
}
//=======================================================================
//                    Loop
//=======================================================================

void loop()
{
  Serial.print("connecting to ");
  Serial.print(server_address);
  Serial.print(':');
  Serial.println(server_port);
  if (!client.connect(server_address, server_port)) {
    Serial.println("connection failed");
    delay(1000);
    digitalWrite(LED_PIN, HIGH);
    delay(1000);
    digitalWrite(LED_PIN, LOW);
    return;
  }

  if (client) {
    if(client.connected())
    {
      Serial.println("Client Connected");
      //playMelody(client_connect_melody, client_connect_duration);
      for (int i=0; i<4; i++)
      {
        delay(100);
        digitalWrite(LED_PIN, HIGH);
        delay(100);
        digitalWrite(LED_PIN, LOW);
      }
    }

    bool prev_button = 0;

    while(client.connected()){
      bool button_pressed = digitalRead(BUTTON_PIN); // 0 if pressed

      if(button_pressed == 0 and prev_button == 0) {
        digitalWrite(LED_PIN, HIGH);
        client.write("Pressed");
        prev_button = 1;
      }
      else if (button_pressed == 1 and prev_button == 1) {
        prev_button = 0;
        digitalWrite(LED_PIN, LOW);
        client.write("Released");
      }
      delay(10);
    }
    client.stop();
    Serial.println("Client disconnected");
    //playMelody(client_disconnect_melody, client_disconnect_duration);
  }

  // LEDs should still work even if no client is connected
  bool button_pressed = digitalRead(BUTTON_PIN); // 0 if pressed
  if(button_pressed == 0) {
    digitalWrite(LED_PIN, HIGH);
  }
  else {
    digitalWrite(LED_PIN, LOW);
  }
  delay(10);
}
//=======================================================================
