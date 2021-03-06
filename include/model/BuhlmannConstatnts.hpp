//
// Created by guyfleeman on 11/28/20.
//

#ifndef LIBBUHLMAN_BUHLMANNCONSTATNTS_HPP
#define LIBBUHLMAN_BUHLMANNCONSTATNTS_HPP

namespace buhlmann {
namespace model {

/**
 * number of compartments in the ZHL 16-C model version
 */
constexpr int ZHL_16C_COMPARTMENTS = 17;

/**
 * number of compartments in the ZHL 16-B model version
 */
constexpr int ZHL_16B_COMPARTMENTS = 16;

/**
 * number of compartments in the ZHL 16-A model version
 */
constexpr int ZHL_16A_COMPARTMENTS = 16;

/**
 * buhlmann rq constant
 */
constexpr float BUHLMANN_RQ = 1.0f;

/**
 * ambient water vapor pressure
 */
constexpr float WATER_VAPOR_PRESSURE = 0.0627f;

/**
 * ambient CO2 pressure for alveolar sub-model
 */
constexpr float CO2_PRESSURE = 0.0534f;

/**
 * compartment constants
 */
typedef struct CompartmentContants {
	float nitrogenHalfTime;
	float nitrogenA;
	float nitrogenB;
	float heliumHalfTime;
	float heliumA;
	float heliumB;
} CompartmentConstants_t;

/**
 *
 */
const CompartmentConstants_t ZHL_16C_CONSTANTS[ZHL_16C_COMPARTMENTS] = {
		/*N2-1/2t, N2 A   , N2 B   , He-1/2t ,  He A   , He B       Cpt No.*/
		{4.0f,   1.2599f, 0.5240f, 1.51f,   1.7424f, 0.4245f}, /*  1  */
		{5.0f,   1.1696f, 0.5578f, 1.88f,   1.6189f, 0.4770f}, /*  1b */
		{8.0f,   1.0000f, 0.6514f, 3.02f,   1.3830f, 0.5747f}, /*  2  */
		{12.5f,  0.8618f, 0.7222f, 4.72f,   1.1919f, 0.6527f}, /*  3  */
		{18.5f,  0.7562f, 0.7825f, 6.99f,   1.0458f, 0.7223f}, /*  4  */
		{27.0f,  0.6491f, 0.8126f, 10.21f,  0.9220f, 0.7582f}, /*  5  */
		{38.3f,  0.5316f, 0.8434f, 14.48f,  0.8205f, 0.7957f}, /*  6  */
		{54.3f,  0.4681f, 0.8693f, 20.53f,  0.7305f, 0.8279f}, /*  7  */
		{77.0f,  0.4301f, 0.8910f, 29.11f,  0.6502f, 0.8553f}, /*  8  */
		{109.0f, 0.4049f, 0.9092f, 41.20f,  0.5950f, 0.8757f}, /*  9  */
		{146.0f, 0.3719f, 0.9222f, 55.19f,  0.5545f, 0.8903f}, /* 10  */
		{187.0f, 0.3447f, 0.9319f, 70.69f,  0.5333f, 0.8997f}, /* 11  */
		{239.0f, 0.3176f, 0.9403f, 90.34f,  0.5189f, 0.9073f}, /* 12  */
		{305.0f, 0.2828f, 0.9477f, 115.29f, 0.5181f, 0.9122f}, /* 13  */
		{390.0f, 0.2716f, 0.9544f, 147.42f, 0.5176f, 0.9171f}, /* 14  */
		{498.0f, 0.2523f, 0.9602f, 188.24f, 0.5172f, 0.9217f}, /* 15  */
		{635.0f, 0.2327f, 0.9653f, 240.03f, 0.5119f, 0.9267f}, /* 16  */
};

/**
 *
 */
const CompartmentConstants_t ZHL_16B_DATA[ZHL_16B_COMPARTMENTS] = {
		/*N2-1/2t, N2 A   , N2 B   , He-1/2t ,  He A   , He B       Cpt No.*/
		{4.0f,   1.2599f, 0.5240f, 1.51f,   1.6189f, 0.4245f}, /*  1  */
		// {    5.0f, 1.1696f, 0.5578f,    1.88f,  1.6189f, 0.4770f }, /*  1b */
		{8.0f,   1.0000f, 0.6514f, 3.02f,   1.3830f, 0.5747f}, /*  2  */
		{12.5f,  0.8618f, 0.7222f, 4.72f,   1.1919f, 0.6527f}, /*  3  */
		{18.5f,  0.7562f, 0.7825f, 6.99f,   1.0458f, 0.7223f}, /*  4  */
		{27.0f,  0.6667f, 0.8126f, 10.21f,  0.9220f, 0.7582f}, /*  5  */
		{38.3f,  0.5505f, 0.8434f, 14.48f,  0.8205f, 0.7957f}, /*  6  */
		{54.3f,  0.4858f, 0.8693f, 20.53f,  0.7305f, 0.8279f}, /*  7  */
		{77.0f,  0.4443f, 0.8910f, 29.11f,  0.6502f, 0.8553f}, /*  8  */
		{109.0f, 0.4187f, 0.9092f, 41.20f,  0.5950f, 0.8757f}, /*  9  */
		{146.0f, 0.3798f, 0.9222f, 55.19f,  0.5545f, 0.8903f}, /* 10  */
		{187.0f, 0.3497f, 0.9319f, 70.69f,  0.5333f, 0.8997f}, /* 11  */
		{239.0f, 0.3223f, 0.9403f, 90.34f,  0.5189f, 0.9073f}, /* 12  */
		{305.0f, 0.2828f, 0.9477f, 115.29f, 0.5181f, 0.9122f}, /* 13  */
		{390.0f, 0.2737f, 0.9544f, 147.42f, 0.5176f, 0.9171f}, /* 14  */
		{498.0f, 0.2523f, 0.9602f, 188.24f, 0.5172f, 0.9217f}, /* 15  */
		{635.0f, 0.2327f, 0.9653f, 240.03f, 0.5119f, 0.9267f}, /* 16  */
};

/**
 *
 */
const CompartmentConstants_t ZHL_16A_DATA[ZHL_16A_COMPARTMENTS] = {
		/*N2-1/2t, N2 A   , N2 B   , He-1/2t ,  He A   , He B       Cpt No.*/
		{4.0f,   1.2599f, 0.5240f, 1.51f,   1.6189f, 0.4245f}, /*  1  */
		// {    5.0f, 1.1696f, 0.5578f,    1.88f,  1.6189f, 0.4770f }, /*  1b */
		{8.0f,   1.0000f, 0.6514f, 3.02f,   1.3830f, 0.5747f}, /*  2  */
		{12.5f,  0.8618f, 0.7222f, 4.72f,   1.1919f, 0.6527f}, /*  3  */
		{18.5f,  0.7562f, 0.7825f, 6.99f,   1.0458f, 0.7223f}, /*  4  */
		{27.0f,  0.6667f, 0.8126f, 10.21f,  0.9220f, 0.7582f}, /*  5  */
		{38.3f,  0.5600f, 0.8434f, 14.48f,  0.8205f, 0.7957f}, /*  6  */
		{54.3f,  0.4947f, 0.8693f, 20.53f,  0.7305f, 0.8279f}, /*  7  */
		{77.0f,  0.4500f, 0.8910f, 29.11f,  0.6502f, 0.8553f}, /*  8  */
		{109.0f, 0.4187f, 0.9092f, 41.20f,  0.5950f, 0.8757f}, /*  9  */
		{146.0f, 0.3798f, 0.9222f, 55.19f,  0.5545f, 0.8903f}, /* 10  */
		{187.0f, 0.3497f, 0.9319f, 70.69f,  0.5333f, 0.8997f}, /* 11  */
		{239.0f, 0.3223f, 0.9403f, 90.34f,  0.5189f, 0.9073f}, /* 12  */
		{305.0f, 0.2850f, 0.9477f, 115.29f, 0.5181f, 0.9122f}, /* 13  */
		{390.0f, 0.2737f, 0.9544f, 147.42f, 0.5176f, 0.9171f}, /* 14  */
		{498.0f, 0.2523f, 0.9602f, 188.24f, 0.5172f, 0.9217f}, /* 15  */
		{635.0f, 0.2327f, 0.9653f, 240.03f, 0.5119f, 0.9267f}, /* 16  */
};
};
};

#endif //LIBBUHLMAN_BUHLMANNCONSTATNTS_HPP
