
void startTimer(Tc *tc, uint32_t channel, IRQn_Type irq, uint32_t frequency) {
  pmc_set_writeprotect(false);
  pmc_enable_periph_clk((uint32_t)irq);
  TC_Configure(tc, channel, TC_CMR_WAVE | TC_CMR_WAVSEL_UP_RC | TC_CMR_TCCLKS_TIMER_CLOCK4);
  uint32_t rc = VARIANT_MCK / 128 / frequency; //128 because we selected TIMER_CLOCK4 above
  TC_SetRA(tc, channel, rc / 2); //50% high, 50% low
  TC_SetRC(tc, channel, rc);
  TC_Start(tc, channel);
  tc->TC_CHANNEL[channel].TC_IER = TC_IER_CPCS;
  tc->TC_CHANNEL[channel].TC_IDR = ~TC_IER_CPCS;
  NVIC_EnableIRQ(irq);
}

void setup() {
  //==Serial==
  Serial.begin(9600);
    pinMode(22, OUTPUT);
  startTimer(TC1, 0, TC3_IRQn, 1);
}
uint8_t light = HIGH;

void TC3_Handler()
{
  TC_GetStatus(TC1, 0);
  digitalWrite(22, light);
  light = !light;
}

void loop() {
}
/*
ISR/IRQ TC          Channel Due pins
TC0 TC0 0 2, 13
TC1 TC0 1 60, 61
TC2 TC0 2 58
TC3 TC1 0 none  <- this line in the example above
TC4 TC1 1 none
TC5 TC1 2 none
TC6 TC2 0 4, 5
TC7 TC2 1 3, 10
TC8 TC2 2 11, 12
 */
