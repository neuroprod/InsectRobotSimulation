
#include <DynamixelSDK.h>


#define ADDRESS_TORQUE_ENABLE           64
#define ADDRESS_GOAL_POSITION           116
#define ADDRESS_PRESENT_POSITION        132
#define LENGTH_GOAL_POSITION            4

// Protocol version
#define PROTOCOL_VERSION                2.0                 // See which protocol version is used in the Dynamixel
#define BAUDRATE                        57600
#define DEVICENAME                      "1"
#define TORQUE_ENABLE                   1                   // Value for enabling the torque
#define TORQUE_DISABLE                  0
dynamixel::PortHandler *portHandler;
dynamixel::PacketHandler *packetHandler;
dynamixel::GroupBulkWrite *groupBulkWrite;
int dxl_comm_result = COMM_TX_FAIL;             // Communication result
uint8_t dxl_error = 0;

int motorIDs[] = {11, 12, 13, 31, 32, 33, 51, 52, 53, 21, 22, 23, 41, 42, 43, 61, 62, 63};

uint8_t input[36];
int inCount = 0;
uint8_t inByte = 0;
uint8_t param_goal_position[4];

union U {
  int16_t s;

  struct Byte {
    int8_t c1, c2;
  }
  byte;
};

void setup() {

  Serial.begin(115200);
  delay(1000);

  portHandler = dynamixel::PortHandler::getPortHandler(DEVICENAME);
  packetHandler = dynamixel::PacketHandler::getPacketHandler(PROTOCOL_VERSION);
  groupBulkWrite = new dynamixel::GroupBulkWrite(portHandler, packetHandler);
  if (portHandler->openPort())
  {

  }
  else
  {


  }

  for (int i = 0; i < 36; i++)
  {
    if (i % 2 == 0) {
      input[i] = 8;
    }
    else {
      input[i] = 8;
    }
  }

  for (int i = 0; i < 18; i++)
  {
    int pos = i * 2;
    dxl_comm_result = packetHandler->write1ByteTxRx(portHandler, motorIDs[i], ADDRESS_TORQUE_ENABLE, TORQUE_ENABLE, &dxl_error);
    float val  = input[pos] * 256 + input[pos + 1];

    param_goal_position[0] = DXL_LOBYTE(DXL_LOWORD(val));
    param_goal_position[1] = DXL_HIBYTE(DXL_LOWORD(val));
    param_goal_position[2] = DXL_LOBYTE(DXL_HIWORD(val));
    param_goal_position[3] = DXL_HIBYTE(DXL_HIWORD(val));
    groupBulkWrite->addParam(motorIDs[i], ADDRESS_GOAL_POSITION, LENGTH_GOAL_POSITION, param_goal_position);

  }
  groupBulkWrite->txPacket();
}

void loop() {


  while ( Serial.available() > 0)
  {
    // get incoming byte:
    uint8_t currentByte = Serial.read();
    if (inByte == 100 && currentByte == 100)
    {
      inCount = 0;
    } else
    {
      input[inCount] = currentByte;
      inCount++;
      if (inCount == 36)
      {


        inCount = 0;
        for (int i = 0; i < 18; i++)
        {

          int pos = i * 2;
          U u;
          u.byte.c1 = input[pos];
          u.byte.c2 = input[pos + 1];
          float val  = u.s;



          param_goal_position[0] = DXL_LOBYTE(DXL_LOWORD(val));
          param_goal_position[1] = DXL_HIBYTE(DXL_LOWORD(val));
          param_goal_position[2] = DXL_LOBYTE(DXL_HIWORD(val));
          param_goal_position[3] = DXL_HIBYTE(DXL_HIWORD(val));
          groupBulkWrite->changeParam(motorIDs[i], ADDRESS_GOAL_POSITION, LENGTH_GOAL_POSITION, param_goal_position);


        }
        groupBulkWrite->txPacket();

      }
    }
    inByte = currentByte;



  }


}
