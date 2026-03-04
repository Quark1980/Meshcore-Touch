#pragma once

#include "../../../src/MeshCore.h"
#include "../../../src/helpers/BaseSerialInterface.h"
#include "../../../src/helpers/SensorManager.h"
#include "../../../src/helpers/sensors/LPPDataHelpers.h"
#include "../../../src/helpers/ui/DisplayDriver.h"
#include "../../../src/helpers/ui/UIScreen.h"

#include <Arduino.h>

#ifndef LED_STATE_ON
#define LED_STATE_ON 1
#endif

#ifdef PIN_BUZZER
#include <helpers/ui/buzzer.h>
#endif
#ifdef PIN_VIBRATION
#include <helpers/ui/GenericVibration.h>
#endif

#include "../AbstractUITask.h"
#include "../NodePrefs.h"

class UITask : public AbstractUITask {
public:
  struct MessageEntry {
    uint32_t timestamp;
    char origin[32];
    char text[96];
    uint8_t channel_idx;
    bool is_group;
    bool is_sent;         // true if sent from this device
    uint8_t status;       // 0=pending/none, 1=repeated/acked, 2=failed (optional)
    uint8_t repeat_count; // number of times this message was heard repeated
    uint32_t ack_hash;    // to match with incoming acks
    uint32_t repeat_id;   // packet hash to match with mesh repeats
  };
  void start();
  void userLedHandler();
  void storeMessage(uint8_t path_len, const char *from_name, const char *text, uint8_t channel_idx = 0xFF,
                    bool is_group = false, bool is_sent = false, uint32_t ack_hash = 0,
                    uint32_t repeat_id = 0) override;
  void updateMessageAck(uint32_t ack_hash);

private:
  static const int MAX_STORED_MESSAGES = 24;

  DisplayDriver *_display;
  SensorManager *_sensors;
#ifdef PIN_BUZZER
  genericBuzzer buzzer;
#endif
#ifdef PIN_VIBRATION
  GenericVibration vibration;
#endif
  unsigned long _next_refresh, _auto_off;
  NodePrefs *_node_prefs;
  char _alert[80];
  unsigned long _alert_expiry;
  int _msgcount;
  unsigned long ui_started_at, next_batt_chck;
  int next_backlight_btn_check = 0;
#ifdef PIN_STATUS_LED
  int led_state = 0;
  int next_led_change = 0;
  int last_led_increment = 0;
#endif

#ifdef PIN_USER_BTN_ANA
  unsigned long _analogue_pin_read_millis = millis();
#endif

  UIScreen *splash;
  UIScreen *home;
  UIScreen *msg_preview;
  UIScreen *curr;
  MessageEntry _messages[MAX_STORED_MESSAGES];
  int _messages_head = -1; // newest entry index in ring
  int _messages_count = 0; // number of valid entries

  // Button action handlers
  char checkDisplayOn(char c);
  char handleLongPress(char c);
  char handleDoubleClick(char c);
  char handleTripleClick(char c);

  void setCurrScreen(UIScreen *c);

public:
  UITask(mesh::MainBoard *board, BaseSerialInterface *serial)
      : AbstractUITask(board, serial), _display(NULL), _sensors(NULL) {
    next_batt_chck = _next_refresh = 0;
    ui_started_at = 0;
    curr = NULL;
  }
  void begin(DisplayDriver *display, SensorManager *sensors, NodePrefs *node_prefs);

  void gotoHomeScreen() { setCurrScreen(home); }
  void showAlert(const char *text, int duration_millis);
  int getMsgCount() const { return _msgcount; }
  int getStoredMessageCount() const { return _messages_count; }
  bool getStoredMessage(int newest_index, MessageEntry &out) const;
  bool hasDisplay() const { return _display != NULL; }
  bool isButtonPressed() const;

  bool isBuzzerQuiet() {
#ifdef PIN_BUZZER
    return buzzer.isQuiet();
#else
    return true;
#endif
  }

  void toggleBuzzer();
  bool getGPSState();
  void toggleGPS();

  // from AbstractUITask
  void msgRead(int msgcount) override;
  void newMsg(uint8_t path_len, const char *from_name, const char *text, int msgcount,
              uint8_t channel_idx = 0xFF, bool is_group = false) override;
  void notify(UIEventType t = UIEventType::none) override;
  void loop() override;

  void shutdown(bool restart = false);
};
