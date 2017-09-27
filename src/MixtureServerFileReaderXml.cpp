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

#if !defined(ALIZE_MixtureServerFileReaderXml_cpp)
#define ALIZE_MixtureServerFileReaderXml_cpp

#include <new>
#include "MixtureServerFileReaderXml.h"
#include "MixtureServer.h"
#include "MixtureGD.h"
#include "MixtureGF.h"
#include "DistribGD.h"
#include "DistribGF.h"
#include "Exception.h"
#include "XmlParser.h"
#include "Config.h"
#include "FileReader.h"
#include "string_util.h"

// see http://babel.alis.com/web_ml/xml/REC-xml.fr.html#NT-XMLDecl

using namespace std; 
using namespace alize;
typedef MixtureServerFileReaderXml R;

//-------------------------------------------------------------------------
R::MixtureServerFileReaderXml(const FileName& f, const Config& c)
:MixtureServerFileReaderAbstract(&FileReader::create(f, getPath(f, c),
 getExt(f, c), false /* no swap */)), XmlParser() {}
//-------------------------------------------------------------------------
R& R::create(const FileName& f, const Config& c)
{
  R* p = new (std::nothrow) R(f, c);
  assertMemoryIsAllocated(p, __FILE__, __LINE__);
  return *p;
}
//-------------------------------------------------------------------------
void R::readMixtureServer(MixtureServer& ms)
{
  assert(_pReader != NULL);
  _line = 1;
  _pMixtureServer = &ms;
  parse();
  _pReader->close();
}
//-------------------------------------------------------------------------
void R::eventOpeningElement(const string& path)
{
  if (false) {}
  else if (endsWith(path, "<mean>"))
  {
    _meanIndexFound = false;
  }
  else if (endsWith(path, "<covInv>"))
  {
    _covInvIndexFound = false;
    _covInvIndexJFound = false;
  }
  else if (endsWith(path, "<cov>"))
  {
    _covIndexFound = false;
  }
  else if (endsWith(path, "<mean><i>"))   {}
  else if (endsWith(path, "<covInv><i>") || endsWith(path, "<covInv><j>")) {}
  else if (endsWith(path, "<cov><i>") || endsWith(path, "<cov><j>"))  {}
  else if (endsWith(path, "<MixtureServer><DistribGD>"))
  {
    _distribTypeDefined = true;
    _distribType = DistribType_GD;
    _pDistrib = NULL;
  }
  else if (endsWith(path, "<MixtureServer><DistribGF>"))
  {
    _distribTypeDefined = true;
    _distribType = DistribType_GF;
    _pDistrib = NULL;
  }
  else if (endsWith(path, "<MixtureServer><DistribGD><i>")) {}
  else if (endsWith(path, "<MixtureServer><DistribGF><i>")) {}
  else if (endsWith(path, "<MixtureServer><MixtureGD><DistribGD>"))
  {
    _distribIndexFound = false;
    _weightFound = false;
    _distribTypeDefined = true;
    _distribType = DistribType_GD;
  }
  else if (endsWith(path, "<MixtureServer><MixtureGF><DistribGF>"))
  {
    _distribIndexFound = false;
    _weightFound = false;
    _distribTypeDefined = true;
    _distribType = DistribType_GF;
  }
  else if (endsWith(path, "<MixtureServer><MixtureGD><DistribGD><i>"))     {}
  else if (endsWith(path, "<MixtureServer><MixtureGF><DistribGF><i>"))     {}
  else if (endsWith(path, "<MixtureServer><MixtureGD><DistribGD><dictIdx>")) {}
  else if (endsWith(path, "<MixtureServer><MixtureGF><DistribGF><dictIdx>")) {}
  else if (endsWith(path, "<MixtureServer><MixtureGD><DistribGD><weight>"))  {}
  else if (endsWith(path, "<MixtureServer><MixtureGF><DistribGF><weight>"))  {}
  else if (endsWith(path, "<MixtureServer><MixtureGD>"))
  {
    _pMixture = NULL;
    _mixtType = DistribType_GD;
    _mixtTypeDefined = true;
    _distribTypeDefined = false;
  }
  else if (endsWith(path, "<MixtureServer><MixtureGF>"))
  {
    _pMixture = NULL;
    _mixtType = DistribType_GF;
    _mixtTypeDefined = true;
    _distribTypeDefined = false;
  }
  else if (endsWith(path, "<MixtureServer><MixtureGD><id>"))       {}
  else if (endsWith(path, "<MixtureServer><MixtureGF><id>"))       {}
  else if (endsWith(path, "<MixtureServer><MixtureGD><distribCount>")) {}
  else if (endsWith(path, "<MixtureServer><MixtureGF><distribCount>")) {}
  else if (endsWith(path, "<MixtureServer><vectSize>"))
  {
    if (_vectSizeFound)
      eventError("More than one tag " + path + " !");
    _vectSizeFound = true;
  }
  else if (endsWith(path, "<MixtureServer><version>"))    {}
  else if (endsWith(path, "<MixtureServer><mixtureCount>")) {}
  else if (endsWith(path, "<MixtureServer><distribCount>")) {}
  else if (endsWith(path, "<MixtureServer><name>"))     {}
  else if (endsWith(path, "<MixtureServer>"))
  {
    _pMixtureServer->reset();
    _pMixtureServer->setServerName("");
    _vectSizeFound = false;
    _mixtTypeDefined = false;
  }
  else
    eventError("Unknown tag in the path " + path);
}
//-------------------------------------------------------------------------
void R::eventClosingElement(const string& path,
                             const string& value)
{
  if (false) {}
  else if (endsWith(path, "<mean>"))
  {
    if (!_meanIndexFound)
      eventError("Index missing for mean");
    if (!_distribTypeDefined)
      eventError("unknown mixture type");
    if (_distribType == DistribType_GD)
      getDistribGD().setMean(stod(value), _meanIndex);
    else if (_distribType == DistribType_GF)
      getDistribGF().setMean(stod(value), _meanIndex);
    _meanIndexFound = false;
  }
  else if (endsWith(path, "<covInv>"))
  {
    if (!_covInvIndexFound)
      eventError("Index i missing for covInv");
    if (!_distribTypeDefined)
      eventError("unknown mixture type");
    if (_distribType == DistribType_GD)
      getDistribGD().setCovInv(K::k, stod(value), _covInvIndex);
    else if (_distribType == DistribType_GF)
    {
      if (!_covInvIndexJFound)
        eventError("Index j missing for covInv");
      getDistribGF().setCovInv(K::k, stod(value), _covInvIndex, _covInvIndexJ);
    }
  }
  else if (endsWith(path, "<cov>"))
  {
    if (!_covIndexFound)
      eventError("Index missing for cov");
    if (!_distribTypeDefined)
      eventError("unknown mixture type");
    if (_distribType == DistribType_GD)
    {
      getDistribGD().setCov(stod(value), _covIndex);
    }
    else if (_distribType == DistribType_GF)
    {
      if (!_covIndexFound)
        eventError("Index j missing for cov");
      getDistribGF().setCov(stod(value), _covIndex, _covIndexJ);
    }
  }
  else if (endsWith(path, "<mean><i>"))
  {
    _meanIndex = stol(value);
    _meanIndexFound = true;
  }
  else if (endsWith(path, "<covInv><i>"))
  {
    _covInvIndex = stol(value);
    _covInvIndexFound = true;
  }
  else if (endsWith(path, "<covInv><j>"))
  {
    _covInvIndexJ = stol(value);
    _covInvIndexJFound = true;
  }
  else if (endsWith(path, "<cov><i>"))
  {
    _covIndex = stol(value);
    _covIndexFound = true;
  }
  else if (endsWith(path, "<cov><j>"))
  {
    _covIndexJ = stol(value);
    _covIndexJFound = true;
  }

  // -----------------------------------------------

  else if (endsWith(path, "<MixtureServer><DistribGD><i>")) {}
  else if (endsWith(path, "<MixtureServer><DistribGF><i>")) {}
  else if (endsWith(path, "<MixtureServer><DistribGD>") ||
           endsWith(path, "<MixtureServer><DistribGF>"))
    _distribTypeDefined = false;

  // -----------------------------------------------

  else if (endsWith(path, "<MixtureServer><MixtureGD><DistribGD><i>"))     {}
  else if (endsWith(path, "<MixtureServer><MixtureGF><DistribGF><i>"))     {}
  else if (endsWith(path, "<MixtureServer><MixtureGD><DistribGD><dictIdx>") ||
           endsWith(path, "<MixtureServer><MixtureGF><DistribGF><dictIdx>"))
  {
    _distribIndex = stol(value);
    _distribIndexFound = true;
  }
  else if (endsWith(path, "<MixtureServer><MixtureGD><DistribGD><weight>") ||
           endsWith(path, "<MixtureServer><MixtureGF><DistribGF><weight>"))
  {
    _weight = stod(value);
    _weightFound = true;
  }
  else if (endsWith(path, "<MixtureServer><MixtureGD><DistribGD>") ||
           endsWith(path, "<MixtureServer><MixtureGF><DistribGF>"))
  {
    if (!_distribIndexFound)
      eventError("dict index missing to assign a distrib to a mixture");
    if (!_weightFound)
      eventError("unknown weigth to assign a distrib to a mixture");
    Distrib& d = _pMixtureServer->getDistrib(_distribIndex);
    _pMixtureServer->addDistribToMixture(getMixture(), d, _weight);
    _distribTypeDefined = false;
  }

  // -----------------------------------------------

  else if (endsWith(path, "<MixtureServer><MixtureGD>") ||
           endsWith(path, "<MixtureServer><MixtureGF>"))
    _mixtTypeDefined = false;
  else if (endsWith(path, "<MixtureServer><MixtureGD><id>"))
  {
    _pMixtureServer->setMixtureId(getMixtureGD(), value);
  }
  else if (endsWith(path, "<MixtureServer><MixtureGF><id>"))
  {
    _pMixtureServer->setMixtureId(getMixtureGF(), value);
  }
  else if (endsWith(path, "<MixtureServer><MixtureGD><distribCount>")) {}
  else if (endsWith(path, "<MixtureServer><MixtureGF><distribCount>")) {}

  // -----------------------------------------------

  else if (endsWith(path, "<MixtureServer><version>"))
  {
    if (value != "1")
      eventError("invalid version");
  }
  else if (endsWith(path, "<MixtureServer><name>"))
  {
    _pMixtureServer->setServerName(value);
  }
  else if (endsWith(path, "<MixtureServer><vectSize>"))
  {
    _vectSize = stol(value);
    _vectSizeFound = true;
  }
  else if (endsWith(path, "<MixtureServer><mixtureCount>"))  {}
  else if (endsWith(path, "<MixtureServer><distribCount>"))  {}
}
//-------------------------------------------------------------------------
void R::eventError(const string& msg)
{
  assert(_pReader != NULL);
  _pReader->close();
  _pMixtureServer->reset();
  _pMixtureServer->setServerName("");
  throw InvalidDataException("Error line " + std::to_string(_line)
           + " : " + msg, __FILE__, __LINE__, _pReader->getFullFileName());
}
//-------------------------------------------------------------------------
const string& R::readOneChar()
{
  assert(_pReader != NULL);
  const string& s = _pReader->readString(1);
  if (s == "\n")
    _line++;
  return s;
}

//-------------------------------------------------------------------------
Mixture& R::getMixture()
{
  if (_pMixture == NULL)
  {
    if (!_mixtTypeDefined)
      eventError("unknown mixture type");
    _pMixture = &_pMixtureServer->createMixture(0, _mixtType);
  }
  return *_pMixture;
}
//-------------------------------------------------------------------------
MixtureGD& R::getMixtureGD()
{ return static_cast<MixtureGD&>(getMixture()); }
//-------------------------------------------------------------------------
MixtureGF& R::getMixtureGF()
{ return static_cast<MixtureGF&>(getMixture()); }
//-------------------------------------------------------------------------
Distrib& R::getDistrib()
{
  if (_pDistrib == NULL)
  {
    if (!_vectSizeFound)
      eventError("Unknown vectSize !");
    if (!_distribTypeDefined)
      eventError("unknown mixture type");
    _pDistrib = &_pMixtureServer->createDistrib(_distribType, _vectSize);
  }
  return *_pDistrib;
}
//-------------------------------------------------------------------------
DistribGD& R::getDistribGD()
{ return static_cast<DistribGD&>(getDistrib()); }
//-------------------------------------------------------------------------
DistribGF& R::getDistribGF()
{ return static_cast<DistribGF&>(getDistrib()); }
//-------------------------------------------------------------------------
string R::getClassName() const { return "MixtureServerFileReaderXml"; }
//-------------------------------------------------------------------------
R::~MixtureServerFileReaderXml() {}
//-------------------------------------------------------------------------

#endif // !defined(ALIZE_MixtureServerFileReaderXml_cpp)

