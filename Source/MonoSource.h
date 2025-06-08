/* ------------------------------------------------------------
name: "monoSource"
Code generated with Faust 2.79.3 (https://faust.grame.fr)
Compilation options: -lang cpp -ct 1 -es 1 -mcd 16 -mdd 1024 -mdy 33 -single -ftz 0
------------------------------------------------------------ */

#ifndef  __mydsp_H__
#define  __mydsp_H__

#ifndef FAUSTFLOAT
#define FAUSTFLOAT float
#endif 

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <math.h>

// Add Faust base classes
#include "faust/dsp/dsp.h"
#include "faust/gui/meta.h"
#include "faust/gui/MapUI.h"

#ifndef FAUSTCLASS 
#define FAUSTCLASS MonoSource
#endif

class MonoSource;

#ifdef __APPLE__ 
#define exp10f __exp10f
#define exp10 __exp10
#endif

#if defined(_WIN32)
#define RESTRICT __restrict
#else
#define RESTRICT __restrict__
#endif

class mydspSIG0 {
	
  private:
	
	int iVec3[2];
	int iRec6[2];
	
  public:
	
	int getNumInputsmydspSIG0() {
		return 0;
	}
	int getNumOutputsmydspSIG0() {
		return 1;
	}
	
	void instanceInitmydspSIG0(int sample_rate) {
		for (int l6 = 0; l6 < 2; l6 = l6 + 1) {
			iVec3[l6] = 0;
		}
		for (int l7 = 0; l7 < 2; l7 = l7 + 1) {
			iRec6[l7] = 0;
		}
	}
	
	void fillmydspSIG0(int count, float* table) {
		for (int i1 = 0; i1 < count; i1 = i1 + 1) {
			iVec3[0] = 1;
			iRec6[0] = (iVec3[1] + iRec6[1]) % 65536;
			table[i1] = std::sin(9.58738e-05f * float(iRec6[0]));
			iVec3[1] = iVec3[0];
			iRec6[1] = iRec6[0];
		}
	}

};

static mydspSIG0* newmydspSIG0() { return (mydspSIG0*)new mydspSIG0(); }
static void deletemydspSIG0(mydspSIG0* dsp) { delete dsp; }

static float mydsp_faustpower2_f(float value) {
	return value * value;
}
static float ftbl0mydspSIG0[65536];

class MonoSource : public dsp {
	
 private:
	
	int iVec0[2];
	FAUSTFLOAT fEntry0;
	FAUSTFLOAT fEntry1;
	int fSampleRate;
	float fConst0;
	float fConst1;
	float fRec2[2];
	FAUSTFLOAT fEntry2;
	float fRec5[2];
	float fVec1[2];
	float fConst2;
	int IOTA0;
	float fVec2[4096];
	float fConst3;
	float fRec4[2];
	float fConst4;
	float fConst5;
	float fRec7[2];
	FAUSTFLOAT fEntry3;
	FAUSTFLOAT fCheckbox0;
	float fRec0[2];
	FAUSTFLOAT fEntry4;
	float fRec9[2];
	float fVec4[2];
	float fVec5[4096];
	float fRec8[2];
	float fRec10[2];
	FAUSTFLOAT fButton0;
	int iVec6[2];
	FAUSTFLOAT fEntry5;
	FAUSTFLOAT fEntry6;
	int iRec12[2];
	FAUSTFLOAT fEntry7;
	FAUSTFLOAT fEntry8;
	float fRec11[2];
	FAUSTFLOAT fEntry9;
	
 public:
	MonoSource() {
	}
	
	void metadata(Meta* m) { 
		m->declare("basics.lib/name", "Faust Basic Element Library");
		m->declare("basics.lib/version", "1.21.0");
		m->declare("compile_options", "-lang cpp -ct 1 -es 1 -mcd 16 -mdd 1024 -mdy 33 -single -ftz 0");
		m->declare("description", "Monophonic midi waveform generator with FM synthesis");
		m->declare("envelopes.lib/adsre:author", "Julius O. Smith III");
		m->declare("envelopes.lib/adsre:licence", "STK-4.3");
		m->declare("envelopes.lib/author", "GRAME");
		m->declare("envelopes.lib/copyright", "GRAME");
		m->declare("envelopes.lib/license", "LGPL with exception");
		m->declare("envelopes.lib/name", "Faust Envelope Library");
		m->declare("envelopes.lib/version", "1.3.0");
		m->declare("filename", "MonoSource.dsp");
		m->declare("filters.lib/lowpass0_highpass1", "MIT-style STK-4.3 license");
		m->declare("filters.lib/name", "Faust Filters Library");
		m->declare("filters.lib/pole:author", "Julius O. Smith III");
		m->declare("filters.lib/pole:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/pole:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/version", "1.7.1");
		m->declare("maths.lib/author", "GRAME");
		m->declare("maths.lib/copyright", "GRAME");
		m->declare("maths.lib/license", "LGPL with exception");
		m->declare("maths.lib/name", "Faust Math Library");
		m->declare("maths.lib/version", "2.8.1");
		m->declare("name", "monoSource");
		m->declare("oscillators.lib/lf_sawpos:author", "Bart Brouns, revised by Stéphane Letz");
		m->declare("oscillators.lib/lf_sawpos:licence", "STK-4.3");
		m->declare("oscillators.lib/name", "Faust Oscillator Library");
		m->declare("oscillators.lib/saw2ptr:author", "Julius O. Smith III");
		m->declare("oscillators.lib/saw2ptr:license", "STK-4.3");
		m->declare("oscillators.lib/sawN:author", "Julius O. Smith III");
		m->declare("oscillators.lib/sawN:license", "STK-4.3");
		m->declare("oscillators.lib/version", "1.6.0");
		m->declare("platform.lib/name", "Generic Platform Library");
		m->declare("platform.lib/version", "1.3.0");
		m->declare("signals.lib/name", "Faust Signal Routing Library");
		m->declare("signals.lib/version", "1.6.0");
	}

	virtual int getNumInputs() {
		return 0;
	}
	virtual int getNumOutputs() {
		return 1;
	}
	
	static void classInit(int sample_rate) {
		mydspSIG0* sig0 = newmydspSIG0();
		sig0->instanceInitmydspSIG0(sample_rate);
		sig0->fillmydspSIG0(65536, ftbl0mydspSIG0);
		deletemydspSIG0(sig0);
	}
	
	virtual void instanceConstants(int sample_rate) {
		fSampleRate = sample_rate;
		fConst0 = std::min<float>(1.92e+05f, std::max<float>(1.0f, float(fSampleRate)));
		fConst1 = 1.0f / fConst0;
		fConst2 = 0.25f * fConst0;
		fConst3 = 0.5f * fConst0;
		fConst4 = 1.76e+03f / fConst0;
		fConst5 = 4.4e+02f / fConst0;
	}
	
	virtual void instanceResetUserInterface() {
		fEntry0 = FAUSTFLOAT(6e+01f);
		fEntry1 = FAUSTFLOAT(0.1f);
		fEntry2 = FAUSTFLOAT(0.0f);
		fEntry3 = FAUSTFLOAT(3.0f);
		fCheckbox0 = FAUSTFLOAT(0.0f);
		fEntry4 = FAUSTFLOAT(0.0f);
		fButton0 = FAUSTFLOAT(0.0f);
		fEntry5 = FAUSTFLOAT(1.0f);
		fEntry6 = FAUSTFLOAT(0.1f);
		fEntry7 = FAUSTFLOAT(0.25f);
		fEntry8 = FAUSTFLOAT(0.5f);
		fEntry9 = FAUSTFLOAT(1.0f);
	}
	
	virtual void instanceClear() {
		for (int l0 = 0; l0 < 2; l0 = l0 + 1) {
			iVec0[l0] = 0;
		}
		for (int l1 = 0; l1 < 2; l1 = l1 + 1) {
			fRec2[l1] = 0.0f;
		}
		for (int l2 = 0; l2 < 2; l2 = l2 + 1) {
			fRec5[l2] = 0.0f;
		}
		for (int l3 = 0; l3 < 2; l3 = l3 + 1) {
			fVec1[l3] = 0.0f;
		}
		IOTA0 = 0;
		for (int l4 = 0; l4 < 4096; l4 = l4 + 1) {
			fVec2[l4] = 0.0f;
		}
		for (int l5 = 0; l5 < 2; l5 = l5 + 1) {
			fRec4[l5] = 0.0f;
		}
		for (int l8 = 0; l8 < 2; l8 = l8 + 1) {
			fRec7[l8] = 0.0f;
		}
		for (int l9 = 0; l9 < 2; l9 = l9 + 1) {
			fRec0[l9] = 0.0f;
		}
		for (int l10 = 0; l10 < 2; l10 = l10 + 1) {
			fRec9[l10] = 0.0f;
		}
		for (int l11 = 0; l11 < 2; l11 = l11 + 1) {
			fVec4[l11] = 0.0f;
		}
		for (int l12 = 0; l12 < 4096; l12 = l12 + 1) {
			fVec5[l12] = 0.0f;
		}
		for (int l13 = 0; l13 < 2; l13 = l13 + 1) {
			fRec8[l13] = 0.0f;
		}
		for (int l14 = 0; l14 < 2; l14 = l14 + 1) {
			fRec10[l14] = 0.0f;
		}
		for (int l15 = 0; l15 < 2; l15 = l15 + 1) {
			iVec6[l15] = 0;
		}
		for (int l16 = 0; l16 < 2; l16 = l16 + 1) {
			iRec12[l16] = 0;
		}
		for (int l17 = 0; l17 < 2; l17 = l17 + 1) {
			fRec11[l17] = 0.0f;
		}
	}
	
	virtual void init(int sample_rate) {
		classInit(sample_rate);
		instanceInit(sample_rate);
	}
	
	virtual void instanceInit(int sample_rate) {
		instanceConstants(sample_rate);
		instanceResetUserInterface();
		instanceClear();
	}
	
	virtual MonoSource* clone() {
		return new MonoSource();
	}
	
	virtual int getSampleRate() {
		return fSampleRate;
	}
	
	virtual void buildUserInterface(UI* ui_interface) {
		ui_interface->openVerticalBox("monoSource");
		ui_interface->addNumEntry("attack", &fEntry6, FAUSTFLOAT(0.1f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.01f));
		ui_interface->addNumEntry("decay", &fEntry7, FAUSTFLOAT(0.25f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.01f));
		ui_interface->addNumEntry("fm_depth", &fEntry3, FAUSTFLOAT(3.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1e+01f), FAUSTFLOAT(0.1f));
		ui_interface->addCheckButton("fm_enable", &fCheckbox0);
		ui_interface->addNumEntry("fm_ratio", &fEntry1, FAUSTFLOAT(0.1f), FAUSTFLOAT(0.01f), FAUSTFLOAT(8.0f), FAUSTFLOAT(0.01f));
		ui_interface->addNumEntry("fm_type", &fEntry2, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(3.0f), FAUSTFLOAT(1.0f));
		ui_interface->addButton("gate", &fButton0);
		ui_interface->addNumEntry("midiNote", &fEntry0, FAUSTFLOAT(6e+01f), FAUSTFLOAT(36.0f), FAUSTFLOAT(96.0f), FAUSTFLOAT(1.0f));
		ui_interface->addNumEntry("release", &fEntry5, FAUSTFLOAT(1.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(5.0f), FAUSTFLOAT(0.01f));
		ui_interface->addNumEntry("sustain", &fEntry8, FAUSTFLOAT(0.5f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.01f));
		ui_interface->addNumEntry("type", &fEntry4, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(3.0f), FAUSTFLOAT(1.0f));
		ui_interface->addNumEntry("velocity", &fEntry9, FAUSTFLOAT(1.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.01f));
		ui_interface->closeBox();
	}
	
	virtual void compute(int count, FAUSTFLOAT** RESTRICT inputs, FAUSTFLOAT** RESTRICT outputs) {
		FAUSTFLOAT* output0 = outputs[0];
		float fSlow0 = std::pow(2.0f, 0.083333336f * (float(int(float(fEntry0))) + -69.0f));
		float fSlow1 = float(fEntry1);
		float fSlow2 = fSlow1 * fSlow0;
		float fSlow3 = 4.4e+02f * fSlow2;
		float fSlow4 = std::max<float>(1.1920929e-07f, std::fabs(fSlow3));
		float fSlow5 = fConst1 * fSlow4;
		float fSlow6 = 1.0f - fConst0 / fSlow4;
		int iSlow7 = int(float(fEntry2));
		float fSlow8 = float(iSlow7 == 3);
		float fSlow9 = std::max<float>(fSlow3, 23.44895f);
		float fSlow10 = std::max<float>(2e+01f, std::fabs(fSlow9));
		float fSlow11 = fConst1 * fSlow10;
		float fSlow12 = fConst2 / fSlow10;
		float fSlow13 = std::max<float>(0.0f, std::min<float>(2047.0f, fConst3 / fSlow9));
		int iSlow14 = int(fSlow13);
		int iSlow15 = iSlow14 + 1;
		float fSlow16 = std::floor(fSlow13);
		float fSlow17 = fSlow13 - fSlow16;
		float fSlow18 = fSlow16 + (1.0f - fSlow13);
		float fSlow19 = fConst4 * fSlow1 * float(iSlow7 == 2) * fSlow0;
		float fSlow20 = float(iSlow7 == 1);
		float fSlow21 = fConst5 * fSlow2;
		float fSlow22 = float(iSlow7 == 0);
		float fSlow23 = float(fCheckbox0) * (float(fEntry3) + -1.0f);
		float fSlow24 = 4.4e+02f * fSlow0;
		int iSlow25 = int(float(fEntry4));
		float fSlow26 = float(iSlow25 == 3);
		float fSlow27 = fConst4 * float(iSlow25 == 2) * fSlow0;
		float fSlow28 = fConst2 * float(iSlow25 == 1);
		float fSlow29 = fConst5 * fSlow0;
		float fSlow30 = float(iSlow25 == 0);
		int iSlow31 = float(fButton0) > 0.0f;
		float fSlow32 = float(fEntry5);
		float fSlow33 = float(fEntry6);
		int iSlow34 = int(fConst0 * fSlow33);
		float fSlow35 = float(fEntry7);
		float fSlow36 = float(iSlow31);
		float fSlow37 = float(fEntry8) * fSlow36;
		float fSlow38 = float(fEntry9);
		for (int i0 = 0; i0 < count; i0 = i0 + 1) {
			iVec0[0] = 1;
			float fTemp0 = fSlow5 + fRec2[1] + -1.0f;
			int iTemp1 = fTemp0 < 0.0f;
			float fTemp2 = fSlow5 + fRec2[1];
			fRec2[0] = ((iTemp1) ? fTemp2 : fTemp0);
			float fRec3 = ((iTemp1) ? fTemp2 : fSlow5 + fRec2[1] + fSlow6 * fTemp0);
			int iTemp3 = 1 - iVec0[1];
			float fTemp4 = ((iTemp3) ? 0.0f : fSlow11 + fRec5[1]);
			fRec5[0] = fTemp4 - std::floor(fTemp4);
			float fTemp5 = mydsp_faustpower2_f(2.0f * fRec5[0] + -1.0f);
			fVec1[0] = fTemp5;
			float fTemp6 = float(iVec0[1]);
			float fTemp7 = fSlow12 * fTemp6 * (fTemp5 - fVec1[1]);
			fVec2[IOTA0 & 4095] = fTemp7;
			float fTemp8 = fSlow18 * fVec2[(IOTA0 - iSlow14) & 4095] + fSlow17 * fVec2[(IOTA0 - iSlow15) & 4095];
			fRec4[0] = fTemp7 + 0.999f * fRec4[1] - fTemp8;
			float fTemp9 = ((iTemp3) ? 0.0f : fSlow21 + fRec7[1]);
			fRec7[0] = fTemp9 - std::floor(fTemp9);
			float fTemp10 = fSlow23 * (fSlow22 * ftbl0mydspSIG0[std::max<int>(0, std::min<int>(int(65536.0f * fRec7[0]), 65535))] + fSlow20 * (fTemp7 - fTemp8) + fSlow19 * fRec4[0] + fSlow8 * (2.0f * fRec3 + -1.0f)) + 1.0f;
			float fTemp11 = fSlow24 * fTemp10;
			float fTemp12 = std::max<float>(1.1920929e-07f, std::fabs(fTemp11));
			float fTemp13 = fRec0[1] + fConst1 * fTemp12;
			float fTemp14 = fTemp13 + -1.0f;
			int iTemp15 = fTemp14 < 0.0f;
			fRec0[0] = ((iTemp15) ? fTemp13 : fTemp14);
			float fRec1 = ((iTemp15) ? fTemp13 : fTemp13 + fTemp14 * (1.0f - fConst0 / fTemp12));
			float fTemp16 = std::max<float>(fTemp11, 23.44895f);
			float fTemp17 = std::max<float>(2e+01f, std::fabs(fTemp16));
			float fTemp18 = ((iTemp3) ? 0.0f : fRec9[1] + fConst1 * fTemp17);
			fRec9[0] = fTemp18 - std::floor(fTemp18);
			float fTemp19 = mydsp_faustpower2_f(2.0f * fRec9[0] + -1.0f);
			fVec4[0] = fTemp19;
			float fTemp20 = fTemp6 * (fTemp19 - fVec4[1]) / fTemp17;
			fVec5[IOTA0 & 4095] = fTemp20;
			float fTemp21 = std::max<float>(0.0f, std::min<float>(2047.0f, fConst3 / fTemp16));
			int iTemp22 = int(fTemp21);
			float fTemp23 = std::floor(fTemp21);
			float fTemp24 = fTemp20 - fVec5[(IOTA0 - iTemp22) & 4095] * (fTemp23 + (1.0f - fTemp21)) - (fTemp21 - fTemp23) * fVec5[(IOTA0 - (iTemp22 + 1)) & 4095];
			fRec8[0] = fConst2 * fTemp24 + 0.999f * fRec8[1];
			float fTemp25 = ((iTemp3) ? 0.0f : fRec10[1] + fSlow29 * fTemp10);
			fRec10[0] = fTemp25 - std::floor(fTemp25);
			iVec6[0] = iSlow31;
			int iTemp26 = iSlow31 - iVec6[1];
			iRec12[0] = iSlow31 * (iRec12[1] + 1);
			int iTemp27 = (iRec12[0] < iSlow34) | (iTemp26 * (iTemp26 > 0));
			float fTemp28 = 0.1447178f * ((iSlow31) ? ((iTemp27) ? fSlow33 : fSlow35) : fSlow32);
			int iTemp29 = std::fabs(fTemp28) < 1.1920929e-07f;
			float fTemp30 = ((iTemp29) ? 0.0f : std::exp(-(fConst1 / ((iTemp29) ? 1.0f : fTemp28))));
			fRec11[0] = (1.0f - fTemp30) * ((iSlow31) ? ((iTemp27) ? fSlow36 : fSlow37) : 0.0f) + fTemp30 * fRec11[1];
			output0[i0] = FAUSTFLOAT(fSlow38 * fRec11[0] * (fSlow30 * ftbl0mydspSIG0[std::max<int>(0, std::min<int>(int(65536.0f * fRec10[0]), 65535))] + fSlow28 * fTemp24 + fSlow27 * fRec8[0] * fTemp10 + fSlow26 * (2.0f * fRec1 + -1.0f)));
			iVec0[1] = iVec0[0];
			fRec2[1] = fRec2[0];
			fRec5[1] = fRec5[0];
			fVec1[1] = fVec1[0];
			IOTA0 = IOTA0 + 1;
			fRec4[1] = fRec4[0];
			fRec7[1] = fRec7[0];
			fRec0[1] = fRec0[0];
			fRec9[1] = fRec9[0];
			fVec4[1] = fVec4[0];
			fRec8[1] = fRec8[0];
			fRec10[1] = fRec10[0];
			iVec6[1] = iVec6[0];
			iRec12[1] = iRec12[0];
			fRec11[1] = fRec11[0];
		}
	}

};

#endif
