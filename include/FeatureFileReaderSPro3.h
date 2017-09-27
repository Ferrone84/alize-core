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

#if !defined(ALIZE_FeatureFileReaderSPro3_h)
#define ALIZE_FeatureFileReaderSPro3_h

#if defined(_WIN32)
#if defined(ALIZE_EXPORTS)
#define ALIZE_API __declspec(dllexport)
#else
#define ALIZE_API __declspec(dllimport)
#endif
#else
#define ALIZE_API
#endif

#include "FeatureFileReaderSingle.h"
#include "RealVector.h"


namespace alize
{
  class LabelServer;
  class Config;
  class FileReader;

  /// Convenient class for reading features from a S-PRO 3.x file
  /// The file is opened only after calling one of the methods readFeature,
  /// get FeatureCount, getVectSize, getFeatureFlags
  ///
  /// @author Frederic Wils  frederic.wils@lia.univ-avignon.fr
  /// @version 1.0
  /// @date 2003

  class ALIZE_API FeatureFileReaderSPro3 : public FeatureFileReaderSingle
  {
    friend class TestFeatureFileReaderSPro3;

  public :

    /// Creates a reader for a SPro 3.x feature file.
    /// @param f a file to read. No path is
    ///    required. It uses the parameter "featureFilesPath" of
    ///    the configuration.
    /// @param c the configuration to use
    /// @param ls address of a label server. can be NULL.
    ///
    FeatureFileReaderSPro3(const FileName& f,
       const Config& c, LabelServer* ls = NULL, BufferUsage b = BUFFER_AUTO,
       unsigned long bufferSize = 0, HistoricUsage = ALL_FEATURES,
       unsigned long historicSize = 0);
    
    /// See constructor with same parameters
    ///
    static FeatureFileReaderSPro3& create(const FileName&, const Config&,
       LabelServer* = NULL, BufferUsage = BUFFER_AUTO,
       unsigned long bufferSize = 0, HistoricUsage = ALL_FEATURES,
       unsigned long historicSize = 0);

    virtual ~FeatureFileReaderSPro3();

    /// Returns the number of features in the file
    /// @return the number of features in the file
    /// @exception IOException if an I/O error occurs
    /// @exception InvalidDataException thrown if the file is not a valid
    ///      SPro file
    ///
    virtual unsigned long getFeatureCount();

    /// Returns the size of the vector inside the features of this file
    /// @return the size of the vector inside the features of this file
    /// @exception IOException if an I/O error occurs
    /// @exception InvalidDataException thrown if the file is not a valid
    ///      SPro file
    ///
    virtual unsigned long getVectSize();

    /// Returns the feature flags of this file
    /// @return the feature flags of this file
    /// @exception IOException if an I/O error occurs
    /// @exception InvalidDataException thrown if the file is not a valid
    ///      SPro file
    ///
    virtual const FeatureFlags& getFeatureFlags();

    /// Returns the sample rate of this file.
    /// @return the sample rate of this file
    /// @exception IOException if an I/O error occurs
    /// @exception FileNotFoundException
    /// @exception InvalidDataException thrown if the file is not a valid
    ///      SPro file
    ///
    virtual real_t getSampleRate();

    virtual std::string getClassName() const;

    typedef SPRO3DataKind datakind_t;  /* Definition of sample kinds     */

  private :

    typedef float param_t; /* Definition of a parameter type */

    SPRO3DataKind    _kind;
    bool             _paramDefined;
    void readParams();
    bool readHeader();
    virtual unsigned long getHeaderLength();

    bool operator==(const FeatureFileReaderSPro3&)
                         const; /*!Not implemented*/
    bool operator!=(const FeatureFileReaderSPro3&)
                         const; /*!Not implemented*/
    const FeatureFileReaderSPro3& operator=(
             const FeatureFileReaderSPro3&); /*!Not implemented*/
    FeatureFileReaderSPro3(
             const FeatureFileReaderSPro3&); /*!Not implemented*/
  };

} // end namespace alize

#endif // !defined(ALIZE_FeatureFileReaderSPro3_h)

