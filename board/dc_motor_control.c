/**
 * @file dc_motor_control.c
 * @brief 直流电机高级控制函数实现
 * @author 林木@江南大学
 * @date 
 *
 * @details
 * 基于EAM20XX电机控制函数库的GM37-520直流电机控制实现
 */

#include "dc_motor_control.h"
#include "supertmr_qd_driver.h"
#include <math.h>  // 添加数学库头文件，用于fabs函数
// 删除对stdint.h的引用，使用RISCV_Typedefs.h中的定义

// 定义时间计算相关变量
#define CONTROL_PERIOD_MS 10  // 控制周期，单位：毫秒
static uint32_t g_u32LastControlTime = 0; // 上次控制时间

// 定义电机校准系数，用于平衡不同电机的速度差异
// 通过调整这些系数可以使所有电机在相同速度参考值下实际速度接近
#define MOTOR1_CAL_FACTOR 2.5f  // 右后轮校准系数(无编码器)，大幅增加补偿
#define MOTOR2_CAL_FACTOR 1.0f  // 右前轮校准系数(有编码器)
#define MOTOR3_CAL_FACTOR 1.0f  // 左前轮校准系数(有编码器)
#define MOTOR4_CAL_FACTOR 2.5f  // 左后轮校准系数(无编码器)，大幅增加补偿

// 定义低速时的死区补偿
#define MOTOR_MIN_SPEED 0.05f   // 最小有效速度
#define MOTOR_MIN_DUTY  0x2000  // 最小占空比(约12.5%)，降低以适应低速需求

// 定义开环电机的补偿增强系数
#define OPEN_LOOP_BOOST_FACTOR 1.5f  // 无编码器电机的输出增强因子

// 定义固定占空比
#define FIXED_DUTY_CYCLE 0.4f   // 所有电机固定占空比设为40%

// 自适应估计相关参数
#define CALIBRATION_PERIOD_MS 2000  // 校准周期(毫秒)
#define SPEED_STABILITY_THRESHOLD 1.0f  // 速度稳定阈值，降低以提高稳定度
#define MIN_STABLE_COUNT 20        // 最小稳定计数，增加以获得更稳定的结果

// 后轮参考速度（开环估算）
static float32_t g_f32RearWheelRefSpeed = 30.0f;  // 估计后轮在30%占空比下的速度

// 自适应估计状态
typedef enum {
    ADAPTIVE_STATE_INIT,      // 初始状态
    ADAPTIVE_STATE_CALIBRATING, // 校准状态(前轮也使用30%占空比)
    ADAPTIVE_STATE_CONTROLLING  // 控制状态(前轮使用闭环控制匹配后轮)
} AdaptiveEstimateState_t;

static AdaptiveEstimateState_t g_eAdaptiveState = ADAPTIVE_STATE_INIT;
static uint32_t g_u32CalibrationStartTime = 0;
static uint32_t g_u32StableCount = 0;
static float32_t g_f32LastFrontWheelSpeed = 0.0f;
static float32_t g_f32SpeedSum = 0.0f;

// 电机编码器配置
supertmr_quad_decode_config_t g_stMotor2EncoderConfig;
supertmr_quad_decode_config_t g_stMotor3EncoderConfig;

// 定义控制结构体
#if (RISCV_SUPPORT_F32 == RISCV_STD_ON)
// 速度PI控制器
E_GFLIB_CONTROLLER_PIAW_R_T_F32 g_stMotor1SpeedController = E_GFLIB_CONTROLLER_PIAW_R_DEFAULT_F32;
E_GFLIB_CONTROLLER_PIAW_R_T_F32 g_stMotor2SpeedController = E_GFLIB_CONTROLLER_PIAW_R_DEFAULT_F32;
E_GFLIB_CONTROLLER_PIAW_R_T_F32 g_stMotor3SpeedController = E_GFLIB_CONTROLLER_PIAW_R_DEFAULT_F32;
E_GFLIB_CONTROLLER_PIAW_R_T_F32 g_stMotor4SpeedController = E_GFLIB_CONTROLLER_PIAW_R_DEFAULT_F32;

// 低通滤波器（用于速度滤波）
E_GDFLIB_FILTER_IIR1_T_F32 g_stMotor1SpeedFilter = E_GDFLIB_FILTER_IIR1_DEFAULT_F32;
E_GDFLIB_FILTER_IIR1_T_F32 g_stMotor2SpeedFilter = E_GDFLIB_FILTER_IIR1_DEFAULT_F32;
E_GDFLIB_FILTER_IIR1_T_F32 g_stMotor3SpeedFilter = E_GDFLIB_FILTER_IIR1_DEFAULT_F32;
E_GDFLIB_FILTER_IIR1_T_F32 g_stMotor4SpeedFilter = E_GDFLIB_FILTER_IIR1_DEFAULT_F32;

// 电机速度和参考速度
float32_t g_af32MotorSpeeds[4] = {0.0f, 0.0f, 0.0f, 0.0f};     // 实际速度（滤波后）
float32_t g_af32MotorSpeedRefs[4] = {0.0f, 0.0f, 0.0f, 0.0f};  // 参考速度
float32_t g_af32RawMotorSpeeds[4] = {0.0f, 0.0f, 0.0f, 0.0f};  // 原始速度（未滤波）

// 编码器计数值
uint16_t g_au16EncoderCounts[4] = {0, 0, 0, 0};       // 当前编码器计数值
uint16_t g_au16LastEncoderCounts[4] = {0, 0, 0, 0};   // 上次编码器计数值

// 电机方向记录（用于防止突然反转）
static uint8_t g_au8LastMotorDirection[4] = {FORWARD, FORWARD, FORWARD, FORWARD};

// 系统时钟计数器
static uint32_t g_u32SystemTickCounter = 0;
#endif

/**
 * @brief 获取系统时间（毫秒）
 * 使用简单的计数器替代OS_GetCurrentTime
 */
static uint32_t GetSystemTime(void)
{
    // 返回系统计数器值
    return g_u32SystemTickCounter;
}

/**
 * @brief 更新系统时间
 * 需要在每个控制循环中调用
 */
void DCMotor_UpdateSystemTime(uint32_t deltaMs)
{
    g_u32SystemTickCounter += deltaMs;
}

/**
 * @brief 获取对应电机的PI控制器指针
 * @param motorIndex 电机索引(0-3)
 * @return PI控制器指针
 */
static E_GFLIB_CONTROLLER_PIAW_R_T_F32* DCMotor_GetController(uint8_t motorIndex)
{
    switch(motorIndex) {
        case 0: return &g_stMotor1SpeedController;
        case 1: return &g_stMotor2SpeedController;
        case 2: return &g_stMotor3SpeedController;
        case 3: return &g_stMotor4SpeedController;
        default: return NULL;
    }
}

/**
 * @brief 获取对应电机的滤波器指针
 * @param motorIndex 电机索引(0-3)
 * @return 滤波器指针
 */
static E_GDFLIB_FILTER_IIR1_T_F32* DCMotor_GetFilter(uint8_t motorIndex)
{
    switch(motorIndex) {
        case 0: return &g_stMotor1SpeedFilter;
        case 1: return &g_stMotor2SpeedFilter;
        case 2: return &g_stMotor3SpeedFilter;
        case 3: return &g_stMotor4SpeedFilter;
        default: return NULL;
    }
}

/**
 * @brief 简单的延时函数
 * @param ms 延时毫秒数
 */
static void SimpleDelay(uint32_t ms)
{
    // 简单的软件延时，不精确但可用于方向切换时的短暂延时
    volatile uint32_t i, j;
    for(i = 0; i < ms; i++)
        for(j = 0; j < 10000; j++); // 延时循环
}

/**
 * @brief 设置电机速度并控制方向
 * @param motorIndex 电机索引(0-3)
 * @param speed 速度值(-1.0到1.0)，负值表示反向
 */
static void DCMotor_SetMotorSpeed(uint8_t motorIndex, float32_t speed)
{
    uint8_t direction;
    uint16_t duty;
    float32_t absSpeed = (speed < 0.0f) ? -speed : speed;
    
    // 确定方向
    direction = (speed >= 0.0f) ? FORWARD : BACKWARD;
    
    // 反向保护：如果方向改变且速度较大，先减速
    if ((g_au8LastMotorDirection[motorIndex] != direction) && 
        (g_af32MotorSpeeds[motorIndex] > 0.3f || g_af32MotorSpeeds[motorIndex] < -0.3f)) {
        
        // 设置为0速度，先停止电机
        switch(motorIndex) {
            case 0: SetMotor1Speed(0); break;
            case 1: SetMotor2Speed(0); break;
            case 2: SetMotor3Speed(0); break;
            case 3: SetMotor4Speed(0); break;
        }
        
        // 延时一段时间让电机减速
        SimpleDelay(50);
    }
    
    // 记录当前方向
    g_au8LastMotorDirection[motorIndex] = direction;
    
    // 设置电机方向
    switch(motorIndex) {
        case 0: SetMotor1Direction(direction); break;
        case 1: SetMotor2Direction(direction); break;
        case 2: SetMotor3Direction(direction); break;
        case 3: SetMotor4Direction(direction); break;
    }
    
    // 将速度值转换为PWM占空比 (0 - 0xFFFF)
    duty = (uint16_t)(absSpeed * 0xFFFF);
    
    // 设置电机速度
    switch(motorIndex) {
        case 0: SetMotor1Speed(duty); break;
        case 1: SetMotor2Speed(duty); break;
        case 2: SetMotor3Speed(duty); break;
        case 3: SetMotor4Speed(duty); break;
    }
}

/**
 * @brief 初始化直流电机控制器（开环控制版）
 */
void DCMotor_Init(void)
{
#if (RISCV_SUPPORT_F32 == RISCV_STD_ON)
    // 由于使用开环控制，不再需要初始化PI控制器和滤波器
    // 所有控制器和滤波器参数保留默认值即可

    // 对于状态显示和监控，仍初始化变量
    for(int i = 0; i < 4; i++) {
        g_af32MotorSpeeds[i] = 0.0f;
        g_af32MotorSpeedRefs[i] = 0.0f;
        g_af32RawMotorSpeeds[i] = 0.0f;
        g_au16EncoderCounts[i] = 0;
        g_au16LastEncoderCounts[i] = 0;
    }
#endif

    // 初始化PWM
    InitAllPWM();
    
    // 初始化系统时间
    g_u32LastControlTime = 0;
    g_u32SystemTickCounter = 0;
}

/**
 * @brief 初始化编码器
 */
void DCMotor_InitEncoders(void)
{
    // 初始化SuperTimer实例1(电机2编码器)
    supertmr_state_t stSupertmr1State;
    supertmr_user_config_t stSupertmr1UserConfig = {
        .syncMethod = {
            .softwareSync     = true,
            .hardwareSync0    = false,
            .hardwareSync1    = false,
            .hardwareSync2    = false,
            .maxLoadingPoint  = false,
            .minLoadingPoint  = false,
            .inverterSync     = SUPERTMR_PWM_SYNC,
            .outRegSync       = SUPERTMR_PWM_SYNC,
            .maskRegSync      = SUPERTMR_PWM_SYNC,
            .initCounterSync  = SUPERTMR_PWM_SYNC,
            .autoClearTrigger = false,
            .syncPoint        = SUPERTMR_UPDATE_NOW,
        },
        .supertmrMode                = SUPERTMR_MODE_QUADRATURE_DECODER, // 直接设置为正交解码器模式
        .supertmrPrescaler           = SUPERTMR_CLOCK_DIVID_BY_1,
        .supertmrClockSource         = SUPERTMR_CLOCK_SOURCE_SYSTEMCLK,
        .BDMMode                     = SUPERTMR_BDM_MODE_00,
        .isTofIsrEnabled             = false,
        .enableInitializationTrigger = false,
        .callback                    = NULL,
        .cbParams                    = NULL,
    };
    
    // 初始化SuperTimer实例1
    status_t status = SUPERTMR_DRV_Init(MOTOR2_ENCODER_INSTANCE, &stSupertmr1UserConfig, &stSupertmr1State);
    if (status != STATUS_SUCCESS) {
        // 如果初始化失败，尝试继续，不进入无限循环
        return; // 直接返回，不再继续初始化
    }
    
    // 初始化SuperTimer实例2(电机3编码器)
    supertmr_state_t stSupertmr2State;
    supertmr_user_config_t stSupertmr2UserConfig = stSupertmr1UserConfig; // 复制相同配置
    
    // 初始化SuperTimer实例2
    status = SUPERTMR_DRV_Init(MOTOR3_ENCODER_INSTANCE, &stSupertmr2UserConfig, &stSupertmr2State);
    if (status != STATUS_SUCCESS) {
        // 如果初始化失败，进入无限循环
        while(1) {}
    }
    
    // 获取电机2编码器默认配置
    SUPERTMR_QD_DRV_GetDefaultConfig(&g_stMotor2EncoderConfig);
    
    // 自定义电机2编码器配置 - 参考MotorDemo的配置
    g_stMotor2EncoderConfig.mode = SUPERTMR_QUAD_COUNT_AND_DIR; // 使用计数和方向模式
    g_stMotor2EncoderConfig.initialVal = 0;
    g_stMotor2EncoderConfig.maxVal = 0xFFFF; // 16位计数器最大值
    g_stMotor2EncoderConfig.phaseAConfig.phaseInputFilter = true;
    g_stMotor2EncoderConfig.phaseAConfig.phaseFilterVal = 2;  // 改为与MotorDemo一致
    g_stMotor2EncoderConfig.phaseAConfig.phasePolarity = SUPERTMR_QUAD_PHASE_NORMAL;
    g_stMotor2EncoderConfig.phaseBConfig.phaseInputFilter = true;
    g_stMotor2EncoderConfig.phaseBConfig.phaseFilterVal = 2;  // 改为与MotorDemo一致
    g_stMotor2EncoderConfig.phaseBConfig.phasePolarity = SUPERTMR_QUAD_PHASE_NORMAL;
    
    // 初始化电机2编码器
    status = SUPERTMR_DRV_QuadDecodeStart(MOTOR2_ENCODER_INSTANCE, &g_stMotor2EncoderConfig);
    if (status != STATUS_SUCCESS) {
        // 初始化失败处理
        while(1) {}
    }
    
    // 复制相同配置到电机3编码器
    g_stMotor3EncoderConfig = g_stMotor2EncoderConfig;
    
    // 初始化电机3编码器
    status = SUPERTMR_DRV_QuadDecodeStart(MOTOR3_ENCODER_INSTANCE, &g_stMotor3EncoderConfig);
    if (status != STATUS_SUCCESS) {
        // 初始化失败处理
        while(1) {}
    }
}

/**
 * @brief 设置电机速度参考
 * @param motorIndex 电机索引(0-3)
 * @param speedRef 速度参考(-1.0到1.0)
 */
void DCMotor_SetSpeedRef(uint8_t motorIndex, float32_t speedRef)
{
#if (RISCV_SUPPORT_F32 == RISCV_STD_ON)
    if(motorIndex < 4) {
        // 限制参考速度范围
        if (speedRef > 1.0f) {
            speedRef = 1.0f;
        } else if (speedRef < -1.0f) {
            speedRef = -1.0f;
        }
        
        g_af32MotorSpeedRefs[motorIndex] = speedRef;
    }
#endif
}

/**
 * @brief 更新编码器计数值
 */
void DCMotor_UpdateEncoderCounts(void)
{
    // 保存旧的编码器计数值
    g_au16LastEncoderCounts[1] = g_au16EncoderCounts[1]; // 电机2
    g_au16LastEncoderCounts[2] = g_au16EncoderCounts[2]; // 电机3
    
    // 读取电机2的编码器
    supertmr_quad_decoder_state_t encoderState2 = SUPERTMR_DRV_QuadGetState(MOTOR2_ENCODER_INSTANCE);
    g_au16EncoderCounts[1] = encoderState2.counter;
    
    // 读取电机3的编码器
    supertmr_quad_decoder_state_t encoderState3 = SUPERTMR_DRV_QuadGetState(MOTOR3_ENCODER_INSTANCE);
    g_au16EncoderCounts[2] = encoderState3.counter;
}

/**
 * @brief 计算电机速度
 */
void DCMotor_CalculateSpeeds(void)
{
    static uint32_t lastCalcTime = 0;
    uint32_t currentTime = GetSystemTime();
    uint32_t timeDiff = currentTime - lastCalcTime;
    
    // 确保有足够的时间间隔来计算速度
    if (timeDiff >= CONTROL_PERIOD_MS) {
        // 计算电机2和电机3的实际速度（使用编码器）
        // 计算脉冲差值（考虑溢出情况）
        int32_t pulses = 0;  // 改为有符号类型以处理正负方向
        
        // 电机2速度计算
        if (g_au16EncoderCounts[1] >= g_au16LastEncoderCounts[1]) {
            pulses = g_au16EncoderCounts[1] - g_au16LastEncoderCounts[1];
            // 如果差值异常大，可能是反向溢出
            if (pulses > 32768) {
                pulses = -(65536 - pulses);
            }
        } else {
            // 编码器值减小的情况
            pulses = -(g_au16LastEncoderCounts[1] - g_au16EncoderCounts[1]);
            // 如果差值异常大，可能是正向溢出
            if (pulses < -32768) {
                pulses = 65536 + pulses;
            }
        }
        
        // 参考MotorDemo计算电机速度的方法
        // RPM = 脉冲数 / (每转脉冲数 * 频率因子) / 时间(分钟) / 减速比
        float32_t timeMinutes = (float32_t)timeDiff / 60000.0f; // 转换为分钟
        
        // 使用脉冲的绝对值计算速度，然后根据方向赋值正负
        g_af32RawMotorSpeeds[1] = (float32_t)abs(pulses) / (float32_t)(MOTOR_ENCODER_PPR * MOTOR_GEAR_RATIO) / timeMinutes;
        
        // 保持速度值在合理范围内
        if (g_af32RawMotorSpeeds[1] > 200.0f) {
            g_af32RawMotorSpeeds[1] = 200.0f; // 限制最大速度
        }
        
        // 如果速度太小，视为停止
        if (g_af32RawMotorSpeeds[1] < 0.1f) {
            g_af32RawMotorSpeeds[1] = 0.0f;
        }
        
        // 根据脉冲方向应用正负号
        if (pulses < 0) {
            g_af32RawMotorSpeeds[1] = -g_af32RawMotorSpeeds[1];
        }
        
        // 根据参考速度方向纠正计算的速度方向（如果编码器安装方向与期望不符）
        if ((g_af32MotorSpeedRefs[1] < 0.0f && g_af32RawMotorSpeeds[1] > 0.0f) || 
            (g_af32MotorSpeedRefs[1] > 0.0f && g_af32RawMotorSpeeds[1] < 0.0f)) {
            g_af32RawMotorSpeeds[1] = -g_af32RawMotorSpeeds[1];
        }
        
        // 电机3速度计算，同上
        pulses = 0;
        if (g_au16EncoderCounts[2] >= g_au16LastEncoderCounts[2]) {
            pulses = g_au16EncoderCounts[2] - g_au16LastEncoderCounts[2];
            // 如果差值异常大，可能是反向溢出
            if (pulses > 32768) {
                pulses = -(65536 - pulses);
            }
        } else {
            // 编码器值减小的情况
            pulses = -(g_au16LastEncoderCounts[2] - g_au16EncoderCounts[2]);
            // 如果差值异常大，可能是正向溢出
            if (pulses < -32768) {
                pulses = 65536 + pulses;
            }
        }
        
        // 使用脉冲的绝对值计算速度，然后根据方向赋值正负
        g_af32RawMotorSpeeds[2] = (float32_t)abs(pulses) / (float32_t)(MOTOR_ENCODER_PPR * MOTOR_GEAR_RATIO) / timeMinutes;
        
        // 保持速度值在合理范围内
        if (g_af32RawMotorSpeeds[2] > 200.0f) {
            g_af32RawMotorSpeeds[2] = 200.0f; // 限制最大速度
        }
        
        // 如果速度太小，视为停止
        if (g_af32RawMotorSpeeds[2] < 0.1f) {
            g_af32RawMotorSpeeds[2] = 0.0f;
        }
        
        // 根据脉冲方向应用正负号
        if (pulses < 0) {
            g_af32RawMotorSpeeds[2] = -g_af32RawMotorSpeeds[2];
        }
        
        // 根据参考速度方向纠正计算的速度方向（如果编码器安装方向与期望不符）
        if ((g_af32MotorSpeedRefs[2] < 0.0f && g_af32RawMotorSpeeds[2] > 0.0f) || 
            (g_af32MotorSpeedRefs[2] > 0.0f && g_af32RawMotorSpeeds[2] < 0.0f)) {
            g_af32RawMotorSpeeds[2] = -g_af32RawMotorSpeeds[2];
        }
        
        // 电机1和电机4没有编码器，使用估算速度
        g_af32RawMotorSpeeds[0] = g_af32MotorSpeedRefs[0] * 0.9f;
        g_af32RawMotorSpeeds[3] = g_af32MotorSpeedRefs[3] * 0.9f;
        
        // 应用滤波器平滑速度
        g_af32MotorSpeeds[0] = E_GDFLIB_FilterIIR1_F32(g_af32RawMotorSpeeds[0], &g_stMotor1SpeedFilter);
        g_af32MotorSpeeds[1] = E_GDFLIB_FilterIIR1_F32(g_af32RawMotorSpeeds[1], &g_stMotor2SpeedFilter);
        g_af32MotorSpeeds[2] = E_GDFLIB_FilterIIR1_F32(g_af32RawMotorSpeeds[2], &g_stMotor3SpeedFilter);
        g_af32MotorSpeeds[3] = E_GDFLIB_FilterIIR1_F32(g_af32RawMotorSpeeds[3], &g_stMotor4SpeedFilter);
        
        // 更新时间
        lastCalcTime = currentTime;
    }
}

/**
 * @brief 获取电机速度
 * @param motorIndex 电机索引(0-3)
 * @return 电机速度(-1.0到1.0)
 */
float32_t DCMotor_GetMotorSpeed(uint8_t motorIndex)
{
    if (motorIndex < 4) {
        return g_af32MotorSpeeds[motorIndex];
    }
    return 0.0f;
}

/**
 * @brief 自适应后轮速度估计
 */
static void AdaptiveRearWheelSpeedEstimate(void)
{
    uint32_t currentTime = GetSystemTime();
    float32_t frontWheelSpeed;
    
    // 计算前轮平均速度(电机2和3的平均值)，使用绝对值确保不受方向影响
    frontWheelSpeed = (fabs(g_af32MotorSpeeds[1]) + fabs(g_af32MotorSpeeds[2])) * 0.5f;
    
    switch(g_eAdaptiveState)
    {
        case ADAPTIVE_STATE_INIT:
            // 初始阶段：记录开始时间，前轮设为30%占空比
            g_u32CalibrationStartTime = currentTime;
            g_eAdaptiveState = ADAPTIVE_STATE_CALIBRATING;
            g_u32StableCount = 0;
            g_f32SpeedSum = 0.0f;
            
            // 默认设置一个合理的参考速度，避免开始时的混乱
            g_f32RearWheelRefSpeed = 30.0f;  // 设置默认值30，相当于30%占空比的预期速度
            
            // 重置PI控制器状态，避免残留的积分项影响
            for(int i = 1; i <= 2; i++) {
                E_GFLIB_CONTROLLER_PIAW_R_T_F32* controller = DCMotor_GetController(i);
                if(controller != NULL) {
                    // 使用库函数重置控制器状态为0
                    E_GFLIB_ControllerPIrAWSetState_F32(0.0f, controller);
                }
            }
            
            // 直接进入控制状态，跳过校准阶段，使用预设值
            g_eAdaptiveState = ADAPTIVE_STATE_CONTROLLING;
            break;
            
        case ADAPTIVE_STATE_CALIBRATING:
            // 跳过前300毫秒数据，等待系统稳定（原来是500ms，缩短以加快响应）
            if (currentTime - g_u32CalibrationStartTime < 300) {
                break;
            }
            
            // 检查速度是否稳定
            if (fabs(frontWheelSpeed - g_f32LastFrontWheelSpeed) < SPEED_STABILITY_THRESHOLD) {
                g_u32StableCount++;
                g_f32SpeedSum += frontWheelSpeed;
                
                // 达到足够的稳定计数或超过校准时间
                // 减少MIN_STABLE_COUNT从20到10，加快校准过程
                if (g_u32StableCount >= 10 || 
                    (currentTime - g_u32CalibrationStartTime > CALIBRATION_PERIOD_MS)) {
                    
                    // 计算平均稳定速度
                    if (g_u32StableCount > 0) {
                        g_f32RearWheelRefSpeed = g_f32SpeedSum / g_u32StableCount;
                    }
                    
                    // 确保估计速度不为零或过小/过大
                    if (g_f32RearWheelRefSpeed < 5.0f) {
                        g_f32RearWheelRefSpeed = 30.0f;  // 如果值太小，使用默认值
                    } else if (g_f32RearWheelRefSpeed > 150.0f) {
                        g_f32RearWheelRefSpeed = 150.0f;
                    }
                    
                    // 进入控制状态
                    g_eAdaptiveState = ADAPTIVE_STATE_CONTROLLING;
                    
                    // 重置PI控制器状态，开始新的控制过程
                    for(int i = 1; i <= 2; i++) {
                        E_GFLIB_CONTROLLER_PIAW_R_T_F32* controller = DCMotor_GetController(i);
                        if(controller != NULL) {
                            // 使用库函数重置控制器状态为0
                            E_GFLIB_ControllerPIrAWSetState_F32(0.0f, controller);
                            
                            // 调整PI参数，提高响应性并减小积分项
                            controller->f32CC1sc = 0.6f;  // 提高比例增益
                            controller->f32CC2sc = 0.08f; // 减小积分增益
                        }
                    }
                }
            } else {
                // 速度不稳定，重置计数
                g_u32StableCount = 0;
                g_f32SpeedSum = 0.0f;
            }
            break;
            
        case ADAPTIVE_STATE_CONTROLLING:
            // 控制状态：已经有了后轮速度估计值，前轮使用闭环控制匹配该速度
            // 无需额外操作
            break;
    }
    
    // 保存前轮速度用于下次比较
    g_f32LastFrontWheelSpeed = frontWheelSpeed;
}

/**
 * @brief 电机速度控制，修改为开环控制方式
 * @param motorIndex 电机索引(0-3)
 */
void DCMotor_SpeedControl(uint8_t motorIndex)
{
#if (RISCV_SUPPORT_F32 == RISCV_STD_ON)
    if(motorIndex < 4) {
        float32_t output = 0.0f;
        
        // 所有电机都使用开环控制，无论是否有编码器
        if (g_af32MotorSpeedRefs[motorIndex] != 0.0f) {
            // 使用固定占空比，保持方向
            output = (g_af32MotorSpeedRefs[motorIndex] > 0.0f) ? 
                     FIXED_DUTY_CYCLE : -FIXED_DUTY_CYCLE;
        } else {
            output = 0.0f;  // 如果参考速度为零，输出为零
        }
        
        // 限制输出范围在[-1.0, 1.0]之间
        if (output > 1.0f) {
            output = 1.0f;
        } else if (output < -1.0f) {
            output = -1.0f;
        }
        
        // 设置电机速度和方向
        DCMotor_SetMotorSpeed(motorIndex, output);
    }
#endif
}

/**
 * @brief 更新所有电机控制，简化为开环控制
 */
void DCMotor_UpdateAllMotors(void)
{
    uint32_t currentTime = GetSystemTime();
    
    // 根据控制周期执行更新
    if ((currentTime - g_u32LastControlTime) >= CONTROL_PERIOD_MS) {
        // 更新编码器计数 - 仅用于监控，不参与控制
        DCMotor_UpdateEncoderCounts();
        
        // 计算电机速度 - 仅用于监控，不参与控制
        DCMotor_CalculateSpeeds();
        
        // 执行电机控制（开环控制）
        for (uint8_t i = 0; i < 4; i++) {
            DCMotor_SpeedControl(i);
        }
        
        // 更新最后控制时间
        g_u32LastControlTime = currentTime;
    }
}

/**
 * @brief 直行 (开环控制)
 * @param speed 速度(0.0-1.0)
 */
void DCMotor_MoveForward(float32_t speed)
{
#if (RISCV_SUPPORT_F32 == RISCV_STD_ON)
    // 限制速度范围
    if (speed > 1.0f) {
        speed = 1.0f;
    } else if (speed < 0.0f) {
        speed = 0.0f;
    }
    
    // 设置电机方向 - 修正左前轮和右前轮方向
    SetMotor1Direction(FORWARD);   // 右后轮 - 前进
    SetMotor2Direction(FORWARD);  // 右前轮 - 反向为前进
    SetMotor3Direction(FORWARD);  // 左前轮 - 反向为前进
    SetMotor4Direction(FORWARD);   // 左后轮 - 前进
    
    // 设置所有电机的速度参考值
    DCMotor_SetSpeedRef(0, speed * MOTOR1_CAL_FACTOR); // 右后轮
    DCMotor_SetSpeedRef(1, speed * MOTOR2_CAL_FACTOR); // 右前轮
    DCMotor_SetSpeedRef(2, speed * MOTOR3_CAL_FACTOR); // 左前轮
    DCMotor_SetSpeedRef(3, speed * MOTOR4_CAL_FACTOR); // 左后轮
#endif
}

/**
 * @brief 后退 (开环控制)
 * @param speed 速度(0.0-1.0)
 */
void DCMotor_MoveBackward(float32_t speed)
{
#if (RISCV_SUPPORT_F32 == RISCV_STD_ON)
    // 限制速度范围
    if (speed > 1.0f) {
        speed = 1.0f;
    } else if (speed < 0.0f) {
        speed = 0.0f;
    }
    
    // 设置电机方向 - 修正左前轮和右前轮方向
    SetMotor1Direction(BACKWARD);  // 右后轮 - 后退
    SetMotor2Direction(FORWARD);   // 右前轮 - 正向为后退
    SetMotor3Direction(FORWARD);   // 左前轮 - 正向为后退
    SetMotor4Direction(BACKWARD);  // 左后轮 - 后退
    
    // 设置所有电机的速度参考值
    DCMotor_SetSpeedRef(0, -speed * MOTOR1_CAL_FACTOR); // 右后轮
    DCMotor_SetSpeedRef(1, -speed * MOTOR2_CAL_FACTOR); // 右前轮
    DCMotor_SetSpeedRef(2, -speed * MOTOR3_CAL_FACTOR); // 左前轮
    DCMotor_SetSpeedRef(3, -speed * MOTOR4_CAL_FACTOR); // 左后轮
#endif
}

/**
 * @brief 左转 (开环控制)
 * @param speed 速度(0.0-1.0)
 */
void DCMotor_TurnLeft(float32_t speed)
{
#if (RISCV_SUPPORT_F32 == RISCV_STD_ON)
    // 限制速度范围
    if (speed > 1.0f) {
        speed = 1.0f;
    } else if (speed < 0.0f) {
        speed = 0.0f;
    }
    
    // 设置电机方向 - 修正左前轮和右前轮方向
    SetMotor1Direction(FORWARD);   // 右后轮前进
    SetMotor2Direction(FORWARD);  // 右前轮前进（反向）
    SetMotor3Direction(BACKWARD);   // 左前轮后退（反向）
    SetMotor4Direction(BACKWARD);  // 左后轮后退
    
    // 设置左侧电机速度
    DCMotor_SetSpeedRef(2, -speed * MOTOR3_CAL_FACTOR); // 左前轮
    DCMotor_SetSpeedRef(3, -speed * MOTOR4_CAL_FACTOR); // 左后轮
    
    // 设置右侧电机速度
    DCMotor_SetSpeedRef(0, speed * MOTOR1_CAL_FACTOR);  // 右后轮
    DCMotor_SetSpeedRef(1, speed * MOTOR2_CAL_FACTOR);  // 右前轮
#endif
}

/**
 * @brief 右转 (开环控制)
 * @param speed 速度(0.0-1.0)
 */
void DCMotor_TurnRight(float32_t speed)
{
#if (RISCV_SUPPORT_F32 == RISCV_STD_ON)
    // 限制速度范围
    if (speed > 1.0f) {
        speed = 1.0f;
    } else if (speed < 0.0f) {
        speed = 0.0f;
    }
    
    // 设置电机方向 - 修正左前轮和右前轮方向
    SetMotor1Direction(BACKWARD);  // 右后轮后退
    SetMotor2Direction(BACKWARD);   // 右前轮后退（正向）
    SetMotor3Direction(FORWARD);  // 左前轮前进（正向）
    SetMotor4Direction(FORWARD);   // 左后轮前进
    
    // 设置右侧电机速度
    DCMotor_SetSpeedRef(0, -speed * MOTOR1_CAL_FACTOR); // 右后轮
    DCMotor_SetSpeedRef(1, -speed * MOTOR2_CAL_FACTOR); // 右前轮
    
    // 设置左侧电机速度
    DCMotor_SetSpeedRef(2, speed * MOTOR3_CAL_FACTOR);  // 左前轮
    DCMotor_SetSpeedRef(3, speed * MOTOR4_CAL_FACTOR);  // 左后轮
#endif
}

/**
 * @brief 停止所有电机（开环控制）
 */
void DCMotor_Stop(void)
{
#if (RISCV_SUPPORT_F32 == RISCV_STD_ON)
    // 设置所有电机参考速度为0
    for(int i = 0; i < 4; i++) {
        DCMotor_SetSpeedRef(i, 0.0f);
    }
    
    // 立即停止所有PWM输出
    StopAllMotors();
#endif
} 
