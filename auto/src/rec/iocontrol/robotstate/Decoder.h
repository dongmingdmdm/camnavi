//  Copyright (C) 2004-2009, Robotics Equipment Corporation GmbH

#ifndef _REC_ROBOTSTATE_DECODER_H_
#define _REC_ROBOTSTATE_DECODER_H_

#include <string>
#include <map>
#include <boost/cstdint.hpp>

#include "rec/iocontrol/robotstate/State.h"

class QDSA_Encoder;

namespace rec
{
	namespace iocontrol
	{
		namespace robotstate
		{
			class Decoder
			{
			public:
				/*
				Devide the raw value optional returned by batteryVoltage by this constant to get the battery voltage in Volt.
				@see batteryVoltage
				*/
				static const float batteryVoltageScaleConstant;

				Decoder( State* state );

				~Decoder();

				/** 
				* Retrieve the actual velocity of the robot.
				*
				* @param vx velocity in x direction in mm/s
				* @param vy velocity in y direction in mm/s
				* @param omega angular velocity in rad/s
				*/
				void actualVelocity( float* vx, float* vy, float* omega ) const;

				float actualVelocity( unsigned int motor ) const;

				float motorCurrent( unsigned int motor ) const;
				
				float powerOutputCurrent() const;

				/**
				* Retrieve the current set point speed for motor in rpm.
				* @param motor the queried motor
				*/
				float setPointSpeed( unsigned int motor ) const;

				/**
				* Retrieve the current set point of the power output
				*/
				int powerOutputSetPoint() const;

				boost::int32_t actualPosition( unsigned int motor ) const;

				/*
				* @param i Range from 0 to State::numAnalogInputs-1
				* @see State::numAnalogInputs
				*/
				float analogInput( unsigned int i ) const;

				/*
				* @param i Range from 0 to State::numDigitalInputs-1
				* @see State::numDigitalInputs
				*/
				bool digitalInput( unsigned int i ) const;

				/*
				* @param i Range from 0 to State::numDistanceSensors-1
				* @return Returns the voltage generated by the infrared distance sensor
				* 0V no object
				* 2.55V object near
				* @see State::numDigitalInputs
				*/
				float distance( unsigned int i ) const;

				/*
				@param Stores the raw voltage reading as coming from the 10bit ADC
				@return Returns the battery voltage in V
				@see batteryVoltageScaleConstant
				*/
				float batteryVoltage( unsigned int* rawValue = NULL ) const;

				float batteryCurrent() const;

				bool bumper() const;

				std::string firmwareVersion() const;

				void getPID( unsigned int motor, float* kp, float* ki, float* kd ) const;

				static void unprojectVelocity( float* vx, float* vy, float* omega, float m1, float m2, float m3, const DriveLayout& layout );

			private:
				State* _state;
				QDSA_Encoder* _dec;
			};
		}
	}
}

#endif //_REC_ROBOTSTATE_ENCODER_H_