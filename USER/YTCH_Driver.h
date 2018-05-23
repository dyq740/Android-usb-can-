

#define TIME_OUT_LIMIT    1000   // 100ms

#define SUCCESS         0       // 读取或者写入正确
#define WRONG_LENGTH   -1       // 函数输入的数据长度错误
#define WRONG_RESPONSE   -2     //接收到错误的回应 例如数据损坏 CRC错误等
#define EXCEPTION_RESPONSE   -3 // 驱动器返回指令错误 例如地址错误等
#define TIME_OUT             -4 // 超过一段时间没有收到回复， 回复包括正确的回复和错误回复

#define PROFILE_POSITION_MODE  1   // 规划位置模式
#define PROFILE_VELOCITY_MODE  3   // 规划速度模式
#define HOMING_MODE            6   // 回零
#define POSITION_MODE          -1  // 位置模式,无轨迹规划
#define VELOCITY_MODE          -2  // 速度模式,无轨迹规划
#define CURRENT_MODE           -3  // 电流模式,扭矩模式
#define STEP_DIRECTION_MODE    -6  // 脉冲方向控制

//状态字位
#define TARGET_REACHED_BIT 0x0400


/*  Function Codes 
   ---------------
  defined in the canopen DS301 
*/
#define NMT	   0x0
#define SYNC       0x1
#define TIME_STAMP 0x2
#define PDO1tx     0x3
#define PDO1rx     0x4
#define PDO2tx     0x5
#define PDO2rx     0x6
#define PDO3tx     0x7
#define PDO3rx     0x8
#define PDO4tx     0x9
#define PDO4rx     0xA
#define SDOtx      0xB
#define SDOrx      0xC
#define NODE_GUARD 0xE
#define LSS 	   0xF


int8_t Read_Driver_Reg(uint8_t dev_addr,uint16_t index, uint8_t subindex ,uint8_t *data, uint8_t len);
int8_t Write_Driver_Reg(uint8_t dev_addr,uint16_t index, uint8_t subindex ,uint8_t *data, uint8_t len);

int8_t Enable_Driver(uint8_t dev_addr);
int8_t Disable_Driver(uint8_t dev_addr);
int8_t Read_Statusword(uint8_t dev_addr,uint16_t *statusword);
int8_t Switch_Driver_Mode(uint8_t dev_addr,int8_t mode);
int8_t PVM_Set_Speed(uint8_t dev_addr,int32_t speed);
int8_t PPM_Set_Position(uint8_t dev_addr,int32_t position,uint8_t relative);
int8_t Read_Position(uint8_t dev_addr,int32_t *position);
