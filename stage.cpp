#include "stage.hpp"


Stage::Stage(char addr,Callback<void(char)> txfunc):slaveID(addr),txByte(txfunc){
  // theTerm=inTerm;

  printf("initialized stage\n");
  txByte(TxPack(slaveID,CMD_CONT));
}



void Stage::printStats(){

}
