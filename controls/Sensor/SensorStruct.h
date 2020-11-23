#ifndef __SensorStruct_STRU__
#define __SensorStruct_STRU__


typedef struct UAVDdataStruct{
	int   year;
	int   mon;
	int   day;
	
	int   hour;
	int   min;
	int   sec;
	
	float longitude;
	float latitude;//纬度
	float hight;
}UAVDATASTR;

typedef struct AirSensorStruct{
	float O3;
	float SO2;
	float CO;
	float NO2;
	float pm1_0;
	float pm2_5;
	float pm10;
	float temperature;
	float humidity;
}AIRSENSORSTR;

typedef union{
	struct{
		unsigned char c0;
		unsigned char c1;
		unsigned char c2;
		unsigned char c3;
	}c;
	int i ;
	float f;
}SWITCHFLOAT2CHAR;


typedef struct GroundDataTimeStruct
{
    int   year;
    int   mon;
    int   day;

    int   hour;
    int   min;
    int   sec;
}GroundDataTime;
#endif //__COMMON_STRU__
