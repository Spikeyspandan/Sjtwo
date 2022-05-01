// void producer() {
//   while (1) {
//     printf("A" / n);
//     vTaskDelay(1000);
//   }
// }
// void consumer() {
//   while (1) {
//     printf("B" / n);
//     vTaskDelay(1000);
//   }
// }

// int main(void) {
//   sj2_cli_init();

//   xTaskCreate(producer, "producer", 1024, NULL, PRIORITY_LOW, NULL);
//   xTaskCreate(consumer, "consumer", 1024, NULL, PRIORITY_HIGH, NULL);

//   vTaskStartScheduler();
//   return 0;
// }