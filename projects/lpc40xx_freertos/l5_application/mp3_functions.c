// #include "mp3_functions.h"

// void bass_control() {
//   adc__initialize();

//   adc__enable_burst_mode();
//   pin_configure_adc_channel_as_io_pin();

//   const uint16_t adc_value = adc__get_channel_reading_with_burst_mode(ADC__CHANNEL_4);
//   const uint16_t a = adc_value / 40.95;

//   if (a == 100) {
//     write_register(0x02, 0x00, 0x00);
//   } else if (a == 0) {
//     write_register(0x02, 0xFE, 0x00);
//   }

//   else {
//     uint16_t value = 0x63 - a;
//     write_register(0x02, value, value);
//   }
// }

// // void treble_control() {
// //   adc__initialize();

// //   adc__enable_burst_mode();
// //   pin_configure_adc_channel_as_io_pin();

// //   const uint16_t adc_value = adc__get_channel_reading_with_burst_mode(ADC__CHANNEL_4);
// //   const uint16_t a = adc_value / 40.95;

// //   if (a == 100) {
// //     write_register(0x02, 0x00, 0x00);
// //   } else if (a == 0) {
// //     write_register(0x02, 0xFE, 0x00);
// //   }

// //   else {
// //     uint16_t value = 0x63 - a;
// //     write_register(0x02, value, value);
// //   }
// // }
