// Copyright 2015-2021 Espressif Systems (Shanghai) PTE LTD
/* SPDX-License-Identifier: GPL-2.0 OR Apache-2.0 */

/** prevent recursive inclusion **/
#ifndef __COMMANDS_H
#define __COMMANDS_H

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#define SUCCESS                 0
#define FAILURE                 -1
#define SSID_LENGTH             32
#define PASSWORD_LENGTH         64
#define BSSID_LENGTH            17
#define STATUS_LENGTH           14
#define NOT_CONNECTED           1
#define NO_AP_FOUND             2
#define INVALID_PASSWORD        3
#define INVALID_ARGUMENT        4
#define OUT_OF_RANGE            5

typedef enum {
    WIFI_MODE_NONE = 0,  /**< null mode */
    WIFI_MODE_STA,       /**< WiFi station mode */
    WIFI_MODE_AP,        /**< WiFi soft-AP mode */
    WIFI_MODE_APSTA,     /**< WiFi station + soft-AP mode */
    WIFI_MODE_MAX
} wifi_mode_t;

typedef enum {
    WIFI_AUTH_OPEN = 0 ,
    WIFI_AUTH_WEP,
    WIFI_AUTH_WPA_PSK ,
    WIFI_AUTH_WPA2_PSK ,
    WIFI_AUTH_WPA_WPA2_PSK ,
    WIFI_AUTH_WPA2_ENTERPRISE ,
    WIFI_AUTH_WPA3_PSK ,
    WIFI_AUTH_WPA2_WPA3_PSK ,
    WIFI_AUTH_MAX ,
} wifi_auth_mode_t;

typedef enum {
	WIFI_BW_HT20 = 1,
	WIFI_BW_HT40,
} wifi_bandwidth_t;

typedef enum {
    WIFI_PS_MIN_MODEM = 1,
    WIFI_PS_MAX_MODEM,
    WIFI_PS_INVALID,
} wifi_ps_type_t;

typedef enum {
    WIFI_VND_IE_TYPE_BEACON = 0,
    WIFI_VND_IE_TYPE_PROBE_REQ,
    WIFI_VND_IE_TYPE_PROBE_RESP,
    WIFI_VND_IE_TYPE_ASSOC_REQ,
    WIFI_VND_IE_TYPE_ASSOC_RESP,
} wifi_vendor_ie_type_t;

typedef enum {
    WIFI_VND_IE_ID_0,
    WIFI_VND_IE_ID_1,
} wifi_vendor_ie_id_t;

typedef struct {
    uint8_t ssid[SSID_LENGTH];
    uint8_t pwd[PASSWORD_LENGTH];
    int channel;
    int encryption_mode;
    int max_connections;
    bool ssid_hidden;
    wifi_bandwidth_t bandwidth;
} esp_hosted_softap_config_t;

typedef struct {
    uint8_t ssid[SSID_LENGTH];
    uint8_t pwd[PASSWORD_LENGTH];
    uint8_t bssid[BSSID_LENGTH];
    bool is_wpa3_supported;
    int rssi;
    int channel;
    int encryption_mode;
    uint16_t listen_interval;
    char status[STATUS_LENGTH];
} esp_hosted_sta_config_t;

typedef union{
    esp_hosted_sta_config_t station;
    esp_hosted_softap_config_t softap;
}esp_hosted_control_config_t;

typedef struct {
    uint8_t ssid[SSID_LENGTH];
    uint8_t bssid[BSSID_LENGTH];
    int rssi;
    int channel;
    int encryption_mode;
} esp_hosted_wifi_scanlist_t;

typedef struct {
    uint8_t bssid[BSSID_LENGTH];
    int rssi;
} esp_hosted_wifi_connected_stations_list;

typedef struct {
    uint8_t element_id;      /**< Should be set to WIFI_VENDOR_IE_ELEMENT_ID (0xDD) */
    uint8_t length;          /**< Length of all bytes in the element data following this field. Minimum 4. */
    uint8_t vendor_oui[3];   /**< Vendor identifier (OUI). */
    uint8_t vendor_oui_type; /**< Vendor-specific OUI type. */
    uint8_t payload[0];      /**< Payload. Length is equal to value in 'length' field, minus 4. */
} vendor_ie_data_t;

/* wifi get mac function returns status SUCCESS(0) or FAILURE(-1)
 * Input parameter
 *      mode : ESP32 wifi mode
 *          (WIFI_MODE_STA  : for station mac
 *           WIFI_MODE_AP   : for softAP mac)
 * Output parameter
 *      char* mac, returns MAC address of respective mode
 */
int wifi_get_mac(int mode, char *mac);

/*
 * wifi set mac function sets custom mac address to ESP32's station and softAP Interface, returns SUCCESS(0) or FAILURE(-1)
 * Input parameter:
 *      mode : ESP32 wifi mode
 *          (WIFI_MODE_STA  : for station mac
 *           WIFI_MODE_AP   : for softAP mac)
 *      mac  : custom MAC Address for ESP32 Interface
 * @attention 1. First set wifi mode before setting MAC address for respective station and softAP Interface
 * @attention 2. ESP32 station and softAP have different MAC addresses, do not set them to be the same.
 * @attention 3. The bit 0 of the first byte of ESP32 MAC address can not be 1.
 * For example, the MAC address can set to be "1a:XX:XX:XX:XX:XX", but can not be "15:XX:XX:XX:XX:XX".
 * @attention 4. MAC address will get reset after esp restarts
 *
 */
int wifi_set_mac(int mode, char *mac);

/* wifi get mode function returns status SUCCESS(0) or FAILURE(-1)
 * Output parameter
 *      int* mode : returns current wifi mode of ESP32
 * wifi modes:
 * WIFI_MODE_NONE   : null Mode, Wi-Fi mode not set
 * WIFI_MODE_STA    : station mode
 * WIFI_MODE_AP     : softAP mode
 * WIFI_MODE_APSTA  : softAP+station mode
 */
int wifi_get_mode(int *mode);

/* wifi set mode function returns status SUCCESS(0) or FAILURE(-1)
 * User should give input mode as follows:
 * wifi modes:
 * WIFI_MODE_NONE   : null Mode, Wi-Fi mode not set
 * WIFI_MODE_STA    : station mode
 * WIFI_MODE_AP     : softAP mode
 * WIFI_MODE_APSTA  : softAP+station mode
 *
 */
int wifi_set_mode(int mode);

/* wifi set ap config function returns status of connect to AP request
 * Input parameter
 *      esp_hosted_control_config_t ap_config ::
 *          ssid                 :   ssid of AP
 *          pwd                  :   password of AP
 *          bssid                :   MAC address of AP
 *          is_wpa3_supported    :   status of WPA3 supplicant present on AP (False: Unsupported, True:  Supported)
 *          listen_interval      :   Listen interval for ESP32 station to receive beacon when WIFI_PS_MAX_MODEM is set.
 *                                   Units: AP beacon intervals. Defaults to 3 if set to 0.
 *  Output:
 *      - SUCCESS(0)            : succeed
 *      - FAILURE(-1)           : failed
 *      - NO_AP_FOUND(2)        : Given AP not found
 *      - INVALID_PASSWORD(3)   : Invalid password for AP
 */
int wifi_set_ap_config(esp_hosted_control_config_t ap_config);

/* wifi get ap config function gives ssid, bssid, channel ID, rssi and encryption mode of connected AP, returns SUCCESS(0) OR FAILURE(-1)
 * Output parameter
 *      esp_hosted_control_config_t* ap_config ::
 *          ssid                :   ssid of connected AP
 *          bssid               :   MAC address of connected AP
 *          channel             :   channel ID, 1 ~ 10
 *          rssi                :   rssi signal strength
 *          encryption_mode     :   encryption mode
 *          (encryption modes are
 *              0 :   OPEN
 *              1 :   WEP
 *              2 :   WPA_PSK
 *              3 :   WPA2_PSK
 *              4 :   WPA_WPA2_PSK
 *              5 :   WPA2_ENTERPRISE
 *              6 :   WPA3_PSK
 *              7 :   WPA2_WPA3_PSK   )
 */
int wifi_get_ap_config(esp_hosted_control_config_t* ap_config);

/* wifi disconnect ap function disconnects ESP32 station from connected AP, returns SUCCESS(0) or FAILURE(-1)
 */
int wifi_disconnect_ap();

/* wifi set softap config function sets ESP32 softAP configurations, returns SUCCESS(0) or FAILURE(-1)
 * Input parameter
 *      esp_hosted_control_config_t softap_config ::
 *          ssid            :   ssid of softAP
 *          pwd             :   password of softAP, length of password should be 8~64 bytes ASCII
 *          channel         :   channel ID, in range of 1 to 11
 *          encryption_mode :   encryption mode
 *          (encryption modes are
 *              0 :   OPEN
 *              2 :   WPA_PSK
 *              3 :   WPA2_PSK
 *              4 :   WPA_WPA2_PSK
 *          max_connections    : maximum number of stations can connect to ESP32 SoftAP (should be in range of 1 to 10)
 *          ssid_hidden : softAP should broadcast its SSID or not
 *              ( 0 : SSID is broadcast
 *                1 : SSID is not broadcast )
 *          bandwidth          : set bandwidth of ESP32 softAP
 *              ( 1 : WIFI_BW_HT20
 *                2 : WIFI_BW_HT40 )
 */
int wifi_set_softap_config(esp_hosted_control_config_t softap_config);

/* wifi get softap config function gives ESP32 softAP credentials, returns SUCCESS(0) or FAILURE (-1)
 * Output parameter
 *      esp_hosted_control_config_t* softap_config ::
 *          ssid                :   ssid of softAP
 *          pwd                 :   password of softAP
 *          channel             :   channel ID, in range of 1 to 11
 *          encryption_mode     :   encryption mode
 *          (encryption modes are
 *              0 :   OPEN
 *              2 :   WPA_PSK
 *              3 :   WPA2_PSK
 *              4 :   WPA_WPA2_PSK
 *          max_connections    : maximum number stations can to ESP32 SoftAP (in range of 1 to 10)
 *          ssid_hidden : softAP broadcast status
 *              ( 0 : SSID is broadcast
 *                1 : SSID is not broadcast )
 *          bandwidth          : bandwidth of ESP32 softAP
 *              ( 1 : WIFI_BW_HT20
 *                2 : WIFI_BW_HT40 )
 */
int wifi_get_softap_config(esp_hosted_control_config_t *softap_config);

/* wifi stop softap function stops ESP32 softAP, returns SUCCESS(0) or FAILURE(-1)
 */
int wifi_stop_softap();

/* wifi ap scan list function gives scanned list of available APs, returns structure pointer of esp_hosted_wifi_scanlist_t.
 * Output parameter
 *      int* count      : number of available APs
 */
esp_hosted_wifi_scanlist_t* wifi_ap_scan_list(int *count);

/* wifi connected stations list function gives list of connected stations to ESP32 softAP, returns structure pointer of esp_hosted_wifi_connected_stations_list
 * Output parameter
 *      int* num      : number of stations connected
 */
esp_hosted_wifi_connected_stations_list*  wifi_connected_stations_list(int *num);

/*
 * wifi set power save mode function sets power save mode of ESP32, returns SUCCESS(0) or FAILURE(-1)
 *  Input parameter:
 *      power save mode : ESP32's power save mode
 *                  (1  : WIFI_PS_MIN_MODEM,   Minimum modem power saving.
 *                        In this mode, station wakes up to receive beacon every DTIM period
 *                   2  : WIFI_PS_MAX_MODEM,   Maximum modem power saving.
 *                        In this mode, interval to receive beacons is determined
 *                        by the listen_interval parameter in wifi_set_ap_config function
 *      Default :: power save mode is WIFI_PS_MIN_MODEM
 */
int wifi_set_power_save_mode(int power_save_mode);

/*
 * wifi get power save mode function gives power save mode of ESP32, returns SUCCESS(0) or FAILURE(-1)
 *  Output parameter:
 *      power save mode : ESP32's power save mode
 *                  (1  : WIFI_PS_MIN_MODEM,   Minimum modem power saving.
 *                        In this mode, station wakes up to receive beacon every DTIM period
 *                   2  : WIFI_PS_MAX_MODEM,   Maximum modem power saving.
 *                        In this mode, interval to receive beacons is determined
 *                        by the listen_interval parameter in wifi_set_ap_config function
 *                   3  : WIFI_PS_INVALID,     Invalid power save mode. In case of failure of command
 */
int wifi_get_power_save_mode(int *power_save_mode);

/*
 * wifi set vendor specific ie function set 802.11 Vendor-Specific Information Element
 * returns SUCCESS(0) or FAILURE(-1)
 *  Input parameter:
 *      enable      : If true, specified IE is enabled. If false, specified IE is removed.
 *      type        : Information Element type. Determines the frame type to associate with the IE.
 *      idx         : Index to set or clear. Each IE type can be associated with up to two elements (indices 0 & 1).
 *      vnd_ie      : Pointer to vendor specific element data. First 6 bytes should be a header with fields matching vendor_ie_data_t. If enable is false, this argument is ignored and can be NULL.
 *      vnd_ie_size : size of vnd_ie data
 */
int wifi_set_vendor_specific_ie(bool enable, wifi_vendor_ie_type_t type,
        wifi_vendor_ie_id_t idx, void* vnd_ie, uint16_t vnd_ie_size);

/*
 * wifi_set_max_tx_power function set maximum transmitting power.
 * returns SUCCESS(0) or FAILURE(-1)
 *
 * @attention 1. The value set by this API will be mapped to the max_tx_power of the structure wifi_country_t variable in wifi driver.
 * @attention 2. Mapping Table {wifi_max_tx_power, max_tx_power} = {{8,   2}, {20,  5}, {28,  7}, {34,  8}, {44, 11},
 *                                                      {52, 13}, {56, 14}, {60, 15}, {66, 16}, {72, 18}, {80, 20}}.
 * @attention 4. Param power unit is 0.25dBm, range is [8, 84] corresponding to 2dBm to 20dBm.
 * @attention 5. Relationship between set value and actual value. As follows: {set value range, actual value} = {{[8,  19],8}, {[20, 27],20}, {[28, 33],28}, {[34, 43],34}, {[44, 51],44}, {[52, 55],52}, {[56, 59],56}, {[60, 65],60}, {[66, 71],66}, {[72, 79],72}, {[80, 84],80}}.
 *
 *  Input parameter:
 *      wifi_max_tx_power   : Maximum WiFi transmitting power.
 *
 * returns OUT_OF_RANGE(5), If `wifi_max_tx_power` range is not in [8, 84] corresponding to `2dBm to 20dBm` tx power.
 */
int wifi_set_max_tx_power(int8_t wifi_max_tx_power);

/*
 * wifi_get_curr_tx_power function gets WiFi current transmiting power
 * returns SUCCESS(0) or FAILURE(-1)
 *
 *  Output parameter:
 *      wifi_curr_tx_power   :  WiFi current transmitting power, unit is 0.25dBm.
 */
int wifi_get_curr_tx_power(int8_t *wifi_curr_tx_power);

/*
 * esp ota begin function performs an OTA begin operation for ESP32
 * which sets partition for OTA write and erase it.
 * returns SUCCESS(0) or FAILURE(-1)
 */
int esp_ota_begin();

/*
 * esp ota write function performs an OTA write operation for ESP32,
 * It writes ota_data buffer to OTA partition in flash
 * returns SUCCESS(0) or FAILURE(-1)
 *  Input parameter:
 *      ota_data        : OTA data buffer
 *      ota_data_len    : length of OTA data buffer
 */
int esp_ota_write(uint8_t* ota_data, uint32_t ota_data_len);

/*
 * esp ota end function performs an OTA end operation for ESP32,
 * It validates written OTA image, set OTA partition as boot partition for next boot,
 * Creates timer which reset ESP32 after 5 sec,
 * returns SUCCESS(0) or FAILURE(-1)
 */
int esp_ota_end();

#ifndef STM32F469xx
/*
 * interface_up function ups the given interface
 * returns SUCCESS(0) or FAILURE(-1)
 */
int interface_up(int sockfd, char* iface);

/*
 * interface_down function downs the given interface
 * returns SUCCESS(0) or FAILURE(-1)
 */
int interface_down(int sockfd, char* iface);

/*
 * Function sets mac address to the given interface
 * returns SUCCESS(0) or FAILURE(-1)
 */
int set_hw_addr(int sockfd, char* iface, char* mac);

/*
 * function creates an endpoint for communication and returns a file descriptor (integer number) that refers to that endpoint
 * function returns the status SUCCESS(0) or FAILURE(-1)
 * int *sock : User should get file descriptor for the new socket on success
 */
int create_socket(int domain, int type, int protocol, int *sock);

/*
 * function closes an endpoint of communication.
 * function returns the status SUCCESS(0) or FAILURE(-1)
 * int sock : file descrepter of the socket to be closed
 */
int close_socket(int sock);
#endif /* STM32F469xx */
#endif /* __COMMANDS_H */
