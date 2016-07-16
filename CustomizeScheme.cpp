#include <Windows.h>

#include "include/cef_client.h"
#include "include/wrapper/cef_helpers.h"
#include "include/cef_request.h"

#include "simple_app.h"
#include "simple_handler.h"

#define __EC_CUSTOMIZE_SCHEME_CPP_
#define __ECKL_SRC_DEV_
#include "SDK\C and C++\EasyCKL.h"

#undef CKLEXPORT
#define CKLEXPORT extern "C" __declspec(dllexport)

typedef bool(WINAPI * SchemeProcessRequest)(void* request, void* data, void* mime_type, UINT* status);

typedef std::vector<unsigned char> BytesVector;
void inline __ECKL_InsertBytesVector(BytesVector& vector_, unsigned char* rawBuffer_, size_t size_) {
	vector_.insert(vector_.end(), rawBuffer_, rawBuffer_ + size_);
}

class EcSchemeHandler;
class EcSchemeHandlerFactory;

CefRefPtr<EcSchemeHandler> SchemeHandler;
CefRefPtr<EcSchemeHandlerFactory> HandlerFactory;
SchemeProcessRequest _callback;

class EcSchemeHandler : public CefResourceHandler {
public:
	EcSchemeHandler() : offset_(0) {}

	virtual bool ProcessRequest(CefRefPtr<CefRequest> request,
		CefRefPtr<CefCallback> callback) OVERRIDE {
		CEF_REQUIRE_IO_THREAD();

		if (_callback) {
			status_ = 200;
			if (_callback(request, &data_, &mime_type_, &status_)) {
				offset_ = 0;
				callback->Continue();
				return true;
			}
		}

		//std::string url = request->GetURL();
		//if (strstr(url.c_str(), "handler.html") != NULL) {
		//	// Build the response html
		//	data_ = "<html><head><title>Client Scheme Handler</title></head>"
		//		"<body bgcolor=\"white\">"
		//		"This contents of this page page are served by the "
		//		"ClientSchemeHandler class handling the client:// protocol."
		//		"<br/>You should see an image:"
		//		"<br/><img src=\"client://tests/logo.png\"><pre>";

		//	data_.append("</pre><br/>Try the test form:"
		//		"<form method=\"POST\" action=\"handler.html\">"
		//		"<input type=\"text\" name=\"field1\">"
		//		"<input type=\"text\" name=\"field2\">"
		//		"<input type=\"submit\">"
		//		"</form></body></html>");

		//	handled = true;

		//	// Set the resulting mime type
		//	mime_type_ = "text/html";
		//}

		return false;
	}

	virtual void GetResponseHeaders(CefRefPtr<CefResponse> response,
		int64& response_length,
		CefString& redirectUrl) OVERRIDE {
		CEF_REQUIRE_IO_THREAD();

		if (!data_.empty()) {
			response->SetMimeType(mime_type_);
			response->SetStatus(status_);
			response_length = data_.size();
		}
	}

	virtual void Cancel() OVERRIDE { CEF_REQUIRE_IO_THREAD(); }

	virtual bool ReadResponse(void* data_out,
		int bytes_to_read,
		int& bytes_read,
		CefRefPtr<CefCallback> callback) OVERRIDE {
		CEF_REQUIRE_IO_THREAD();

		bool has_data = false;
		bytes_read = 0;
		int transfer_size = (std::min)(bytes_to_read, (int)(data_.size() - offset_));

		if (offset_ < data_.size()) {
			memcpy(data_out, &data_[offset_], transfer_size);
			offset_ += transfer_size;
			bytes_read = transfer_size;
			has_data = true;
		}

		return has_data;
	}

private:
	BytesVector data_;
	std::wstring mime_type_;
	size_t offset_;
	UINT status_;

	IMPLEMENT_REFCOUNTING(EcSchemeHandler);
};

class EcSchemeHandlerFactory : public CefSchemeHandlerFactory {
public:
	virtual CefRefPtr<CefResourceHandler> Create(CefRefPtr<CefBrowser> browser,
		CefRefPtr<CefFrame> frame,
		const CefString& scheme_name,
		CefRefPtr<CefRequest> request) OVERRIDE {
		CEF_REQUIRE_IO_THREAD();
		return SchemeHandler;
	}

	IMPLEMENT_REFCOUNTING(EcSchemeHandlerFactory);
};

CKLEXPORT void WINAPI Chrome_RegisterSchemeInitialize(SchemeProcessRequest callback) {
	SchemeHandler = new EcSchemeHandler();
	HandlerFactory = new EcSchemeHandlerFactory();
	_callback = callback;
}

CKLEXPORT void WINAPI Chrome_RegisterScheme(const wchar_t* szSchemeName, const wchar_t* szDomainName) {
	CefRegisterSchemeHandlerFactory(szSchemeName, szDomainName, HandlerFactory);
}

CKLEXPORT SIZE_T WINAPI EcCSGetRequestUrlLength(CefRequest* lpRequest) {
	if (lpRequest) {
		return lpRequest->GetURL().length();
	}
	return 0;
}

CKLEXPORT void WINAPI EcCSGetRequestUrl(CefRequest* lpRequest, wchar_t* lpUrlBuffer, ULONG ulLength) {
	if (lpUrlBuffer && lpRequest) {
		_ECKL_CopyWString(lpRequest->GetURL(), lpUrlBuffer, ulLength * sizeof(wchar_t));
	}
}

CKLEXPORT SIZE_T WINAPI EcCSGetRequestHeaderStringLength(CefRequest* lpRequest, wchar_t* szHeaderName) {
	if (lpRequest) {
		CefRequest::HeaderMap HeaderMap;
		lpRequest->GetHeaderMap(HeaderMap);
		CefRequest::HeaderMap::iterator iterator = HeaderMap.find(szHeaderName);
		return (*iterator).second.length();
	}
	return 0;
}

CKLEXPORT void WINAPI EcCSGetRequestHeaderString(CefRequest* lpRequest, wchar_t* szHeaderName, wchar_t* lpBuffer, ULONG ulLength) {
	if (lpRequest) {
		CefRequest::HeaderMap HeaderMap;
		lpRequest->GetHeaderMap(HeaderMap);
		CefRequest::HeaderMap::iterator iterator = HeaderMap.find(szHeaderName);
		_ECKL_CopyWString((*iterator).second, lpBuffer, ulLength * sizeof(wchar_t));
	}
}

CKLEXPORT SIZE_T WINAPI EcCSGetRequestPostDataSize(CefRequest* lpRequest) {
	CefRefPtr<CefPostData> postData = lpRequest->GetPostData();
	SIZE_T ret_size = 0;
	if (postData.get()) {
		CefPostData::ElementVector elements;
		postData->GetElements(elements);
		if (elements.size() > 0) {
			CefRefPtr<CefPostDataElement> element;
			CefPostData::ElementVector::const_iterator it = elements.begin();
			for (; it != elements.end(); ++it) {
				element = (*it);
				if (element->GetType() == PDE_TYPE_BYTES) {
					if (element->GetBytesCount() == 0)
						continue;
					else {
						size_t size = element->GetBytesCount();
						ret_size += size;
					}
				}
			}
		}
	}
	return ret_size;
}

CKLEXPORT void WINAPI EcCSGetRequestPostData(CefRequest* lpRequest, BYTE* lpDataBuffer, ULONG_PTR notused) {
	CefRefPtr<CefPostData> postData = lpRequest->GetPostData();
	ULONG_PTR Offset = 0;
	if (postData.get()) {
		CefPostData::ElementVector elements;
		postData->GetElements(elements);
		if (elements.size() > 0) {
			CefRefPtr<CefPostDataElement> element;
			CefPostData::ElementVector::const_iterator it = elements.begin();
			for (; it != elements.end(); ++it) {
				element = (*it);
				if (element->GetType() == PDE_TYPE_BYTES) {
					if (element->GetBytesCount() == 0)
						continue;
					else {
						size_t size = element->GetBytesCount();
						unsigned char* _bytes = new unsigned char[size];
						element->GetBytes(size, _bytes);
						memcpy(lpDataBuffer + Offset, _bytes, size);
						Offset += size;
						delete[] _bytes;
					}
				}
			}
		}
	}
}

CKLEXPORT void WINAPI EcCSSetData(BytesVector* lpData, unsigned char* lpDataBuffer, SIZE_T nSize) {
	(*lpData).clear();
	__ECKL_InsertBytesVector(*lpData, lpDataBuffer, nSize);
}

CKLEXPORT void WINAPI EcCSSetMimeType(std::wstring* lpMimeType, const wchar_t* szMimeType) {
	(*lpMimeType) = szMimeType;
}

CKLEXPORT void WINAPI EcCSSetStatus(int* lpStatus, int iStatus) {
	(*lpStatus) = iStatus;
}
