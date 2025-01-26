#include "wifi_module.h"
#include "WiFi.h"
#include <HardwareSerial.h>

String wifiSID = "";
String wifiPW = "";

void wifi_init(String ssid, String password)
{
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);
  wifiSID = ssid;
  wifiPW = password;
  Serial.println("[WiFi]\t setup done.");
}

void wifi_config()
{
  // SCAN STARTS
  Serial.println("[WiFi]\t Scan starts...");
  // WiFi.scanNetworks will return the number of networks found.
  int n = WiFi.scanNetworks();
  if (n == 0)
  {
    Serial.println("[WiFi]\t no networks found");
  }
  else
  {
    Serial.print("[WiFi]\t ");
    Serial.print(n);
    Serial.println(" networks found");
    Serial.println("Nr | SSID                             | RSSI | CH | Encryption");
    for (int i = 0; i < n; ++i)
    {
      // Print SSID and RSSI for each network found
      Serial.printf("%2d", i + 1);
      Serial.print(" | ");
      Serial.printf("%-32.32s", WiFi.SSID(i).c_str());
      Serial.print(" | ");
      Serial.printf("%4ld", WiFi.RSSI(i));
      Serial.print(" | ");
      Serial.printf("%2ld", WiFi.channel(i));
      Serial.print(" | ");
      switch (WiFi.encryptionType(i))
      {
      case WIFI_AUTH_OPEN:
        Serial.print("open");
        break;
      case WIFI_AUTH_WEP:
        Serial.print("WEP");
        break;
      case WIFI_AUTH_WPA_PSK:
        Serial.print("WPA");
        break;
      case WIFI_AUTH_WPA2_PSK:
        Serial.print("WPA2");
        break;
      case WIFI_AUTH_WPA_WPA2_PSK:
        Serial.print("WPA+WPA2");
        break;
      case WIFI_AUTH_WPA2_ENTERPRISE:
        Serial.print("WPA2-EAP");
        break;
      case WIFI_AUTH_WPA3_PSK:
        Serial.print("WPA3");
        break;
      case WIFI_AUTH_WPA2_WPA3_PSK:
        Serial.print("WPA2+WPA3");
        break;
      case WIFI_AUTH_WAPI_PSK:
        Serial.print("WAPI");
        break;
      default:
        Serial.print("unknown");
      }
      Serial.println();
      delay(10);
    }
  }
  Serial.println("");
  // SCAN ENDS

  Serial.print("[WiFi]\t Enter wifi ID to connect:\t");
  while (Serial.available() == 0)
  {
  }
  int inputInt = Serial.parseInt();
  Serial.print("[WiFi]\t Selected:\t");
  Serial.printf(WiFi.SSID(inputInt - 1).c_str());
  Serial.println("");
  wifiSID = WiFi.SSID(inputInt - 1).c_str();
  // clear leftovers
  while (Serial.available() > 0)
  {
    Serial.read();
  }
  Serial.print("[WiFi]\t Input password:\t");
  while (Serial.available() == 0)
  {
  }
  Serial.println("");
  String password = Serial.readString();
  password.trim();
  wifiPW = password;
  // clear leftovers
  while (Serial.available() > 0)
  {
    Serial.read();
  }
}

void wifi_connect()
{
  int tryDelay = 500;
  int numberOfTries = 20;

  WiFi.begin(wifiSID, wifiPW);

  // Wait for the WiFi event
  while (true)
  {

    switch (WiFi.status())
    {
    case WL_NO_SSID_AVAIL:
      Serial.println("[WiFi]\t SSID not found");
      break;
    case WL_CONNECT_FAILED:
      Serial.print("[WiFi]\t Failed - WiFi not connected! Reason: ");
      return;
      break;
    case WL_CONNECTION_LOST:
      Serial.println("[WiFi]\t Connection was lost");
      break;
    // case WL_SCAN_COMPLETED:  Serial.println("[WiFi]\t Scan is completed"); break;
    // case WL_DISCONNECTED:    Serial.println("[WiFi]\t WiFi is disconnected"); break;
    case WL_CONNECTED:
      Serial.println("[WiFi]\t WiFi is connected!");
      Serial.print("[WiFi]\t IP address: ");
      Serial.println(WiFi.localIP());
      return;
      break;
    default:
      // Serial.print("[WiFi]\t WiFi Status: ");
      // Serial.println(WiFi.status());
      break;
    }
    delay(tryDelay);

    if (numberOfTries <= 0)
    {
      Serial.print("[WiFi]\t Failed to connect to WiFi!");
      // Use disconnect function to force stop trying to connect
      WiFi.disconnect();
      return;
    }
    else
    {
      numberOfTries--;
    }
  }
}

void wifi_off()
{
  WiFi.mode(WIFI_OFF);
  WiFi.disconnect(true);
  Serial.println("[WiFi]\t Wi-Fi is OFF.");
}

void wifi_on()
{
  WiFi.mode(WIFI_STA);
  wifi_connect();
  Serial.println("[WiFi]\t Wi-Fi is ON.");
}