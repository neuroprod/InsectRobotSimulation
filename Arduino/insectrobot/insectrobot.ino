#include <math.h>
#include <DynamixelSDK.h>
#include <Model.h>
#include <ModelControl.h>
#include <ModelConfig.h>

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

int8_t input[20];
int inCount = 0;
int8_t inByte = 0;
uint8_t param_goal_position[4];

union U {
  int16_t s;

  struct Byte {
    int8_t c1, c2;
  }
  byte;
};
ModelControl mControl;
ModelConfig mConfig;
Model model;
bool useIK = true;
bool useWalk = true;
unsigned long previousTime;
void setup() {

  Serial.begin(115200);
  Serial2.begin(57600);
  delay(1000);

  portHandler = dynamixel::PortHandler::getPortHandler(DEVICENAME);
  packetHandler = dynamixel::PacketHandler::getPacketHandler(PROTOCOL_VERSION);
  groupBulkWrite = new dynamixel::GroupBulkWrite(portHandler, packetHandler);
  portHandler->openPort();

  
    
    for (int i = 0; i < 18; i++)
    {
      int pos = i * 2;
      dxl_comm_result = packetHandler->write1ByteTxRx(portHandler, motorIDs[i], ADDRESS_TORQUE_ENABLE, TORQUE_ENABLE, &dxl_error);
      float val  = 2000.f;

      param_goal_position[0] = DXL_LOBYTE(DXL_LOWORD(val));
      param_goal_position[1] = DXL_HIBYTE(DXL_LOWORD(val));
      param_goal_position[2] = DXL_LOBYTE(DXL_HIWORD(val));
      param_goal_position[3] = DXL_HIBYTE(DXL_HIWORD(val));
      groupBulkWrite->addParam(motorIDs[i], ADDRESS_GOAL_POSITION, LENGTH_GOAL_POSITION, param_goal_position);

    }
    groupBulkWrite->txPacket();
  previousTime = millis();
}

void loop() {
  updateSerial();

  if (mConfig.isDirty) {
    model.setConfig(&mConfig);
    mControl.isDirty = true;
  }
  if (mControl.isDirty) {
    model.setControl(&mControl);
  }
  unsigned long currentTime = millis();
  float delta = (float)( currentTime - previousTime) / 1000.f;

  previousTime = currentTime;
  if (useWalk && useIK) {
    model.update(delta);
  }
  else
  {
    model.updateIK();
  }
  float adj = - 0.141;
  if (useIK) {
    for (int i = 0; i < 6; i++)
    {
      float factor = 1;
      if (i > 2)factor = -1;

      int j = i * 3;

      float s1 = -1 * model.legs[i]->shoulder1Angle / 3.14159 * 2048 + 2048;
      param_goal_position[0] = DXL_LOBYTE(DXL_LOWORD(s1));
      param_goal_position[1] = DXL_HIBYTE(DXL_LOWORD(s1));
      param_goal_position[2] = DXL_LOBYTE(DXL_HIWORD(s1));
      param_goal_position[3] = DXL_HIBYTE(DXL_HIWORD(s1));
      groupBulkWrite->changeParam(motorIDs[j], ADDRESS_GOAL_POSITION, LENGTH_GOAL_POSITION, param_goal_position);


      float s2 = factor * model.legs[i]->shoulder2Angle / 3.14159 * 2048 + 2048;

      param_goal_position[0] = DXL_LOBYTE(DXL_LOWORD(s2));
      param_goal_position[1] = DXL_HIBYTE(DXL_LOWORD(s2));
      param_goal_position[2] = DXL_LOBYTE(DXL_HIWORD(s2));
      param_goal_position[3] = DXL_HIBYTE(DXL_HIWORD(s2));
      groupBulkWrite->changeParam(motorIDs[j + 1], ADDRESS_GOAL_POSITION, LENGTH_GOAL_POSITION, param_goal_position);


      float _kneeAngle = model.legs[i]->shoulder3Angle;
      if (_kneeAngle < -3.1415) _kneeAngle += 3.141592 * 2;
      if (_kneeAngle > 3.1415) _kneeAngle -= 3.141592 * 2;

      int s3 = (_kneeAngle  + adj * factor) / 3.14159 * 2048 + 2048;

      param_goal_position[0] = DXL_LOBYTE(DXL_LOWORD(s3));
      param_goal_position[1] = DXL_HIBYTE(DXL_LOWORD(s3));
      param_goal_position[2] = DXL_LOBYTE(DXL_HIWORD(s3));
      param_goal_position[3] = DXL_HIBYTE(DXL_HIWORD(s3));
      groupBulkWrite->changeParam(motorIDs[j + 2], ADDRESS_GOAL_POSITION, LENGTH_GOAL_POSITION, param_goal_position);


    }

    groupBulkWrite->txPacket();



  }
  mConfig.isDirty = false;
  mControl.isDirty = false;
  /*
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

  */
}


void updateSerial() {
  while ( Serial2.available() > 0)
  {

    int8_t currentByte =  Serial2.read();

    if (inByte == 100 && currentByte == 100)
    {
      inCount = 0;
    } else
    {
      input[inCount] = currentByte;

      inCount++;

      if (inCount == 17)
      {
        U u;
        u.byte.c1 = input[0];
        u.byte.c2 = input[1];
        float lX = u.s;

        u.byte.c1 = input[2];
        u.byte.c2 = input[3];
        float lY = u.s;

        u.byte.c1 = input[4];
        u.byte.c2 = input[5];
        float lZ = u.s;

        u.byte.c1 = input[6];
        u.byte.c2 = input[7];
        float rX = u.s;

        u.byte.c1 = input[8];
        u.byte.c2 = input[9];
        float rY = u.s;

        u.byte.c1 = input[10];
        u.byte.c2 = input[11];
        float rZ = u.s;

        inCount = 0;
        Vector2 r = Vector2(rX, rY);
        float size = std::min(1000.f, r.length() / 1000.f);
        float angle = atan2(r.x, r.y);

        mControl.isDirty = true;
        mControl.moveAngle = angle;
        mControl.moveDistance = size * 45;
        mControl.turnAngle = rZ / 1000.0f * 0.13f;
        if (lY < 0) {
          mControl.timeScale = 1 + lY / -500;
        }
        else {
          mControl.timeScale = 1.f - (lY / 1000.f * 0.8f);
        }

      }

    }
    inByte = currentByte;
  }

}
