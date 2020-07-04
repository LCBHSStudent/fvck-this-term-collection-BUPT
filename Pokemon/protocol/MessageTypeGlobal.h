#ifndef MESSAGETYPEGLOBAL_H
#define MESSAGETYPEGLOBAL_H

enum MessageType: int {
    UserSignUpRequest = 0,
    UserSignUpResponse,
    
    UserLoginRequest,
    UserLoginResponse,
    
    UserInfoRequest,
    UserInfoResponse,
};

#endif // MESSAGETYPEGLOBAL_H
