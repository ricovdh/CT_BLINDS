void set_publish_clear(int msg) {
  set_cur_position(msg);
  sprintf(position, "position,%ld", msg);
  client.publish(out_topic, position);
  clear_message();
}

void move() {
  if (int_message >= 0 && int_message <= 100) {
    if (get_cur_position() <= int_message) {
      Serial.println("opening");
      digitalWrite(close, LOW);
      digitalWrite(open, HIGH);
      int delaytime = ((open_time * 1000) / 100 * (int_message - get_cur_position()));
      Serial.println(delaytime);
      delay(delaytime);
      digitalWrite(open, LOW);
      set_publish_clear(int_message);
    } else if (get_cur_position() >= int_message) {
      Serial.println("closing");
      digitalWrite(open, LOW);
      digitalWrite(close, HIGH);
      int delaytime = ((close_time * 1000) / 100 * (get_cur_position() - int_message));
      Serial.println(delaytime);
      delay(delaytime);
      digitalWrite(close, LOW);
      set_publish_clear(int_message);
    } else {
      Serial.println("same position");
    }
  }
  clear_message();
}

void incoming_message() {
  if (check_incoming_message) {
    switch (int_message) {
      case 0 ... 100:
        move();
        break;
      case 101: // update
        client.publish(out_topic, "status,Checking for update");
        clear_message();
        update();
        break;
      case 102: // start config ap
        client.publish(out_topic, "status,Starting AP");
        clear_message();
        config_ap();
        break;
      case 103: // reboot
        client.publish(out_topic, "status,Rebooting");
        delay(500);
        reboot();
        break;
      case 104: // full close manual
        clear_message();
        digitalWrite(open, LOW);
        digitalWrite(close, HIGH);
        manual_override = true;
        set_cur_position(0);
        Serial.println("status,Going full down");
        client.publish(out_topic, "status,Going full down");
        break;
      case 105: // full open manual
        clear_message();
        digitalWrite(close, LOW);
        digitalWrite(open, HIGH);
        manual_override = true;
        set_cur_position(100);
        Serial.println("status,Going full up");
        client.publish(out_topic, "status,Going full up");
        break;
      case 106: // stop manual
        clear_message();
        digitalWrite(close, LOW);
        digitalWrite(open, LOW);
        manual_override = false;
        Serial.println("status,stopped");
        client.publish(out_topic, "status,stopped");
        break;
      case 107: // start OTA
        client.publish(out_topic, "status,OTA started");
        clear_message();
        ota_setup_flag = true;
        ota_setup();
        ota_run = true;
        break;
      default:
        // statements
        break;
    }
    check_incoming_message = false;
  }
}

void check_button() {
  if (digitalRead(button) == LOW) {

    if (buttonActive == false) {

      buttonActive = true;
      buttonTimer = millis();
    }

    if ((millis() - buttonTimer > LONG_PRESS_TIME) && (longPressActive == false)) {

      longPressActive = true;
      Serial.println("long press");
      config_ap();
    }

  } else {

    if (buttonActive == true) {

      if (longPressActive == true) {

        longPressActive = false;

      } else {

        Serial.println("short press");
      }

      buttonActive = false;
    }
  }
}

void check_switch() {

  if (!manual_override) {
    // knop omlaag
    int cur_state_down = digitalRead(switch_down);
    int time_to_close = ((close_time * 1000) / 100) * current_position;  // resterende tijd om volledig te sluiten in millis
    int current_close_position = map(time_to_close - (millis() - moving_start_time), 0, (close_time * 1000), 0, 100);

    if (cur_state_down != switch_down_last_state) {  // ga hier in als de knop veranderd van positie
      Serial.println("down state change");
      current_position = get_cur_position();

      if (cur_state_down == 0 && current_position != 0) {  // knop omlaag aan
        moving_start_time = millis();
        digitalWrite(open, LOW);
        digitalWrite(close, HIGH);
        close_active = true;
        Serial.println("close aan");
      }
      if (cur_state_down == 1 && close_active) {  // knop omlaag uit
        digitalWrite(close, LOW);
        close_active = false;
        set_publish_clear(current_close_position);
        Serial.println("close uit");
        Serial.print("current close position = ");
        Serial.println(current_close_position);
      }
      switch_down_last_state = cur_state_down;
    }

    if (((millis() - moving_start_time) >= time_to_close) && close_active == true) {  // ga hier in als knop langer aan staat dan de maximum tijd om te sluiten
      digitalWrite(close, LOW);
      close_active = false;
      set_publish_clear(current_close_position);
      Serial.println("close uit");
    }

    // knop omhoog
    int cur_state_up = digitalRead(switch_up);
    int time_to_open = ((open_time * 1000) / 100) * (100 - current_position);  // resterende tijd om volledig te openen in millis
    int current_open_position = 100 - map(time_to_open - (millis() - moving_start_time), 0, (open_time * 1000), 0, 100);

    if (cur_state_up != switch_up_last_state) {  // ga hier in als de knop veranderd van positie
      Serial.println("up state change");
      current_position = get_cur_position();

      if (cur_state_up == 0 && current_position != 100) {  // knop omhoog aan
        moving_start_time = millis();
        digitalWrite(close, LOW);
        digitalWrite(open, HIGH);
        open_active = true;
        Serial.println("open aan");
      }
      if (cur_state_up == 1 && open_active) {  // knop omhoog uit
        digitalWrite(open, LOW);
        open_active = false;
        set_publish_clear(current_open_position);
        Serial.println("open uit");
        Serial.print("current open position = ");
        Serial.println(current_open_position);
      }
      switch_up_last_state = cur_state_up;
    }

    if (((millis() - moving_start_time) >= time_to_open) && open_active == true) {  // ga hier in als knop langer aan staat dan de maximum tijd om te sluiten
      digitalWrite(open, LOW);
      open_active = false;
      set_publish_clear(current_open_position);
      Serial.println("open uit");
    }
  }
}
