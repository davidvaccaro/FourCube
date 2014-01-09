// FourCube.h

#pragma once

using namespace System;
using namespace System::Collections;
using namespace System::Collections::Generic;
using namespace System::Drawing;
using namespace System::Runtime::InteropServices;

// ------------------------------------------------------------------------------------------------------------------------------------------
// Global Defines
// ------------------------------------------------------------------------------------------------------------------------------------------

// Default timeout for "Process" operations
#define DEFULT_TIMEOUT 60000

namespace FourCube {

	// ------------------------------------------------------------------------------------------------------------------------------------------
	// Enumeration: FourCube.ExtractType
	// ------------------------------------------------------------------------------------------------------------------------------------------
	// Author: David Vaccaro
	// Description: Represents different types of extract.
	// ------------------------------------------------------------------------------------------------------------------------------------------
	// Revision History (latest at the top!)
	// ------------------------------------------------------------------------------------------------------------------------------------------
	// WHEN			| WHO	| WHAT
	// ------------------------------------------------------------------------------------------------------------------------------------------
	// 12/19/2013	| DMV	| Initial version.
	// ------------------------------------------------------------------------------------------------------------------------------------------

	public enum class ExtractType
	{
		Block,
		Paragraph,
		Symbol, 
		Line,
		Word 
	};

	// ------------------------------------------------------------------------------------------------------------------------------------------
	// Class: FourCube.Extract
	// ------------------------------------------------------------------------------------------------------------------------------------------
	// Author: David Vaccaro
	// Description: Represents an extracted text part, it's bound ( ... bounding rect), confidence level and other important details.
	// ------------------------------------------------------------------------------------------------------------------------------------------
	// Revision History (latest at the top!)
	// ------------------------------------------------------------------------------------------------------------------------------------------
	// WHEN			| WHO	| WHAT
	// ------------------------------------------------------------------------------------------------------------------------------------------
	// 12/19/2013	| DMV	| Initial version.
	// ------------------------------------------------------------------------------------------------------------------------------------------

	public ref class Extract 
	{


	public:

		// --------------------------------------------------------------------------------------
		// Property: Type
		// --------------------------------------------------------------------------------------
		// Author: David Vaccaro
		// Description: Gets and Sets the type of the extracted text part.
		// --------------------------------------------------------------------------------------
		// Revision History (latest at the top!)
		// --------------------------------------------------------------------------------------
		// WHEN			| WHO	| WHAT
		// --------------------------------------------------------------------------------------
		// 12/19/2013	| DMV	| Initial version.
		// --------------------------------------------------------------------------------------

		property ExtractType Type;

		// --------------------------------------------------------------------------------------
		// Property: Text
		// --------------------------------------------------------------------------------------
		// Author: David Vaccaro
		// Description: Gets and Sets the extracted text part.
		// --------------------------------------------------------------------------------------
		// Revision History (latest at the top!)
		// --------------------------------------------------------------------------------------
		// WHEN			| WHO	| WHAT
		// --------------------------------------------------------------------------------------
		// 12/19/2013	| DMV	| Initial version.
		// --------------------------------------------------------------------------------------

		property String ^ Text;

		// --------------------------------------------------------------------------------------
		// Property: Bound
		// --------------------------------------------------------------------------------------
		// Author: David Vaccaro
		// Description: Gets and Sets the bounding rect of this piece of text.
		// --------------------------------------------------------------------------------------
		// Revision History (latest at the top!)
		// --------------------------------------------------------------------------------------
		// WHEN			| WHO	| WHAT
		// --------------------------------------------------------------------------------------
		// 12/19/2013	| DMV	| Initial version.
		// --------------------------------------------------------------------------------------

		property System::Drawing::Rectangle ^ Bound;

		// --------------------------------------------------------------------------------------
		// Property: Confidence
		// --------------------------------------------------------------------------------------
		// Author: David Vaccaro
		// Description: Gets and Sets the confidence level for the extracted text.
		// --------------------------------------------------------------------------------------
		// Revision History (latest at the top!)
		// --------------------------------------------------------------------------------------
		// WHEN			| WHO	| WHAT
		// --------------------------------------------------------------------------------------
		// 12/19/2013	| DMV	| Initial version.
		// --------------------------------------------------------------------------------------

		property float ^ Confidence;

	};

	// ------------------------------------------------------------------------------------------------------------------------------------------
	// Class: FourCube.OCR
	// ------------------------------------------------------------------------------------------------------------------------------------------
	// Author: David Vaccaro
	// Description: Performs Optical Charachter Recognition (OCR) using the Tesseract engine.
	// ------------------------------------------------------------------------------------------------------------------------------------------
	// Revision History (latest at the top!)
	// ------------------------------------------------------------------------------------------------------------------------------------------
	// WHEN			| WHO	| WHAT
	// ------------------------------------------------------------------------------------------------------------------------------------------
	// 12/19/2013	| DMV	| 
	// 12/17/2013	| DMV	| Process(Pix) function.
	// 12/16/2013	| DMV	| Process(Bitmap) function, some image helper functions, Bound. 
	// 12/15/2013	| DMV	| ProcessAll() and Process(STRING) functions, constructor, destructor, Timeout property.
	// 12/14/2013	| DMV	| Initial version and Create function.
	// ------------------------------------------------------------------------------------------------------------------------------------------

	public ref class OCR
	{

	private:

		// The reference to the Tesseract API
		tesseract::TessBaseAPI * _pAPI;
		String ^ _dataPath;
		String ^ _languageCode;
		System::Drawing::Rectangle ^ _bound;

		// --------------------------------------------------------------------------------------
		// Method: BitmapToBitmap32BPP
		// --------------------------------------------------------------------------------------
		// Author: David Vaccaro
		// Description: Convert any Bitmap to a 32BPP Bitmap.
		// --------------------------------------------------------------------------------------
		// Parameters:
		//
		//	- bitmap = The bitmap to convert.
		//
		// --------------------------------------------------------------------------------------
		// Returns: 
		//
		// - Success: Bitmap @ 32BBP
		// - Failure: NULL
		//
		// --------------------------------------------------------------------------------------
		// Revision History (latest at the top!)
		// --------------------------------------------------------------------------------------
		// WHEN			| WHO	| WHAT
		// --------------------------------------------------------------------------------------
		// 12/16/2013	| DMV	| Initial version.
		// --------------------------------------------------------------------------------------

		Bitmap ^ BitmapToBitmap32BPP(System::Drawing::Bitmap ^ bitmap);

		// --------------------------------------------------------------------------------------
		// Method: BitmapToPIX
		// --------------------------------------------------------------------------------------
		// Author: David Vaccaro
		// Description: Converts the specified System::Drawing::Bitmap to a comparable Leptonica PIX.
		// --------------------------------------------------------------------------------------
		// Parameters:
		//
		//	- bitmap = The bitmap to be converted.
		//
		// --------------------------------------------------------------------------------------
		// Returns: 
		//
		// - Success: A new Leptonica PIX instance.
		// - Failure: NULL
		//
		// --------------------------------------------------------------------------------------
		// Revision History (latest at the top!)
		// --------------------------------------------------------------------------------------
		// WHEN			| WHO	| WHAT
		// --------------------------------------------------------------------------------------
		// 12/16/2013	| DMV	| Initial version.
		// --------------------------------------------------------------------------------------

		Pix * BitmapToPix(System::Drawing::Bitmap ^ bitmap);

		// --------------------------------------------------------------------------------------
		// Method: Process (PIX img)
		// --------------------------------------------------------------------------------------
		// Author: David Vaccaro
		// Description: OCR process the specified Leptonica PIX and return the resultant text. 
		// --------------------------------------------------------------------------------------
		// Parameters:
		//
		//	- pPix	= The Leptonica PIX to be OCR processed.
		//
		// --------------------------------------------------------------------------------------
		// Returns:
		//
		// - Success: A String containing the recognized text.
		// - Failure: NULL
		//
		// --------------------------------------------------------------------------------------
		// Revision History (latest at the top!)
		// --------------------------------------------------------------------------------------
		// WHEN			| WHO	| WHAT
		// --------------------------------------------------------------------------------------
		// 12/17/2013	| DMV	| Initial version.
		// --------------------------------------------------------------------------------------

		String ^ Process(Pix * pPix);

		// --------------------------------------------------------------------------------------
		// Method: Extract (PIX img)
		// --------------------------------------------------------------------------------------
		// Author: David Vaccaro
		// Description: OCR process the specified image and return the resultant extractions. 
		// --------------------------------------------------------------------------------------
		// Parameters:
		//
		//	- pPix	= The Leptonica PIX to be OCR processed.
		//	- type = The type of extract to be returned.
		//
		// --------------------------------------------------------------------------------------
		// Returns:
		//
		// - Success: A List of Extracts that were recognized from the image.
		// - Failure: NULL
		//
		// --------------------------------------------------------------------------------------
		// Revision History (latest at the top!)
		// --------------------------------------------------------------------------------------
		// WHEN			| WHO	| WHAT
		// --------------------------------------------------------------------------------------
		// 01/08/2014	| DMV	| Initial version.
		// --------------------------------------------------------------------------------------

		List<FourCube::Extract ^>^ Extract(Pix * pPix, FourCube::ExtractType type);

	public:

		// --------------------------------------------------------------------------------------
		// Property: Timeout
		// --------------------------------------------------------------------------------------
		// Author: David Vaccaro
		// Description: Gets and Sets the Timeout value used to specify the maximum time (in  
		// milliseconds) that Tesseract will take when processing a file/image.
		// DEFAULT: 60000 (1 minute)
		// --------------------------------------------------------------------------------------
		// Revision History (latest at the top!)
		// --------------------------------------------------------------------------------------
		// WHEN			| WHO	| WHAT
		// --------------------------------------------------------------------------------------
		// 12/15/2013	| DMV	| Initial version.
		// --------------------------------------------------------------------------------------

		property int Timeout;

		// --------------------------------------------------------------------------------------
		// Property: Bound
		// --------------------------------------------------------------------------------------
		// Author: David Vaccaro
		// Description: Gets and Sets the Bound value used to specify the rectangular region to
		// be processed for all subsequent file/image recognitions. 
		// DEFAULT: Rectangle::Empty (process the whole page/image)
		// --------------------------------------------------------------------------------------
		// Revision History (latest at the top!)
		// --------------------------------------------------------------------------------------
		// WHEN			| WHO	| WHAT
		// --------------------------------------------------------------------------------------
		// 12/16/2013	| DMV	| Initial version.
		// --------------------------------------------------------------------------------------

		property System::Drawing::Rectangle ^ Bound {

			// The "Get" decleration
			System::Drawing::Rectangle^ get();

			// The "Set" declaration
			void set(System::Drawing::Rectangle^ value);

		}

		// --------------------------------------------------------------------------------------
		// Method: Create
		// --------------------------------------------------------------------------------------
		// Author: David Vaccaro
		// Description: Create and initialize a new instance of the OCR system
		// --------------------------------------------------------------------------------------
		// Parameters:
		//
		//	- dataPath	= (Optional) The full path to the Tesseract data directory containing the language 
		//				  data files.  If NULL or "empty string" is specified, a default value is formulated
		//				  as {AppDomain::BaseDirectory}\data\
		//
		//		Example: C:\Program Files\Program\data
		//
		//		\tessdata
		//			- eng.DangAmbigs
		//			- eng.freq-dawg
		//			- eng.inttemp
		//			- eng.normproto
		//			- eng.pffmtable
		//			- eng.unicharset
		//			- eng.user-words
		//			- eng.word-dawg
		//
		//	- languageCode = (Optional) The desired Tesseract language code.  If NULL or "empty string"
		//					 is specified, the default value is "eng".
		//
		//		Example: "eng"
		//
		// --------------------------------------------------------------------------------------
		// Returns:
		//
		// - Success: A new OCR instance
		// - Failure: NULL
		//
		// --------------------------------------------------------------------------------------
		// Revision History (latest at the top!)
		// --------------------------------------------------------------------------------------
		// WHEN			| WHO	| WHAT
		// --------------------------------------------------------------------------------------
		// 12/14/2013	| DMV	| Initial version.
		// --------------------------------------------------------------------------------------

		static OCR ^ Create(String ^ dataPath, String ^ languageCode);

		// --------------------------------------------------------------------------------------
		// Method: Constructor
		// --------------------------------------------------------------------------------------
		// Author: David Vaccaro
		// Description: Constructs a new OCR instance and initializes the default state.
		// --------------------------------------------------------------------------------------
		// Parameters: NONE
		// --------------------------------------------------------------------------------------
		// Returns: NONE
		// --------------------------------------------------------------------------------------
		// Revision History (latest at the top!)
		// --------------------------------------------------------------------------------------
		// WHEN			| WHO	| WHAT
		// --------------------------------------------------------------------------------------
		// 12/15/2013	| DMV	| Initial version.
		// --------------------------------------------------------------------------------------

		OCR();

		// --------------------------------------------------------------------------------------
		// Method: Destructor
		// --------------------------------------------------------------------------------------
		// Author: David Vaccaro
		// Description: Destructs an existing OCR instance freeing all resources.
		// --------------------------------------------------------------------------------------
		// Parameters: NONE
		// --------------------------------------------------------------------------------------
		// Returns: NONE
		// --------------------------------------------------------------------------------------
		// Revision History (latest at the top!)
		// --------------------------------------------------------------------------------------
		// WHEN			| WHO	| WHAT
		// --------------------------------------------------------------------------------------
		// 12/15/2013	| DMV	| Initial version.
		// --------------------------------------------------------------------------------------

		~OCR();

		// --------------------------------------------------------------------------------------
		// Method: ProcessAll (STRING filename)
		// --------------------------------------------------------------------------------------
		// Author: David Vaccaro
		// Description: OCR process all pages (complete page too... Bound igonred) of the specified 
		// file and return the resultant text. 
		// --------------------------------------------------------------------------------------
		// Parameters:
		//
		//	- filename	= The full path to the file to be OCR processed.
		//
		// --------------------------------------------------------------------------------------
		// Returns:
		//
		// - Success: A String containing ALL recognized text.
		// - Failure: NULL
		//
		// --------------------------------------------------------------------------------------
		// Revision History (latest at the top!)
		// --------------------------------------------------------------------------------------
		// WHEN			| WHO	| WHAT
		// --------------------------------------------------------------------------------------
		// 12/15/2013	| DMV	| Initial version.
		// --------------------------------------------------------------------------------------

		String ^ ProcessAll(String ^ filenanme);

		// --------------------------------------------------------------------------------------
		// Method: Process (STRING filename)
		// --------------------------------------------------------------------------------------
		// Author: David Vaccaro
		// Description: OCR process the specified file and return the resultant text. 
		// --------------------------------------------------------------------------------------
		// Parameters:
		//
		//	- filename	= The full path to the file to be OCR processed.
		//
		// --------------------------------------------------------------------------------------
		// Returns:
		//
		// - Success: A String containing the recognized text.
		// - Failure: NULL
		//
		// --------------------------------------------------------------------------------------
		// Revision History (latest at the top!)
		// --------------------------------------------------------------------------------------
		// WHEN			| WHO	| WHAT
		// --------------------------------------------------------------------------------------
		// 12/15/2013	| DMV	| Initial version.
		// --------------------------------------------------------------------------------------

		String ^ Process(String ^ filename);

		// --------------------------------------------------------------------------------------
		// Method: Process (BITMAP image)
		// --------------------------------------------------------------------------------------
		// Author: David Vaccaro
		// Description: OCR process the specified image and return the resultant text. 
		// --------------------------------------------------------------------------------------
		// Parameters:
		//
		//	- image	= The image to be OCR processed.
		//
		// --------------------------------------------------------------------------------------
		// Returns:
		//
		// - Success: A String containing the recognized text.
		// - Failure: NULL
		//
		// --------------------------------------------------------------------------------------
		// Revision History (latest at the top!)
		// --------------------------------------------------------------------------------------
		// WHEN			| WHO	| WHAT
		// --------------------------------------------------------------------------------------
		// 12/16/2013	| DMV	| Initial version.
		// --------------------------------------------------------------------------------------

		String ^ Process(Bitmap ^ image);

		// --------------------------------------------------------------------------------------
		// Method: Extract (STRING filename)
		// --------------------------------------------------------------------------------------
		// Author: David Vaccaro
		// Description: OCR process the specified image file and return the resultant extractions. 
		// --------------------------------------------------------------------------------------
		// Parameters:
		//
		//	- filename	= The full path to the file to be OCR processed.
		//	- type = The type of extract to be returned.
		//
		// --------------------------------------------------------------------------------------
		// Returns:
		//
		// - Success: A List of Extracts that were recognized from the image.
		// - Failure: NULL
		//
		// --------------------------------------------------------------------------------------
		// Revision History (latest at the top!)
		// --------------------------------------------------------------------------------------
		// WHEN			| WHO	| WHAT
		// --------------------------------------------------------------------------------------
		// 12/15/2013	| DMV	| Initial version.
		// --------------------------------------------------------------------------------------

		List<FourCube::Extract ^>^ Extract(String ^ filename, ExtractType type);

		// --------------------------------------------------------------------------------------
		// Method: Extract (BITMAP image)
		// --------------------------------------------------------------------------------------
		// Author: David Vaccaro
		// Description: OCR process the specified image and return the resultant extractions. 
		// --------------------------------------------------------------------------------------
		// Parameters:
		//
		//	- image	= The image to be OCR processed.
		//	- type = The type of extract to be returned.
		//
		// --------------------------------------------------------------------------------------
		// Returns:
		//
		// - Success: A List of Extracts that were recognized from the image.
		// - Failure: NULL
		//
		// --------------------------------------------------------------------------------------
		// Revision History (latest at the top!)
		// --------------------------------------------------------------------------------------
		// WHEN			| WHO	| WHAT
		// --------------------------------------------------------------------------------------
		// 12/16/2013	| DMV	| Initial version.
		// --------------------------------------------------------------------------------------

		List<FourCube::Extract ^>^ Extract(Bitmap ^ image, ExtractType type);

	};
}
