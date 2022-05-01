#include i2c_slave.h


static volatile uint8_t slave_memory[256];


int main(void) {
  i2c__init_slave(0x86);
  
  /**
   * Note: When another Master interacts with us, it will invoke the I2C interrupt
   *.      which will then invoke our i2c_slave_callbacks_*() from above
   *       And thus, we only need to react to the changes of memory
   */
  while (1) {
    if (slave_memory[0] == 0) {
      turn_on_an_led(); // TODO
    } else {
      turn_off_an_led(); // TODO
    }
    
    // of course, your slave should be more creative than a simple LED on/off
  }

  return -1;
}