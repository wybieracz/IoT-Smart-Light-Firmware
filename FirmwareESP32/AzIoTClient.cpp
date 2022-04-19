#include <azure_ca.h>
#include "AzIoTClient.h"

static az_iot_hub_client client;
static esp_mqtt_client_handle_t mqtt_client;

static AzIoTSasToken sasToken(
  &client,
  AZ_SPAN_FROM_STR(IOT_CONFIG_DEVICE_KEY),
  AZ_SPAN_FROM_BUFFER(sas_signature_buffer),
  AZ_SPAN_FROM_BUFFER(mqtt_password)
);

static void sendResponse(az_span rid, uint16_t status) {

  if (az_result_failed(az_iot_hub_client_methods_response_get_publish_topic(
    &client, rid, status, response_topic, sizeof(response_topic), NULL))) {
    Logger.Error("Failed to get response topic!");
    return;
  }

  if (esp_mqtt_client_publish(
      mqtt_client,
      response_topic,
      NULL,
      0,
      MQTT_QOS1,
      DO_NOT_RETAIN_MSG) == -1
  ) {
    Logger.Error("Failed to response direct method!");
  }
  else {
    Logger.Info("Responsed to direct method.");
  }
}

esp_err_t mqtt_event_handler(esp_mqtt_event_handle_t event) {

  switch (event->event_id) {
    int i, r, rid;
    char* ptr;
    uint16_t status;

    case MQTT_EVENT_ERROR:
      Logger.Info("MQTT event MQTT_EVENT_ERROR");
      break;
      
    case MQTT_EVENT_CONNECTED:
      Logger.Info("MQTT event MQTT_EVENT_CONNECTED");
      r = esp_mqtt_client_subscribe(mqtt_client, AZ_IOT_HUB_CLIENT_METHODS_SUBSCRIBE_TOPIC, 1);

      if (r == -1) Logger.Error("Could not subscribe for direct methods messages.");
      else Logger.Info("Subscribed for direct methods messages; message id:"  + String(r));
      break;

    case MQTT_EVENT_DISCONNECTED:
      digitalWrite(LED_YELLOW, LOW);
      Logger.Info("MQTT event MQTT_EVENT_DISCONNECTED");
      break;

    case MQTT_EVENT_SUBSCRIBED:
      Logger.Info("MQTT event MQTT_EVENT_SUBSCRIBED");
      Logger.Info("Message ID:" + (String)event->msg_id);
      break;

    case MQTT_EVENT_UNSUBSCRIBED:
      Logger.Info("MQTT event MQTT_EVENT_UNSUBSCRIBED");
      break;

    case MQTT_EVENT_PUBLISHED:
      Logger.Info("MQTT event MQTT_EVENT_PUBLISHED");
      break;

    case MQTT_EVENT_DATA:
      Logger.Info("MQTT event MQTT_EVENT_DATA");
      Logger.Info("Message ID:" + (String)event->msg_id);

      for (i = 0; i < (INCOMING_DATA_BUFFER_SIZE - 1) && i < event->topic_len; i++) {
        incoming_data[i] = event->topic[i]; 
      }

      incoming_data[i] = '\0';
      Logger.Info("Topic: " + String(incoming_data));

      //Get method name from topic. Works only when subscribed "$iothub/methods/POST/#"
      i = 0;
      ptr = incoming_data + 21;

      while (*ptr && *ptr != '/') {
        direct_method_name[i] = *ptr;
        i++;
        ptr++;
      }

      direct_method_name[i] = '\0';
      Logger.Info("Method: " + String(direct_method_name));

      while (*ptr && *ptr != '=') {
        ptr++;
      }
      ptr++;

      Logger.Info("rid: " + String(ptr));
      
      for (i = 0; i < (INCOMING_DATA_BUFFER_SIZE - 1) && i < event->data_len; i++) {
        incoming_data[i] = event->data[i]; 
      }

      incoming_data[i] = '\0';
      Logger.Info("Data: " + String(incoming_data));

      if(String(direct_method_name).equals("toggleLed")){
        toggleLed(incoming_data[0] - 48) ? status = 200 : status = 400;
      }
      else {
        status = 404;
      }
      Logger.Info("Code: " + String(status));
      sendResponse(az_span_create_from_str(ptr), status);
      break;

    case MQTT_EVENT_BEFORE_CONNECT:
      Logger.Info("MQTT event MQTT_EVENT_BEFORE_CONNECT");
      break;

    default:
      Logger.Error("MQTT event UNKNOWN");
      break;
  }

  return ESP_OK;
}

void initializeIoTHubClient() {

  if (az_result_failed(az_iot_hub_client_init(
      &client,
      az_span_create((uint8_t*)host, strlen(host)),
      az_span_create((uint8_t*)device_id, strlen(device_id)),
      NULL)
  )) {
    Logger.Error("Failed initializing Azure IoT Hub client");
    return;
  }

  size_t client_id_length;
  
  if (az_result_failed(az_iot_hub_client_get_client_id(&client, mqtt_client_id, sizeof(mqtt_client_id) - 1, &client_id_length))) {
    Logger.Error("Failed getting client id");
    return;
  }

  if (az_result_failed(az_iot_hub_client_get_user_name(&client, mqtt_username, sizeofarray(mqtt_username), NULL))) {
    Logger.Error("Failed to get MQTT clientId, return code");
    return;
  }

  Logger.Info("Client ID: " + String(mqtt_client_id));
  Logger.Info("Username: " + String(mqtt_username));
}

int initializeMqttClient() {

  if (sasToken.Generate(SAS_TOKEN_DURATION_IN_MINUTES) != 0) {
    Logger.Error("Failed generating SAS token");
    return 1;
  }

  esp_mqtt_client_config_t mqtt_config;
  memset(&mqtt_config, 0, sizeof(mqtt_config));
  mqtt_config.uri = mqtt_broker_uri;
  mqtt_config.port = mqtt_port;
  mqtt_config.client_id = mqtt_client_id;
  mqtt_config.username = mqtt_username;
  mqtt_config.password = (const char*)az_span_ptr(sasToken.Get());
  mqtt_config.keepalive = 30;
  mqtt_config.disable_clean_session = 0;
  mqtt_config.disable_auto_reconnect = false;
  mqtt_config.event_handle = mqtt_event_handler;
  mqtt_config.user_context = NULL;
  mqtt_config.cert_pem = (const char*)ca_pem;

  mqtt_client = esp_mqtt_client_init(&mqtt_config);

  if (mqtt_client == NULL) {
    Logger.Error("Failed creating mqtt client");
    return 1;
  }

  esp_err_t start_result = esp_mqtt_client_start(mqtt_client);

  if (start_result != ESP_OK) {
    Logger.Error("Could not start mqtt client; error code:" + start_result);
    return 1;
  }
  else {
    Logger.Info("MQTT client started; output code:" + (int)start_result);
    digitalWrite(LED_YELLOW, HIGH);
    return 0;
  }
}

void checkSession() {

  if (sasToken.IsExpired()) {
    digitalWrite(LED_YELLOW, LOW);
    Logger.Info("SAS token expired! Reconnecting with a new one.");
    (void)esp_mqtt_client_destroy(mqtt_client);
    initializeMqttClient();
  }
}