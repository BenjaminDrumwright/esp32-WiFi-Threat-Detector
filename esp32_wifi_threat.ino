#include <WiFi.h>
#include "esp_wifi.h"
#include "BluetoothSerial.h"

BluetoothSerial SerialBT;

#define DEAUTH 0xC0
#define DISASSOC 0xA0
#define PROBE_REQ 0x40
#define BEACON 0x80

// struct for wifi IEEE 802.11 management packet
typedef struct {
	unsigned frame_ctrl:16;
	unsigned duration_id:16;
	uint8_t addr1[6];
	uint8_t addr2[6];
	uint8_t addr3[6];
	unsigned seq_ctrl:16;
	uint8_t payload[];
} wifi_ieee80211_packet_t;


int deauthCount = 0;
int probeCount = 0;
int beaconCount = 0;
bool threatDetected = false;
unsigned long lastStatusTime = 0;

// callback function that is called when a packet is captured
void sniffer_callback(void* buf, wifi_promiscuous_pkt_type_t type) {
	if (type != WIFI_PKT_MGMT) return;

	wifi_promiscuous_pkt_t* pkt = (wifi_promiscuous_pkt_t*)buf;
	wifi_ieee80211_packet_t* ipkt = (wifi_ieee80211_packet_t*)pkt->payload;

	// extracts the frame subtype
	uint8_t subtype = ((ipkt->frame_ctrl) >> 4) & 0xF0;

	// checks which attack is being used based on subtype
	switch (subtype) { 
		case DEAUTH:
			deauthCount++;
			SerialBT.println("Deauth Attack Detected!");
			threatDetected = true;
			break;

		case DISASSOC:
			SerialBT.println("Disassociation Attack!");
			threatDetected = true;
			break;

		case PROBE_REQ:
			probeCount++;
			if (probeCount > 100) { // if over 100 probe request are found it flags it
				SerialBT.println("Probe Request Flood!");
				probeCount = 0;
				threatDetected = true;
			}
			break;

		case BEACON:
			beaconCount++;
			if (beaconCount > 100) {
				SerialBT.println("Fake AP Beacon Flood!");
				beaconCount = 0;
				threatDetected = true;
			}
			break;
	}
}

void setup() {
	SerialBT.begin("ESP32-WiFiMonitor"); // starts bluetooth communication
	WiFi.mode(WIFI_MODE_NULL); // disables normal wifi operations
	esp_wifi_set_promiscuous(true); // enables promiscuous mode on esp32 
	esp_wifi_set_promiscuous_rx_cb(&sniffer_callback); // set packet callback
	SerialBT.println("Bluetooth Threat Monitor Started"); // prints to serial that monitoring has started
}

void loop() {
	// checks for threats every 5 second, prints out status
	if (millis() - lastStatusTime > 5000) {
		if (!threatDetected) {
			SerialBT.println("No active threats...");
		}
		threatDetected = false;
		lastStatusTime = millis();
	}
}

