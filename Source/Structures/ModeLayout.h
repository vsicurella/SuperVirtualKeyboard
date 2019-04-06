/*
  ==============================================================================

    ModeLayout.h
    Created: 31 Mar 2019 2:16:09pm
    Author:  Vincenzo

  ==============================================================================
*/

#pragma once

#include <vector>
#include <string>

//#include "../Structures/Scale.h"

/*
	A class for representing the layout of a piano
	based on a given mode. The mode can be applied to different scales.
*/

struct ModeLayout
{
	//Scale* scale;

	int scaleSize;
	int modeSize;

	std::string strSteps;
	std::string mos;

	std::vector<int> order;
	std::vector<int> steps;
	std::vector<float> modeDegrees;

	/*
		Simply parses a string reprsenting step sizes and returns a vector
	*/
	static std::vector<int> parse_steps(std::string stepsIn)
	{
		std::vector<int> stepsOut;

		char c;
		int step;
		int digits;
		int i = 0;

		while(i < stepsIn.length())
		{
			digits = 0;
			c = stepsIn[i];

			while ((c != ',' && c!= ' ') && (i + digits) < stepsIn.length())
			{
				digits++;
				c = stepsIn[i + digits];
			}
			
			if (digits > 0)
			{	
				std::string::size_type sz;
				step = std::stoi(stepsIn.substr(i, i + digits), &sz);
				stepsOut.push_back(step);
				i += digits + 1;
			}
			else
				i++;
		}

		stepsOut.shrink_to_fit();
		return stepsOut;
	}

	/*
	Takes in step vector like {2, 2 , 1 , 2 , 2 , 2 , 1}
	Returns a vector of key orders {0, 1, 0, 1, 0, 0, 1,...}
	*/
	static std::vector<int> steps_to_order(std::vector<int> stepsIn)
	{
		std::vector<int> orderOut;

		for (int i = 0; i < stepsIn.size(); i++)
		{
			for (int j = 0; j < stepsIn.at(i); j++)
				orderOut.push_back(j);
		}

		orderOut.shrink_to_fit();
		return orderOut;
	}

	/*
	Takes in step vector as a string
	Returns an order vector
	*/
	static std::vector<int> steps_to_order(std::string strStepsIn)
	{
		return steps_to_order(parse_steps(strStepsIn));
	}

	/*
	Takes in array of a scale layout of note orders (1:1) {0, 1, 0, 1 , 0, 0, 1,...}
	and returns scale step size layout "2, 2, 1, 2,..."
	*/
	static std::vector<int> order_to_steps(std::vector<int> layoutIn)
	{
		std::vector<int> stepsOut;

		int i = 0;
		int j = 0;
		int step = 0;

		while (i < layoutIn.size())
		{
			if (layoutIn.at(i) == 0)
			{
				j = i + 1;
				step = 1;

				while (layoutIn.at(j) != 0 && j < layoutIn.size())
				{
					j++;
					step++;
				}
			}
			i = j;
			stepsOut.push_back(step);
		}
		stepsOut.shrink_to_fit();
		return stepsOut;
	}

	/*
	Takes in step vector like {2, 2 , 1 , 2 , 2 , 2 , 1}
	Returns a vector of mode degrees {0, 0.5, 1, 1.5, 2, 3, 3.5,...}
	*/
	static std::vector<float> steps_to_degrees(std::vector<int> stepsIn)
	{
		std::vector<float> degreesOut;
		float deg = -1;

		for (int i = 0; i < stepsIn.size(); i++)
		{
			degreesOut.push_back(++deg);

			if (stepsIn.at(i) > 1)
			{
				for (int j = 1; j < stepsIn.at(i); j++)
				{
					degreesOut.push_back(deg + (float)j / stepsIn.at(i));
				}
			}
		}

		degreesOut.shrink_to_fit();
		return degreesOut;
	}


	int get_num_notes()
	{
		return scaleSize;
	}

	int get_num_modal_notes()
	{
		return floor(128.0f / scaleSize * modeSize);
	}

	int get_mode_size()
	{
		return steps.size();
	}

	std::vector<int> get_steps()
	{
		return steps;
	}

	/*
		Get a vector representing a full MIDI layout of the steps of the mode
	*/

	std::vector<int> get_steps(int modeStartNote)
	{
		std::vector<int> stepsOut;

		int size = ceil(128.0 / scaleSize * modeSize);

		stepsOut.reserve(size);

		int startStep = 60 % scaleSize;

		// i don't think this will work if note 0 lands between steps
		for (int i = 0; i < 128; i++)
		{
			stepsOut.at(i) = steps.at((i + startStep) % modeSize);
		}

		return stepsOut;
	}

	std::vector<int> get_order()
	{
		return order;
	}

	std::vector<int> get_order(int modeStartNote)
	{
		std::vector<int> orderOut;
		orderOut.reserve(128);

		int offset = modeStartNote % scaleSize;

		for (int i = 0; i < 128; i++)
		{
			orderOut.at(i) = order.at((i + offset) % scaleSize);
		}

		return orderOut;
	}

	int get_highest_order()
	{
		int o = 0;

		for (int i = 0; i < scaleSize; i++)
		{
			if (order[i] > o)
				o = order[i];
		}

		return o;
	}

	std::string toString()
	{
		std::string out;

		for (int i = 0; i < steps.size(); i++)
		{
			out += steps[i];

			if (i < steps.size() - 1)
				out += ' ';
		}

		return out;
	}

	static std::string steps_to_string(std::vector<int> stepsIn)
	{
		std::string out;

		for (int i = 0; i < stepsIn.size(); i++)
		{
			out += stepsIn[i];

			if (i < stepsIn.size() - 1)
				out += ' ';
		}

		return out;
	}

	ModeLayout()
	{
		scaleSize = 1;
		strSteps = "1";
		steps = parse_steps(strSteps);
		order = steps_to_order(steps);
		modeDegrees = steps_to_degrees(steps);

		modeSize = steps.size();
	}

	ModeLayout(std::string stepsIn)
	{
		strSteps = stepsIn;
		steps = parse_steps(stepsIn);
		order = steps_to_order(steps);
		modeDegrees = steps_to_degrees(steps);

		scaleSize = order.size();
		modeSize = steps.size();
	}

	ModeLayout(std::vector<int> stepsIn)
	{
		steps = stepsIn;
		order = steps_to_order(steps);
		modeDegrees = steps_to_degrees(steps);

		strSteps = steps_to_string(stepsIn);
		scaleSize = order.size();
		modeSize = steps.size();
	}

	~ModeLayout() {}
};