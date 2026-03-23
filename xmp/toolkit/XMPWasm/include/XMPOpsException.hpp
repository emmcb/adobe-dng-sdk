//
//  XMPOpsException.hpp
//  XMPCore
//
//  Created by Lovlesh Malik on 13/12/18.
//

#ifndef XMPOpsException_hpp
#define XMPOpsException_hpp

#include <stdio.h>
#include "XMP_Const.h"

#include <string>

#define TRY \
try {   \
*id=kXMPOpsErr_NoError;

#if ! XMP_DebugBuild
    #define XMPOps_Assert_1(c)    ((void) 0)
    #define XMPOps_Assert_2(c1,c2) ((void) 0)
	#define XMPOps_Assert_3(c1,c2,c3)   ((void) 0)
	#define XMPOps_Assert_4(c1,c2,c3,c4)    ((void) 0)
	#define XMPOps_Assert_5(c1,c2,c3,c4,c5)    ((void) 0)
#else
    #include "assert.h"
    #define XMPOps_Assert_1(c)    assert ( c !=NULL )
    #define XMPOps_Assert_2(c1,c2)    assert ( c1 !=NULL && c2 !=NULL )
    #define XMPOps_Assert_3(c1,c2,c3)    assert ( c1 !=NULL && c2 !=NULL && c3 !=NULL )
    #define XMPOps_Assert_4(c1,c2,c3,c4)    assert ( c1 !=NULL && c2 !=NULL && c3 !=NULL && c4!=NULL )
    #define XMPOps_Assert_5(c1,c2,c3,c4,c5)    assert ( c1 !=NULL && c2 !=NULL && c3 !=NULL && c4!=NULL && c5!=NULL )
#endif

#define XMPOPS_CATCH_EXCEPTIONS                                        \
} catch (XMPOpsException e) { \
*id=e.GetID();  \
memcpy(msg, e.GetErrMsg().c_str(), e.GetErrMsg().length());   \
msg[e.GetErrMsg().length()] = '\0'; \
}\
catch (...) { \
*id=kXMPOpsErr_InternalFailure; \
std::string message="Internal Failure"; \
memcpy(msg,message.c_str() ,message.length() ); \
msg[message.length()] = '\0'; \
}


//std::string msg1=e.GetErrMsg();

enum{
    /// No error
    kXMPOpsErr_NoError          =  0,
    
	kXMPOpsErr_Unknown          =   1,
    /// Generic bad parameter error
    kXMPOpsErr_BadParam         =   2,
    /// Generic bad value error
    kXMPOpsErr_BadValue         =   3,
    /// Generic internal failure
    kXMPOpsErr_InternalFailure  =   4,
    /// Generic deprecated error
    kXMPOpsErr_BadOptions       =  5,
    /// Generic external failure
    kXMPOpsErr_BadIndex  =  6,
    /// Generic standard exception
    kXMPOpsErr_BadXMP     =  7,
    /// Generic unknown exception
    kXMPOpsErr_BadStream =  8,
    /// Generic user abort error
    kXMPOpsErr_BadFileIO        =  9,
    /// Generic out-of-memory error
    kXMPOpsErr_XMPException         =  10,
    /// Generic unknown error

	kXMPOpsErr_NonExistingObject = 11
    
};

/// XMPOps Exception, associates an error code with a descriptive error string.
class XMPOpsException {
public:
    
    /// @brief Constructor for an XMPOpsException.
    ///
    /// @param _id The numeric code.
    ///
    /// @param _errMsg The descriptive string, for debugging use only. It must not be shown to users
    /// in a final product. It is written for developers, not users, and never localized.
    XMPOpsException ( XMP_Int32 _id, XMP_StringPtr _errMsg ) : id(_id), errMsg(_errMsg) {};
    
    /// Retrieves the numeric code from an XMPOpsException.
    inline XMP_Int32     GetID() const     { return id; };
    
    /// Retrieves the descriptive string from an XMPOpsException.
    inline std::string GetErrMsg() const { return errMsg; };
    
private:
    /// Exception code. See constants \c #kXMPErr_Unknown and following.
    XMP_Int32     id;
    /// Descriptive string, for debugging use only. It must not be shown to users in a final
    /// product. It is written for developers, not users, and never localized.
    std::string errMsg;
};

#endif /* XMPOpsException_hpp */



