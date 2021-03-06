/*
	This file is part of ALIZE which is an open-source tool for 
	speaker recognition.

    ALIZE is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as 
    published by the Free Software Foundation, either version 3 of 
    the License, or any later version.

    ALIZE is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public 
    License along with ALIZE.
    If not, see <http://www.gnu.org/licenses/>.
        
	ALIZE is a development project initiated by the ELISA consortium
	[alize.univ-avignon.fr/] and funded by the French Research 
	Ministry in the framework of the TECHNOLANGUE program 
	[www.technolangue.net]

	The ALIZE project team wants to highlight the limits of voice
	authentication in a forensic context.
	The "Person  Authentification by Voice: A Need of Caution" paper 
	proposes a good overview of this point (cf. "Person  
	Authentification by Voice: A Need of Caution", Bonastre J.F., 
	Bimbot F., Boe L.J., Campbell J.P., Douglas D.A., Magrin-
	chagnolleau I., Eurospeech 2003, Genova].
	The conclusion of the paper of the paper is proposed bellow:
	[Currently, it is not possible to completely determine whether the 
	similarity between two recordings is due to the speaker or to other 
	factors, especially when: (a) the speaker does not cooperate, (b) there 
	is no control over recording equipment, (c) recording conditions are not 
	known, (d) one does not know whether the voice was disguised and, to a 
	lesser extent, (e) the linguistic content of the message is not 
	controlled. Caution and judgment must be exercised when applying speaker 
	recognition techniques, whether human or automatic, to account for these 
	uncontrolled factors. Under more constrained or calibrated situations, 
	or as an aid for investigative purposes, judicious application of these 
	techniques may be suitable, provided they are not considered as infallible.
	At the present time, there is no scientific process that enables one to 
	uniquely characterize a person=92s voice or to identify with absolute 
	certainty an individual from his or her voice.]
	Contact Jean-Francois Bonastre for more information about the licence or
	the use of ALIZE

	Copyright (C) 2003-2010
	Laboratoire d'informatique d'Avignon [lia.univ-avignon.fr]
	ALIZE admin [alize@univ-avignon.fr]
	Jean-Francois Bonastre [jean-francois.bonastre@univ-avignon.fr]
*/

#if !defined(ALIZE_FeatureFileReaderHTK_cpp)
#define ALIZE_FeatureFileReaderHTK_cpp

#include <new>
#include "FeatureFileReaderHTK.h"
#include "Feature.h"
#include "Exception.h"
#include "LabelServer.h"
#include "Label.h"
#include "Config.h"
#include "FileReader.h"

// parameter

static const int WAVEFORM  = 0;
static const int LPC     = 1;
static const int LPREFC  = 2;
static const int LPCEPSTRA = 3;
static const int LPDELCEP  = 4;
static const int IREFC   = 5;
static const int MFCC    = 6;
static const int FBANK   = 7;
static const int MELSPEC   = 8;
static const int USER    = 9;
static const int DISCRETE  = 10;

// qualifiers (octal)

static const int E = 000100; // has energy
static const int N = 000200; // absolute energy suppressed
static const int D = 000400; // has delta coefficients
static const int A = 001000; // has delta coefficients
static const int C = 002000; // is compressed
static const int Z = 004000; // has zero mean static coef
static const int K = 010000; // has CRC checksum
static const int O = 020000; // has 0'th cepstral coef

using namespace std;
using namespace alize;
typedef FeatureFileReaderHTK R;

//-------------------------------------------------------------------------
R::FeatureFileReaderHTK(const FileName& f, const Config& c, LabelServer* l,
                     BigEndian be, BufferUsage b, unsigned long bufferSize,
                     HistoricUsage h, unsigned long historicSize)
 :FeatureFileReaderSingle(&FileReader::create(f, getPath(f, c),
 getExt(f, c), getBigEndian(c, be)), NULL, c, l, b, bufferSize, h, historicSize),
 _paramDefined(false) {}
//-------------------------------------------------------------------------
R& R::create(const FileName& f, const Config& c, LabelServer* l,
             BigEndian be, BufferUsage b, unsigned long bufferSize,
             HistoricUsage h, unsigned long historicSize)
{
  R* p = new (std::nothrow) R(f, c, l, be, b, bufferSize, h, historicSize);
  assertMemoryIsAllocated(p, __FILE__, __LINE__);
  return *p;
}
//-------------------------------------------------------------------------
void R::readParams() // private virtual
{
  assert(_pReader != NULL);
  _pReader->open(); // can throw FileNotFoundException

  if (!readHeader())
  {
    _pReader->close();
    throw InvalidDataException("Wrong header", __FILE__, __LINE__,
                  _pReader->getFullFileName());
  }
}
//-------------------------------------------------------------------------
unsigned long R::getFeatureCount()
{
  if (!_paramDefined)
    const_cast<FeatureFileReaderHTK*>(this)->readParams();  // can throw FileNotFoundException
  return _featureCount;
}
//-------------------------------------------------------------------------
unsigned long R::getVectSize()
{
  if (!_paramDefined)
    readParams(); // can throw FileNotFoundException
  return _vectSize;
}
//-------------------------------------------------------------------------
const FeatureFlags& R::getFeatureFlags()
{
  if (!_paramDefined)
    readParams(); // can throw FileNotFoundException
  return _flags;
}
//-------------------------------------------------------------------------
real_t R::getSampleRate()
{
  if (!_paramDefined)
    readParams(); // can throw FileNotFoundException
  return _sampleRate;
}
//-------------------------------------------------------------------------
string R::getClassName() const { return "FeatureFileReaderHTK"; }
//-------------------------------------------------------------------------
bool R::readHeader()
{
  assert(_pReader != NULL);
  _featureCount = _pReader->readInt4();
  _sampleRate = 10000000/_pReader->readInt4(); // 1/samplePeriod
                     // samplePeriod = multiple of 100 ns
  _vectSize = _pReader->readInt2()/4;
  
  int kind = 0;
  kind = _pReader->readInt2();
  if ( (int)(kind ^ 0xffffffc0) == WAVEFORM)
    return false;
  // TODO : test all cases

  _flags.set("100000");
  if (kind & E)
    _flags.useE = true;
  if (kind & D)
  {
    _flags.useD = true;
    if (kind & E)
      _flags.useDE = true;
  }
  if (kind & A)
  {
    _flags.useDD = true;
    if (kind & E)
      _flags.useDDE = true;
  }
  if (kind & N)
    _flags.useE = false;
  
  _paramDefined = true;

  return true;
}
//-------------------------------------------------------------------------
unsigned long R::getHeaderLength() { return 12; }
//-------------------------------------------------------------------------
R::~FeatureFileReaderHTK() {}
//-------------------------------------------------------------------------

#endif // !defined(ALIZE_FeatureFileReaderHTK_cpp)


