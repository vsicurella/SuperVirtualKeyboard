/*
==============================================================================

ModeLayout.cpp
Created: 19 Apr 2019 11:15:14pm
Author:  Vincenzo

==============================================================================
*/

#include "Mode.h"


Mode::Mode()
{
	stepsString = "1";
	family = "undefined";
	rootNote = 60;

	steps = parse_steps(stepsString);
	ordersDefault = steps_to_orders(steps);
	mosClass = interval_sizes(steps);
	scaleSize = ordersDefault.size();
	modeSize = steps.size();
	name = getDescription();
	offset = getOffset() * -1;

	orders = expand_orders(ordersDefault, offset);
	modeDegrees = orders_to_modeDegrees(ordersDefault);
	scaleDegrees = scale_degrees(scaleSize, offset);
	keyboardOrdersSizes = interval_sizes(orders);
	updateStepsOfOrders();

	updateNode(true);
}

Mode::Mode(String stepsIn, String familyIn, int rootNoteIn, String nameIn, String infoIn)
{
	stepsString = stepsIn;
	family = familyIn;
	rootNote = rootNoteIn;
    info = infoIn;

	steps = parse_steps(stepsIn);
	ordersDefault = steps_to_orders(steps);
	mosClass = interval_sizes(steps);
	scaleSize = ordersDefault.size();
	modeSize = steps.size();
	offset = getOffset() * -1;
    
    if (nameIn == "")
        name = getDescription();
    else
        name = nameIn;

	orders = expand_orders(ordersDefault, offset);
	modeDegrees = orders_to_modeDegrees(orders);
	scaleDegrees = scale_degrees(scaleSize, offset);
	keyboardOrdersSizes = interval_sizes(orders);
	updateStepsOfOrders();

	updateNode(true);
}

Mode::Mode(Array<int> stepsIn, String familyIn, int rootNoteIn, String nameIn, String infoIn)
{
	stepsString = steps_to_string(stepsIn);
	family = familyIn;
	rootNote = rootNoteIn;
    info = infoIn;

	steps = stepsIn;
	ordersDefault = steps_to_orders(steps);
	mosClass = interval_sizes(steps);
	scaleSize = ordersDefault.size();
	modeSize = steps.size();
	offset = getOffset() * -1;
    
    if (nameIn == "")
        name = getDescription();
    else
        name = nameIn;

	orders = expand_orders(ordersDefault, offset);
	modeDegrees = orders_to_modeDegrees(orders);
	scaleDegrees = scale_degrees(scaleSize, offset);
	keyboardOrdersSizes = interval_sizes(orders);
    updateStepsOfOrders();
    
	updateNode(true);
}

Mode::Mode(ValueTree modeNodeIn, int rootNoteIn)
: Mode()
{
	bool hasModeChild;

	if (isValidMode(modeNodeIn, hasModeChild))
	{
		if (hasModeChild)
			modeNodeIn = modeNodeIn.getChildWithName(IDs::modePresetNode);

        modeNode = ValueTree(IDs::modePresetNode);
		modeNode.copyPropertiesAndChildrenFrom(modeNodeIn, nullptr);

        stepsString = modeNode[IDs::stepString];
        steps = parse_steps(stepsString);
        mosClass = interval_sizes(steps);
        ordersDefault = steps_to_orders(steps);
        
        scaleSize = modeNode[IDs::scaleSize];
        if (scaleSize == 0)
            scaleSize = ordersDefault.size();
        
        modeSize = modeNode[IDs::modeSize];
        if (modeSize == 0)
            modeSize = steps.size();
    
		family = modeNode[IDs::family];
        if (family == "")
            family = "undefined";
        
        info = modeNode[IDs::modeInfo];
        
        name = modeNode[IDs::modeName];
        if (name == "")
            name = getDescription();
        
		rootNote = rootNoteIn;
		offset = getOffset() * -1;

		orders = expand_orders(ordersDefault, offset);
		modeDegrees = orders_to_modeDegrees(orders);
		scaleDegrees = scale_degrees(scaleSize, offset);
		updateStepsOfOrders();
        
        updateNode();
	}
}

Mode::~Mode() {}


void Mode::updateNode(bool initializeNode)
{
    if (initializeNode)
        modeNode = ValueTree(IDs::modePresetNode);

	modeNode.setProperty(IDs::modeName, name, nullptr);
	modeNode.setProperty(IDs::scaleSize, scaleSize, nullptr);
	modeNode.setProperty(IDs::modeSize, modeSize, nullptr);
	modeNode.setProperty(IDs::stepString, stepsString, nullptr);
	modeNode.setProperty(IDs::family, family, nullptr);
    modeNode.setProperty(IDs::modeInfo, info, nullptr);
	modeNode.setProperty(IDs::factoryPreset, false, nullptr);
    modeNode.setProperty(IDs::rootMidiNote, rootNote, nullptr);
}

void Mode::restoreNode(ValueTree nodeIn, bool useNodeRoot)
{
	bool hasModeChild;

	if (isValidMode(nodeIn, hasModeChild))
	{
		if (hasModeChild)
			nodeIn = nodeIn.getChildWithName(IDs::modePresetNode);

		modeNode = nodeIn;
        
        stepsString = modeNode[IDs::stepString];
        steps = parse_steps(stepsString);
        mosClass = interval_sizes(steps);
        ordersDefault = steps_to_orders(steps);
        
        scaleSize = modeNode[IDs::scaleSize];
        if (scaleSize == 0)
            scaleSize = ordersDefault.size();
        
        modeSize = modeNode[IDs::modeSize];
        if (modeSize == 0)
            modeSize = steps.size();
        
        family = modeNode[IDs::family];
        if (family == "")
            family = "undefined";
        
        info = modeNode[IDs::modeInfo];
        
        name = modeNode[IDs::modeName];
        if (name == "")
            name = getDescription();
        
        if (useNodeRoot)
            rootNote = totalModulus(modeNode[IDs::rootMidiNote], 128);
        
        offset = -getOffset();
        
        orders = expand_orders(ordersDefault, offset);
        modeDegrees = orders_to_modeDegrees(orders);
        scaleDegrees = scale_degrees(scaleSize, offset);
        updateStepsOfOrders();
        
        updateNode();
	}
}

bool Mode::isValidMode(ValueTree nodeIn, bool& hasModeChild)
{
    bool isValid = true;
	bool isMode = nodeIn.hasType(IDs::modePresetNode);
	hasModeChild = nodeIn.getChildWithName(IDs::modePresetNode).isValid();

	String steps;

	if (isMode)
		steps = nodeIn.getProperty(IDs::stepString).toString();
	else
		steps = nodeIn.getChildWithName(IDs::modePresetNode).getProperty(IDs::stepString).toString();

	int stepsLength = steps.length();
	bool hasSteps = stepsLength > 0;
    
    return isValid && (isMode || hasModeChild) && hasSteps;
}


ValueTree Mode::createNode(String stepsIn, String familyIn, String nameIn, String infoIn, bool factoryPreset)
{
	ValueTree modeNodeOut = ValueTree(IDs::modePresetNode);
	Array<int> steps = parse_steps(stepsIn);
	Array<int> orders = steps_to_orders(steps);
	Array<int> mosClass = interval_sizes(steps);

	modeNodeOut.setProperty(IDs::scaleSize, orders.size(), nullptr);
	modeNodeOut.setProperty(IDs::modeSize, steps.size(), nullptr);
	modeNodeOut.setProperty(IDs::stepString, stepsIn, nullptr);
	modeNodeOut.setProperty(IDs::family, familyIn, nullptr);
    modeNodeOut.setProperty(IDs::modeInfo, infoIn, nullptr);
	modeNodeOut.setProperty(IDs::factoryPreset, factoryPreset, nullptr);

    if (nameIn == "")
    {
        String modeName = familyIn + "[" + String(steps.size()) + "] " + String(orders.size());
        modeNodeOut.setProperty(IDs::modeName, modeName, nullptr);
    }
    else
        modeNodeOut.setProperty(IDs::modeName, nameIn, nullptr);
    
	return modeNodeOut;
}

ValueTree Mode::createNode(Array<int> stepsIn, String familyIn, String nameIn, String infoIn, bool factoryPreset)
{
	ValueTree modeNodeOut = ValueTree(IDs::modePresetNode);
	Array<int> orders = steps_to_orders(stepsIn);
	Array<int> mosClass = interval_sizes(stepsIn);
	String stepsStr = steps_to_string(stepsIn);

	modeNodeOut.setProperty(IDs::scaleSize, orders.size(), nullptr);
	modeNodeOut.setProperty(IDs::modeSize, stepsIn.size(), nullptr);
	modeNodeOut.setProperty(IDs::stepString, stepsStr, nullptr);
	modeNodeOut.setProperty(IDs::family, familyIn, nullptr);
    modeNodeOut.setProperty(IDs::modeInfo, infoIn, nullptr);
	modeNodeOut.setProperty(IDs::factoryPreset, factoryPreset, nullptr);

    if (nameIn == "")
    {
        String modeName = familyIn + "[" + String(stepsStr.length()) + "] " + String(orders.size());
        modeNodeOut.setProperty(IDs::modeName, modeName, nullptr);
    }
    else
        modeNodeOut.setProperty(IDs::modeName, nameIn, nullptr);

	return modeNodeOut;
}


void Mode::setFamily(String familyIn)
{
	family = familyIn;
	modeNode.setProperty(IDs::family, family, nullptr);
}

void Mode::setName(String nameIn)
{
    name = nameIn;
    modeNode.setProperty(IDs::modeName, name, nullptr);
}


void Mode::setInfo(String infoIn)
{
    info = infoIn;
    modeNode.setProperty(IDs::modeInfo, infoIn, nullptr);
}

void Mode::setRootNote(int rootNoteIn)
{
	if (rootNote != rootNoteIn)
	{
		rootNote = rootNoteIn;
		offset = getOffset() * -1;
		orders = expand_orders(ordersDefault, offset);
		modeDegrees = orders_to_modeDegrees(orders);
		scaleDegrees = scale_degrees(scaleSize, offset);
		keyboardOrdersSizes = interval_sizes(orders);
		updateStepsOfOrders();
	}
}

void Mode::addTag(String tagIn)
{
    tags.add(tagIn);
}

int Mode::removeTag(String tagIn)
{
    int index = tags.indexOf(tagIn);
    tags.remove(index);
    
    return index;
}

int Mode::getRootNote() const
{
	return rootNote;
}

int Mode::getOffset() const
{
	return totalModulus(rootNote, scaleSize);
}

int Mode::getScaleSize() const
{
	return scaleSize;
}

int Mode::getModeSize() const
{
	return modeSize;
}

Array<int> Mode::getStepsOfOrders()  const
{
	return stepsOfOrders;
}

String Mode::getFamily() const
{
	return family;
}

String Mode::getName() const
{
    return name;
}

String Mode::getInfo() const
{
    return info;
}

Array<int> Mode::getSteps()  const
{
	return steps;
}

String Mode::getStepsString()  const
{
	return stepsString;
}

Array<int>* Mode::getKeyboardOrdersSizes()
{
	return &keyboardOrdersSizes;
}

int Mode::getKeyboardOrdersSize(int ordersIn) const
{
	int o = ordersIn % getMaxStep();
	return keyboardOrdersSizes[o];
}

Array<int> Mode::getOrdersDefault() const
{
	return ordersDefault;
}

Array<int> Mode::getOrders() const
{
	return orders;
}

Array<float> Mode::getModalDegrees() const
{
	return modeDegrees;
}

Array<int> Mode::getScaleDegrees() const
{
	return scaleDegrees;
}

int Mode::getScaleDegree(int midiNoteIn) const
{
	return totalModulus(midiNoteIn - rootNote, scaleSize);
}

float Mode::getModeDegree(int midiNoteIn) const
{
	return modeDegrees[getScaleDegree(midiNoteIn)];
}

int Mode::getMidiNote(int scaleDegreeIn) const
{
	int periods = scaleDegreeIn / scaleSize;
	int offsetDeg = totalModulus(scaleDegreeIn - rootNote, scaleSize);

	return periods * scaleSize + offsetDeg;
}

int Mode::getMidiNote(int periodIn, int scaleDegreeIn) const
{
	scaleDegreeIn = totalModulus(scaleDegreeIn - rootNote, scaleSize);

	return periodIn * scaleSize + scaleDegreeIn;
}

int Mode::getMaxStep() const
{
	int step = 1;

	for (int i = 0; i < steps.size(); i++)
	{
		if (steps[i] > step)
			step = steps[i];
	}

	return step;
}

Array<int> Mode::getNotesOfOrder(int order) const
{
	Array<int> notesOut;

	for (int i = 0; i < orders.size(); i++)
	{
		if (orders.getUnchecked(i) == order)
			notesOut.add(i);
	}

	return notesOut;
}

Array<int> Mode::getMOSClass() const
{
	return mosClass;
}

String Mode::getDescription()  const
{
	return family + "[" + String(modeSize) + "] " + String(scaleSize);
}

String Mode::getScaleDescription()  const
{
	return String(scaleSize) + " " + getDescription().dropLastCharacters(2);
}

String Mode::getModeDescription()  const
{
	return String(modeSize) + " " + family + " " + String(scaleSize);
}

void Mode::updateStepsOfOrders()
{
	stepsOfOrders = expand_steps(orders_to_steps(orders));
}

int Mode::indexOfTag(String tagNameIn)
{
    return tags.indexOf(tagNameIn);
}

Array<int> Mode::parse_steps(String stepsIn)
{
	Array<int> stepsOut;

	std::string theSteps = stepsIn.toStdString();

	char c;
	int step;
	int digits;
	int i = 0;

	while (i < stepsIn.length())
	{
		digits = 0;
		c = stepsIn[i];


		while ((c >= 48 && c < 58) && (i + digits) < stepsIn.length())
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

Array<int> Mode::expand_steps(Array<int> stepsIn)
{
    Array<int> stepsOut;
    
    for (int i = 0; i < stepsIn.size(); i++)
    {
        for (int s = 0; s < stepsIn[i]; s++)
            stepsOut.add(stepsIn[i]);
    }
    
    stepsOut.minimiseStorageOverheads();
    return stepsOut;
}

String Mode::steps_to_string(Array<int> stepsIn)
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

Array<int> Mode::steps_to_orders(Array<int> stepsIn)
{
	Array<int> ordersOut;

	for (int i = 0; i < stepsIn.size(); i++)
	{
		for (int j = 0; j < stepsIn[i]; j++)
			ordersOut.add(j);
	}

	ordersOut.minimiseStorageOverheads();
	return ordersOut;
}

Array<int> Mode::expand_orders(Array<int> ordersIn, int offsetIn)
{
	Array<int> ordersOut;
	int period = ordersIn.size();
	int off = totalModulus(offsetIn, ordersIn.size());

	int index;

	for (int i = 0; i < 128; i++)
	{
		index = (off + i) % period;


		// adjust first few notes if it starts in the middle of a whole step
		if (i == 0 && ordersIn[index] != 0)
		{
			int localOffset = ordersIn[index];

			while (ordersIn[index] != 0)
			{
				ordersOut.add(ordersIn[index] - localOffset);
				i++;
				index = (off + i) % period;
			}
		}


		ordersOut.add(ordersIn[index]);
	}

	jassert(ordersOut.size() == 128);
	ordersOut.minimiseStorageOverheads();
	return ordersOut;
}

Array<int> Mode::orders_to_steps(Array<int> layoutIn)
{
	Array<int> stepsOut;

	int i = 0;
	int j = 0;
	int step = 0;

	while (i < layoutIn.size())
	{
		if (layoutIn[i] == 0 || i == 0)
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

Array<float> Mode::orders_to_modeDegrees(Array<int> ordersIn)
{
	Array<float> degreesOut;
	float deg = -1;

	int stepSize = 0;
	int i = 0;
	while (degreesOut.size() < ordersIn.size())
	{
		stepSize++;

		if ((i + stepSize) == (ordersIn.size() - 1) || 
			ordersIn[i + stepSize] == 0)
		{
			deg++;

			for (int j = 0; j < stepSize; j++)
				degreesOut.add(deg + (j / (float) stepSize));

			i += stepSize;
			stepSize = 0;
		}
	}

	degreesOut.minimiseStorageOverheads();
	return degreesOut;
}

Array<int> Mode::scale_degrees(int scaleSize, int offset)
{
	Array<int> degreesOut;

	auto mod = [](int a, int n) {return ((a % n) + n) % n; };

	for (int i = 0; i < scaleSize; i++)
	{
		degreesOut.add(mod(i - offset, scaleSize));
	}

	degreesOut.minimiseStorageOverheads();
	return degreesOut;
}


Array<int> Mode::interval_sizes(Array<int> stepsIn)
{
	Array<int> intervals;
	Array<int> intervalsOut;
	int step;

	for (int i = 0; i < stepsIn.size(); i++)
	{
		step = stepsIn[i];

		if (intervals.size() < step)
			intervals.resize(step);

		intervals.set(step, intervals[step] + 1);
	}

	// reverse an array
	for (int i = 0; i < intervals.size(); i++)
	{
		int val = intervals[intervals.size() - i - 1];
		if (val != 0)
			intervalsOut.add(val);
	}

	intervalsOut.minimiseStorageOverheads();

	return intervalsOut;
}

Array<int> Mode::sum_of_steps(Array<int> stepsIn, int offsetIn, bool includePeriod)
{
	Array<int> sumsOut;
	int sum = -offsetIn;

	sumsOut.add(sum);

	for (int i = 1; i < stepsIn.size(); i++)
	{
		sum += stepsIn[i - 1];
		sumsOut.add(sum);
	}

	if (includePeriod)
		sumsOut.add(sum + stepsIn[stepsIn.size() - 1]);

	//DBGArray(sumsOut, "Sums Out");

	sumsOut.minimiseStorageOverheads();
	return sumsOut;
}
