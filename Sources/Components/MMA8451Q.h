/**
 *
 *
 * @file MMA8451Q.h
 * @author Valentin Korenblit
 * @date June, 2016
 * @brief Header file for MMA8451Q.c
 *
 *
 */

#ifndef SOURCES_COMPONENTS_MMA8451Q_H_
#define SOURCES_COMPONENTS_MMA8451Q_H_

#include "i2cCom1.h"
#include "accel.h"
#include "CONSOLE.h"
/**
 * @brief MMA8451Q I2C ADDRESS
 *
 */
#define MMA8451Q_I2C_ADDRESS 0x1D
/**
 * @brief BAUDRATE SET TO 100kbps
 *
 */
#define MMA8451Q_I2C_BAUDRATE 100

/**
 * @brief MMA8451 MAX Y ANGLE VALUE WHEN BOARD PLACED HORIZONTAL
 * @details 45 DEGREES IS THE MAX Y ANGLE VALUE
 *
 */
#define MMA8451_Y_ANGLE_HORIZONTAL_OK 70

/**
 * @brief MMA8451 MAX X ANGLE VALUE WHEN BOARD PLACED HORIZONTAL
 * @details 45 DEGREES IS THE MAX X ANGLE VALUE
 *
 */
#define MMA8451_X_ANGLE_HORIZONTAL_OK 70
/**
 * @brief Struct for MMA8451Q properties
 *
 */
typedef struct{
	accel_dev_t accDev;
	accel_dev_interface_t accDevice;
	accel_sensor_data_t accelData;
	accel_i2c_interface_t i2cInterface;
	int16_t xData ;
	int16_t yData;
	int16_t xAngle;
	int16_t yAngle;
}mma8451q_t;

typedef struct{
	MMA8451_state_t state = MMA8451_OK;
}mma8451_position;

typedef enum{
	MMA8451_OK,
	MMA8451_FALL
}MMA8451_state_t;

typedef enum{
	MMA8451_VERTICAL,
	MMA8451_HORIZONTAL
}
MMA8451_orientation_t;

/**
 *
 * This method initializes onboard accelerometer
 *
 *
 * @author Valentin Korenblit
 *
 */
void MMA8451Q_INIT();
/**
 *
 * This method turns off I2C0 master
 *
 *
 * @author Valentin Korenblit
 *
 */
void MMA8451Q_DEINIT();
/**
 *
 * This method gets current accelerometer data and updates structure members
 * @author Valentin Korenblit
 *
 */

void MMA8451Q_GET_DATA();
/**
 *
 * This method returns x value
 * @author Valentin Korenblit
 *
 */
int16_t MMA8451Q_GET_X();
/**
 *
 * This method returns y value
 * @author Valentin Korenblit
 *
 */
int16_t MMA8451Q_GET_Y();
/**
 *
 * This method returns x angle
 * @author Valentin Korenblit
 *
 */
int16_t MMA8451Q_GET_X_ANGLE();
/**
 *
 * This method returns y angle
 * @author Valentin Korenblit
 *
 */
int16_t MMA8451Q_GET_Y_ANGLE();
/**
 *
 * This method checks if the board has changed its normal state
 * @param orientation, MMA8451_VERTICAL if board is placed vertical (USB ports up) or MMA8451_HORIZONTAL if board is placed horizontal
 * @return MMA8451_OK if board stays in the desired place or MMA8451_FALL if it has changed
 * @author Valentin Korenblit
 *
 */
MMA8451_state_t MMA8451_GET_STATE(MMA8451_orientation_t orientation);
#endif /* SOURCES_COMPONENTS_MMA8451Q_H_ */
