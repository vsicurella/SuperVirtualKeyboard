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

    steps = parseIntStringToArray(stepsString);
    ordersDefault = unfoldStepsToOrders(steps);
    scaleSize = 1;
    modeSize = 1;
    name = getDescription();
    
    updateProperties();
    updateNode(true);
}

Mode::Mode(String stepsIn, String familyIn, int rootNoteIn, String nameIn, String infoIn)
{
    stepsString = stepsIn;
    family = familyIn;
    rootNote = rootNoteIn;
    info = infoIn;

    steps = parseIntStringToArray(stepsIn);
    ordersDefault = unfoldStepsToOrders(steps);
    scaleSize = ordersDefault.size();
    modeSize = steps.size();
    offset = getOffset() * -1;
    
    if (nameIn == "")
        name = getDescription();
    else
        name = nameIn;

    updateProperties();
    updateNode(true);
}

Mode::Mode(Array<int> stepsIn, String familyIn, int rootNoteIn, String nameIn, String infoIn)
{
    stepsString = intArrayToString(stepsIn);
    family = familyIn;
    rootNote = rootNoteIn;
    info = infoIn;

    steps = stepsIn;
    ordersDefault = unfoldStepsToOrders(steps);
    scaleSize = ordersDefault.size();
    modeSize = steps.size();
    offset = getOffset() * -1;
    
    if (nameIn == "")
        name = getDescription();
    else
        name = nameIn;

    updateProperties();
    updateNode(true);
}

Mode::Mode(ValueTree modeNodeIn, bool copyNode)
: Mode()
{
    if (isValidMode(modeNodeIn))
    {
        if (copyNode)
            modeNode = modeNodeIn.createCopy();
        else
            modeNode = modeNodeIn;

        stepsString = modeNode[IDs::stepString];
        steps = parseIntStringToArray(stepsString);
        ordersDefault = unfoldStepsToOrders(steps);
        
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
        
        rootNote = modeNode[IDs::rootMidiNote];
        if (rootNote < 0 || rootNote > 127)
            rootNote = 60;

        updateProperties();
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
    if (isValidMode(nodeIn))
    {
        modeNode = nodeIn;
        
        stepsString = modeNode[IDs::stepString];
        steps = parseIntStringToArray(stepsString);
        ordersDefault = unfoldStepsToOrders(steps);
        
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
            rootNote = modeNode[IDs::rootMidiNote];

        if (rootNote < 0 || rootNote > 127)
            rootNote = 60;
        
        updateProperties();
        updateNode();
    }
}

bool Mode::isValidMode(ValueTree nodeIn)
{
    String steps = nodeIn.getProperty(IDs::stepString).toString();
    Array<int> stepsArray = Mode::parseIntStringToArray(steps);

    int length = stepsArray.size();
    int numDegrees = sumUpToIndex(stepsArray, length);

    return length > 0 && numDegrees <= 128;
}


ValueTree Mode::createNode(String stepsIn, String familyIn, String nameIn, String infoIn, int rootNoteIn, bool factoryPreset)
{
    ValueTree modeNodeOut = ValueTree(IDs::modePresetNode);
    Array<int> steps = parseIntStringToArray(stepsIn);
    Array<int> orders = unfoldStepsToOrders(steps);
    Array<int> mosClass = intervalAmounts(steps);

    if (rootNoteIn < 0 || rootNoteIn > 127)
        rootNoteIn = 60;

    modeNodeOut.setProperty(IDs::scaleSize, orders.size(), nullptr);
    modeNodeOut.setProperty(IDs::modeSize, steps.size(), nullptr);
    modeNodeOut.setProperty(IDs::stepString, stepsIn, nullptr);
    modeNodeOut.setProperty(IDs::family, familyIn, nullptr);
    modeNodeOut.setProperty(IDs::modeInfo, infoIn, nullptr);
    modeNodeOut.setProperty(IDs::rootMidiNote, rootNoteIn, nullptr);
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

ValueTree Mode::createNode(Array<int> stepsIn, String familyIn, String nameIn, String infoIn, int rootNoteIn, bool factoryPreset)
{
    ValueTree modeNodeOut = ValueTree(IDs::modePresetNode);
    Array<int> orders = unfoldStepsToOrders(stepsIn);
    Array<int> mosClass = intervalAmounts(stepsIn);
    String stepsStr = intArrayToString(stepsIn);

    if (rootNoteIn < 0 || rootNoteIn > 127)
        rootNoteIn = 60;

    modeNodeOut.setProperty(IDs::scaleSize, orders.size(), nullptr);
    modeNodeOut.setProperty(IDs::modeSize, stepsIn.size(), nullptr);
    modeNodeOut.setProperty(IDs::stepString, stepsStr, nullptr);
    modeNodeOut.setProperty(IDs::family, familyIn, nullptr);
    modeNodeOut.setProperty(IDs::modeInfo, infoIn, nullptr);
    modeNodeOut.setProperty(IDs::rootMidiNote, rootNoteIn, nullptr);
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

void Mode::updateProperties()
{
    offset = -getOffset();
    fixedOffset = (rootNote / scaleSize) * scaleSize + rootNote % scaleSize;

    orders = repeatArray(ordersDefault, 128, offset);
    modeDegrees = ordersToModalDegrees(orders);
    scaleDegrees = generateScaleDegrees(scaleSize, offset);
    mosClass = intervalAmounts(steps);
    keyboardOrdersSizes = intervalAmounts(orders);
    stepsOfOrders = repeatIndicies(foldOrdersToSteps(orders));
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
        updateProperties();
    }

    modeNode.setProperty(IDs::rootMidiNote, rootNote, nullptr);
}

void Mode::rotate(int rotateAmt)
{
    int amt = totalModulus(rotateAmt, modeSize);

    Array<int> newSteps;
    for (int i = 0; i < steps.size(); i++)
        newSteps.add(steps[(amt + i) % steps.size()]);

    steps = newSteps;
    stepsString = intArrayToString(steps);

    ValueTree degreeColors = modeNode.getChildWithName(IDs::pianoKeyColorsNode).getChildWithName(IDs::pianoKeyColorsDegree);
    for (auto degreeNode : degreeColors)
    {
        int newDeg = totalModulus((int)degreeNode[IDs::pianoKeyColorsDegree] - rotateAmt, scaleSize);
        degreeNode.setProperty(IDs::pianoKeyColorsDegree, newDeg, nullptr);
    }
    
    updateProperties();
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

Array<int> Mode::getIntervalSizeCount() const
{
    return intervalAmounts(steps);
}

Array<int> Mode::getStepsOfOrders()  const
{
    return stepsOfOrders;
}

int Mode::getNoteStep(int noteNumberIn) const
{
    return stepsOfOrders[noteNumberIn];
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

StringArray Mode::getTags() const
{
    return tags;
}

Array<int> Mode::getSteps(int rotationIn)  const
{
    if (rotationIn == 0)
        return steps;
    else
    {
        Array<int> rotatedSteps;
        rotationIn = totalModulus(rotationIn, modeSize);

        for (int i = 0; i < modeSize; i++)
            rotatedSteps.add(steps[(i + rotationIn) % modeSize]);

        return rotatedSteps;
    }
}

String Mode::getStepsString(int rotationIn)  const
{
    if (rotationIn == 0)
        return stepsString;
    else
    {
        // lol feelin lazy about this boring function
        Array<int> stepsRotated = getSteps(rotationIn);
        return intArrayToString(stepsRotated);
    }
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

int Mode::getOrder(int noteNumberIn) const
{
    return orders[noteNumberIn];
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
    return modeDegrees[midiNoteIn % 128];
}

int Mode::getFixedDegree(int midiNoteIn) const
{
    return midiNoteIn - fixedOffset;
}

int Mode::fixedDegreeToNoteNumber(int fixedDegreeIn) const
{
    return fixedDegreeIn + fixedOffset;
}

Point<int> Mode::getPeriodsAndDegree(int midiNoteIn) const
{
    return Point<int>((midiNoteIn - rootNote) / scaleSize, (midiNoteIn - rootNote) % scaleSize);
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

int Mode::indexOfTag(String tagNameIn)
{
    return tags.indexOf(tagNameIn);
}

int Mode::isSimilarTo(Mode* modeToCompare) const
{
    Array<int> mode1Steps = steps;
    Array<int> mode2Steps = modeToCompare->getSteps();
    
    int rotations = 0;
    
    if (mode1Steps.size() != mode2Steps.size())
        return -1;
    
    while (mode1Steps != mode2Steps || rotations < mode1Steps.size())
    {
        mode2Steps.move(0, mode2Steps.size());
    }
    
    return (rotations % (mode1Steps.size() + 1)) - 1;
}

Array<Array<int>> Mode::getNotesOrders()
{
    Array<Array<int>> notesOut;
    notesOut.resize(getMaxStep());

    for (int i = 0; i < getMaxStep(); i++)
    {
        notesOut.getReference(i).addArray(getNotesOfOrder(i));
    }
    
    return notesOut;
}

Array<Array<int>> Mode::getNotesInScaleDegrees()
{
    Array<Array<int>> notesOut;
    notesOut.resize(getScaleSize());
    
    return notesOut;
}

Array<Array<int>> Mode::getNotesInModalDegrees()
{
    Array<Array<int>> notesOut;
    notesOut.resize(getModeSize());
    
    return notesOut;
}


Array<int> Mode::parseIntStringToArray(String stepsIn)
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

Array<int> Mode::repeatIndicies(Array<int> arrayToRepeat, Array<int> repeatMask)
{
    Array<int> stepsOut;

    if (repeatMask.size() == 0)
        repeatMask = arrayToRepeat;
    
    for (int i = 0; i < arrayToRepeat.size(); i++)
    {
        for (int s = 0; s < repeatMask[i % repeatMask.size()]; s++)
            stepsOut.add(arrayToRepeat[i]);
    }
    
    stepsOut.minimiseStorageOverheads();
    return stepsOut;
}

String Mode::intArrayToString(Array<int> stepsIn)
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

Array<int> Mode::unfoldStepsToOrders(Array<int> stepsIn)
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

// needs revision to allow for notes to start in the middle of a step
Array<int> Mode::repeatArray(Array<int> ordersIn, int sizeIn, int offsetIn)
{
    Array<int> ordersOut;
    int period = ordersIn.size();
    int off = totalModulus(offsetIn, ordersIn.size());

    int index;

    for (int i = 0; i < sizeIn; i++)
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

Array<int> Mode::foldOrdersToSteps(Array<int> layoutIn)
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

Array<float> Mode::ordersToModalDegrees(Array<int> ordersIn)
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

Array<int> Mode::generateScaleDegrees(int scaleSize, int offset)
{
    Array<int> degreesOut;

    for (int i = 0; i < scaleSize; i++)
    {
        degreesOut.add(totalModulus(i - offset, scaleSize));
    }

    degreesOut.minimiseStorageOverheads();
    return degreesOut;
}


Array<int> Mode::intervalAmounts(Array<int> stepsIn)
{
    HashMap<int, int> intervalCount;
    Array<int> stepTypes;

    for (auto step : stepsIn)
    {
        if (intervalCount.contains(step))
            intervalCount.set(step, intervalCount[step] + 1);
        else
        {
            intervalCount.set(step, 1);
            stepTypes.add(step);
        }
    }

    stepTypes.sort();

    Array<int> intervalAmountsOut;

    for (int i = stepTypes.size() - 1; i >= 0; i--)
    {
        intervalAmountsOut.add(intervalCount[stepTypes[i]]);
    }

    return intervalAmountsOut;
}

Array<int> Mode::sumArray(Array<int> stepsIn, int offsetIn, bool includePeriod)
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
