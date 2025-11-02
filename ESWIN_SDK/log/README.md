# 使用说明

## 1. 默认配置与接口
| function name | port number | function number | board connector |
|:------------- |:------------|:----------------|:----------------|
| UART2 TX      |    PA16     |      ALT3       |    J8           |
| UART2 RX      |    PA18     |      ALT3       |    J8           |
| GND           |      \      |       \         |    J8           |


* 115200 baud
* One stop bit
* No parity
* No flow control

## 2. 使用方法
封装了以下接口，无需任何初始化，直接调用即可。
```c
    log_debug("Hello world\n");
	log_info("Hello world\n");
	log_warn("Hello world\n");
	log_err("Hello world\n");
	log_fatal("Hello world\n");
```

打印效果如下：
```shell
[DEBUG] Hello world
[INFO] Hello world
[WARN @main:112] Hello world
[ERROR @main:113] Hello world
[FATAL @main:114] Hello world
```

log.h中通过_DEBUG宏使能log功能，使用LOG_LV配置log打印级别。

```c
/**
 * @brief Log module switch,comment out '__DEBUG' if you not want log output
 *
 */
#define __DEBUG

/**
 * @brief log level,print current or higher level logs:
 * LOG_FATAL,
 * LOG_ERROR,
 * LOG_WARN,
 * LOG_INFO,
 * LOG_DEBUG
 */
#define LOG_LV LOG_DEBUG
```

## 3. 注意事项
* 默认使用UART2 PA16/PA18管脚，如果存在硬件资源冲突，可重写 logInit函数改到其他的端口，参考log.c文件。
* log打印为阻塞打印，请勿在中断等时间敏感函数中使用log打印。