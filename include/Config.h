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

#if !defined(ALIZE_Config_h)
#define ALIZE_Config_h

#include "alize_util.h"
#include "Object.h"
#include "FeatureFlags.h"
#include "XList.h"

namespace alize
{
  /*!
  Class for configuration objects. A configuration objets is used to store
  parameters useful to create others objects like MixtureServer, StatServer
  ... Each parameter can be set and read.
    
  @author Frederic Wils  frederic.wils@lia.univ-avignon.fr
  @version 1.0
  @date 2003
  */

  class ALIZE_API Config : public Object
  {
    friend class TestConfig;

  public :

    Config();

    /// Loads configuration data from a file
    /// @param f the file to read (path + name)
    /// 
    explicit Config(const FileName& f);

    Config(const Config&);
    const Config& operator=(const Config& c);
    bool operator==(const Config& c) const;
    bool operator!=(const Config& c) const;
    virtual ~Config();
    
    /// Save a configuration in a file
    /// @param f the file to write in (path + name)
    /// @exception IOException if an I/O error occurs
    /// @exception FileNotFoundException
    ///
    void save(const FileName& f) const;

    /// Resets this Config object and loads configurations from a file
    /// @param f the file to read (path + name)
    /// @exception IOException if an I/O error occurs
    /// @exception FileNotFoundException
    ///
    void load(const FileName& f);

    /// Resets this configuration. Deletes all parameters.
    ///
    void reset();

    /// Returns the number of parameters stored.
    /// @return the number of parameters stored
    ///
    unsigned long getParamCount() const;

    /// Returns the parameter name number i
    /// @return a std::string object with the parameter name
    /// @exception IndexOutOfBoundsException
    ///
    const std::string& getParamName(unsigned long) const;

    /// Returns the parameter content number i
    /// @return a std::string object with the parameter content
    /// @exception IndexOutOfBoundsException
    ///
    const std::string& getParamContent(unsigned long) const;

    /// Adds (or replaces) a parameter in this config
    ///
    void setParam(const std::string& name, const std::string& content);

    /// Copy the parameters of the config c in this config
    ///
    void setParam(const Config& c);

    /// @exception if the param does not exist
    ///
    const std::string& getParam(const std::string& name) const;

    /// @exception if the param does not exist
    ///
    long getIntegerParam(const std::string& name) const;

    /// @exception if the param does not exist
    ///
    double getFloatParam(const std::string& name) const;

    /// @exception if the param does not exist
    ///
    bool getBooleanParam(const std::string& name) const;

    /// Tests whether a parameter exists
    ///
    bool existsParam(const std::string& name) const;

    //------------------------------------------------------------------

    /// @exception if the param does not exist
    ///
    real_t getParam_minCov() const;

    /// @exception if the param does not exist
    ///
    unsigned long getParam_vectSize() const;

    /// @exception if the param does not exist
    ///
    bool getParam_computeLLKWithTopDistribs() const;

    ///
    bool getParam_debug() const;

    /// @exception if the param does not exist
    ///
    unsigned long getParam_topDistribsCount() const;

    /// @exception if the param does not exist
    ///
    unsigned long getParam_mixtureDistribCount() const;

    /// @exception if the param does not exist
    ///
    lk_t getParam_minLLK() const;

    /// @exception if the param does not exist
    ///
    lk_t getParam_maxLLK() const;

    /// @exception if the param does not exist
    ///
    DistribType getParam_distribType() const;

    /// @exception if the param does not exist
    ///
    MixtureFileWriterFormat getParam_saveMixtureFileFormat() const;

    /// @exception if the param does not exist
    ///
    MixtureServerFileWriterFormat getParam_saveMixtureServerFileFormat() const;

    /// @exception if the param does not exist
    ///
    SegServerFileWriterFormat getParam_saveSegServerFileFormat() const;


    //------------------------------------------------------------------
    // Features
    //------------------------------------------------------------------


    /// @exception if the param does not exist
    ///
    const FeatureFlags& getParam_featureFlags() const;
    
    /// Can be a number or "ALL_FEATURES"
    /// @exception if the param does not exist
    ///
    const std::string& getParam_featureServerBufferSize() const;
    
    /// See method FeatureInputStreamModifier::setMask()
    /// @exception if the param does not exist
    ///
    const std::string& getParam_featureServerMask() const;

    /// @exception if the param does not exist
    ///
    const std::string& getParam_featureFilesPath() const;

    /// @exception if the param does not exist
    ///
    const std::string& getParam_audioFilesPath() const;

    /// @exception if the param does not exist
    ///
    unsigned long getParam_loadFeatureFileMemAlloc() const;

    /// @exception if the param does not exist
    ///
    unsigned long getParam_featureServerMemAlloc() const;

    /// @exception if the param does not exist
    ///
    FeatureFileReaderFormat getParam_loadFeatureFileFormat() const;

    /// @exception if the param does not exist
    ///
    unsigned long getParam_loadFeatureFileVectSize() const;

    /// @exception if the param does not exist
    ///
    unsigned long getParam_loadAudioFileChannel() const;

    /// @exception if the param does not exist
    ///
    const std::string& getParam_loadFeatureFileExtension() const;

    /// @exception if the param does not exist
    ///
    const std::string& getParam_loadAudioFileExtension() const;

    /// @exception if the param does not exist
    ///
    bool getParam_loadFeatureFileBigEndian() const;

    /// @exception if the param does not exist
    ///
    bool getParam_loadAudioFileBigEndian() const;

    /// @exception if the param does not exist
    ///
    const std::string& getParam_featureServerMode() const;

    /// @exception if the param does not exist
    ///
    bool getParam_loadMixtureFileBigEndian() const;

    /// @exception if the param does not exist
    ///
    FeatureFileWriterFormat getParam_saveFeatureFileFormat() const;

    /// @exception if the param does not exist
    ///
    const std::string& getParam_saveFeatureFileExtension() const;

    /// @exception if the param does not exist
    ///
    SPRO3DataKind getParam_saveFeatureFileSPro3DataKind() const;

    //------------------------------------------------------------------


    /// @exception if the param does not exist
    ///
    MixtureFileReaderFormat getParam_loadMixtureFileFormat() const;

    /// @exception if the param does not exist
    ///
    SegServerFileReaderFormat getParam_loadSegServerFileFormat() const;

    /// @exception if the param does not exist
    ///
    const std::string& getParam_loadMixtureFileExtension() const;

    /// @exception if the param does not exist
    ///
    const std::string& getParam_loadSegServerFileExtension() const;

    /// @exception if the param does not exist
    ///
    const std::string& getParam_saveMixtureFileExtension() const;

    /// @exception if the param does not exist
    ///
    const std::string& getParam_saveMixtureServerFileExtension() const;

    /// @exception if the param does not exist
    ///
    const std::string& getParam_saveSegServerFileExtension() const;

    /// @exception if the param does not exist
    ///
    const std::string& getParam_mixtureFilesPath() const;

    /// @exception if the param does not exist
    ///
    const std::string& getParam_segServerFilesPath() const;

    /// @exception if the param does not exist
    ///
    bool getParam_bigEndian() const;

    /// @exception if the param does not exist
    ///
    real_t getParam_sampleRate() const;

    virtual std::string getClassName() const;
    virtual std::string toString() const;

    bool  existsParam_minCov;
    bool  existsParam_vectSize;
    bool  existsParam_loadFeatureFileMemAlloc;
    bool  existsParam_featureServerMemAlloc;
    bool  existsParam_computeLLKWithTopDistribs;
    bool  existsParam_debug;
    bool  existsParam_topDistribsCount;
    bool  existsParam_featureServerBufferSize;
    bool  existsParam_featureServerMask;
    bool  existsParam_featureFlags;
    bool  existsParam_mixtureDistribCount;
    bool  existsParam_minLLK;
    bool  existsParam_maxLLK;
    bool  existsParam_distribType;

    bool  existsParam_saveMixtureFileFormat;
    bool  existsParam_saveMixtureServerFileFormat;
    bool  existsParam_saveSegServerFileFormat;
    bool  existsParam_saveFeatureFileFormat;
    bool  existsParam_saveFeatureFileSPro3DataKind;
    bool  existsParam_loadFeatureFileFormat;
    bool  existsParam_loadFeatureFileVectSize;
    bool  existsParam_loadAudioFileChannel;
    bool  existsParam_loadMixtureFileFormat;
    bool  existsParam_loadSegServerFileFormat;
    bool  existsParam_saveMixtureFileExtension;
    bool  existsParam_saveMixtureServerFileExtension;
    bool  existsParam_saveSegServerFileExtension;
    bool  existsParam_saveFeatureFileExtension;
    bool  existsParam_loadFeatureFileExtension;
    bool  existsParam_loadAudioFileExtension;
    bool  existsParam_loadFeatureFileBigEndian;
    bool  existsParam_loadAudioFileBigEndian;
    bool  existsParam_featureServerMode;
    bool  existsParam_loadMixtureFileBigEndian;
    bool  existsParam_loadMixtureFileExtension;
    bool  existsParam_loadSegServerFileExtension;
    bool  existsParam_bigEndian;
    bool  existsParam_sampleRate;
    bool  existsParam_featureFilesPath;
    bool  existsParam_audioFilesPath;
    bool  existsParam_segServerFilesPath;
    bool  existsParam_mixtureFilesPath;

  private :
    real_t              _param_minCov;
    unsigned long       _param_vectSize;
    unsigned long       _param_loadFeatureFileMemAlloc;
    unsigned long       _param_featureServerMemAlloc;
    bool                _param_computeLLKWithTopDistribs;
    bool                _param_debug;
    unsigned long       _param_topDistribsCount;
    std::string              _param_featureServerBufferSize; // can be a number
                               // or "ALL_FEATURES"
    std::string              _param_featureServerMask;
    FeatureFlags        _param_featureFlags;
    unsigned long       _param_mixtureDistribCount;
    MixtureFileWriterFormat _param_saveMixtureFileFormat;
    MixtureServerFileWriterFormat _param_saveMixtureServerFileFormat;
    SegServerFileWriterFormat _param_saveSegServerFileFormat;
    std::string              _param_saveMixtureFileExtension;
    std::string              _param_saveMixtureServerFileExtension;
    std::string              _param_saveSegServerFileExtension;
    MixtureFileReaderFormat _param_loadMixtureFileFormat;
    SegServerFileReaderFormat _param_loadSegServerFileFormat;
    std::string              _param_loadMixtureFileExtension;
    std::string              _param_loadSegServerFileExtension;
    std::string              _param_mixtureFilesPath;
    FeatureFileWriterFormat _param_saveFeatureFileFormat;
    SPRO3DataKind       _param_saveFeatureFileSPro3DataKind;
    std::string              _param_saveFeatureFileExtension;
    FeatureFileReaderFormat _param_loadFeatureFileFormat;
    unsigned long _param_loadFeatureFileVectSize;
    unsigned long _param_loadAudioFileChannel;
    std::string              _param_loadFeatureFileExtension;
    std::string              _param_loadAudioFileExtension;
    bool                _param_loadFeatureFileBigEndian;
    bool                _param_loadAudioFileBigEndian;
    std::string              _param_featureServerMode;
    bool                _param_loadMixtureFileBigEndian;
    std::string              _param_featureFilesPath;
    std::string              _param_audioFilesPath;
    std::string              _param_segServerFilesPath;
    lk_t         _param_minLLK;
    lk_t         _param_maxLLK;
    DistribType  _param_distribType;
    bool         _param_bigEndian;
    real_t       _param_sampleRate;

    XList        _set;

    void assign(const Config&);
    void err(const std::string&) const;
  };

} // end namespace alize

#endif // !defined(ALIZE_Config_h)
