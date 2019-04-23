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

struct Mode
{
	ValueTree modeNode;

	Mode();

	Mode(String stepsIn, String familyIn="undefined", int offsetIn=0);

	Mode(Array<int> stepsIn, String familyIn="undefined", int offsetIn=0);

	~Mode();

	void init_node();

	void restore_from_node(ValueTree nodeIn);

	/*
		Sets custom name of the mode. 
	*/
	String setName(String nameIn);

	/*
		Sets the offset of the mode and updates parameters so that the 
		offset replaces the current visualization of the mode
	*/
	void setOffset(int offsetIn);

	int getScaleSize() const;

	int getModeSize() const;

	String getFamily() const;

	Array<int>* getKeyboardOrdersSizes();

	int getKeyboardOrdersSize(int ordersIn);
    
    Array<int> getStepsOfOrders();

	Array<int> getSteps();

	String getStepsString();

	Array<int> getOrdersDefault();

	Array<int> getOrders();

	Array<float> getDegrees();

	int getMaxStep();

	Array<int> getMOSClass();

	String getDescription();

	String getScaleDescription();

	String getModeDescription();

	/*
	Simply parses a string reprsenting step sizes and returns a vector
	*/
	static Array<int> parse_steps(String stepsIn);
    
    /*
     Takes in step vector like {2, 2, 1, 2, 2, 2, 1}
     Returns a vector of each index repeated by it's own magnitude {2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1}
     */
    static Array<int> expand_steps(Array<int> ordersIn);
    
    /*
     Takes in step vector like {2, 2 , 1 , 2 , 2 , 2 , 1}
     Returns a string like "2 2 1 2 2 2 1"
     */
    static String steps_to_string(Array<int> stepsIn);

	/*
	Takes in step vector like {2, 2, 1, 2, 2, 2, 1}
	Returns a vector of key orders {0, 1, 0, 1, 0, 0, 1,...}
	*/
	static Array<int> steps_to_orders(Array<int> stepsIn);

	/*
	Takes in a period-size vector of key orders and offset
	Returns a keyboard-size vector of offset key orders
	*/
	static Array<int> expand_orders(Array<int> ordersIn, int offsetIn);
    
	/*
	Takes in array of a scale layout of note orders (1:1) {0, 1, 0, 1 , 0, 0, 1,...}
	and returns scale step size layout "2, 2, 1, 2,..."
	*/
	static Array<int> orders_to_steps(Array<int> layoutIn);

	/*
	Takes in array of a scale layout of note orders (1:1) {0, 1, 0, 1 , 0, 0, 1,...}
	and returns scale step size layout "0, 0.5, 1, 1.5,..."
	*/
	static Array<float> orders_to_degrees(Array<int> stepsIn);

	/*
	Takes in a vector like {2, 2, 1, 2, 2, 2, 1}
	Returns a vector of sizes large->small like {5, 2} 
	*/
	static Array<int> interval_sizes(Array<int> stepsIn);


private:
    
    // Hopefully I come up with a better way to either resize keyboard keys, give keys a step value, or
    // place keys in a grid such that I won't need this
    void updateStepsOfOrders();

	template <class T>
	void add_array_to_node(const Array<T>& arrayIn, Identifier arrayID, Identifier itemId);

	template <class T>
	void get_array_from_node(Array<T>& arrayIn, const ValueTree nodeIn, Identifier arrayID);

	String name;
	int scaleSize;
	int modeSize;
	String family;
	int offset;

	String stepsString;
	Array<int> steps;
	Array<int> ordersDefault;
	Array<int> mosClass;

	Array<int> orders;
	Array<float> degrees;

    // Keyboard Convenience
    Array<int> stepsOfOrders; // each index is the step which the note is associated with
	Array<int> keyboardOrdersSizes; // amount of keys in each key order groupings
};


