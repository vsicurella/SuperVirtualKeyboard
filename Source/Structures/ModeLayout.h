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

#include "../PluginState.h"

//#include "../Structures/Scale.h"

/*
	A class for representing the layout of a piano
	based on a given mode. The mode can be applied to different scales.
*/

struct ModeLayout
{
	//Scale* scale;
	ValueTree modeLayoutNode;

	String modeName;
	int scaleSize;
	int modeSize;

	String strSteps;
	String family;
	String mos;

	Array<int> order;
	Array<int> steps;
	Array<float> modeDegrees;

	enum Properties
	{
		fullModeName
	};

	/*
		Sets the name of the mode
	*/
	String set_name(String nameIn)
	{
		modeName = nameIn;
		return modeName;
	}

	/*
		Simply parses a string reprsenting step sizes and returns a vector
	*/
	static Array<int> parse_steps(String stepsIn)
	{
		Array<int> stepsOut;

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
				step = stepsIn.substring(i, i + digits).getIntValue();
				stepsOut.add(step);
				i += digits + 1;
			}
			else
				i++;
		}

		stepsOut.minimiseStorageOverheads();
		return stepsOut;
	}

	/*
	Takes in step vector like {2, 2 , 1 , 2 , 2 , 2 , 1}
	Returns a vector of key orders {0, 1, 0, 1, 0, 0, 1,...}
	*/
	static Array<int> steps_to_order(Array<int> stepsIn)
	{
		Array<int> orderOut;

		for (int i = 0; i < stepsIn.size(); i++)
		{
			for (int j = 0; j < stepsIn[i]; j++)
				orderOut.add(j);
		}

		orderOut.minimiseStorageOverheads();
		return orderOut;
	}

	/*
	Takes in step vector as a string
	Returns an order vector
	*/
	static Array<int> steps_to_order(String strStepsIn)
	{
		return steps_to_order(parse_steps(strStepsIn));
	}

	/*
	Takes in array of a scale layout of note orders (1:1) {0, 1, 0, 1 , 0, 0, 1,...}
	and returns scale step size layout "2, 2, 1, 2,..."
	*/
	static Array<int> order_to_steps(Array<int> layoutIn)
	{
		Array<int> stepsOut;

		int i = 0;
		int j = 0;
		int step = 0;

		while (i < layoutIn.size())
		{
			if (layoutIn[i] == 0)
			{
				j = i + 1;
				step = 1;

				while (layoutIn[j] != 0 && j < layoutIn.size())
				{
					j++;
					step++;
				}
			}
			i = j;
			stepsOut.add(step);
		}
		stepsOut.minimiseStorageOverheads();
		return stepsOut;
	}

	/*
	Takes in step vector like {2, 2 , 1 , 2 , 2 , 2 , 1}
	Returns a vector of mode degrees {0, 0.5, 1, 1.5, 2, 3, 3.5,...}
	*/
	static Array<float> steps_to_degrees(Array<int> stepsIn)
	{
		Array<float> degreesOut;
		float deg = -1;

		for (int i = 0; i < stepsIn.size(); i++)
		{
			degreesOut.add(++deg);

			if (stepsIn[i] > 1)
			{
				for (int j = 1; j < stepsIn[i]; j++)
				{
					degreesOut.add(deg + (float)j / stepsIn[i]);
				}
			}
		}

		degreesOut.minimiseStorageOverheads();
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

	Array<int> get_steps()
	{
		return steps;
	}

	Array<int> get_order()
	{
		return order;
	}

	Array<int> get_order(int modeStartNote)
	{
		Array<int> orderOut;

		int offset = modeStartNote % scaleSize;

		for (int i = 0; i < 128; i++)
		{
			orderOut.add(order[(i + offset) % scaleSize]);
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

	static String steps_to_string(Array<int> stepsIn)
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

	String get_full_name()
	{
		return family + "[ " + String(modeSize) + "] " + String(scaleSize);
	}

	String get_name_scale_size()
	{
		return String(scaleSize) + " " + get_full_name().dropLastCharacters(2);
	}

	String get_name_mode_size()
	{
		return String(modeSize) + " " + family + " " + String(scaleSize);
	}

	void update_node()
	{
		modeLayoutNode.setProperty(IDs::scaleSize, var(scaleSize), nullptr);
		modeLayoutNode.setProperty(IDs::modeSize, var(modeSize), nullptr);
		modeLayoutNode.setProperty(IDs::stepString, var(strSteps), nullptr);
		modeLayoutNode.setProperty(IDs::modeFullName, var(get_full_name()), nullptr);
		modeLayoutNode.setProperty(IDs::modeScaleName, var(get_name_scale_size()), nullptr);
		modeLayoutNode.setProperty(IDs::modeModeName, var(get_name_mode_size()), nullptr);
		modeLayoutNode.setProperty(IDs::temperamentFamily, var(family), nullptr);
		//modeLayoutNode.setProperty(IDs::stepArray, var(steps), nullptr);
		//modeLayoutNode.setProperty(IDs::keyboardOrderArray, var(order), nullptr);
		//modeLayoutNode.setProperty(IDs::keyboardModeDegrees, var(modeDegrees), nullptr);
	}

	void set_valuetree_node(ValueTree nodeIn, bool update=false)
	{
		modeLayoutNode = nodeIn;
		if (update)
			update_node();
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

	ModeLayout(String stepsIn)
	{
		strSteps = stepsIn;
		steps = parse_steps(stepsIn);
		order = steps_to_order(steps);
		modeDegrees = steps_to_degrees(steps);

		scaleSize = order.size();
		modeSize = steps.size();

		if (modeLayoutNode.isValid())
			update_node();
	}

	ModeLayout(Array<int> stepsIn)
	{
		steps = stepsIn;
		order = steps_to_order(steps);
		modeDegrees = steps_to_degrees(steps);

		strSteps = steps_to_string(stepsIn);
		scaleSize = order.size();
		modeSize = steps.size();

		if (modeLayoutNode.isValid())
			update_node();
	}

	ModeLayout(String stepsIn, String familyIn)
	{
		strSteps = stepsIn;
		steps = parse_steps(stepsIn);
		order = steps_to_order(steps);
		modeDegrees = steps_to_degrees(steps);
		family = familyIn;

		scaleSize = order.size();
		modeSize = steps.size();

		if (modeLayoutNode.isValid())
			update_node();
	}

	ModeLayout(Array<int> stepsIn, String familyIn)
	{
		steps = stepsIn;
		order = steps_to_order(steps);
		modeDegrees = steps_to_degrees(steps);
		family = familyIn;

		strSteps = steps_to_string(stepsIn);
		scaleSize = order.size();
		modeSize = steps.size();

		if (modeLayoutNode.isValid())
			update_node();
	}

	~ModeLayout() {}
};