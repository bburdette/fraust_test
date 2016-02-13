/************************************************************************
 ************************************************************************
    FAUST Architecture File
	Copyright (C) 2006-2011 Albert Graef <Dr.Graef@t-online.de>
    ---------------------------------------------------------------------
    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as 
	published by the Free Software Foundation; either version 2.1 of the 
	License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
 	License along with the GNU C Library; if not, write to the Free
  	Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
  	02111-1307 USA. 
 ************************************************************************
 ************************************************************************/

/* Q architecture file. Derived from minimal.cpp included in the Faust
   distribution. Please note that this is to be compiled as a shared library,
   which is then loaded dynamically by Q's faust module. */

#include <stdlib.h>
#include <math.h>

// 'null' implementation of Meta; nothing is recorded!
struct Meta
{
    void declare (const char* key, const char* value) { }
};

// abs is now predefined
//template<typename T> T abs (T a)			{ return (a<T(0)) ? -a : a; }

inline int		lsr (int x, int n)			{ return int(((unsigned int)x) >> n); }

/******************************************************************************
*******************************************************************************

							       VECTOR INTRINSICS

*******************************************************************************
*******************************************************************************/

//inline void *aligned_calloc(size_t nmemb, size_t size) { return (void*)((unsigned)(calloc((nmemb*size)+15,sizeof(char)))+15 & 0xfffffff0); }
//inline void *aligned_calloc(size_t nmemb, size_t size) { return (void*)((size_t)(calloc((nmemb*size)+15,sizeof(char)))+15 & ~15); }

// <<includeIntrinsic>>

/******************************************************************************
*******************************************************************************

			ABSTRACT USER INTERFACE

*******************************************************************************
*******************************************************************************/

class UI
{
  bool	fStopped;
public:
		
  UI() : fStopped(false) {}
  virtual ~UI() {}
	
  virtual void addButton(char* label, float* zone) = 0;
  virtual void addCheckButton(char* label, float* zone) = 0;
  virtual void addVerticalSlider(char* label, float* zone, float init, float min, float max, float step) = 0;
  virtual void addHorizontalSlider(char* label, float* zone, float init, float min, float max, float step) = 0;
  virtual void addNumEntry(char* label, float* zone, float init, float min, float max, float step) = 0;

  virtual void addHorizontalBargraph(char* label, float* zone, float min, float max) = 0;
  virtual void addVerticalBargraph(char* label, float* zone, float min, float max) = 0;
	
  virtual void openTabBox(char* label) = 0;
  virtual void openHorizontalBox(char* label) = 0;
  virtual void openVerticalBox(char* label) = 0;
  virtual void closeBox() = 0;
	
  virtual void run() = 0;
	
  void stop()	{ fStopped = true; }
  bool stopped() 	{ return fStopped; }

  virtual void declare(float* zone, const char* key, const char* value) {}
};

/***************************************************************************
   Q UI interface
 ***************************************************************************/

enum ui_elem_type_t {
  UI_BUTTON, UI_CHECK_BUTTON,
  UI_V_SLIDER, UI_H_SLIDER, UI_NUM_ENTRY,
  UI_V_BARGRAPH, UI_H_BARGRAPH,
  UI_END_GROUP, UI_V_GROUP, UI_H_GROUP, UI_T_GROUP
};

struct ui_elem_t {
  ui_elem_type_t type;
  char *label;
  float *zone;
  void *ref;
  float init, min, max, step;
};

class NullUI : public UI
{
public:
  int nelems;
  ui_elem_t *elems;
		
  NullUI();
  virtual ~NullUI();

protected:
  void add_elem(ui_elem_type_t type, char *label = NULL) {}
  void add_elem(ui_elem_type_t type, char *label, float *zone) {}
  void add_elem(ui_elem_type_t type, char *label, float *zone,
		float init, float min, float max, float step) {}
  void add_elem(ui_elem_type_t type, char *label, float *zone,
		float min, float max) {}

public:
  virtual void addButton(char* label, float* zone) {}
  virtual void addCheckButton(char* label, float* zone) {}
  virtual void addVerticalSlider(char* label, float* zone, float init, float min, float max, float step) {}
  virtual void addHorizontalSlider(char* label, float* zone, float init, float min, float max, float step) {}
  virtual void addNumEntry(char* label, float* zone, float init, float min, float max, float step) {}

  virtual void addHorizontalBargraph(char* label, float* zone, float min, float max) {}
  virtual void addVerticalBargraph(char* label, float* zone, float min, float max) {}
  
  virtual void openTabBox(char* label) {}
  virtual void openHorizontalBox(char* label) {}
  virtual void openVerticalBox(char* label) {}
  virtual void closeBox() {}
	
  virtual void run() {}
};


/******************************************************************************

			    FAUST DSP

*******************************************************************************/



//----------------------------------------------------------------
//  abstract definition of a signal processor
//----------------------------------------------------------------
			
class dsp {
 protected:
	int fSamplingFreq;
 public:
	dsp() {}
	virtual ~dsp() {}
	virtual int getNumInputs() = 0;
	virtual int getNumOutputs() = 0;
	virtual void buildUserInterface(UI* interface) = 0;
	virtual void init(int samplingRate) = 0;
 	virtual void compute(int len, float** inputs, float** outputs) = 0;
};

//----------------------------------------------------------------------------
//  FAUST generated signal processor
//----------------------------------------------------------------------------
		
#include "noise.cpp"

/* The class factory, used to create and destroy mydsp objects in the
   client. Implemented using C linkage to facilitate dlopen access. */

static dsp *mydsp_INSTANCE;

// void init(int samplerate) 
extern "C" void init(int samplerate) 
{
  mydsp_INSTANCE = new mydsp();
  mydsp_INSTANCE->init(samplerate);
}

//void compute(int len, float** inputs, float** outputs)
extern "C" void compute(int len, float** inputs, float** outputs)
{
  mydsp_INSTANCE->compute(len, inputs, outputs);   
}