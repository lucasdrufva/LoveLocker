#include "network.h"

extern Storage storage;

void Network::begin()
{
    WiFiManager wm;
    wm.autoConnect();
    delay(1000);
    Serial.println("ESP32 is connected to Wi-Fi network");

    auth = "Basic " + base64::encode(storage.getChipId() + ":" + storage.getPassword());

    if (!checkRegistred())
    {
        registerDevice();
    }
    Serial.println("Check again");
    checkRegistred();
}

void Network::registerDevice()
{
    HTTPClient http;

    http.begin(baseUrl + "/device/register/" + storage.getChipId());

    int httpCode = http.POST("");

    if (httpCode == 201)
    { //Check for the returning code

        String payload = http.getString();
        Serial.println(httpCode);
        Serial.println(payload);

        storage.storePassword(payload);
        auth = "Basic " + base64::encode(storage.getChipId() + ":" + payload);
    }
    else
    {
        Serial.println("Error on HTTP request");
    }

    http.end();

    //Show setup code on display
    xTaskNotify(displayTaskHandle, DISPLAY_EVENT_CONFIG, eSetValueWithOverwrite);
}

bool Network::checkRegistred()
{
    if ((WiFi.status() == WL_CONNECTED))
    { //Check the current connection status

        HTTPClient http;

        http.begin(baseUrl + "/client");
        http.addHeader("Authorization", auth);

        int httpCode = http.GET();

        if (httpCode == 200)
        { //Check for the returning code
            String payload = http.getString();
            Serial.println(httpCode);
            Serial.println(payload);
            http.end();
            return true;
        }
        else
        {
            Serial.println("Error on HTTP request");
        }

        http.end();
    }
    return false;
}

StatusUpdate Network::getStatus()
{
    StatusUpdate message;
    HTTPClient http;

    http.begin(baseUrl + "/device/status");
    http.addHeader("Authorization", auth);

    int httpCode = http.GET();

    if (httpCode == 200)
    { //Check for the returning code
        String payload = http.getString();

        DynamicJsonDocument doc(1024);

        deserializeJson(doc, payload);

        message.statusId = doc["id"];
        message.type = doc["type"];
        message.contentId = doc["contentId"];
        message.notifier = doc["notifier"];
    }
    else
    {
        message.statusId = 0;
        Serial.println("Error on HTTP request");
    }

    http.end();
    return message;
}

TextStatus Network::getText(int contentId){
    TextStatus response;
    HTTPClient http;

    http.begin(baseUrl + "/text/" + String(contentId));
    http.addHeader("Authorization", auth);

    int httpCode = http.GET();

    if (httpCode == 200)
    { //Check for the returning code
        String payload = http.getString();

        DynamicJsonDocument doc(1024);

        deserializeJson(doc, payload);

        response.text = doc["text"].as<String>();
        response.color = doc["color"].as<uint16_t>();
        response.backgroundColor = doc["backgroundColor"].as<uint16_t>();
        response.size = doc["size"].as<uint8_t>();
        
    }
    else
    {
        Serial.println("Error on HTTP request");
    }

    http.end();
    return response;
}

//template <typename T, size_t N>
void byteToWord(uint8_t byteArray[], uint16_t* wordArray, int len)
{
  Serial.println("convert bytes to words");
  int j = 0;
  for (int i = 1; i < len; i += 2)
  {
    wordArray[j] = byteArray[i + 1] + (byteArray[i] << 8);
    j++;
  }
}

//template <typename T, size_t N>
void Network::getImagePart(int contentId, int part, uint16_t* onlineImage){
    HTTPClient http;

    // Your Domain name with URL path or IP address with path
    http.begin(baseUrl + "/image/" + String(contentId) + "/part/" + String(part));

    // Send HTTP GET request
    int httpResponseCode = http.GET();

    if (httpResponseCode > 0)
    {
      Serial.println(part);
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
      WiFiClient &stream = http.getStream();
      uint8_t testIn[4800] = {0};
      stream.readBytes(testIn, 4800);
      byteToWord(testIn, onlineImage, 4800);
    }
    else
    {
      Serial.print("Error code: ");
      Serial.println(httpResponseCode);
    }
    http.end();
}

void Network::reportSeen(int statusId){
    HTTPClient http;

    http.begin(baseUrl + "/device/status/" + String(statusId) + "/seen");
    http.addHeader("Authorization", auth);

    int httpCode = http.PUT("");

    if (httpCode == 200){
        Serial.println("Opened reported");
    }else {
        Serial.println("Failed to report openenig");
    }

    http.end();
}

void Network::sendTouch(int size){
    HTTPClient http;

    http.begin(baseUrl + "/device/touch?size=" + String(size));
    http.addHeader("Authorization", auth);

    int httpCode = http.PUT("");

    if (httpCode == 200){
        Serial.println("touch sent");
    }else {
        Serial.println("Failed to send touch");
    }
    http.end();
}