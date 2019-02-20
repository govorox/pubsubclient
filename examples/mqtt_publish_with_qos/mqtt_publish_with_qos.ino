/*
 Publish MQTT with QoS=1 example

 This sketch demonstrates how to publish to a server with QoS
 (Quality of Service) = 1 (message is received at least once).
 It verifies that a published message is successfully received
 before sending another message. This is useful if the client 
 requires guaranteed delivery of data to the server.
*/

#include <SPI.h>
#include <Ethernet.h>
#include <PubSubClient.h>

// Update these with values suitable for your hardware/network.
byte mac[]    = {  0xDE, 0xED, 0xBA, 0xFE, 0xFE, 0xED };
IPAddress ip(172, 16, 0, 100);
IPAddress server(172, 16, 0, 2);

EthernetClient ethClient;
PubSubClient client(ethClient);

long lastPulished = 0, lastReconnect = 0;

boolean reconnect() {
  return client.connect("arduinoClient"));
}

void setup()
{
  client.setServer(server, 1883);
  // set default quality of service for published messages
  client.setPubQoS(1);

  Ethernet.begin(mac, ip);
  delay(1500);

  lastReconnect = 0;
  lastPublished = 0;
}


void loop()
{
  long now = millis();

  if (client.connected()) {
    // process MQTT events
    client.loop();
    // publish a new message
    if (now - lastPublished > 1000) {
      // check that the last published message received first
      if (client.isLastReceived()) {
        log_i("Received id=%d", client.getLastConfirmedId());
        if (client.publish("outTopic","hello")) {
          log_i("Published id=%d", client.getLastConfirmedId());
          lastPublished = now;
        }
      }
    }
  } else {
    if (now - lastReconnect > 5000) {
      // Attempt to reconnect
      if (reconnect()) {
        lastReconnectAttempt = 0;
      }
    }
  }

}


