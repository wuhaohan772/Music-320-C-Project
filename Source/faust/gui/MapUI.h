#ifndef FAUST_MAPUI_H
#define FAUST_MAPUI_H

#include <map>
#include <string>
#include "../gui/meta.h"
#include "../dsp/dsp.h"

/*******************************************************************************
 * MapUI : map-based user interface
 ******************************************************************************/

class MapUI : public UI, public Meta
{
private:
    std::map<std::string, FAUSTFLOAT*> fPathZoneMap;
    std::map<std::string, FAUSTFLOAT> fPathValueMap;

public:
    MapUI() {}
    virtual ~MapUI() {}

    // -- widget's layouts
    virtual void openTabBox(const char* label) {}
    virtual void openHorizontalBox(const char* label) {}
    virtual void openVerticalBox(const char* label) {}
    virtual void closeBox() {}

    // -- active widgets
    virtual void addButton(const char* label, FAUSTFLOAT* zone)
    {
        fPathZoneMap[label] = zone;
        fPathValueMap[label] = 0.0f;
    }
    
    virtual void addCheckButton(const char* label, FAUSTFLOAT* zone)
    {
        fPathZoneMap[label] = zone;
        fPathValueMap[label] = 0.0f;
    }
    
    virtual void addVerticalSlider(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT step)
    {
        fPathZoneMap[label] = zone;
        fPathValueMap[label] = init;
        *zone = init;
    }
    
    virtual void addHorizontalSlider(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT step)
    {
        fPathZoneMap[label] = zone;
        fPathValueMap[label] = init;
        *zone = init;
    }
    
    virtual void addNumEntry(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT step)
    {
        fPathZoneMap[label] = zone;
        fPathValueMap[label] = init;
        *zone = init;
    }

    // -- passive widgets
    virtual void addHorizontalBargraph(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT min, FAUSTFLOAT max)
    {
        fPathZoneMap[label] = zone;
        fPathValueMap[label] = min;
    }
    
    virtual void addVerticalBargraph(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT min, FAUSTFLOAT max)
    {
        fPathZoneMap[label] = zone;
        fPathValueMap[label] = min;
    }

    // -- soundfiles
    virtual void addSoundfile(const char* label, const char* filename, Soundfile** sf_zone) {}

    // -- metadata declarations
    virtual void declare(FAUSTFLOAT* zone, const char* key, const char* val) {}

    // -- set/get
    void setParamValue(const std::string& path, FAUSTFLOAT value)
    {
        if (fPathZoneMap.find(path) != fPathZoneMap.end()) {
            *fPathZoneMap[path] = value;
            fPathValueMap[path] = value;
        }
    }

    FAUSTFLOAT getParamValue(const std::string& path)
    {
        if (fPathValueMap.find(path) != fPathValueMap.end()) {
            return fPathValueMap[path];
        }
        return 0.0f;
    }

    // -- Meta interface
    virtual void declare(const char* key, const char* value) {}
};

#endif 