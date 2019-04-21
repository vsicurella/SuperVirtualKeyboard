/*
==============================================================================

ModeLayout.cpp
Created: 19 Apr 2019 11:15:14pm
Author:  Vincenzo

==============================================================================
*/

#pragma once

#include "ModeLayout.h"


ModeLayout::ModeLayout()
{
	scaleSize = 1;
	strSteps = "1";
	steps = parse_steps(strSteps);
	order = steps_to_order(steps);
	degrees = steps_to_degrees(steps);

	family = "undefined";

	modeSize = steps.size();
	init_node();
}

ModeLayout::ModeLayout(String stepsIn)
{
	strSteps = stepsIn;
	steps = parse_steps(stepsIn);
	order = steps_to_order(steps);
	degrees = steps_to_degrees(steps);

	family = "undefined";

	scaleSize = order.size();
	modeSize = steps.size();

	init_node();
}

ModeLayout::ModeLayout(Array<int> stepsIn)
{
	steps = stepsIn;
	order = steps_to_order(steps);
	degrees = steps_to_degrees(steps);

	family = "undefined";

	strSteps = steps_to_string(stepsIn);
	scaleSize = order.size();
	modeSize = steps.size();

	init_node();
}

ModeLayout::ModeLayout(String stepsIn, String familyIn)
{
	strSteps = stepsIn;
	steps = parse_steps(stepsIn);
	order = steps_to_order(steps);
	degrees = steps_to_degrees(steps);
	family = familyIn;

	scaleSize = order.size();
	modeSize = steps.size();

	init_node();
}

ModeLayout::ModeLayout(Array<int> stepsIn, String familyIn)
{
	steps = stepsIn;
	order = steps_to_order(steps);
	degrees = steps_to_degrees(steps);
	family = familyIn;

	strSteps = steps_to_string(stepsIn);
	scaleSize = order.size();
	modeSize = steps.size();

	init_node();
}

ModeLayout::~ModeLayout() {}


void ModeLayout::init_node()
{
	modeLayoutNode = ValueTree(IDs::modePresetNode);

	modeLayoutNode.setProperty(IDs::scaleSize, scaleSize, nullptr);
	modeLayoutNode.setProperty(IDs::modeSize, modeSize, nullptr);
	modeLayoutNode.setProperty(IDs::stepString, strSteps, nullptr);
	modeLayoutNode.setProperty(IDs::modeOffset, offset, nullptr);
	modeLayoutNode.setProperty(IDs::modeFullName, get_full_name(), nullptr);
	modeLayoutNode.setProperty(IDs::modeScaleName, get_name_scale_size(), nullptr);
	modeLayoutNode.setProperty(IDs::modeModeName, get_name_mode_size(), nullptr);
	modeLayoutNode.setProperty(IDs::family, family, nullptr);

	add_array_to_node(steps, IDs::stepArray, IDs::stepValue);
	add_array_to_node(order, IDs::keyboardOrderArray, IDs::orderValue);
	add_array_to_node(degrees, IDs::keyboardModeDegrees, IDs::degreeValue);
}

void ModeLayout::restore_from_node(ValueTree nodeIn)
{
	if (nodeIn.hasType(IDs::modePresetNode))
	{
		modeLayoutNode = nodeIn;

		modeName = modeLayoutNode[IDs::modeUserName];
		scaleSize = modeLayoutNode[IDs::scaleSize];
		modeSize = modeLayoutNode[IDs::modeSize];
		strSteps = modeLayoutNode[IDs::stepString];
		family = modeLayoutNode[IDs::family];
		offset = modeLayoutNode[IDs::modeOffset];

		steps.clear();
		get_array_from_node(steps, modeLayoutNode, IDs::stepArray);

		order.clear();
		get_array_from_node(order, modeLayoutNode, IDs::keyboardOrderArray);

		degrees.clear();
		get_array_from_node(degrees, modeLayoutNode, IDs::keyboardModeDegrees);
	}
}

/*
Sets user name of the mode
*/
String ModeLayout::set_user_name(String nameIn)
{
	modeName = nameIn;
	modeLayoutNode.setProperty(IDs::modeUserName, modeName, nullptr);
	return modeName;
}

/*
Simply parses a string reprsenting step sizes and returns a vector
*/
Array<int> ModeLayout::parse_steps(String stepsIn)
{
	Array<int> stepsOut;

	char c;
	int step;
	int digits;
	int i = 0;

	while (i < stepsIn.length())
	{
		digits = 0;
		c = stepsIn[i];

		while ((c != ',' && c != ' ') && (i + digits) < stepsIn.length())
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
Array<int> ModeLayout::steps_to_order(Array<int> stepsIn)
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
Array<int> ModeLayout::steps_to_order(String strStepsIn)
{
	return steps_to_order(parse_steps(strStepsIn));
}

/*
Takes in array of a scale layout of note orders (1:1) {0, 1, 0, 1 , 0, 0, 1,...}
and returns scale step size layout "2, 2, 1, 2,..."
*/
Array<int> ModeLayout::order_to_steps(Array<int> layoutIn)
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
Array<float> ModeLayout::steps_to_degrees(Array<int> stepsIn)
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


int ModeLayout::get_num_notes()
{
	return scaleSize;
}

int ModeLayout::get_num_modal_notes()
{
	return floor(128.0f / scaleSize * modeSize);
}

int ModeLayout::get_mode_size()
{
	return steps.size();
}

Array<int> ModeLayout::get_steps()
{
	return steps;
}

int ModeLayout::get_step(int indexIn)
{
	int index = (((indexIn + offset) % modeSize + modeSize) % modeSize);
	jassert(index >= 0 && index < modeSize);
	return steps[index];
}

Array<int> ModeLayout::get_order()
{
	return order;
}

Array<int> ModeLayout::get_order_array_offset()
{
	Array<int> orderOut;

	for (int i = 0; i < modeSize; i++)
	{
		for (int j = 0; j < get_step(i + offset); j++)
			orderOut.add(j);
	}

	orderOut.set(0, 0);

	orderOut.minimiseStorageOverheads();
	return orderOut;
}

Array<int> ModeLayout::get_order(int modeStartNote)
{
	Array<int> orderOut;

	int offset = modeStartNote % scaleSize;

	for (int i = 0; i < 128; i++)
	{
		orderOut.add(order[(i + offset) % scaleSize]);
	}

	return orderOut;
}

int ModeLayout::get_highest_order()
{
	int o = 0;

	for (int i = 0; i < scaleSize; i++)
	{
		if (order[i] > o)
			o = order[i];
	}

	return o;
}

void ModeLayout::update_mode_offset(int offsetIn)
{
	offset = offsetIn;
	orderOffset = get_order_array_offset();

	stepsOffset.clear();

	for (int i = 0; i < modeSize; i++)
		stepsOffset.add(get_step(i));

	degreesOffset = steps_to_degrees(stepsOffset);

	steps = stepsOffset;
	order = orderOffset;
	degrees = degreesOffset;

	init_node();
}

std::string ModeLayout::toString()
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

String ModeLayout::steps_to_string(Array<int> stepsIn)
{
	String out;

	for (int i = 0; i < stepsIn.size(); i++)
	{
		out += String(stepsIn[i]);

		if (i < stepsIn.size() - 1)
			out += ' ';
	}

	return out;
}

String ModeLayout::get_full_name()
{
	return family + "[" + String(modeSize) + "] " + String(scaleSize);
}

String ModeLayout::get_name_scale_size()
{
	return String(scaleSize) + " " + get_full_name().dropLastCharacters(2);
}

String ModeLayout::get_name_mode_size()
{
	return String(modeSize) + " " + family + " " + String(scaleSize);
}

template <class T>
void ModeLayout::add_array_to_node(const Array<T>& arrayIn, Identifier arrayID, Identifier itemId)
{
	ValueTree arrayTree = ValueTree(arrayID);
	ValueTree item;

	for (int i = 0; i < arrayIn.size(); i++)
	{
		item = ValueTree(itemId);
		item.setProperty(IDs::modeArrayValue, arrayIn[i], nullptr);
		arrayTree.addChild(item, i, nullptr);
	}

	modeLayoutNode.addChild(arrayTree, -1, nullptr);
}

template <class T>
void ModeLayout::get_array_from_node(Array<T>& arrayIn, const ValueTree nodeIn, Identifier arrayID)
{
	ValueTree childArray = nodeIn.getChildWithName(arrayID);

	if (childArray.isValid())
	{
		for (int i = 0; i < childArray.getNumChildren(); i++)
		{
			arrayIn.add(childArray.getChild(i).getProperty(IDs::modeArrayValue));
			DBG(arrayIn[i]);
		}
	}
}
