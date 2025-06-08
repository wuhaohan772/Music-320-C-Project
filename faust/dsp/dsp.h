#ifndef __dsp__
#define __dsp__

#include <string>
#include <vector>
#include <algorithm>
#include <cmath>
#include <cstdint>

#ifdef _MSC_VER
#include <iso646.h>  // For alternative operators (and, or, not) in MSVC
#endif

#ifndef FAUSTFLOAT
#define FAUSTFLOAT float
#endif

// Forward declarations
class UI;
struct Meta;
struct dsp_memory_manager;

/**
 * Soundfile
 */
struct Soundfile {
    void* fBuffers;     // will correspond to a double** or float** pointer chosen at compile time
    int* fLength;       // length of each part (so fLength[P] contains the length in frames of part P)
    int* fSR;           // sample rate of each part (so fSR[P] contains the SR of part P)
    int* fOffset;       // offset of each part in the global buffer (so fOffset[P] contains the offset in frames of part P)
    int fChannels;      // max number of channels of all concatenated files
    int fParts;         // the total number of loaded parts
    bool fIsDouble;     // keep the sample format (float or double)

    Soundfile() {
        fBuffers = nullptr;
        fLength = nullptr;
        fSR = nullptr;
        fOffset = nullptr;
        fChannels = 0;
        fParts = 0;
        fIsDouble = false;
    }
};

/**
 * Memory manager
 */
struct dsp_memory_manager {
    virtual ~dsp_memory_manager() {}
    virtual void* allocate(size_t size) = 0;
    virtual void destroy(void* ptr) = 0;
};

/**
 * User Interface
 */
class UI
{
public:
    UI() {}
    virtual ~UI() {}

    // -- widget's layouts
    virtual void openTabBox(const char* label) = 0;
    virtual void openHorizontalBox(const char* label) = 0;
    virtual void openVerticalBox(const char* label) = 0;
    virtual void closeBox() = 0;

    // -- active widgets
    virtual void addButton(const char* label, FAUSTFLOAT* zone) = 0;
    virtual void addCheckButton(const char* label, FAUSTFLOAT* zone) = 0;
    virtual void addVerticalSlider(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT step) = 0;
    virtual void addHorizontalSlider(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT step) = 0;
    virtual void addNumEntry(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT step) = 0;

    // -- passive widgets
    virtual void addHorizontalBargraph(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT min, FAUSTFLOAT max) = 0;
    virtual void addVerticalBargraph(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT min, FAUSTFLOAT max) = 0;

    // -- soundfiles
    virtual void addSoundfile(const char* label, const char* filename, Soundfile** sf_zone) = 0;

    // -- metadata declarations
    virtual void declare(FAUSTFLOAT* zone, const char* key, const char* val) {}

    // -- Meta interface
    virtual void declare(const char* key, const char* val) {}
};

/**
 * Signal processor definition.
 */
class dsp {
public:
    dsp() {}
    virtual ~dsp() {}

    /* Return instance number of audio inputs */
    virtual int getNumInputs() = 0;

    /* Return instance number of audio outputs */
    virtual int getNumOutputs() = 0;

    /**
     * Trigger the ui_interface parameter with instance specific calls
     * to 'openTabBox', 'addButton', 'addVerticalSlider'... in order to build the UI.
     *
     * @param ui_interface - the user interface builder
     */
    virtual void buildUserInterface(UI* ui_interface) = 0;

    /* Returns the sample rate currently used by the instance */
    virtual int getSampleRate() = 0;

    /**
     * Global init, calls the following methods:
     * - static class 'classInit': static tables initialization
     * - 'instanceInit': constants and instance state initialization
     *
     * @param sample_rate - the sampling rate in Hertz
     */
    virtual void init(int sample_rate) = 0;

    /**
     * Init instance state
     *
     * @param sample_rate - the sampling rate in Hertz
     */
    virtual void instanceInit(int sample_rate) = 0;

    /**
     * Init instance constant state
     *
     * @param sample_rate - the sampling rate in Hertz
     */
    virtual void instanceConstants(int sample_rate) = 0;

    /* Init default control parameters values */
    virtual void instanceResetUserInterface() = 0;

    /* Init instance state (delay lines...) */
    virtual void instanceClear() = 0;

    /**
     * Return a clone of the instance.
     *
     * @return a copy of the instance on success, otherwise a null pointer.
     */
    virtual dsp* clone() = 0;

    /**
     * Trigger the Meta* parameter with instance specific calls to 'declare' (key, value) metadata.
     *
     * @param m - the Meta* meta user
     */
    virtual void metadata(Meta* m) = 0;

    /**
     * DSP instance computation, to be called with successive in/out audio buffers.
     *
     * @param count - the number of frames to compute
     * @param inputs - the input audio buffers as an array of non-interleaved FAUSTFLOAT samples (outs[num_inputs][count])
     * @param outputs - the output audio buffers as an array of non-interleaved FAUSTFLOAT samples (outs[num_outputs][count])
     *
     */
    virtual void compute(int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs) = 0;

    /**
     * DSP instance computation: alternative method to be used by subclasses.
     *
     * @param date_usec - the timestamp in microsec given by audio driver.
     * @param count - the number of frames to compute
     * @param inputs - the input audio buffers as an array of non-interleaved FAUSTFLOAT samples (outs[num_inputs][count])
     * @param outputs - the output audio buffers as an array of non-interleaved FAUSTFLOAT samples (outs[num_outputs][count])
     *
     */
    virtual void compute(double /*date_usec*/, int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs) { compute(count, inputs, outputs); }
};

/**
 * Generic DSP decorator.
 */
class decorator_dsp : public dsp {
protected:
    dsp* fDSP;

public:
    decorator_dsp(dsp* dsp = nullptr):fDSP(dsp) {}
    virtual ~decorator_dsp() { delete fDSP; }

    virtual int getNumInputs() { return fDSP->getNumInputs(); }
    virtual int getNumOutputs() { return fDSP->getNumOutputs(); }
    virtual void buildUserInterface(UI* ui_interface) { fDSP->buildUserInterface(ui_interface); }
    virtual int getSampleRate() { return fDSP->getSampleRate(); }
    virtual void init(int sample_rate) { fDSP->init(sample_rate); }
    virtual void instanceInit(int sample_rate) { fDSP->instanceInit(sample_rate); }
    virtual void instanceConstants(int sample_rate) { fDSP->instanceConstants(sample_rate); }
    virtual void instanceResetUserInterface() { fDSP->instanceResetUserInterface(); }
    virtual void instanceClear() { fDSP->instanceClear(); }
    virtual decorator_dsp* clone() { return new decorator_dsp(fDSP->clone()); }
    virtual void metadata(Meta* m) { fDSP->metadata(m); }
    // Beware: subclasses usually have to overload the two 'compute' methods
    virtual void compute(int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs) { fDSP->compute(count, inputs, outputs); }
    virtual void compute(double date_usec, int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs) { fDSP->compute(date_usec, count, inputs, outputs); }
};

/**
 * DSP factory class.
 */
class dsp_factory {
protected:
    // So that to force sub-classes to use deleteDSPFactory(dsp_factory* factory);
    virtual ~dsp_factory() {}

public:
    virtual std::string getName() = 0;
    virtual std::string getSHAKey() = 0;
    virtual std::string getDSPCode() = 0;
    virtual std::string getCompileOptions() = 0;
    virtual std::vector<std::string> getLibraryList() = 0;
    virtual std::vector<std::string> getIncludePathnames() = 0;

    virtual dsp* createDSPInstance() = 0;

    virtual void setMemoryManager(dsp_memory_manager* manager) = 0;
    virtual dsp_memory_manager* getMemoryManager() = 0;
};

/**
 * On Intel set FZ (Flush to Zero) and DAZ (Denormals Are Zero)
 * flags to avoid costly denormals.
 */
#ifdef __SSE__
    #include <xmmintrin.h>
    #ifdef __SSE2__
        #define AVOIDDENORMALS _mm_setcsr(_mm_getcsr() | 0x8040)
    #else
        #define AVOIDDENORMALS _mm_setcsr(_mm_getcsr() | 0x8000)
    #endif
#else
    #define AVOIDDENORMALS
#endif

#endif 