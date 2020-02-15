#include "stage.hpp"


Stage::Stage(char addr,Callback<void(char)> txfunc):slaveID(addr),txByte(txfunc){
  // theTerm=inTerm;

  printf("initialized stage\n");
  txByte(TxPack(slaveID,CMD_CONT));
  stageThId= ThisThread::get_id();
}
// these have to be blocking calls
void Stage::arm(){
  ThisThread::flags_clear(ACK_FLAG);
  printf("Arming stage %i\n",slaveID);
  _RxState = s_PING;
  ACCEPTING=true;
  txByte(TxPack(slaveID,CMD_ARM));
  ThisThread::flags_wait_any_for(ACK_FLAG,1);
  ACCEPTING=false;
  if(_RxState!=s_IDLE){
    printf("Waiting for arm %i timeout\n",slaveID);
    return ;//abaort
  }
}
void Stage::disarm(){
  txByte(TxPack(slaveID,CMD_DISARM));
}

void Stage::fire(){
  ThisThread::flags_clear(ACK_FLAG);
  printf("Firing stage %i\n",slaveID);
  _RxState = s_PING;
  ACCEPTING=true;
  txByte(TxPack(slaveID,CMD_FIRE));
  ThisThread::flags_wait_any_for(ACK_FLAG,1);
  ACCEPTING=false;
  if(_RxState!=s_IDLE){
    printf("Waiting for fire %i timeout\n",slaveID);
    return ;//abaort
  }
}
uint8_t Stage::getObstacles(){
  ThisThread::flags_clear(ACK_FLAG);
  printf("reading stage obs %i\n",slaveID);
  _RxState = s_PING;
  ACCEPTING=true;
  txByte(TxPack(slaveID,CMD_OBSTACLE));
  ThisThread::flags_wait_any_for(ACK_FLAG,1);
  ACCEPTING=false;
  if(_RxState!=s_IDLE){
    printf("Stage %i Readreg obs NACK\n",slaveID);
    return 0x00;//abaort
  }
  return regData;
}

void Stage::reset(){
  txByte(TxPack(slaveID,CMD_RST));
}
void Stage::stream(){
  ThisThread::flags_clear(DONE_FLAG);
  _RxState = s_STREAMING;
  xfCnt=0;
  streamLn=0;
  ACCEPTING=true;
  txByte(TxPack(slaveID,CMD_STREAM));
  ThisThread::flags_wait_any_for(DONE_FLAG,10);
  ACCEPTING=false;
  if(_RxState!=s_IDLE){
    printf("Waiting for ADC %i timeout\n",slaveID);
    return ;//abaort
  }
  printf("%i bytes recieved\n",streamLn);
  for (int i=0; i < streamLn; i++){
    printf("0x%X ",ADCBuff[i]);
  }
  printf("\nExit strem\n");
}
uint8_t Stage::readReg(uint8_t reg){
  ThisThread::flags_clear(ACK_FLAG);
  // printf("reading stage %i\n",slaveID);
  _RxState = s_PING;
  ACCEPTING=true;
  txByte(TxPack(slaveID,CMD_REGR));
  ThisThread::flags_wait_any_for(ACK_FLAG,1);
  ACCEPTING=false;
  if(_RxState!=s_IDLE){
    printf("Stage %i Readreg NACK\n",slaveID);
    return 0x00;//abaort
  }

  ThisThread::flags_clear(DONE_FLAG);
  _RxState = s_REGR;
  ACCEPTING=true;
  txByte(reg); // send reg addr
  ThisThread::flags_wait_any_for(DONE_FLAG,1);
  ACCEPTING=false;
  if(_RxState!=s_IDLE){
    printf("Stage %i Readreg data timeout\n",slaveID);
    return 0x00;//abaort
  }
  return regData;
}

void Stage::writeReg(uint8_t reg , uint8_t data){
  ThisThread::flags_clear(ACK_FLAG);
  // printf("reading stage %i\n",slaveID);
  _RxState = s_PING;
  ACCEPTING=true;
  txByte(TxPack(slaveID,CMD_REGW));
  ThisThread::flags_wait_any_for(ACK_FLAG,1);
  ACCEPTING=false;
  if(_RxState!=s_IDLE){
    printf("Stage %i writereg NACK\n",slaveID);
    return ;//abaort
  }

  ThisThread::flags_clear(ACK_FLAG);
  _RxState = s_PING;
  ACCEPTING=true;

  txByte(reg); // send reg addr
  txByte(data); // send reg addr

  ThisThread::flags_wait_any_for(ACK_FLAG,1);
  ACCEPTING=false;
  if(_RxState!=s_IDLE){
    printf("Stage %i writereg NACK\n",slaveID);
    return ;//abaort
  }
  printf("write success\n");
}

float Stage::getSpeed(){
  ThisThread::flags_clear(DONE_FLAG);
  _RxState = s_METERING;
  xfCnt=0;
  ACCEPTING=true;
  txByte(TxPack(slaveID,CMD_METER));
  ThisThread::flags_wait_any_for(DONE_FLAG,1);
  ACCEPTING=false;
  if(_RxState!=s_IDLE){
    printf("Waiting for meter %i timeout\n",slaveID);
    return 0;//abaort
  }
  // printf("0x%X",(unsigned int)(meterH<<8)+meterL);
  return (float)METER_WIDTH/((unsigned int)(meterH<<8)+meterL);
}
void Stage::processRx(char bytein){

  if(_RxState == s_STREAMING){
    if(xfCnt==1){
      streamLn=bytein-1;
    }
    else if (xfCnt>1){
      ADCBuff[xfCnt-2]=bytein;
      printf(".");
      if(xfCnt==streamLn){
        _RxState=s_IDLE;
        osSignalSet(stageThId,DONE_FLAG);
      }
    }
    xfCnt++;
  }
  else if(_RxState == s_REGR){
    regData=bytein;
    _RxState=s_IDLE;
  }
  else if(_RxState == s_METERING){
    switch (xfCnt) {
      case 1:
        meterH=bytein;
        break;
      case 2:
        meterL=bytein;
        _RxState=s_IDLE;
        osSignalSet(stageThId,DONE_FLAG);
        break;
      default:
        break;
    }
    xfCnt++;
  }
  else if(_RxState == s_PING){
    if (CK_CMD_ACK(bytein)){
      osSignalSet(stageThId,ACK_FLAG);
      _RxState=s_IDLE;
      // printf("Stage %i ACK\n",slaveID);
    }
    else{
      printf("Stage %i NACK\n",slaveID);
    }
  }
}
void Stage::report(){
  printf("%i\t%.3f\t%i\t%i\t%i\t%i\n",slaveID,getSpeed(),getOnDelayN(),getOnDelayP(),getOffDelay(),getSafetyTO());
}

void Stage::setOnDelayN(uint16_t xus){
  writeReg(REG_ONDELAYN,xus/dTickUs);
} // in us
uint16_t Stage::getOnDelayN(){
  uint8_t data=readReg(REG_ONDELAYN);
  return data*dTickUs;
} // in us
void Stage::setOnDelayP(uint16_t xus){
  writeReg(REG_ONDELAYP,xus/dTickUs);
} // in us
uint16_t Stage::getOnDelayP(){
  uint8_t data=readReg(REG_ONDELAYP);
  return data*dTickUs;
} // in us
void Stage::setOffDelay(uint16_t xus){
  writeReg(REG_OFFDELAY,xus/dTickUs);
} // in us
uint16_t Stage::getOffDelay(){
  uint8_t data=readReg(REG_OFFDELAY);
  return data*dTickUs;
} // in us
void Stage::setSafetyTO(uint16_t xms){
  writeReg(REG_SAFETYTO,xms);
} // in ms
uint16_t Stage::getSafetyTO(){
  return readReg(REG_SAFETYTO);
} // in ms
