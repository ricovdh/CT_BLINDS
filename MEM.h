/** MEMORY **/

void eeprom_commit() {
  if (EEPROM.commit()) {
    Serial.println("EEPROM successfully committed");
  } else {
    Serial.println("ERROR! EEPROM commit failed");
  }
}

void set_cur_position(int pos) {
  EEPROM.write(cur_position_address, pos);
  eeprom_commit();
  delay(50);
}

int get_cur_position() {
  int cur_pos = EEPROM.read(cur_position_address);
  return cur_pos;
}

void mem_setup() {
  EEPROM.begin(512);
  first_boot = EEPROM.read(first_boot_address);
  if (first_boot == 255) {
    EEPROM.write(first_boot_address, 1);
    EEPROM.write(open_time_address, DEFAULT_BLINDS_TRAVEL_TIME);
    EEPROM.write(close_time_address, DEFAULT_BLINDS_TRAVEL_TIME);
    EEPROM.write(cur_position_address, 100);
    eeprom_commit();
    delay(100);
  }
  open_time = EEPROM.read(open_time_address);
  Serial.print("Open time: ");
  Serial.println(open_time);
  close_time = EEPROM.read(close_time_address);
  Serial.print("Close time: ");
  Serial.println(close_time);
}