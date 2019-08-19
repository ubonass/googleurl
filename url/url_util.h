// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef URL_URL_UTIL_H_
#define URL_URL_UTIL_H_

#include <string>
#include <vector>

#include "polyfills/base/component_export.h"
#include "base/strings/string16.h"
#include "base/strings/string_piece.h"
#include "url/third_party/mozilla/url_parse.h"
#include "url/url_canon.h"
#include "url/url_constants.h"

namespace url {

// Init ------------------------------------------------------------------------

// Resets all custom schemes to the default values.  Not thread-safe.
COMPONENT_EXPORT(URL) void ResetForTests();

// Schemes ---------------------------------------------------------------------

// Changes the behavior of SchemeHostPort / Origin to allow non-standard schemes
// to be specified, instead of canonicalizing them to an invalid SchemeHostPort
// or opaque Origin, respectively. This is used for Android WebView backwards
// compatibility, which allows the use of custom schemes: content hosted in
// Android WebView assumes that one URL with a non-standard scheme will be
// same-origin to another URL with the same non-standard scheme.
//
// Not thread-safe.
COMPONENT_EXPORT(URL) void EnableNonStandardSchemesForAndroidWebView();

// Whether or not SchemeHostPort and Origin allow non-standard schemes.
COMPONENT_EXPORT(URL) bool AllowNonStandardSchemesForAndroidWebView();

// A pair for representing a standard scheme name and the SchemeType for it.
struct COMPONENT_EXPORT(URL) SchemeWithType {
  const char* scheme;
  SchemeType type;
};

// The following Add*Scheme method are not threadsafe and can not be called
// concurrently with any other url_util function. They will assert if the lists
// of schemes have been locked (see LockSchemeRegistries).

// Adds an application-defined scheme to the internal list of "standard-format"
// URL schemes. A standard-format scheme adheres to what RFC 3986 calls "generic
// URI syntax" (https://tools.ietf.org/html/rfc3986#section-3).

COMPONENT_EXPORT(URL)
void AddStandardScheme(const char* new_scheme, SchemeType scheme_type);

// Adds an application-defined scheme to the internal list of schemes allowed
// for referrers.
COMPONENT_EXPORT(URL)
void AddReferrerScheme(const char* new_scheme, SchemeType scheme_type);

// Adds an application-defined scheme to the list of schemes that do not trigger
// mixed content warnings.
COMPONENT_EXPORT(URL) void AddSecureScheme(const char* new_scheme);
COMPONENT_EXPORT(URL) const std::vector<std::string>& GetSecureSchemes();

// Adds an application-defined scheme to the list of schemes that normal pages
// cannot link to or access (i.e., with the same security rules as those applied
// to "file" URLs).
COMPONENT_EXPORT(URL) void AddLocalScheme(const char* new_scheme);
COMPONENT_EXPORT(URL) const std::vector<std::string>& GetLocalSchemes();

// Adds an application-defined scheme to the list of schemes that cause pages
// loaded with them to not have access to pages loaded with any other URL
// scheme.
COMPONENT_EXPORT(URL) void AddNoAccessScheme(const char* new_scheme);
COMPONENT_EXPORT(URL) const std::vector<std::string>& GetNoAccessSchemes();

// Adds an application-defined scheme to the list of schemes that can be sent
// CORS requests.
COMPONENT_EXPORT(URL) void AddCorsEnabledScheme(const char* new_scheme);
COMPONENT_EXPORT(URL) const std::vector<std::string>& GetCorsEnabledSchemes();

// Adds an application-defined scheme to the list of web schemes that can be
// used by web to store data (e.g. cookies, local storage, ...). This is
// to differentiate them from schemes that can store data but are not used on
// web (e.g. application's internal schemes) or schemes that are used on web but
// cannot store data.
COMPONENT_EXPORT(URL) void AddWebStorageScheme(const char* new_scheme);
COMPONENT_EXPORT(URL) const std::vector<std::string>& GetWebStorageSchemes();

// Adds an application-defined scheme to the list of schemes that can bypass the
// Content-Security-Policy (CSP) checks.
COMPONENT_EXPORT(URL) void AddCSPBypassingScheme(const char* new_scheme);
COMPONENT_EXPORT(URL) const std::vector<std::string>& GetCSPBypassingSchemes();

// Adds an application-defined scheme to the list of schemes that are strictly
// empty documents, allowing them to commit synchronously.
COMPONENT_EXPORT(URL) void AddEmptyDocumentScheme(const char* new_scheme);
COMPONENT_EXPORT(URL) const std::vector<std::string>& GetEmptyDocumentSchemes();

// Sets a flag to prevent future calls to Add*Scheme from succeeding.
//
// This is designed to help prevent errors for multithreaded applications.
// Normal usage would be to call Add*Scheme for your custom schemes at
// the beginning of program initialization, and then LockSchemeRegistries. This
// prevents future callers from mistakenly calling Add*Scheme when the
// program is running with multiple threads, where such usage would be
// dangerous.
//
// We could have had Add*Scheme use a lock instead, but that would add
// some platform-specific dependencies we don't otherwise have now, and is
// overkill considering the normal usage is so simple.
COMPONENT_EXPORT(URL) void LockSchemeRegistries();

// Locates the scheme in the given string and places it into |found_scheme|,
// which may be NULL to indicate the caller does not care about the range.
//
// Returns whether the given |compare| scheme matches the scheme found in the
// input (if any). The |compare| scheme must be a valid canonical scheme or
// the result of the comparison is undefined.
COMPONENT_EXPORT(URL)
bool FindAndCompareScheme(const char* str,
                          int str_len,
                          const char* compare,
                          Component* found_scheme);
COMPONENT_EXPORT(URL)
bool FindAndCompareScheme(const gurl_base::char16* str,
                          int str_len,
                          const char* compare,
                          Component* found_scheme);
inline bool FindAndCompareScheme(const std::string& str,
                                 const char* compare,
                                 Component* found_scheme) {
  return FindAndCompareScheme(str.data(), static_cast<int>(str.size()),
                              compare, found_scheme);
}
inline bool FindAndCompareScheme(const gurl_base::string16& str,
                                 const char* compare,
                                 Component* found_scheme) {
  return FindAndCompareScheme(str.data(), static_cast<int>(str.size()),
                              compare, found_scheme);
}

// Returns true if the given scheme identified by |scheme| within |spec| is in
// the list of known standard-format schemes (see AddStandardScheme).
COMPONENT_EXPORT(URL)
bool IsStandard(const char* spec, const Component& scheme);
COMPONENT_EXPORT(URL)
bool IsStandard(const gurl_base::char16* spec, const Component& scheme);

// Returns true if the given scheme identified by |scheme| within |spec| is in
// the list of allowed schemes for referrers (see AddReferrerScheme).
COMPONENT_EXPORT(URL)
bool IsReferrerScheme(const char* spec, const Component& scheme);

// Returns true and sets |type| to the SchemeType of the given scheme
// identified by |scheme| within |spec| if the scheme is in the list of known
// standard-format schemes (see AddStandardScheme).
COMPONENT_EXPORT(URL)
bool GetStandardSchemeType(const char* spec,
                           const Component& scheme,
                           SchemeType* type);
COMPONENT_EXPORT(URL)
bool GetStandardSchemeType(const gurl_base::char16* spec,
                           const Component& scheme,
                           SchemeType* type);

// Hosts  ----------------------------------------------------------------------

// Returns true if the |canonical_host| matches or is in the same domain as the
// given |canonical_domain| string. For example, if the canonicalized hostname
// is "www.google.com", this will return true for "com", "google.com", and
// "www.google.com" domains.
//
// If either of the input StringPieces is empty, the return value is false. The
// input domain should match host canonicalization rules. i.e. it should be
// lowercase except for escape chars.
COMPONENT_EXPORT(URL)
bool DomainIs(gurl_base::StringPiece canonical_host,
              gurl_base::StringPiece canonical_domain);

// Returns true if the hostname is an IP address. Note: this function isn't very
// cheap, as it must re-parse the host to verify.
COMPONENT_EXPORT(URL) bool HostIsIPAddress(gurl_base::StringPiece host);

// URL library wrappers --------------------------------------------------------

// Parses the given spec according to the extracted scheme type. Normal users
// should use the URL object, although this may be useful if performance is
// critical and you don't want to do the heap allocation for the std::string.
//
// As with the Canonicalize* functions, the charset converter can
// be NULL to use UTF-8 (it will be faster in this case).
//
// Returns true if a valid URL was produced, false if not. On failure, the
// output and parsed structures will still be filled and will be consistent,
// but they will not represent a loadable URL.
COMPONENT_EXPORT(URL)
bool Canonicalize(const char* spec,
                  int spec_len,
                  bool trim_path_end,
                  CharsetConverter* charset_converter,
                  CanonOutput* output,
                  Parsed* output_parsed);
COMPONENT_EXPORT(URL)
bool Canonicalize(const gurl_base::char16* spec,
                  int spec_len,
                  bool trim_path_end,
                  CharsetConverter* charset_converter,
                  CanonOutput* output,
                  Parsed* output_parsed);

// Resolves a potentially relative URL relative to the given parsed base URL.
// The base MUST be valid. The resulting canonical URL and parsed information
// will be placed in to the given out variables.
//
// The relative need not be relative. If we discover that it's absolute, this
// will produce a canonical version of that URL. See Canonicalize() for more
// about the charset_converter.
//
// Returns true if the output is valid, false if the input could not produce
// a valid URL.
COMPONENT_EXPORT(URL)
bool ResolveRelative(const char* base_spec,
                     int base_spec_len,
                     const Parsed& base_parsed,
                     const char* relative,
                     int relative_length,
                     CharsetConverter* charset_converter,
                     CanonOutput* output,
                     Parsed* output_parsed);
COMPONENT_EXPORT(URL)
bool ResolveRelative(const char* base_spec,
                     int base_spec_len,
                     const Parsed& base_parsed,
                     const gurl_base::char16* relative,
                     int relative_length,
                     CharsetConverter* charset_converter,
                     CanonOutput* output,
                     Parsed* output_parsed);

// Replaces components in the given VALID input URL. The new canonical URL info
// is written to output and out_parsed.
//
// Returns true if the resulting URL is valid.
COMPONENT_EXPORT(URL)
bool ReplaceComponents(const char* spec,
                       int spec_len,
                       const Parsed& parsed,
                       const Replacements<char>& replacements,
                       CharsetConverter* charset_converter,
                       CanonOutput* output,
                       Parsed* out_parsed);
COMPONENT_EXPORT(URL)
bool ReplaceComponents(const char* spec,
                       int spec_len,
                       const Parsed& parsed,
                       const Replacements<gurl_base::char16>& replacements,
                       CharsetConverter* charset_converter,
                       CanonOutput* output,
                       Parsed* out_parsed);

// String helper functions -----------------------------------------------------

enum class DecodeURLMode {
  // UTF-8 decode only. Invalid byte sequences are replaced with U+FFFD.
  kUTF8,
  // Try UTF-8 decoding. If the input contains byte sequences invalid
  // for UTF-8, apply byte to Unicode mapping.
  kUTF8OrIsomorphic,
};

// Unescapes the given string using URL escaping rules.
COMPONENT_EXPORT(URL)
void DecodeURLEscapeSequences(const char* input,
                              int length,
                              DecodeURLMode mode,
                              CanonOutputW* output);

// Escapes the given string as defined by the JS method encodeURIComponent. See
// https://developer.mozilla.org/en/JavaScript/Reference/Global_Objects/encodeURIComponent
COMPONENT_EXPORT(URL)
void EncodeURIComponent(const char* input, int length, CanonOutput* output);

}  // namespace url

#endif  // URL_URL_UTIL_H_
