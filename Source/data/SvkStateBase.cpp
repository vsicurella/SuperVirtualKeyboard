/*
  ==============================================================================

    SvkStateBase.cpp
    Created: 4 Jun 2023 4:56:00pm
    Author:  Vincenzo

  ==============================================================================
*/

#include "SvkStateBase.h"

void SvkStateBase::setStateProperty(const juce::Identifier &id, juce::var value)
{
    state.setPropertyExcludingListener(this, id, value, nullptr);
}

bool SvkStateBase::getBoolProperty(const juce::Identifier key, bool fallback) const
{
    auto prop = state.getProperty(key, juce::var((bool)fallback));
    return (bool)prop;
}

int SvkStateBase::getIntProperty(const juce::Identifier key, int fallback) const
{
    auto prop = state.getProperty(key, juce::var((int)fallback));
    return (int)prop;
}

juce::String SvkStateBase::getStringProperty(const juce::Identifier key, juce::String fallback) const
{
    auto prop = state.getProperty(key, juce::var(fallback));
    return prop.toString();
}

void SvkStateBase::valueTreePropertyChanged(juce::ValueTree& treeWhosePropertyHasChanged, const juce::Identifier &property)
{
    handleStatePropertyChange(treeWhosePropertyHasChanged, property);
}
