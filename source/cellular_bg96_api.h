#ifndef CELLULAR_BG96_API_H
#define CELLULAR_BG96_API_H

/* The config header is always included first. */
#include "cellular_config.h"
#include "cellular_config_defaults.h"

/* IoT Cellular data types. */
#include "cellular_types.h"

#define BG96_API_NO_SSL_CONTEXT_ID    (-1)
#define BG96_API_MQTT_SSL_CONTEXT_ID  (1)
#define BG96_API_HTTPS_SSL_CONTEXT_ID (2)

typedef enum CellularMqttState
{
    MQTTSTATE_ALLOCATED = 0, /**< Socket is created. */
    MQTTSTATE_OPENING,
    MQTTSTATE_OPENED,
    MQTTSTATE_CONNECTING,    /**< Socket is connecting in progress with remote peer. */
    MQTTSTATE_CONNECTED,     /**< Socket is connected. */
    MQTTSTATE_DISCONNECTED,   /**< Socket is disconnected by remote peer or due to network error. */
} CellularMqttState_t;

typedef enum CellulartUrcMqttEvent
{
    CELLULAR_URC_MQTT_PUBLISH = 0,
    CELLULAR_URC_MQTT_SUBSCRIBE,
    CELLULAR_URC_MQTT_UNSUBSCRIBE,
} CellularUrcMqttEvent_t;

typedef enum CellularMqttOutgoingResult
{
    CELLULAR_MQTT_OUTGOING_SUCCESS = 0,
    CELLULAR_MQTT_OUTGOING_RETRY,
    CELLULAR_MQTT_OUTGOING_FAILURE,
} CellularMqttOutgoingResult_t;

//! @brief Enumeration representing various URC status events for the BG96 module.
enum Bg96UrcStatusEvent
{
    BG96_URC_STATUS_PEER_CLOSED = 1,
    BG96_URC_STATUS_PINGREQ_FAIL,
    BG96_URC_STATUS_CONNECT_FAIL,
    BG96_URC_STATUS_CONNACK_FAIL,
    BG96_URC_STATUS_DISCONNECT,
    BG96_URC_STATUS_MULTIPLE_SEND_FAIL,
    BG96_URC_STATUS_NOT_VALID
};

typedef struct CellularSocketSslConfig
{
    uint8_t useSsl;
    uint8_t sslContextId;
} CellularSocketSslConfig_t;

typedef struct CellularMqttSocket CellularMqttSocket_t;

typedef void (* CellularMqttOpenCallback_t )( CellularMqttSocket_t * mqttContext, void* context );
typedef void (* CellularMqttCloseCallback_t )( CellularMqttSocket_t * mqttContext, void* context );
typedef void (* CellularMqttConnectCallback_t )( CellularMqttSocket_t * mqttContext, void* context );
typedef void (* CellularMqttDisconnectCallback_t )( CellularMqttSocket_t * mqttContext, void* context );
typedef void (* CellularMqttOutgoingResponseCallback_t )( CellularUrcMqttEvent_t event, CellularMqttOutgoingResult_t result,
                                                          CellularMqttSocket_t * mqttContext, void* context );
typedef void (* CellularMqttReceiveCallback_t )( uint8_t bufferIndex, CellularMqttSocket_t * mqttContext, void* context );
typedef void (* CellularMqttStateCallback_t )( uint8_t status, CellularMqttSocket_t * mqttContext, void* context );

typedef struct CellularMqttSocket
{
    CellularHandle_t cellularHandle;
    uint8_t          cellularMqttIndex;
    int8_t           sslIndex;

    CellularMqttState_t mqttState;

    /* Callback functions */
    CellularMqttOpenCallback_t openCallback;

    CellularMqttCloseCallback_t closeCallback;
    CellularMqttConnectCallback_t connectCallback;
    CellularMqttDisconnectCallback_t disconnectCallback;
    CellularMqttOutgoingResponseCallback_t outgoingCallback;
    CellularMqttReceiveCallback_t receiveCallback;
    CellularMqttStateCallback_t stateCallback;
    void* callbackContext;
} CellularMqttSocket_t;

/* Upload credential to the file system. Later it can be used to configure the secure socket context. */
CellularError_t Cellular_UploadFileToStorage( CellularHandle_t cellularHandle,
                                              const char* filename,
                                              const char* fileContent,
                                              uint32_t fileSize,
                                              uint32_t* pSentDataLength );

/* Configure the secure socket context. */
CellularError_t Cellular_ConfigureSSLContext( CellularHandle_t cellularHandle,
                                              uint8_t sslContextId,
                                              const char* sslConfigurationParameter,
                                              const char* inputArg,
                                              bool isNumeric );

/* Used to delete credentials from file system */
CellularError_t Cellular_DeleteFileFromStorage( CellularHandle_t cellularHandle,
                                                const char* filename );

/* Used to check if credentials exist on file system */
CellularError_t Cellular_CheckFileExistsInStorage( CellularHandle_t cellularHandle,
                                                   const char* filename,
                                                   bool * fileExists );

//MQTT Modifications

CellularError_t Cellular_CreateMqttSocket( CellularContext_t * pContext, int8_t* pMqttIndex);

CellularMqttSocket_t * Cellular_GetMqttSocket( uint8_t mqttIndex );

void Cellular_DeleteMqttSocket( uint8_t mqttIndex );

CellularError_t Cellular_MqttConfigureGeneric(CellularHandle_t cellularHandle,
                                              uint8_t mqttContextId,
                                              const char* mqttConfigurationParameter,
                                              const char* inputArg,
                                              bool isNumeric);

CellularError_t Cellular_MqttConfigureSsl(CellularHandle_t cellularHandle,
                                          uint8_t mqttContextId,
                                          bool enable_ssl,
                                          uint8_t sslContextId);

CellularError_t Cellular_MqttConfigureTimeout(CellularHandle_t cellularHandle,
                                              uint8_t mqttContextId,
                                              uint8_t packet_timeout,
                                              uint8_t max_retry_attempts,
                                              bool report_timeout);

CellularError_t Cellular_MqttConfigureWill(CellularHandle_t cellularHandle,
                                           uint8_t mqttContextId,
                                           bool will_flag_required,
                                           uint8_t will_qos,
                                           bool retain_will_message,
                                           const char* will_topic,
                                           const char* will_message);

CellularError_t Cellular_MqttConfigureReceiveMode(CellularHandle_t cellularHandle,
                                                  uint8_t mqttContextId,
                                                  bool message_not_in_urc,
                                                  bool message_length_in_urc);

CellularError_t Cellular_MqttConfigureSendMode(CellularHandle_t cellularHandle,
                                                uint8_t mqttContextId,
                                                bool message_not_in_urc);

CellularError_t Cellular_MqttOpen(CellularHandle_t cellularHandle,
                                  uint8_t mqttContextId,
                                  const char* endpoint,
                                  uint32_t port);

CellularError_t Cellular_MqttClose(CellularHandle_t cellularHandle,
                                   uint8_t mqttContextId);

CellularError_t Cellular_MqttConnect(CellularHandle_t cellularHandle,
                                     uint8_t mqttContextId,
                                     const char* clientId);

CellularError_t Cellular_MqttDisconnect(CellularHandle_t cellularHandle,
                                        uint8_t mqttContextId);

CellularError_t Cellular_MqttPublish(CellularHandle_t cellularHandle,
                                     uint8_t mqttContextId,
                                     uint16_t messageId,
                                     uint8_t qos,
                                     bool retain,
                                     const char* topic,
                                     const char* message,
                                     uint32_t messageLength,
                                     uint32_t* sentDataLength);

CellularError_t Cellular_MqttSubscribe(CellularHandle_t cellularHandle,
                                       uint8_t mqttContextId,
                                       uint16_t messageId,
                                       const char* topic,
                                       uint8_t qos);

CellularError_t Cellular_MqttUnsubscribe(CellularHandle_t cellularHandle,
                                         uint8_t mqttContextId,
                                         uint16_t messageId,
                                         const char* topic);

CellularError_t Cellular_MqttReadIncomingPublish( CellularHandle_t cellularHandle,
                                                  uint8_t mqttContextId,
                                                  uint8_t mqttBufferIndex,
                                                  uint8_t * pBuffer,
                                                  uint32_t bufferLength,
                                                  uint32_t * pReceivedDataLength,
                                                  char * topic,
                                                  uint32_t topicBufferLength,
                                                  uint32_t * receivedTopicLength);


#endif
