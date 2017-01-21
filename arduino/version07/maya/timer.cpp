#include "timer.h"

const uint32_t timerDividerCoder[5] = {TC_CMR_TCCLKS_TIMER_CLOCK1, TC_CMR_TCCLKS_TIMER_CLOCK2, TC_CMR_TCCLKS_TIMER_CLOCK3, TC_CMR_TCCLKS_TIMER_CLOCK4, TC_CMR_TCCLKS_TIMER_CLOCK5 };
const uint8_t timerDivider[5] = { 2, 8, 32, 128, 1 };

void setTimerSpeed(Tc *tc, uint32_t channel, uint32_t frequency, uint8_t divider) {
	//TC_Configure(tc, channel, TC_CMR_WAVE | TC_CMR_WAVSEL_UP_RC | timerDividerCoder[divider]);
	uint32_t rc = VARIANT_MCK / (uint32_t)timerDivider[divider] / frequency; //128 because we selected TIMER_CLOCK4 above
	TC_SetRA(tc, channel, rc / 2); //50% high, 50% low
	TC_SetRC(tc, channel, rc);
}
void startTimer(Tc *tc, uint32_t channel, IRQn_Type irq, uint32_t frequency, uint8_t divider) {
	pmc_set_writeprotect(false);
	pmc_enable_periph_clk((uint32_t)irq);
	TC_Configure(tc, channel, TC_CMR_WAVE | TC_CMR_WAVSEL_UP_RC | timerDividerCoder[divider]);
	//uint32_t rc = VARIANT_MCK / 128 / frequency; //128 because we selected TIMER_CLOCK4 above
	//TC_SetRA(tc, channel, rc / 2); //50% high, 50% low
	//TC_SetRC(tc, channel, rc);
	setTimerSpeed(tc, channel, frequency, divider);
	TC_Start(tc, channel);
	tc->TC_CHANNEL[channel].TC_IER = TC_IER_CPCS;
	tc->TC_CHANNEL[channel].TC_IDR = ~TC_IER_CPCS;
	NVIC_EnableIRQ(irq);
}
void setTimerSpeed(Tc *tc, uint32_t channel, uint32_t frequency) {
	//setTimerSpeed(tc, channel, frequency, 3);
	uint32_t rc = (uint32_t)(VARIANT_MCK / 128.0 / frequency); //128 because we selected TIMER_CLOCK4 above
	TC_SetRA(tc, channel, rc / 2); //50% high, 50% low
	TC_SetRC(tc, channel, rc);
}
void startTimer(Tc *tc, uint32_t channel, IRQn_Type irq, uint32_t frequency) {
	//startTimer(tc, channel, irq, frequency, 3);
	pmc_set_writeprotect(false);
	pmc_enable_periph_clk((uint32_t)irq);
	TC_Configure(tc, channel, TC_CMR_WAVE | TC_CMR_WAVSEL_UP_RC | TC_CMR_TCCLKS_TIMER_CLOCK4);
	//uint32_t rc = VARIANT_MCK / 128 / frequency; //128 because we selected TIMER_CLOCK4 above
	//TC_SetRA(tc, channel, rc / 2); //50% high, 50% low
	//TC_SetRC(tc, channel, rc);
	setTimerSpeed(tc, channel, frequency);
	TC_Start(tc, channel);
	tc->TC_CHANNEL[channel].TC_IER = TC_IER_CPCS;
	tc->TC_CHANNEL[channel].TC_IDR = ~TC_IER_CPCS;
	NVIC_EnableIRQ(irq);
}