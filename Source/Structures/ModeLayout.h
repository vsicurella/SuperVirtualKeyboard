		/*
  ==============================================================================

    ModeLayout.h
    Created: 31 Mar 2019 2:16:09pm
    Author:  Vincenzo

  ==============================================================================
*/

#pragma once

#include "../PluginIDs.h"

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

	int offset = 0;

	String strSteps;
	String family;
	String mos;

	Array<int> order;
	Array<int> steps;
	Array<float> degrees;

	Array<int> orderOffset;
	Array<int> stepsOffset;
	Array<float> degreesOffset;

	ModeLayout();

	ModeLayout(String stepsIn);

	ModeLayout(Array<int> stepsIn);

	ModeLayout(String stepsIn, String familyIn);

	ModeLayout(Array<int> stepsIn, String familyIn);

	~ModeLayout() {}

	void init_node();

	void restore_from_node(ValueTree nodeIn);

private:

	/*
		Sets user name of the mode
	*/
	String set_user_name(String nameIn);

	/*
		Simply parses a string reprsenting step sizes and returns a vector
	*/
	static Array<int> parse_steps(String stepsIn);

	/*
	Takes in step vector like {2, 2 , 1 , 2 , 2 , 2 , 1}
	Returns a vector of key orders {0, 1, 0, 1, 0, 0, 1,...}
	*/
	static Array<int> steps_to_order(Array<int> stepsIn);

	/*
	Takes in step vector as a string
	Returns an order vector
	*/
	static Array<int> steps_to_order(String strStepsIn);

	/*
	Takes in array of a scale layout of note orders (1:1) {0, 1, 0, 1 , 0, 0, 1,...}
	and returns scale step size layout "2, 2, 1, 2,..."
	*/
	static Array<int> order_to_steps(Array<int> layoutIn);

	/*
	Takes in step vector like {2, 2 , 1 , 2 , 2 , 2 , 1}
	Returns a vector of mode degrees {0, 0.5, 1, 1.5, 2, 3, 3.5,...}
	*/
	static Array<float> steps_to_degrees(Array<int> stepsIn);

	int get_num_notes();

	int get_num_modal_notes();

	int get_mode_size();

	Array<int> get_steps();

	int get_step(int indexIn);

	Array<int> get_order();

	Array<int> get_order_array_offset();

	Array<int> get_order(int modeStartNote);

	int get_highest_order();

	void update_mode_offset(int offsetIn);

	std::string toString();

	static String steps_to_string(Array<int> stepsIn);

	String get_full_name();

	String get_name_scale_size();

	String get_name_mode_size();

	template <class T>
	void add_array_to_node(const Array<T>& arrayIn, Identifier arrayID, Identifier itemId);

	template <class T>
	void get_array_from_node(Array<T>& arrayIn, const ValueTree nodeIn, Identifier arrayID);

};


