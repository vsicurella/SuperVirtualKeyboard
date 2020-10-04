        /*
  ==============================================================================

    ModeLayout.h
    Created: 31 Mar 2019 2:16:09pm
    Author:  Vincenzo

  ==============================================================================
*/

#pragma once

#include "../PluginIDs.h"
#include "../CommonFunctions.h"

/*
    A class for representing the layout of a virtualKeyboard
    based on a given mode. The mode can be applied to different scales.
*/

class Mode
{
    String name;
    int scaleSize;
    int modeSize;
    String family;
    String info;
    StringArray tags;
    
    int rootNote = 60;
    int offset;
    int fixedOffset;
    
    String stepsString;
    Array<int> steps;
    Array<int> ordersDefault;
    Array<int> mosClass;
    
    Array<int> orders;
    Array<int> scaleDegrees;
    Array<float> modeDegrees;
    
    // Keyboard Convenience
    Array<int> stepsOfOrders; // each index is the step which the note is associated with
    Array<int> keyboardOrdersSizes; // amount of keys in each key order groupings
    
public:
    
    ValueTree modeNode;

    Mode();
    Mode(String stepsIn, String familyIn="undefined", int rootNoteIn=60, String nameIn="", String infoIn="");
    Mode(Array<int> stepsIn, String familyIn="undefined", int rootNoteIn=60, String nameIn="", String infoIn="");
    Mode(ValueTree modeNodeIn, bool copyNode=false);

    ~Mode();

    void updateNode(bool initializeNode=false);
    void restoreNode(ValueTree nodeIn, bool useNodeRoot=true);

    static bool isValidMode(ValueTree nodeIn);

    static ValueTree createNode(String stepsIn, String familyIn = "undefined", String nameIn = "", String infoIn="", int rootNoteIn=60, bool factoryPreset = false);
    static ValueTree createNode(Array<int> stepsIn, String familyIn = "undefined", String nameIn = "", String infoIn="", int rootNoteIn=60, bool factoryPreset = false);

    /*
        Sets temperament family name.
    */
    void setFamily(String familyIn);
    
    /*
        Sets custom name of the mode.
    */
    void setName(String nameIn);

    /*
        Sets info regarding the mode.
    */
    void setInfo(String infoIn);
    
    /*
        Recalculates mode properties based on parameter change
     */
    void updateProperties();
    
    /*
        Sets the offset of the mode and updates parameters so that the 
        offset replaces the current visualization of the mode
    */
    void setRootNote(int rootNoteIn);
    
    /*
        Rotates the scale by moving the index of the first interval
        by the rotation amount given.
    */
    void rotate(int rotateAmt);
    
    void addTag(String tagIn);
    
    int removeTag(String tagIn);

    int getRootNote() const;

    int getOffset()  const;

    int getScaleSize() const;

    int getModeSize() const;
    
    Array<int> getIntervalSizeCount() const;

    String getFamily() const;
    
    String getName() const;

    String getInfo() const;
    
    StringArray getTags() const;

    Array<int>* getKeyboardOrdersSizes();

    int getKeyboardOrdersSize(int ordersIn) const;
    
    Array<int> getStepsOfOrders() const;

    int getNoteStep(int noteNumberIn) const;

    Array<int> getSteps(int rotationIn=0) const;

    String getStepsString(int rotationIn=0) const;

    Array<int> getOrdersDefault() const;

    Array<int> getOrders() const;

    int getOrder(int noteNumberIn) const;

    Array<float> getModalDegrees() const;

    Array<int> getScaleDegrees() const;

    int getScaleDegree(int midiNoteIn) const;

    float getModeDegree(int midiNoteIn) const;

    int getFixedDegree(int midiNoteIn) const;

    int fixedDegreeToNoteNumber(int fixedDegreeIn) const;

    Point<int> getPeriodsAndDegree(int midiNoteIn) const;

    int getMidiNote(int scaleDegreeIn) const;

    int getMidiNote(int periodIn, int scaleDegreeIn) const;

    int getMaxStep() const;

    Array<int> getMOSClass() const;

    String getDescription() const;

    String getScaleDescription() const;

    String getModeDescription() const;
    
    Array<int> getNotesOfOrder(int order = 0) const;
    
    int indexOfTag(String tagNameIn);
    
    /*
     Returns -1 if modes are different, otherwise returns the number of rotations
     needed to make the modes equivalent.
     */
    int isSimilarTo(Mode* modeToCompare) const;

    /*
        Returns a table of note numbers (with current root) organized by key order.
    */
    Array<Array<int>> getNotesOrders();

    /*
        Returns a table of note numbers (with current root) organized by scale degree.
    */
    Array<Array<int>> getNotesInScaleDegrees();

    /*
        Returns a table of note numbers (with current root) organized by modal degree.
    */
    Array<Array<int>> getNotesInModalDegrees();

    /*
    Simply parses a string reprsenting step sizes and returns a vector
    */
    static Array<int> parseIntStringToArray(String stepsIn);
    
    /*
    Takes in step vector like {2, 2 , 1 , 2 , 2 , 2 , 1}
    Returns a string like "2 2 1 2 2 2 1"
    */
    static String intArrayToString(Array<int> stepsIn);

    /*
     Takes in a vector like {2, 2, 1, 2, 2, 2, 1} and a mask like {0, 0, 2, 0, 0, 0, 2}
     Returns a vector of each index repeated by the mask {2, 2, 1, 1, 2, 2, 2, 1, 1}
     If no mask supplied, then it uses it's own magnitudes.
     */
    static Array<int> repeatIndicies(Array<int> arrayToRepeat, Array<int> repeatMask=Array<int>());

    /*
    Takes in step vector like {2, 2, 1, 2, 2, 2, 1}
    Returns a vector of key orders {0, 1, 0, 1, 0, 0, 1,...}
    */
    static Array<int> unfoldStepsToOrders(Array<int> stepsIn);

    /*
    Takes in array of a scale layout of note orders (1:1) {0, 1, 0, 1 , 0, 0, 1,...}
    and returns scale step size layout "2, 2, 1, 2,..."
    */
    static Array<int> foldOrdersToSteps(Array<int> layoutIn);

    /*
        Takes in a period-size vector, a new vector size, and an offset
        Returns a vector at new size using original vector's period for repetition with the given offset
    */
    // needs revision to allow for notes to start in the middle of a step
    static Array<int> repeatArray(Array<int> ordersIn, int sizeIn, int offsetIn=0);

    /*
    Takes in array of a scale layout of note orders (1:1) {0, 1, 0, 1 , 0, 0, 1,...}
    and returns scale step size layout "0, 0.5, 1, 1.5,..."
    */
    static Array<float> ordersToModalDegrees(Array<int> ordersIn);

    /*
    Simply creates an array of scale degrees based off of scale size and offset
    */
    static Array<int> generateScaleDegrees(int scaleSize, int offset = 0);

    /*
    Takes in a vector like {2, 2, 1, 2, 2, 2, 1}
    Returns a vector of sizes large->small like {5, 2} 
    */
    static Array<int> intervalAmounts(Array<int> stepsIn);

    /*
    Takes in a vector like {2, 2, 1, 2, 2, 2, 1}
    And returns a sum of the previous indicies at each index like {0, 2, 4, 5, 7, 9, 11}
    */
    static Array<int> sumArray(Array<int> stepsIn, int offsetIn = 0, bool includePeriod = false);

};


