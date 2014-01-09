// This is the main DLL file.

#include "stdafx.h"

#using <system.drawing.dll>

#include "tesseract\baseapi.h"
#include "leptonica\allheaders.h"

#include "FourCube.h"

FourCube::OCR::OCR() 
{

	// Establish the initial state
	_pAPI = NULL;
	_dataPath = nullptr;
	_languageCode = nullptr;
	_bound = System::Drawing::Rectangle::Empty;
	this->Timeout = DEFULT_TIMEOUT;

	// Return
	return;

}

FourCube::OCR::~OCR() 
{

	// Free the Tesseract API instance
	if (_pAPI != NULL) {

		// End the Terresact API
		_pAPI->End();

		// Free
		delete _pAPI;

		// NUll
		_pAPI = NULL;

	}

	// Return
	return;

}

System::Drawing::Rectangle ^ FourCube::OCR::Bound::get() {
	return _bound;
}

void FourCube::OCR::Bound::set(System::Drawing::Rectangle ^ value) {

	// Chedck the params
	if (value == nullptr)
		value = System::Drawing::Rectangle::Empty;

	// Set the value
	_bound = value;

	// Return
	return;

}

Bitmap ^ FourCube::OCR::BitmapToBitmap32BPP(System::Drawing::Bitmap ^ bitmap)
{

	// Create the new 32BPP bitmap
	Bitmap ^ bitmap32 = gcnew Bitmap(bitmap->Size.Width, bitmap->Size.Height, System::Drawing::Imaging::PixelFormat::Format24bppRgb);

	// Create a Graphics from the new bitmap
	Graphics ^ g = Graphics::FromImage(bitmap32);

	// Render the image into the graphics
	g->DrawImageUnscaled(bitmap, 0, 0);

	// Free the graphics
	delete g;

	// Return the new bitmap
	return bitmap32;

}

Pix * FourCube::OCR::BitmapToPix(System::Drawing::Bitmap ^ bitmap)
{

	// First, Check the params
	if (bitmap == nullptr)
		throw gcnew ArgumentNullException();

	// Convert the specified bitmap into a streight 32BPP bitmap
	Bitmap ^ bitmap32 = BitmapToBitmap32BPP(bitmap);

	if (bitmap32 == nullptr)
		throw gcnew System::ArgumentException();

	try {

		// Create the new PIX 
		Pix * pPix = pixCreate(bitmap32->Width, bitmap32->Height, 32);

		if (pPix == NULL)
			throw gcnew System::OutOfMemoryException();

		System::Drawing::Imaging::BitmapData^ bmpData;

		try {

			// Access the BMP data
			bmpData = bitmap32->LockBits(System::Drawing::Rectangle(0, 0, bitmap32->Width, bitmap32->Height), System::Drawing::Imaging::ImageLockMode::ReadOnly, bitmap32->PixelFormat);

			if (bmpData != nullptr) {

				// Access the PIX data
				l_uint32 * pixData = pixGetData(pPix);

				if (pixData != NULL) {

					// Loop over the lines (Y)
					for (int y = 0; y < bitmap32->Height; y++) {
					
						// Loop over the colums (X)
						for (int x = 0; x < bitmap32->Width; x++) {
						
							// Access the "source" pixel BYTE
							BYTE * pSource = (((BYTE *)bmpData->Scan0.ToPointer() + (y * bmpData->Stride)) + (x * 3));

							// Access the "destination" pixel 4-BYTE
							l_uint32 * pDestination = (((l_uint32 *)pixData + (y * (bitmap32->Width * 4))) + (x * 4));

							// Set the value
							*pDestination = (UINT)((pSource[0] << 24) | (pSource[1] << 16) | (pSource[2] << 8) | 255);

						}
					
					}

					// Return
					return pPix;

				}

			}

		}
		catch (...) {

			// Free any Pix
			if (pPix != NULL) {
				pixDestroy(&pPix);
				pPix = NULL;
			}

		}
		finally {

			// Unlock the bitmap data
			if (bmpData != nullptr)
				bitmap32->UnlockBits(bmpData);

		}

	}
	finally
	{

		// Cleaup the bitmap
		if (bitmap32 != nullptr)
			delete bitmap;

	}

	// Return
	return NULL;

}

String^ FourCube::OCR::Process(Pix * pPix)
{

	// Check the params
	if (pPix == NULL)
		throw gcnew System::ArgumentNullException();

	// Check the state
	if (_pAPI == NULL)
		throw gcnew System::NullReferenceException();

	// The result text
	char * pText = NULL;

	try {

		// Set the image to be processed
		_pAPI->SetImage(pPix);

		// Establish the processing area
		if ((_bound == System::Drawing::Rectangle::Empty) || (_bound->IsEmpty == true))
			_pAPI->SetRectangle(0, 0, pPix->w, pPix->h);
		else
			_pAPI->SetRectangle(_bound->X, _bound->Y, _bound->Width, _bound->Height);

		// Return the result string
		return gcnew String(_pAPI->GetUTF8Text());

	}
	finally {

		// Clear the API state
		_pAPI->Clear();

	}

	// Return
	return nullptr;

}

List<FourCube::Extract ^>^ FourCube::OCR::Extract(Pix * pPix, FourCube::ExtractType type)
{

	// Check the params
	if (pPix == NULL)
		throw gcnew System::ArgumentNullException();

	// Check the state
	if (_pAPI == NULL)
		throw gcnew System::NullReferenceException();

	// The result collection
	List<FourCube::Extract ^>^ res = gcnew List<FourCube::Extract ^>();

	try {

		tesseract::PageIteratorLevel level = tesseract::RIL_WORD;

		// Set the image to be processed
		_pAPI->SetImage(pPix);

		// Establish the processing area
		if ((_bound == System::Drawing::Rectangle::Empty) || (_bound->IsEmpty == true))
			_pAPI->SetRectangle(0, 0, pPix->w, pPix->h);
		else
			_pAPI->SetRectangle(_bound->X, _bound->Y, _bound->Width, _bound->Height);

		// Recognize the image
		_pAPI->Recognize(0);

		// Establish the page-level-iterator
		switch (type)
		{
			case FourCube::ExtractType::Block: level = tesseract::RIL_BLOCK;  break;
			case FourCube::ExtractType::Paragraph: level = tesseract::RIL_PARA;  break;
			case FourCube::ExtractType::Symbol: level = tesseract::RIL_SYMBOL;  break;
			case FourCube::ExtractType::Line: level = tesseract::RIL_TEXTLINE;  break;
			case FourCube::ExtractType::Word: level = tesseract::RIL_WORD;  break;
		default:
			break;
		}

		// Access the iterator
		tesseract::ResultIterator * pIterator = _pAPI->GetIterator();

		if (pIterator != NULL) {

			do {

				// Access the text
				const char * pText = pIterator->GetUTF8Text(level);

				// Access the confidence
				float confidence = pIterator->Confidence(level);

				// Access the bounding rectangle
				int x1, y1, x2, y2;
				
				pIterator->BoundingBox(level, &x1, &y1, &x2, &y2);

				// Build an extract
				FourCube::Extract ^ extract = gcnew FourCube::Extract();

				// Setup the extract
				extract->Text = gcnew String(pText);
				extract->Confidence = confidence;
				extract->Bound = gcnew System::Drawing::Rectangle(x1, y1, (x2 - x1), (y2 - y1));
				extract->Type = type;

				// Add the extract
				res->Add(extract);

			}
			while (pIterator->Next(level) == true);
		
		}

	}
	finally {

		// Clear the API state
		_pAPI->Clear();

	}

	// Return
	return res;

}

FourCube::OCR^ FourCube::OCR::Create(String^ dataPath, String^ languageCode)
{

	// Check the params
	if (String::IsNullOrEmpty(dataPath) == true)
		dataPath = nullptr;

	if (String::IsNullOrEmpty(languageCode) == true)
		languageCode = nullptr;

	// Establish the default dataPath value
	if (dataPath == nullptr)
		dataPath = System::IO::Path::Combine(System::AppDomain::CurrentDomain->BaseDirectory, "data\\");

	// Establish the default languageCode value
	if (languageCode == nullptr)
		languageCode = "eng";

	// Create a new instance of the OCR
	OCR^ ocr = gcnew OCR();

	// Create the instance of the Tesseract base API
	ocr->_pAPI = new tesseract::TessBaseAPI();

	// Check the API
	if (ocr->_pAPI == NULL)
		throw gcnew System::InsufficientMemoryException();

	// Set the parameters (for possible future use)
	ocr->_dataPath = dataPath;
	ocr->_languageCode = languageCode;

	// Create the params
	IntPtr pDataPath = Marshal::StringToHGlobalAnsi(dataPath);			
	IntPtr pLanguageCode = Marshal::StringToHGlobalAnsi(languageCode);

	// Init the Tesseract API instance
	int status = ocr->_pAPI->Init((char *)pDataPath.ToPointer(), (char *)pLanguageCode.ToPointer());

	// If the initialization failed, free up all the resources
	if (status != 0) {

		// Free the Tesseract API
		delete ocr->_pAPI;

		// NULL the ocr instance
		ocr = nullptr;

	}

	// Free the params
	Marshal::FreeHGlobal(pDataPath);
	Marshal::FreeHGlobal(pLanguageCode);

	// Return
	return ocr;

}

String^ FourCube::OCR::ProcessAll(String^ filename)
{

	// Check the params
	if (filename == nullptr)
		throw gcnew System::ArgumentNullException();

	// Check the state
	if (_pAPI == NULL)
		throw gcnew System::NullReferenceException();

	// Create a STRING to store the restulant text
	STRING * pResult = new STRING();

	// Create the params
	IntPtr pFilename = Marshal::StringToHGlobalAnsi(filename);			

	try {

		// Process the page
		if (_pAPI->ProcessPages((char *)pFilename.ToPointer(), NULL, this->Timeout, pResult) == true) {

			// Return the result string
			return gcnew String(pResult->string());

		}

	}
	finally {

		// Free the result STRING
		if (pResult != NULL)
			delete pResult;

		// Free the filename
		Marshal::FreeHGlobal(pFilename);

	}

	// Return
	return nullptr;

}

String^ FourCube::OCR::Process(String^ filename)
{

	// Check the params
	if (filename == nullptr)
		throw gcnew System::ArgumentNullException();

	// Check the state
	if (_pAPI == NULL)
		throw gcnew System::NullReferenceException();

	// Create the params
	IntPtr pFilename = Marshal::StringToHGlobalAnsi(filename);			

	// Open input image with leptonica library
	Pix * image = pixRead((char *)pFilename.ToPointer());

	try {

		// Process the PIX
		return Process(image);

	}
	finally {

		// Destroy the image
		pixDestroy(&image);

		// Free the filename
		Marshal::FreeHGlobal(pFilename);

	}

	// Return
	return nullptr;

}

String^ FourCube::OCR::Process(Bitmap^ image)
{

	// Check the params
	if (image == nullptr)
		throw gcnew System::ArgumentNullException();

	// Check the state
	if (_pAPI == NULL)
		throw gcnew System::NullReferenceException();

	// Convert the image to a PIX
	Pix * pPix = BitmapToPix(image);

	try {

		// Process the PIX
		return Process(image);

	}
	finally {

		// Destroy the image
		pixDestroy(&pPix);

	}

	// Return
	return nullptr;

}

List<FourCube::Extract ^>^ FourCube::OCR::Extract(String ^ filename, FourCube::ExtractType type)
{

	// Check the params
	if (filename == nullptr)
		throw gcnew System::ArgumentNullException();

	// Check the state
	if (_pAPI == NULL)
		throw gcnew System::NullReferenceException();

	// Create the params
	IntPtr pFilename = Marshal::StringToHGlobalAnsi(filename);			

	// Open input image with leptonica library
	Pix * image = pixRead((char *)pFilename.ToPointer());

	try {

		// Extract the results
		return Extract(image, type);

	}
	finally {

		// Destroy the image
		pixDestroy(&image);

		// Free the filename
		Marshal::FreeHGlobal(pFilename);

	}

	// Return
	return gcnew List<FourCube::Extract ^>();

}

List<FourCube::Extract ^>^ FourCube::OCR::Extract(Bitmap ^ image, FourCube::ExtractType type)
{

	// Check the params
	if (image == nullptr)
		throw gcnew System::ArgumentNullException();

	// Check the state
	if (_pAPI == NULL)
		throw gcnew System::NullReferenceException();

	// Convert the image to a PIX
	Pix * pPix = BitmapToPix(image);

	try {

		// Extract the results
		return Extract(image, type);

	}
	finally {

		// Destroy the image
		pixDestroy(&pPix);

	}

	// Return
	return gcnew List<FourCube::Extract ^>();

}
