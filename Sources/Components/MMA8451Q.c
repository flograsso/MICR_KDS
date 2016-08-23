/**
 *
 *
 * @file MMA8451Q.c
 * @author Valentin Korenblit
 * @date June, 2016
 * @brief Driver for MMA8451Q, onboard accelerometer connected through I2C0
 *
 * Uses PTE24 (SCL) and PTE25 (SDA)
 * 100 kbps
 * MMA8451Q I2C ADDRESS = 0x1D
 *
 *
 */

#include "MMA8451Q.h"

mma8451q_t Mma8451q;



void MMA8451Q_INIT()
{
	I2C_DRV_MasterInit(i2cCom1_IDX, &i2cCom1_MasterState);
	I2C_DRV_MasterSetBaudRate(i2cCom1_IDX, &i2cCom1_MasterConfig0);
    // Register callback func for I2C
	Mma8451q.i2cInterface.i2c_init       =  I2C_DRV_MasterInit;
	Mma8451q.i2cInterface.i2c_read       =  I2C_DRV_MasterReceiveDataBlocking;
	Mma8451q.i2cInterface.i2c_write      =  I2C_DRV_MasterSendDataBlocking;

	Mma8451q.accDev.i2c      = &(Mma8451q.i2cInterface);
	Mma8451q.accDev.accel    = &(Mma8451q.accDevice);

	Mma8451q.accDev.slave.baudRate_kbps  = MMA8451Q_I2C_BAUDRATE;
	Mma8451q.accDev.slave.address        = MMA8451Q_I2C_ADDRESS;
	Mma8451q.accDev.bus                  = 0;

    // Initialize the Accel.
    accel_init(&(Mma8451q.accDev));
}

void MMA8451Q_DEINIT()
{
	I2C_DRV_MasterDeinit(i2cCom1_IDX);
}

void MMA8451Q_GET_DATA()
{
	// Get new accelerometer data.
	Mma8451q.accDev.accel->accel_read_sensor_data(&(Mma8451q.accDev),&(Mma8451q.accelData));

	// Get the X and Y data from the sensor data structure.fxos_data
	Mma8451q.xData = (int16_t)((Mma8451q.accelData.data.accelXMSB << 8) | Mma8451q.accelData.data.accelXLSB);
	Mma8451q.yData = (int16_t)((Mma8451q.accelData.data.accelYMSB << 8) | Mma8451q.accelData.data.accelYLSB);

	// Convert raw data to angle (normalize to 0-90 degrees).  No negative angles.
	Mma8451q.xAngle = abs((int16_t)(Mma8451q.xData * 0.011));
	Mma8451q.yAngle = abs((int16_t)(Mma8451q.yData * 0.011));
}


int16_t MMA8451Q_GET_X_ANGLE()
{
	return Mma8451q.xAngle;
}

int16_t MMA8451Q_GET_Y_ANGLE()
{
	return Mma8451q.yAngle;
}

int16_t MMA8451Q_GET_X()
{
	return Mma8451q.xData;
}

int16_t MMA8451Q_GET_Y()
{
	return Mma8451q.yData;
}

MMA8451_state_t MMA8451_GET_STATE(MMA8451_orientation_t orientation)
{
	MMA8451Q_GET_DATA();
	MMA8451_state_t exitCode;

	if(orientation == MMA8451_VERTICAL)
	{
		if( (Mma8451q.xData > MMA8451_X_AXIS_VERTICAL_OK) && (Mma8451q.yData < MMA8451_Y_AXIS_VERTICAL_OK))
		{
			exitCode = MMA8451_OK;
		}
		else
		{
			exitCode = MMA8451_FALL;
		}
	}
	return exitCode;
}



