//==Encoder==
const int8_t fsmEncoder[4][4]={{0,1,-1,0},{-1,0,0,1},{1,0,0,-1},{0,-1,1,0}};

//==KnobEncoder==
const uint8_t knobCnt = 2;
const uint8_t knobEncoderPin[knobCnt][2]={{49,51},{48,50}};
const uint8_t knobEncoderButtonPin[knobCnt]={53,52};
uint8_t knobEncoderLastState[knobCnt][2]={{0,0},{0,0}};
int32_t knobEncoderPos[knobCnt];

//==Stepper==
const int stepperTypeCount = 2;
const int stepperPinCnt = 4;
const int stepperStateCnt = 8;
const int stepperPins[2][stepperPinCnt]={{8,9,10,11},{8,9,10,11}};
const uint8_t fsmStepper[stepperTypeCount][stepperStateCnt][stepperPinCnt]={{
  {1,0,0,0},
  {1,1,0,0},
  {0,1,0,0},
  {0,1,1,0},
  {0,0,1,0},
  {0,0,1,1},
  {0,0,0,1},
  {1,0,0,1}
},{
  {1,0,0,0},
  {1,0,1,0},
  {0,0,1,0},
  {0,1,1,0},
  {0,1,0,0},
  {0,1,0,1},
  {0,0,0,1},
  {1,0,0,1}
}};
volatile int32_t stepperPos[2]={0,0};
volatile int32_t stepperPosDelta[2]={0,0};
volatile int32_t stepperPosReal[2]={0,0};
#define STEPPER_H 0
#define STEPPER_V 1
#define STEPPER_UNIPOLAR 0
#define STEPPER_BIPOLAR 1


void startTimer(Tc *tc, uint32_t channel, IRQn_Type irq, uint32_t frequency) {
        pmc_set_writeprotect(false);
        pmc_enable_periph_clk((uint32_t)irq);
        TC_Configure(tc, channel, TC_CMR_WAVE | TC_CMR_WAVSEL_UP_RC | TC_CMR_TCCLKS_TIMER_CLOCK4);
        uint32_t rc = VARIANT_MCK/128/frequency; //128 because we selected TIMER_CLOCK4 above
        TC_SetRA(tc, channel, rc/2); //50% high, 50% low
        TC_SetRC(tc, channel, rc);
        TC_Start(tc, channel);
        tc->TC_CHANNEL[channel].TC_IER=TC_IER_CPCS;
        tc->TC_CHANNEL[channel].TC_IDR=~TC_IER_CPCS;
        NVIC_EnableIRQ(irq);
}
/*
ISR/IRQ TC          Channel Due pins
TC0 TC0 0 2, 13
TC1 TC0 1 60, 61
TC2 TC0 2 58
TC3 TC1 0 none
TC4 TC1 1 none
TC5 TC1 2 none
TC6 TC2 0 4, 5
TC7 TC2 1 3, 10
TC8 TC2 2 11, 12
 */

void setup() {
  //==Stepper==
  for(int i = 0; i < stepperPinCnt; i++){
    pinMode(stepperPins[0][i], OUTPUT);
    //pinMode(stepperPins[1][i], OUTPUT);
  }

  stepperSpeedTest();
}

void loop() {
  //900-1200
  stepperPosDelta[STEPPER_V] = 100000;
  updateKnobEncoder(0);
  if(knobEncoderPos[0] != 0){
    static uint32_t frequency = 900;
    frequency += knobEncoderPos[0];
    knobEncoderPos[0] = 0;
    startTimer(TC1, 0, TC3_IRQn, frequency);
    Serial.println(frequency);
  }
  delay(1);
}

void stepperSpeedTest(){
  uint16_t frequency = 200; //Max: 12000
  stepperPos[STEPPER_V] = stepperPos[STEPPER_H] = 2000000000;
  startTimer(TC1, 0, TC3_IRQn, frequency);
  while(1){
    updateKnobEncoder(0);
    updateKnobEncoder(1);
    if(knobEncoderPos[0] != 0 || knobEncoderPos[1] != 0){
      frequency += knobEncoderPos[0] * 10;
      frequency += knobEncoderPos[1];
      knobEncoderPos[0] = 0;
      knobEncoderPos[1] = 0;
      startTimer(TC1, 0, TC3_IRQn, frequency);
      Serial.println(frequency);
    }
  }
}

//==Stepper==
void TC3_Handler()
{
  TC_GetStatus(TC1, 0);
  updateStepper(STEPPER_H);
  //updateStepper(STEPPER_V);
}
void updateStepper(uint8_t stepper){
  int32_t pos = stepperPos[stepper] + stepperPosDelta[stepper];
  if(pos > stepperPosReal[stepper]){
    stepperPosReal[stepper]++;
  }else if(pos < stepperPosReal[stepper]){
    stepperPosReal[stepper]--;
  }else{
    for(int i = 0; i < stepperPinCnt; i++){
      //digitalWrite(stepperPins[stepper][i],LOW);
    }
    return;
  }
  int stepperState = stepperPosReal[stepper] % stepperStateCnt;
  if(stepperState < 0){
    stepperState += stepperStateCnt;
  }
  for(int i = 0; i < stepperPinCnt; i++){
    digitalWrite(stepperPins[stepper][i],fsmStepper[1][stepperState][i]);
  }
}

//Knob Encoder
void updateKnobEncoder(uint8_t nr){
  uint8_t lasta = knobEncoderLastState[nr][0];
  uint8_t lastb = knobEncoderLastState[nr][1];
  uint8_t ina = digitalRead(knobEncoderPin[nr][0])==HIGH;
  uint8_t inb = digitalRead(knobEncoderPin[nr][1])==HIGH;
  if(ina==lasta && inb==lastb){
    return;
  }
  knobEncoderPos[nr]+=fsmEncoder[(ina<<1)+inb][(lasta<<1)+lastb];
  knobEncoderLastState[nr][0] = ina;
  knobEncoderLastState[nr][1] = inb;
}

